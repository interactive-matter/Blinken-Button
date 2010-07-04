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
 *  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *  Created on: 26.01.2010
 */
//sometimes AVR Eclipse gets the frequency wrong
#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/power.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "core/state.h"
#include "core/animation.h"
#include "core/display.h"

//display test pattern
//#define TEST_MODE

void
animation_start_update_timer(void);



int
main(void)
{

  display_prog_led_enable();
  _delay_ms(500);
  //switch anything off Ð we will switch on needed components later manually
  power_all_disable();
  //now start the animations
#ifndef TEST_MODE
  animation_init();
#else
  display_init(0);
#endif

  //and now start everything by enabling interrupts
   sei();

  display_prog_led_disable();

   for( ; ;)
    {
     //if the renderer needs some time to do some calculations
      state_process();
    }
}

