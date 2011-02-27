/*
 * random.c
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
//include the definitions for our chip, like pins, ports & so on
#include <stdio.h>

//include our own definitions
#include "random.h"

static uint32_t RandomSeedA = 65537;
static uint32_t RandomSeedB = 12345;


/*
 * To randomize the seed we simply read add up all memory content.
 * To generate a god randomness it is useful to do this as early as possible
 * in the startup.
 */
void
randomize_seed(void)
{
 	uint16_t *addr = 0;
	for (addr = 0; addr < (uint16_t*)0xFFFF; addr++)
		RandomSeedB += (*addr);
}

/*
 * This generates a new random number of maximum 'max'
 */
unsigned int
get_random(unsigned int max)
{
  	RandomSeedA = 36969 * (RandomSeedA & 65535) + (RandomSeedA >> 16);
	RandomSeedB = 18000 * (RandomSeedB & 65535) + (RandomSeedB >> 16);
 	return ((RandomSeedA << 16) + RandomSeedB) % max;
}

