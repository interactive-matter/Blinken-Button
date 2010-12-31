/*
 * display.c
 *
 *  http://interactive-matter.org/
 *
 *  This file is part of Blinken Button.
 *
 *  Blinken Button is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Blinken Button is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with Blinken Button.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *  Created on: 26.01.2010
 *
 *  The display is a pretty simple module. It consists of a timer (Timer 0)
 *  which goes through the rows 0 to 7 and sets the ports to the corresponding
 *  values to light the correct LEDs.
 *  The values are stored directly as values used to apply to the ports.
 *  The display has two buffers to store the output values. One buffer that is
 *  currently displayed and one buffer where the next image can be stored.
 *  While loading a sprite from an 8 byte array (each byte representing a row)
 *  it gets converted to the port values to ensure a fast computation.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/pgmspace.h>
#include <string.h>

#include "state.h"
#include "../flash-content/sprites.h"
#include "display.h"

/*
 * Here we prototype some private functions we only need in this module.
 * Therefore we define them here and not in the include file
 */

/*
 * configure & start the main render timer - Timer 0
 */
void
display_start_column_timer(void);

/*
 * the current column, which is rendered. It is stored in a register
 * to ensure a fast update of the value - since it will get updated
 * pretty often.
 */
register uint8_t display_curr_column asm("r2");

/*
 * Which of the buffers is currently displayed 0 or 1
 */
uint8_t display_current_buffer;
/*
 * For the display we track an additional state:
 *  - is the buffer locked
 *  - should we switch buffers?
 *  TODO: get rid of this ore make it clear!
 */

register uint8_t display_status asm("r3");
#define DISPLAY_BUFFER_LOCKED _BV(0)
#define DISPLAY_BUFFER_ADVANCE _BV(1)

/**
 * This structure contains the display optimized values of the current image,
 * for one row.
 * pb - the values to apply for Port B
 * pc - the values to apply on Port C
 * pd - the values to apply for Port D
 * num_bit -  the number of bits in the current row
 * this is used for the bit correction.
 * If we light only one LED in a row it gets much brighter than if we display
 * all 8 LEDs in a row, since the internal resistance of the battery is that high.
 * (and the ATmega struggles to sink all the current).
 * Therefore we light up one LED shorter than 8 LEDs - called dot correction.
 */
typedef struct
{
  uint8_t pb;
  uint8_t pc;
  uint8_t pd;
  uint8_t num_bit;
} display_line;

/*
 * This is the double bugffer for the images:
 * 2 Buffers
 * each 8 rows.
 */
display_line display_buffer[2][8];

/*
 * This method initializes the display. It sets the output ports, loads the
 * default sequence and starts the display timer (Timer 0).
 */
void
display_init(void)
{
  //set all unused pins as inputs & and all display pins as output
  DDRB = 0x3;
  DDRC = 0x3f;
  DDRD = 0xff;

  //kick off the display timers to start rendering
  display_start_column_timer();
}

/*
 * This routines loads an 8x8 bit matrix (8 bytes) into the internal buffer in
 * the format of the  display struct. The display struct contains all port
 * settings to increase the render speed.
 * The result is allways written into the unused buffer.
 * While loading it is converted to direct bits for the ports. The unmber of
 * activated bits (=LEDs) is coounte to enable some dot correction.
 */
void
display_load_sprite(uint8_t origin[])
{
  //we select the next buffer by xoring either 0 or 1 with 1
  uint8_t number = display_current_buffer ^ 1;
  //lock the buffer to signal the display to wait with switching display buffers
  //by that we got enough time to completely prepare the unused buffer.
  display_status |= DISPLAY_BUFFER_LOCKED;
  uint8_t column;
  for (column = 0; column < 8; column++)
    {
      //first we set all pins to low
      uint8_t pb = 0;
      uint8_t pc = 0;
      uint8_t pd = 0;

      //select the correct column
      //this will switch on the column transistor
      //bits 0 to 5 are set on the port c, bit 6-8
      //is set on port c.
      if (column < 6)
        {
          pc |= _BV(column);
        }
      else
        {
          pb |= _BV(column) >> 6;
        }
      //calculate the number of active bits
      display_buffer[number][column].num_bit = 0;
      for (int i = 0; i < 8; i++)
        {
          if (origin[column] & _BV(i))
            {
              display_buffer[number][column].num_bit++;
            }
        }
      //enable the drain for the selected lines
      //TODO do we still need this
      pd = origin[column];

      //save the calculated values to the sprite
      display_buffer[number][column].pb = pb;
      display_buffer[number][column].pc = pc;
      display_buffer[number][column].pd = pd;
    }
  //unlock the buffer
  display_status &= ~(DISPLAY_BUFFER_LOCKED);
}

/*
 * Switch buffers.
 * This enables the rendering of the previously unused buffer (hopefully with
 * a new image) and frees the previuosly rendered buffer for writing to it.
 * The switching of the buffers is done by the display timer. Since only the
 * display timer knows when it does not need the buffer any longer.
 */
void
display_advance_buffer(void)
{
  //TODO don't we have to obey the display locked?
  display_status |= DISPLAY_BUFFER_ADVANCE;
}

/*
 * Loads the test pattern.
 * TODO come up with a better test pattern solution!
 */
void
display_load_default_sequence(void)
{
  uint8_t default_load_buffer[8] =
    { 0, 0, 0, 0, 0, 0, 0, 0 };

  display_current_buffer = 0;
  copy_to_buffer(predefined_sprites[DEFAULT_1], default_load_buffer);
  display_load_sprite(default_load_buffer);
  display_current_buffer = 1;
  copy_to_buffer(predefined_sprites[DEFAULT_2], default_load_buffer);
  display_load_sprite(default_load_buffer);
  display_current_buffer = 0;
}

/*
 * Configures the column timer (Timer 0).
 * at 156.25kHz?
 * TODO why OCR0A 200?
 */
void
display_start_column_timer(void)
{
  power_timer0_enable();
  //nothing to set on TCCR0A
  TCCR0A = 0;
  TCCR0B = _BV(CS01) || _BV(CS00);
  TIMSK0 = _BV(OCIE0A);
  OCR0A = 200;
}

/*
 * The overflow event for Timer 0.
 * This is the heart of the display routine. It is triggered every time Timer 0
 * hits OCR0A as upper limit.
 * The timer interrupt reoutine does the following:
 * - it disables all interrupts to be uninterrupted by other interrupts
 * - it renders the next row and increases the row counter
 * - if needed it switches the display buffer
 * - it enables all the interrupts again
 * TODO remove test mode
 */
ISR(TIMER0_COMPA_vect )
{
  //we want no other interrupts
  cli();
  //set all row drains to high
  PORTB = 0;
  PORTC = 0;
  PORTD = 0;
  //we strip the first two bits - they are for dimming
  //TODO do we get an 2 bit resolution too?
  uint8_t display_column = display_curr_column & 7;
  if ((display_curr_column & 8)
      && !(display_buffer[display_current_buffer][display_column].num_bit & 0xFC))
    {
      //do nothing - disable
      ;
    }
  else
    {
      //the set the ports line still off
      PORTB = 0;
      PORTC = 0;
      PORTD = display_buffer[display_current_buffer][display_column].pd;
      //the set the ports line enable
      PORTB = display_buffer[display_current_buffer][display_column].pb;
      PORTC = display_buffer[display_current_buffer][display_column].pc;
    }

  //mask to max 8 + 2 dimming bits
  display_curr_column++;
  display_curr_column &= 0x1f;

  if (display_curr_column == 0)
    {
      //if we reached the last column (and wrap) & we should advance to the next sprite display the next sprite
      if ((display_status & (DISPLAY_BUFFER_LOCKED | DISPLAY_BUFFER_ADVANCE))
          == DISPLAY_BUFFER_ADVANCE)
        {
          //advance one buffer
          display_current_buffer = display_current_buffer ^ 1;
          display_status &= ~(DISPLAY_BUFFER_ADVANCE);
        }
    }
  //now other interrupts are fine
  sei();
}

