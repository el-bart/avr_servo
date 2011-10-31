/*
 * AVR_servo
 *
 * servo controller driver, written for AVR uC.
 *
 */
#include "config.hpp"       // this file must be included as first one!

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "USART.hpp"
#include "Timer1.hpp"
#include "Timer2.hpp"
#include "PowerSave.hpp"
#include "ChronoTable.hpp"
#include "CommProtocol.hpp"
#include "PhaseGenerator.hpp"
#include "uassert.hpp"


// handle unknown interruption(s)
ISR(BADISR_vect)
{
  uassert(!"unhandled interrupt");
}

volatile int bytes_=0;      


inline ChronoTable::Positions getDefaultPositions(const PersistentSettings &s)
{
  ChronoTable::Positions out;
  for(uint8_t i=0; i<SERVO_COUNT; ++i)
    out[i]=s.posDef().read(i);
  return out;
}


//
// MAIN
//
int main(void)
{
#if 0
  #define USART_UBRR(baud,f) ( ((f)/((baud)*16L)) -1 )
  DDRB|=0xFF;       // PB operates as output

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
    UDR=c+1;
    while( !(UCSRA&(1<<UDRE)) );
  }
#endif

  PersistentSettings settings;                                      // persistent settings
  ChronoTable        chronoTable( getDefaultPositions(settings) );  // chronology table
  QueueSend          qSend;                                         // output queue
  QueueRecv          qRecv;                                         // input queue
  USART              rs(qSend, qRecv);                              // prepare USART to work
  Timer1             t1;                                            // configure T1
  Timer2             t2;                                            // configure T2
  PhaseGenerator     phaseGen;                                      // controling facility
  CommProtocol       proto(qSend, qRecv, settings);                 // protocol parser
  sei();                                                            // allow interrupts globally

  while(true)
  {

    // cycle throught stages.
    switch( t2.currentStep() )
    {
      // initial 'high' state
      case 0:
           phaseGen.rise();
           chronoTable.update();
           break;

      // PWM generation for servos
      case 1:
           phaseGen.generate( t1, chronoTable.currentEntries() );
           phaseGen.fall();
           break;

      // 'wait' in 'low' state
      case 2:
      case 3:
      case 4:
           proto.process( chronoTable.currentPos() );
           break;

      case 5:
      default:
           t2.resetStep();
           break;
    } // switch(current_step)

    PowerSave::idle();

  } // while(true)

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

