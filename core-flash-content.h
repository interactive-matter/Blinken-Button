/*
 * core-flash-content.h
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

#ifndef FONT_H_
#define CORE_FLASH_CONTENT_H_

/*
 * copy_to_buffer
 * Copies the given sprite from PROGMEM to RAM.
 */
#define copy_to_buffer(sprite, buffer) memcpy_P(buffer, sprite, 8)

/*
 * The default sprites which are always present as fallback
 */
extern const prog_uint8_t default_sprites[][8];
//where does the first character of the font start. The ASCII number - CHAR_OFFSET
//is the index in the font
#define CHAR_OFFSET 0x20

extern const prog_uint8_t font[];
#endif /* FONT_H_ */
