/*
 * main.c
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
 * Blinken Buttons for Beginners is an implementation of a animated
 * 8x8 Led Matrix. It is capable of displaying animations and text messages.
 * The images for the animations, the sequences of images building the
 * animations, the texts to display and the font to render the texts to images
 * is stored in the flash memory.
 *
 * The wohle program is cotrolled using timers:
 * - Timer 0 is responsible for painting the images on the display Ð hence it
 *   is called the 'Display Timer'. The timer controls the rendering of the
 *   single rows of the display. It cycles thorugh row 0 to 7, switches the
 *   row on and enables the correct Leds. By cycling thrugh this sequence really
 *   fast the human eye assembles all those rows to a complete image
 * - Timer 2 is responsible for building animations from single images.
 *   It controls the timing when the displayed image is switched and when a new
 *   image needs to be loaded.
 *
 * The main loop is implemented using so called states. Since the main loop is completely
 * controlled  by Timer 2 it has a 'state' which is activated when a new image is displayed
 * an the net animation needs to be loaded.
 * Therfore you will only find the cryptic 'process_state' in the main reoutine.
 */


/*
 * sometimes AVR Eclipse gets the frequency wrong, so I need t set it here fixed
 */
#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/power.h>
#include <stdlib.h>
#include <avr/interrupt.h>

// state manages the triggering of calculations
// according to Timer 2
#include "core/state.h"
// animations.c contains all routines for rendering the animations from single images
#include "rendering.h"
// display.c is responsible for rendering the images on the display.
#include "core/display.h"

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
   * So here we switch anything of
   */
  power_all_disable();
  //now start the animations
  animation_init();

  /*
   * now we have initialized all components and can now switch to reactive mode.
   * We enable all interrupts globally. By that the selected interrupts start
   * triggering the interrupt routines.
   */
   sei();

   /*
    * Now we can switch to the main loop and loop until the power is switched off
    */
   for( ; ;)
    {
     /*
      * by state_process we check if a new image has to be loaded and call the load routine
      */
      state_process();
    }
}

