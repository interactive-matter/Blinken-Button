/*
 * display.c
 *
 *  http://interactive-matter.eu/
 *
 * This file contains routines to display images on the display. The low level
 * display driver stuff like going through rows, mapping images.
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
//include the definitions for our chip, like pins, ports & so on
#include <avr/io.h>
//we are using interrupts & timers as schedule - here we have the def. of the
//interrupt routines and names
#include <avr/interrupt.h>
//we power up & down chip components as needed, here are the functions to do this
#include <avr/power.h>
//since we are using stuff for the flash memory we need the routines and
//definitions for the flash
#include <avr/pgmspace.h>

//we are using states to track activity
#include "state.h"
//we need some basic definitions for fonts & default sprites
#include "core-flash-content.h"
//and we need our own definitions
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
 * This is the double buffer for the images:
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
 * The result is always written into the unused buffer.
 * While loading it is converted to direct bits for the ports. The number of
 * activated bits (=LEDs) is counted to enable some dot correction.
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
 * a new image) and frees the previously rendered buffer for writing to it.
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
 */
void
display_load_default_sequence(void)
{
  uint8_t default_load_buffer[8] =
    { 0, 0, 0, 0, 0, 0, 0, 0 };

  display_current_buffer = 0;
  copy_to_buffer(default_sprites[0], default_load_buffer);
  display_load_sprite(default_load_buffer);
  display_current_buffer = 1;
  copy_to_buffer(default_sprites[1], default_load_buffer);
  display_load_sprite(default_load_buffer);
  display_current_buffer = 0;
}

/*
 * Configures the column timer (Timer 0) at 13.9 kHz
 *
 * The estimated interrupt frequency
 *   F_OC = F_CPU/(prescaler*(OCR0A+1))
 *        = 8,000,000 Hz / (64 * (8+1))
 *        =  13,889 Hz (~14kHz)
 * results in a 'frame rate' of 13,339/8 = 1,736 FPS.
 * This _should_ be enough to prevent LED flicker.
 *
 * FPS rates below 1,000Hz will _definitely_ lead to POV
 * flicker effects! Test it for yourself by increasing
 * OCR0A to e.g. 17 (= 868 FPS, slight POV effects) or 35
 * (= 434 FPS, strong POV effects when shaking the device)
 */

void
display_start_column_timer(void)
{
  power_timer0_enable();
  //setting Timer 0 to CTC mode
  TCCR0A = (1<<WGM01);
  //setting prescaler to f_CPU/64
  TCCR0B = (1<<CS01) | (1<<CS00);
  //Output Compare Interrupt Enable
  TIMSK0 = _BV(OCIE0A);
  //setting TOP to 8
  OCR0A = 8;




}

/*
 * The output compare match event for Timer 0.
 * This is the heart of the display routine. It is triggered every time Timer 0
 * hits OCR0A as upper limit.
 * The timer interrupt routine does the following:
 * - it renders the next row and increases the row counter
 * - if needed it switches the display buffer
 * - it enables all the interrupts again
 */
ISR(TIMER0_COMPA_vect )
{
  //we don't need to disable interrupts by ourself, because
  //inside ISRs interrupts are disabled by default

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

  //neither do we need to enable interrupts, as they will be
  //automagically be enabled when returning from the ISR
}
