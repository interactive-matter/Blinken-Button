/*
 * random.h
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

#ifndef RANDOM_H_
#define RANDOM_H_

/*
 * To randomize the seed we simply read add up all memory content.
 * To generate a god randomness it is useful to do this as early as possible
 * in the startup.
 */
void randomize_seed(void);

/*
 * This generates a new random number of maximum 'max'
 */
unsigned int get_random(unsigned int max);

#endif /* RANDOM_H_ */
