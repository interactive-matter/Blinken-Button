/*
 * font.h
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
 */
#include <avr/io.h>
#include <avr/pgmspace.h>

#ifndef FONT_H_
#define CORE_FLASH_CONTENT_H_

/*
 * copy_to_buffer
 * Copies the given sprite from PROGMEM to RAM.
 */
#define copy_to_buffer(sprite, buffer) memcpy_P(buffer, sprite, 8)


extern const prog_uint8_t default_sprites[][8];

#define MAX_CHARS 59
#define CHAR_OFFSET 0x20

extern const prog_uint8_t font[];
#endif /* FONT_H_ */
