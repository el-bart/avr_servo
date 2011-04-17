/*
 * main.cpp
 *
 */
#include "config.hpp"       // this file must be included as first one!

#include <avr/io.h>
#include <avr/interrupt.h>

#include "USART.hpp"
#include "Timer0.hpp"
#include "Timer1.hpp"
#include "PowerSave.hpp"
#include "ChronoTable.hpp"
#include "uassert.hpp"


// handle unknown interruption(s)
ISR(BADISR_vect)
{
  uassert(!"unhandled interrupt");
}


//
// MAIN
//
int main(void)
{
  DDRB|=0xFF;       // PB operates as output
  ChronoTable ct;   // table of computed positions to use
  USART::init(ct);  // prepare USART to work
  Timer1      t1;   // configure T1
  Timer0      t0;   // configure T0
  sei();            // allow interrupts globally

  while(1)
  {
    // initial preparations
    t0.clearInterruptFlag();
    t1.start();
    // start cycle
    PORTB=0xFF;
    // end cycles high-cycles for servos in proper order
    for(uint8_t i=0; i<8; ++i)
    {
      const ChronoTable::Entry e=ct.get(i); // get copy for faster access
      while( t1.get()<e.time_ );            // wait until to be triggered
      PORTB&=e.mask_;                       // apply mask, as requested
    }
    // end high-value
    PORTB=0x00;
    t1.stop();
    // now wait until the end of the period
    while( !t0.interruptCame() )
      PowerSave::idle();
    // compute new servo positions table
    ct.compute();
  }

  // code never reaches here
  return 0;
} // main()

