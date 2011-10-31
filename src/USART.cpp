#include "config.hpp"

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "uassert.hpp"
#include "USART.hpp"

#define USART_UBRR(baud,f) ( ((f)/((baud)*16L)) -1 )

// TODO: add test for baud-rate error and show error if value is over 1%

//
// I/O queues
//
namespace
{
// input queue
QueueRecv *g_recvQueue=NULL;
// output queue
QueueSend *g_sendQueue=NULL;
// flag checking if transmition is in progress

bool canSend(void)
{
  return (UCSRA & _BV(UDRE));
}
} // unnamed namespace


static inline void sendDataImpl(void)
{
  uassert( canSend() );
  uassert(g_sendQueue==NULL);
  UDR=g_sendQueue->pop();
}

// USART RX completed interrupt
ISR(USART_RXC_vect)
{
  const uint8_t b=UDR;          // read form hardware ASAP
  if(g_recvQueue==NULL)
    return;
  g_recvQueue->push(b);         // enqueue new byte
}

// USART TX completed interrupt
ISR(USART_TXC_vect)
{
  if(g_sendQueue==NULL)
    return;
  if( g_sendQueue->size()>0 )   // if have something to send
    sendDataImpl();
}

// USART data register is empty interrupt
ISR(USART_UDRE_vect)
{
  UCSRB&=~_BV(UDRIE);           // data registry empty - disable interrupt
  if(g_sendQueue==NULL)
    return;
  if( g_sendQueue->size()>0 )   // if data register is empty and we have data to send
    sendDataImpl();             // we can send it now
}


USART::USART(QueueSend &qSend, QueueRecv &qRecv)
{
  // clock divider register (computed from baud rate and oscilator frequency)
  UBRRH=(uint8_t)( (USART_UBRR(USART_BAUD, F_CPU)>>8) & 0x00FF );
  UBRRL=(uint8_t)( (USART_UBRR(USART_BAUD, F_CPU)>>0) & 0x00FF );

  // enable interrupts
  UCSRB|= _BV(RXCIE);   // RX complete
  UCSRB|= _BV(TXCIE);   // TX complete
  // enable transciever
  UCSRB|= _BV(RXEN);    // RX enable
  UCSRB|= _BV(TXEN);    // TX enable

  // configure proper pins as in (RX) and out (TX)
  DDRD &=~_BV(PD0);     // RX as in
  PORTD|= _BV(PD0);     // RX high with pull-up
  DDRD |= _BV(PD1);     // TX as out

  // set global pointers
  g_recvQueue=&qRecv;
  g_sendQueue=&qSend;
}

USART::~USART(void)
{
  g_recvQueue=NULL;
  g_sendQueue=NULL;
}

void USART::sendData(void)
{
  uassert(g_sendQueue!=NULL);
  if( g_sendQueue->size()>0 )       // this might be first char to send
    if( canSend() )                 // no transmition is ongoing right now
      UCSRB|=_BV(UDRIE);            // signal on data registry empty.
                                    // if transmition has not yet started this will
                                    // send initial (first) byte as soon as USART is ready
}
