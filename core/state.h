/*
 * state.h
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

#ifndef STATE_H_
#define STATE_H_

typedef void(*state_callback)(void);

uint8_t state_register_task(state_callback callback);
uint8_t state_register_state(void);
void state_process(void);
uint8_t state_is_active(uint8_t state_number);
void state_activate(uint8_t state_number);
void state_deactivate(uint8_t state_number);

#endif /* STATE_H_ */
