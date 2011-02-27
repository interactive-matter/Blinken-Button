/*
 * rendering.h
 *
 * This file contains all the stuff needed to animate the animations and
 * display the text. here are the calculations and update
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
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

//initialization routine
void animation_init(void);
//render text
void animation_display_message(char* message);
//animate a sequence of sprites (images) to form an animation
void animation_set_sequence(int8_t start, int8_t end, uint8_t speed);
//the routine for the animation timer to change animations & display texts
void animation_switch_sprite(void);
//the routine to switch between different animations & texts - used by the update timer
void aimation_update(void);


#endif /* ANIMATION_H_ */
