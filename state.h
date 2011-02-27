/*
 * state.h
 *
 * This file contains small helper routine to remember what needs to be done or is
 * going on in order to do the right thing at the right time
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

#ifndef STATE_H_
#define STATE_H_

/**
 * States are used to remember what to do or trigger certain activities.
 * There are two different type of states:
 * - states can either be on or off. This is useful to remember what is going
 *          on. E.g. if we are displaying text or an animations.
 * - tasks  are the same thing as states but have an associated function which
 *          is called - after that the task is marked as done, and inactive.
 *          This is e.g. useful if you have a timer which requires a calculation
 *          until the next timeout happens. The function of the taks is called
 *          'callback'.
 * You can only have 8 states & tasks in sum. The status of each state & task
 * is stored in a register variable which has only 8 bits.
 */

//this is the definition of a callback
//simply a function without parameters and return like:
//void callbak(void);
typedef void(*state_callback)(void);

/*
 * The state & task register routines return an identifier for the state or task
 * keep this value in a variable since it is used to identify the state or task.
 */

// register a task with it's callback
uint8_t state_register_task(state_callback callback);
// register a state
uint8_t state_register_state(void);

// process the states. This is needed to check which task is active and
//call the corresponding callback routine. It is a good idea to do this
//as often as possible in the main routine
void state_process(void);
//check is a certain state or task is active
uint8_t state_is_active(uint8_t state_number);
//activate a certain state or task
void state_activate(uint8_t state_number);
//or deactivate a certain state & task
void state_deactivate(uint8_t state_number);

#endif /* STATE_H_ */
