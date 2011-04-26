/*
 * main.cpp
 *
 */
#include "config.hpp"       // this file must be included as first one!

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

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

volatile int bytes_=0;      

//
// MAIN
//
int main(void)
{
  DDRB|=0xFF;       // PB operates as output
  //ChronoTable ct;   // table of computed positions to use
  //USART::init(ct);  // prepare USART to work
  //Timer1      t1;   // configure T1
  //Timer0      t0;   // configure T0
  //sei();            // allow interrupts globally

#if 1
#define USART_UBRR(baud,f) ( ((f)/((baud)*16L)) -1 )

  // clock divider register (computed from baud rate and oscilator frequency)
  UBRRH=(uint8_t)( (USART_UBRR(USART_BAUD, F_CPU)>>8) & 0x00FF );
  UBRRL=(uint8_t)( (USART_UBRR(USART_BAUD, F_CPU)>>0) & 0x00FF );

  // enable transciever
  UCSRB|= _BV(RXEN);    // RX enable
  UCSRB|= _BV(TXEN);    // TX enable

  // configure proper pins as in (RX) and out (TX)
  DDRD &=~_BV(PD0);     // RX as in
  PORTD|= _BV(PD0);     // RX high with pull-up
  DDRD |= _BV(PD1);     // TX as out

  while(true)
  {
    // read
    while( !(UCSRA&(1<<RXC)) );
    const uint8_t c=UDR;
    UCSRA&=~(1<<RXC);
    // send
    UDR='x';
    while( !(UCSRA&(1<<UDRE)) );
  }
#endif

#if 0
  while(true)
  {
    USART::send('0'+bytes_);
    _delay_ms(250);
    USART::send(' ');
    _delay_ms(250);
  }
#endif

#if 0
  while(true)
  {
    const uint8_t b=USART::recv();
    /*
    USART::send('\n');
    USART::send(b-1);
    USART::send(b+1);
    USART::send('\n');
    */
    //USART::send('0'+bytes_);
    USART::send(b);
    _delay_ms(250);
    //USART::send(' ');
    _delay_ms(250);
  }
#endif

#if 0
  while(true)
  {
    for(char c='a'; c<='z'; ++c)
    {
      USART::send(c);
      _delay_ms(100);
    }
    USART::send('\n');
    _delay_ms(100);
  }
#endif

#if 0
  cli();
  while(true)
  {
    for(char c='a'; c<='z'; ++c)
    {
      UDR=c;
      _delay_ms(100);
    }
    UDR='\n';
    _delay_ms(100);
  }
#endif

#if 0
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
#endif

  // code never reaches here
  return 0;
} // main()

