#include "config.hpp"
#include <avr/interrupt.h>

#include "Timer2.hpp"
#include "ScopedIntLock.hpp"


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
  // start counting every 0.5ms, that is every 5529.6 machine cycles,
  // that is every 250 timer cycles, when prescaller is set to 8:
  //   c=f/1000/2 -> 5529.6
  //   o=f/8/250  -> 5529.6
  OCR2  =250;           // set TOP to 250
  TCCR2|=_BV(WGM21);    // set CTC mode
  TCCR2|=_BV(CS21);     // set preskaler to 8
  // enable interrrupts from timer2
  TIFR |=_BV(OCF2);
  TIMSK|=_BV(OCIE2);
}

uint8_t Timer2::currentStep(void) const
{
  ScopedIntLock lock;
  return g_stepCounter;
}

void Timer2::resetStep(void)
{
  ScopedIntLock lock;
  g_stepCounter=0;
}
