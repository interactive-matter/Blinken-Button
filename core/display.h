/*
 * display.h
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

#ifndef DISPLAY_H_
#define DISPLAY_H_

//Enable this if you use PNP transistors - disable else
#define PNP_TRANSISTOR

//sprite display
void display_init();

//display the next sprite
void display_advance_buffer(void);

void display_render(void);

//render a character
void
display_load_sprite(uint8_t origin[]);

//two little helpr routines to control the prog led on the back
void display_prog_led_enable(void);
void display_prog_led_disable(void);

#endif /* DISPLAY_H_ */
