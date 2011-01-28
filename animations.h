/*
 * animations.h
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

#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_

#include <avr/pgmspace.h>

//how often should we display messages
#define MESSAGE_PROBABILITY 10
//how many chars has the longest message
#define MAX_MESSAGE_LENGTH 40
//how much messages do we have in total (see message_00 _01 ...)
#define MAX_MESSAGE 3
//and now to the messages
const prog_char message_00[] = "INTERACTIVE-MATTER.ORG";
const prog_char message_01[] = "SPACE INVADERS BUTTON";
const prog_char message_02[] = "SPACE INVADERS AGAINST RACISM";
//if you define new messages remember to add it here
const PGM_P PROGMEM messages[] =
  { message_00, message_01, message_02 };
//a buffer for loading messages from flash
char message[MAX_MESSAGE_LENGTH];

//our animations
//first number is # sprites
const prog_uint8_t sprite_0[] =
  { 2, 0, 1 };
const prog_uint8_t sprite_1[] =
  { 2, 2, 3 };
const prog_uint8_t sprite_2[] =
  { 2, 4, 5 };
const prog_uint8_t sprite_3[] =
  { 8, 14, 15, 16, 11, 11, 16, 15, 14 };
const prog_uint8_t sprite_4[] =
  { 7, 8, 9, 10, 11, 10, 9, 8 };
//a sequence is a animation + display speed an length
typedef struct
{
  uint8_t display_speed;
  uint8_t display_length;
  const prog_uint8_t* sprites;
} _sequence_struct;

//how many sequences do we have?
#define MAX_SEQUENCE 6
/*the definition of the sequences as array of _sequence_struct (see above):
 * First the display speed (lower numbers are faster since it is more a wait timer.
 * Second the length, how long the animation is shown.
 * Third the animation.
 */
const _sequence_struct sequences[] PROGMEM =
  {
  //the first two sprites are defined twice - to ensure that they are shown more often
        { 14, 20, sprite_0 },
        { 14, 20, sprite_1 },
        { 14, 20, sprite_0 },
        { 14, 20, sprite_1 },
        { 3, 5, sprite_3 },
        { 3, 5, sprite_4 } };

#endif /* ANIMATIONS_H_ */
