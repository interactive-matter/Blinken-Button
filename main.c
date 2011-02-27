/*
 * main.c
 *
 *  http://interactive-matter.eu/
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
 * Blinken Buttons for Beginners is an implementation of a animated
 * 8x8 Led Matrix. It is capable of displaying animations and text messages.
 * The images for the animations, the sequences of images building the
 * animations, the texts to display and the font to render the texts to images
 * is stored in the flash memory.
 *
 * You will find the following source files in this package:
 * main.c - the main program, you are already there
 * Makefile - a custom make file to compile the Blinken Button and install it
 *            with your AVR programmer, check if the programmer settings are
 *            appropriate for your computer.
 * custom-flash-content.c/.h - the definition of the various images, animations
 *                             & texts for the Blinken Button
 * core-flash-content.c/.h - animations you probably don't want to change,
 *                           the font to render the text.
 * rendering.c/.h - all the stuff needed to animate the animations and display
 *                  the text. here are the calculations and update routines.
 *                  The display driver itself is in the next file.
 * display.c/.h - routines to display images on the display. The low level
 *                display driver stuff like going through rows, mapping images
 *                to output pins & the display buffer.
 * random.c/.h - small random routine to state with a new animation every time
 *               the Blinken Button is switched on. And to randomly sequence
 *               animations and texts.
 * state.c/.h - a small helper routine to remember what needs to be done or is
 *              going on in order to do the right thing at the right time.
 *
 * If you want to tinker with the animations, images and texts have a look in
 * the file 'custom-flash-content.c'. There you can change or create new text
 * messages, new animations or define new images for the animations.
 *
 * If you want to understand how text and images are rendered to proper
 * animations just check the file 'rendering.c'. There you will find all the
 * magic to select texts and messages and decide when to display what.
 *
 * If you want to see how the images that are calculated in 'rendering.c' are
 * displayed check 'display.c'. There you will find a double buffer, routines
 * to write to the buffers and routines to display the buffers on the LED matrix
 *
 * You can safely ignore the files 'state.c' and 'random.c'
 * - In 'state.c' is a tiny state machine with some parallel states that can be
 *   defined programatically. It is used to coordinate the different activities
 * - In 'random.c' there are just some routines to get better random values than
 *   the default AVR routines can provide.
 *
 * The whole program is controlled using timers:
 * - Timer 0 is responsible for painting the images on the display Ð hence it
 *   is called the 'Display Timer'. The timer controls the rendering of the
 *   single rows of the display. It cycles through row 0 to 7, switches the
 *   row on and enables the correct LEDs. By cycling through this sequence really
 *   fast the human eye assembles all those rows to a complete image
 * - Timer 2 is responsible for building animations from single images.
 *   It controls the timing when the displayed image is switched and when a new
 *   image needs to be loaded.
 *
 * The main loop is implemented using so called states. Since the main loop is completely
 * controlled  by Timer 2 it has a 'state' which is activated when a new image is displayed
 * an the net animation needs to be loaded.
 * Therefore you will only find the cryptic 'process_state' in the main routine.
 */

//sometimes AVR Eclipse gets the frequency wrong, so I need t set it here fixed
#define F_CPU 8000000

//include the definitions for our chip, like pins, ports & so on
#include <avr/io.h>
//we power up & down chip components as needed, here are the functions to do this
#include <avr/power.h>
//we are using interrupts & timers as schedule - here we have the def. of the
//interrupt routines and names
#include <avr/interrupt.h>

// state manages the triggering of calculations
// according to Timer 2
#include "state.h"
// animations.c contains all routines for rendering the animations from single images
#include "rendering.h"
// display.c is responsible for rendering the images on the display.
#include "display.h"

/*
 * This is the main routine. The main routine gets executed when the ATmega powers up.
 * It initializes all components and keeps them running, by checking if one of the state
 * routines needs to be executed.
 */
int
main(void)
{
  /*
   * Even though the LEDs waste a lot of power it is good practice to use to disable all
   * CPU components first and power up only those components we need later.
   * So here we switch anything of like UART, ADC, timers and so on.
   */
  power_all_disable();
  //now start the animations
  animation_init();

  /*
   * now we have initialized all components and can now switch to reactive mode.
   * We enable all interrupts globally. By that the selected interrupts start
   * triggering the interrupt routines.
   * The interrupts are needed to schedule our program activities according to
   * the timers - for which we are using timer overflow interrupts.
   */
  sei();

  /*
   * Now we can switch to the main loop and loop until the power is switched off
   */
  for (;;)
    {
      /*
       * by state_process we check if a new image has to be loaded and call the load routine
       */
      state_process();
    }
}

ISR(TIMER0_COMPA_vect )
{
  display_render_row();
}

ISR (TIMER1_OVF_vect)
{
  aimation_update();
}

ISR(TIMER2_OVF_vect)
{
  animation_switch_sprite();
}
