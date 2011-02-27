/*
 * custom-flash-content.c
 *
 * This file contains the definition of the various images, animations
 * & texts for the Blinken Button
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
//since we define stuff for the flash memory we need the routines and definitions
//for the flash
#include <avr/pgmspace.h>

#include "custom-flash-content.h"

//how often should we display messages - bigger values mean
//less probability to display a message
const uint8_t message_probability = 10;
//how many chars has the longest message
#define MAX_MESSAGE_LENGTH 40
//a buffer for loading messages from flash
char message[MAX_MESSAGE_LENGTH];

/*
 * Here you can define the messages. If you want to have more than three
 * messages just add another message_0x. But remember to also increase the
 * total number of messages in max_messages and add it to the array messages.
 */
const prog_char message_00[] = "INTERACTIVE-MATTER.ORG";
const prog_char message_01[] = "SPACE INVADERS BUTTON";
const prog_char message_02[] = "SPACE INVADERS AGAINST RACISM";
//how much messages do we have in total (see message_00 _01 ...)
const uint8_t max_messages = 3;
//if you define new messages remember to add it here
const PGM_P PROGMEM messages[] =
  { message_00, message_01, message_02 };


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

//how many sequences do we have?
const uint8_t max_sequence = 6;
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

/*
 * This is the definition of sprites that are usable. The bit pattern on the
 * display is directly converted to a number and here defined as a number (hex)
 */
const prog_uint8_t predefined_sprites[][8] = {
  {
    0x18,    // ___XX___ 0
    0x3C,    // __XXXX__
    0x7E,    // _XXXXXX_
    0xDB,    // X_XXXX_X
    0xFF,    // XXXXXXXX
    0x24,    // __X__X__
    0x5A,    // _X_XX_X_
    0xA5     // X_X__X_X
  },
  {
    0x18,    // ___XX___  1
    0x3C,    // __XXXX__
    0x7E,    // _XXXXXX_
    0xDB,    // X_XXXX_X
    0xFF,    // XXXXXXXX
    0x24,    // __X__X__
    0x42,    // _X____X_
    0x24     // __X__X__
  },
  {
    0x24,    // __X__X__  2
    0x7E,    // _XXXXXX_
    0xDB,    // XX_XX_XX
    0xFF,    // XXXXXXXX
    0xA5,    // X_X__X_X
    0x99,    // X__XX__X
    0x81,    // X______X
    0xC3     // XX____XX
  },
  {
    0x24,    // __X__X__  3
    0x18,    // ___XX___
    0x7E,    // X_XXXX_X
    0xDB,    // XX_XX_XX
    0xFF,    // XXXXXXXX
    0xDB,    // X_XXXX_X
    0x99,    // X__XX__X
    0xC3     // XX____XX
  },
    {
      0x00,    // ________ 4
      0x00,    // ________
      0x14,    // ___X_X__
      0x3E,    // __XXXXX_
      0x3E,    // __XXXXX_
      0x1C,    // ___XXX__
      0x08,    // ____X___
      0x00     // ________
    },
    {
      0x00,    // ________  5
      0x66,    // _XX__XX_
      0xFF,    // XXXXXXXX
      0xFF,    // XXXXXXXX
      0xFF,    // XXXXXXXX
      0x7E,    // _XXXXXX_
      0x3C,    // __XXXX__
      0x18     // ___XX___
    },
  {
    0xCC,    // XX__XX__  6
    0xCC,    // XX__XX__
    0x33,    // __XX__XX
    0x33,    // __XX__XX
    0xCC,    // XX__XX__
    0xCC,    // XX__XX__
    0x33,    // __XX__XX
    0x33     // __XX__XX
  },
  {
    0x33,    // __XX__XX  7
    0x33,    // __XX__XX
    0xCC,    // XX__XX__
    0xCC,    // XX__XX__
    0x33,    // __XX__XX
    0x33,    // __XX__XX
    0xCC,    // XX__XX__
    0xCC     // XX__XX__
  },
  {
    0x00,    // ________  8
    0x00,    // ________
    0x00,    // ________
    0x18,    // ___XX___
    0x18,    // ___XX___
    0x00,    // ________
    0x00,    // ________
    0x00     // ________
  },
  {
    0x00,    // ________  9
    0x00,    // ________
    0x3C,    // __XXXX__
    0x3C,    // __XXXX__
    0x3C,    // __XXXX__
    0x3C,    // __XXXX__
    0x00,    // ________
    0x00     // ________
  },
  {
    0x00,    // ________  10
    0x7E,    // _XXXXXX_
    0x7E,    // _XXXXXX_
    0x7E,    // _XXXXXX_
    0x7E,    // _XXXXXX_
    0x7E,    // _XXXXXX_
    0x7E,    // _XXXXXX_
    0x00     // ________
  },
  {
    0xFF,    // XXXXXXXX  11
    0xFF,    // XXXXXXXX
    0xFF,    // XXXXXXXX
    0xFF,    // XXXXXXXX
    0xFF,    // XXXXXXXX
    0xFF,    // XXXXXXXX
    0xFF,    // XXXXXXXX
    0xFF     // XXXXXXXX
  },
  {
    0x0F,    // ____XXXX  12
    0x0F,    // ____XXXX
    0x0F,    // ____XXXX
    0x0F,    // ____XXXX
    0xF0,    // XXXX____
    0xF0,    // XXXX____
    0xF0,    // XXXX____
    0xF0,    // XXXX____
  },
  {
    0xF0,    // XXXX____  13
    0xF0,    // XXXX____
    0xF0,    // XXXX____
    0xF0,    // XXXX____
    0x0F,    // ____XXXX
    0x0F,    // ____XXXX
    0x0F,    // ____XXXX
    0x0F,    // ____XXXX
  },
  {
    0xFF,    // XXXXXXXX  14
    0x00,    // ________
    0x00,    // ________
    0x00,    // ________
    0x00,    // ________
    0x00,    // ________
    0x00,    // ________
    0xFF,    // XXXXXXXX
  },
  {
    0xFF,    // XXXXXXXX  15
    0xFF,    // XXXXXXXX
    0x00,    // ________
    0x00,    // ________
    0x00,    // ________
    0x00,    // ________
    0xFF,    // XXXXXXXX
    0xFF,    // XXXXXXXX
  },
  {
    0xFF,    // XXXXXXXX  16
    0xFF,    // XXXXXXXX
    0xFF,    // XXXXXXXX
    0x00,    // ________
    0x00,    // ________
    0xFF,    // XXXXXXXX
    0xFF,    // XXXXXXXX
    0xFF,    // XXXXXXXX
  },
  {
    0x10,    // ___X____  17
    0x10,    // ___X____
    0x10,    // ___X____
    0x1F,    // ___XXXXX
    0xF8,    // XXXXX___
    0x08,    // ____X___
    0x08,    // ____X___
    0x08,    // ____X___
  },
  {
    0x70,    // _XXX____  18
    0x31,    // __XX___X
    0x13,    // ___X__XX
    0x1F,    // ___XXXXX
    0xF8,    // XXXXX___
    0xC8,    // XX__X___
    0x8C,    // X___XX__
    0x0E,    // ____XXX_
  },
  {
    0xF1,    // XXXX___X  19
    0x73,    // _XXX__XX
    0x37,    // __XX_XXX
    0x1F,    // ___XXXXX
    0xF8,    // XXXXX___
    0xEC,    // XXX_XX__
    0xCE,    // XX__XXX_
    0x8F,    // X___XXXX
  },
  {
    0x04,    // _____X__  20
    0x00,    // ________
    0x80,    // X_______
    0x00,    // ________
    0x00,    // ________
    0x01,    // _______X
    0x00,    // ________
    0x20,    // __X_____
  },
  {
    0x0E,    // ____XXX_  21
    0x84,    // X____X__
    0xC0,    // XX______
    0x80,    // X_______
    0x01,    // _______X
    0x03,    // ______XX
    0x21,    // __X____X
    0x70,    // _XXX____
  },
  {
    0x9F,    // X__XXXXX  22
    0xCE,    // XX__XXX_
    0xE4,    // XXX__X__
    0xC1,    // XX_____X
    0x83,    // X_____XX
    0x27,    // __X__XXX
    0x73,    // _XXX__XX
    0xF9,    // XXXXX__X
  }
};
