/*
 * sprites.h
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

#ifndef CUSTOM_FLASH_CONTENT_H_
#define CUSTOM_FLASH_CONTENT_H_

//a sequence is a animation + display speed an length
typedef struct
{
  uint8_t display_speed;
  uint8_t display_length;
  const prog_uint8_t* sprites;
} _sequence_struct;

//a buffer for loading messages from flash
extern char message[];
//how much messages do we have in total
extern const uint8_t max_messages;
//the messages
extern const PGM_P PROGMEM messages[];
//how often should we display messages
extern const uint8_t message_probability;

//how many sequences do we have?
extern const uint8_t max_sequence;

extern const _sequence_struct sequences[] PROGMEM;


//the sprites defined in custom-flash-content.c
extern const prog_uint8_t predefined_sprites[][8];

#endif /* CUSTOM_FLASH_CONTENT_H_ */
