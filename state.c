/*
 * state.c
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
//we need some special register and tool definitions
#include <avr/sfr_defs.h>

#include "state.h"

/*
 * Here are the bits stored to indicate that a task or state is active.
 * To access the state variable faster we permanently bind it to a register
 * see http://www.nongnu.org/avr-libc/user-manual/FAQ.html#faq_regbind
 */
register uint8_t state asm("r4");
//with each time state_process is called we just check one task if it needs
//calculations to return faster, e.g. to do other things in the main routine
//here is the number of the task stored that is checked.
uint8_t status_step = 0;
//how much states & tasks are registers - this defines the handle for the
//next task or state that is registered
uint8_t registered_tasks = 0;

//an array to store all callbacks
state_callback state_callbacks[8] =
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

// register a task
uint8_t
state_register_task(state_callback callback)
{
  //we do our work only if all input parameters are correct
  if ((registered_tasks < 8) && (callback != NULL))
    {
      //store the callback handle
      state_callbacks[registered_tasks] = callback;
      //the result is the bit pattern to check the state & task register
      uint8_t result = _BV(registered_tasks);
      //to be really sure the the corresponding bit to 0
      state_deactivate(result);
      //we have a task more - so the next one is this task number plus 1
      registered_tasks++;
      //and return the handle to the task
      return result;
    }
  //if there was something wrong return an improbable number to indicate the error
  else
    {
      return 0xff;
    }
}

//register a state which can be either active or not active
uint8_t
state_register_state(void)
{
  //we do our work only if all input parameters are correct
  if (registered_tasks < 8)
    {
      //the result is the bit pattern to check the state & task register
      uint8_t result = _BV(registered_tasks);
      //to be really sure the the corresponding bit to 0
      state_deactivate(result);
      //we have a state more - so the next one is this task number plus 1
      registered_tasks++;
      //and return the handle to the task
      return result;
    }
  //if there was something wrong return an improbable number to indicate the error
  else
    {
      return 0xff;
    }
}

//activate the state bit for the given state or task
void
state_activate(uint8_t state_number)
{
  state |= state_number;
}

//deactivate the state bit for the given state or task
void
state_deactivate(uint8_t state_number)
{
  state &= ~(state_number);
}

//check if the state bit is set for the given state or task
uint8_t
state_is_active(uint8_t state_number)
{
  return state & state_number;
}

//look for a active task and call its callbak
void
state_process(void)
{
  //advance one task and ensure that you zero after 7
  status_step = (status_step + 1) & 7;
  //if the corresponding task is active and we now a callback for this
  //(i.e. it is no state) we call the callback
  if ((state & _BV(status_step)) && (state_callbacks[status_step] != NULL))
    {
      //we deactivate the task
      state_deactivate(_BV(status_step));
      //and call the callback of the task
      state_callbacks[status_step]();
    }
}
