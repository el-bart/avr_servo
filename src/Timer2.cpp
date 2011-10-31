#include "Timer2.hpp"
#include <avr/interrupt.h>

namespace
{
volatile uint8_t g_stepCounter;
} // unnamed namespace


// interruption called each 1ms
ISR(TIMER2_COMP_vect)
{
  ++g_stepCounter;
}


Timer2::Timer2(void)
{
  resetStep();
  // initial value for counter
  TCNT2 =0;
  // start counting every 1ms, that is every 8000 machine cycles,
  // that is every 125 timer cycles, when prescaller is set to 64.
  OCR2  =125;           // set TOP to 125
  TCCR2|=_BV(WGM21);    // set CTC mode
  TCCR2|=_BV(CS22);     // set preskaler to 64
  // enable interrrupts from timer2
  TIFR |=_BV(OCF2);
  TIMSK|=_BV(OCIE2);
}

uint8_t Timer2::currentStep(void)
{
  return g_stepCounter;
}

void Timer2::resetStep(void)
{
  g_stepCounter=0;
}
