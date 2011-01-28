/*
 * state.c
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
#include <stdio.h>
#include <avr/sfr_defs.h>

#include "state.h"

register uint8_t state asm("r4");
uint8_t status_step = 0;
uint8_t registered_tasks = 0;

state_callback state_callbacks[8] =
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

uint8_t
state_register_task(state_callback callback)
{
  if ((registered_tasks < 8) && (callback != NULL))
    {
      state_callbacks[registered_tasks] = callback;
      uint8_t result = _BV(registered_tasks);
      state_deactivate(result);
      registered_tasks++;
      return result;
    }
  else
    {
      return 0xff;
    }
}

uint8_t
state_register_state(void)
{
  if (registered_tasks < 7)
    {
      uint8_t result = _BV(registered_tasks);
      state_deactivate(result);
      registered_tasks++;
      return result;
    }
  else
    {
      return 0xff;
    }
}

void
state_activate(uint8_t state_number)
{
  state |= state_number;
}

void
state_deactivate(uint8_t state_number)
{
  state &= ~(state_number);
}

uint8_t
state_is_active(uint8_t state_number)
{
  return state & state_number;
}

void
state_process(void)
{
  //advance one task and ensure that you zero after 7
  status_step = (status_step + 1) & 7;
  if ((state & _BV(status_step)) && (state_callbacks[status_step] != NULL))
    {
      //we can deactivate the state - perhaps it wants to be activated later
      state_deactivate(_BV(status_step));
      //and call the function of the state
      state_callbacks[status_step]();
    }
}
