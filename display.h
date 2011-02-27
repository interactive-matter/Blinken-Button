/*
 * display.h
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

#ifndef DISPLAY_H_
#define DISPLAY_H_

//sprite display
void display_init(void);

//render a sprite (or image)
void
display_load_sprite(uint8_t origin[]);

//display the next sprite
void display_advance_buffer(void);

#endif /* DISPLAY_H_ */
