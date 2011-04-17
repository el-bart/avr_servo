/*
 * Timer0.cpp
 *
 */
#include "config.hpp"

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "Timer0.hpp"
#include "uassert.hpp"

namespace
{
uint8_t       g_initCount=0;
volatile bool g_int=false;
} // unnamed namespace


// timer 0 interrupt
//ISR(TIMER0_OVF_vect)
ISR(TIMER0_COMPA_vect)
{
  g_int=true;
}


Timer0::Timer0(void)
{
  // set timer 0
  TCNT0  =0;            // BOTTOM: start counting from 0
  OCR0A  =78;           // TOP: aprox. of f*ms/presk, i.e. (8*10^6)*(10/1000)/1024
  TCCR0A&=~_BV(COM0A1); // disconnect OC0A
  TCCR0A&=~_BV(COM0A0); // ...
  TCCR0A&=~_BV(COM0B1); // disconnect OC0B
  TCCR0A&=~_BV(COM0B0); // ...
  TCCR0B&=~_BV(WGM02);  // work in CTC mode
  TCCR0A|= _BV(WGM01);  // ...
  TCCR0A&=~_BV(WGM00);  // ...
  TCCR0B&=~_BV(FOC0A);  // disable force output compare A
  TCCR0B&=~_BV(FOC0B);  // disable force output compare B
  TCCR0B|= _BV(CS02);   // clk/1024
  TCCR0B&=~_BV(CS01);   // ...
  TCCR0B|= _BV(CS00);   // ...
  TIMSK |= _BV(OCIE0A); // enable interrupt on timer 0 CTC

  ++g_initCount;
  uassert( g_initCount==1 );
}

Timer0::~Timer0(void)
{
  --g_initCount;
}

void Timer0::clearInterruptFlag(void)
{
  g_int=false;
}

bool Timer0::interruptCame(void) const
{
  return g_int;
}
