/*
 * main.cpp
 *
 */
#include "config.hpp"       // this file must be included as first one!

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <util/delay.h>     // <avr/delay.h> once
#include <avr/sleep.h>

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
  // TODO: enable usart
  //USART::init(ct);  // prepare USART to work
  Timer0      t0;   // configure T0
  Timer1      t1;   // configure T1
  sei();            // allow interrupts globally

#if 1
  uint16_t tmp=8000;              
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

    // TODO
    tmp+=1000;                             
    ct.currentPos().set(3, tmp);        
  }
#endif

#if 0
  while(1)
  {
    PORTB=0xFF;
    _delay_ms(2);
    PORTB=0x00;
    _delay_ms(18);
  }
#endif

#if 0
  while(1)
  {
    t0.clearInterruptFlag();
    PORTB=0xFF;
    _delay_ms(2);
    PORTB=0x00;
    while( !t0.interruptCame() )
      PowerSave::idle();
  }
#endif

#if 0
  while(1)
  {
    for(int i=0; i<2; ++i)
      for(int j=0; j<4; ++j)
        _delay_ms(250);
    PORTB^=0xFF;
  }
#endif

  // TODO

  while(1);
  return 0;
} // main()

