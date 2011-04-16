/*
 * main.cpp
 *
 */
#include "config.hpp"       // this file must be included as first one!

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>     // <avr/delay.h> once

#include "Queue.hpp"
#include "USART.hpp"
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
  USART::init();
  DDRB|=0xFF;

  while(1)
  {
    for(int i=0; i<2; ++i)
      for(int j=0; j<4; ++j)
        _delay_ms(250);
    PORTB^=0xFF;
  }

  // TODO
  return 0;
} // main()

