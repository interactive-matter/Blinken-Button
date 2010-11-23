/*
 * random.c
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
#include <stdlib.h>
#include <avr/io.h>
#include <avr/power.h>

#include "random.h"

//channel 7 is unused and left floating - it should get enough EMI ;)
#define ADC_CHANNEL 14

//to randomize zhe seed we read signals from the ADC and use the noise as seed
void
randomize_seed(void)
{
  unsigned int seed = 0;
  //switch on adc
  power_adc_enable();

  //set the channel as input
  DDRC &= ~_BV(7);


  ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // activate adc
  //use a low prescaler for maximum speed and error

  ADMUX = ADC_CHANNEL; // Kanal waehlen
  ADMUX |= _BV(6); //use vcc as voltage reference

  ADCSRA |= (1 << ADEN);//enable ADC with dummy conversion

  //we stupidly perform 16 measurements and always use the incorrect last bit
  uint8_t i;
  for (i = 0; i < 16; i++)
    {

      ADCSRA |= (1 << ADSC); // eine ADC-Wandlung
      while (ADCSRA & (1 << ADSC))
        {
          ; // auf Abschluss der Konvertierung warten
        }
      seed |= (ADCW & 1);
      seed <<= 1;
    }

  ADCSRA &= (unsigned char) ~((1 << ADEN)); // ADC deaktivieren (2)
  //switch off adc
  power_adc_disable();

  set_seed(seed);

}

void
set_seed(unsigned int seed)
{
  srand(seed);
}

unsigned int
get_random(unsigned int upper_bound)
{
  /* This routine is supposed to be better
  unsigned int x = (RAND_MAX + 1u) / upper_bound;
  unsigned int y = x * upper_bound;
  unsigned int r;
  do
    {
      r = rand();
    }
  while (r >= y);
  return r / x;
  */
  //but we still stick to the dumb implementation
  return rand() % upper_bound;

}
