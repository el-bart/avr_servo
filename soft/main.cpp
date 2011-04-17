/*
 * main.cpp
 *
 */
#include "config.hpp"       // this file must be included as first one!

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>     // <avr/delay.h> once
#include <avr/sleep.h>

#include "Queue.hpp"
#include "USART.hpp"
#include "Timer0.hpp"
#include "PowerSave.hpp"
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
  USART::init();    // prepare USART to work
  DDRB|=0xFF;       // PB operates as output
  Timer0 t0;        // configure T0
  sei();            // allow interrupts globally

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

