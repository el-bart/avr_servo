#include "config.hpp"
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "USART.hpp"
#include "uassert.hpp"

#define USART_UBRR(baud,f) ( ((f)/((baud)*16L)) -1 )

// TODO: add test for baud-rate error and show error if value is over 1%

//
// I/O queues and helper funcitons
//
namespace
{
// input queue
QueueRecv *g_recvQueue=NULL;
// output queue
QueueSend *g_sendQueue=NULL;


bool canSend(void)
{
  return (UCSRA & _BV(UDRE));
} // canSend()


inline void sendDataImpl(void)
{
  uassert( canSend() );
  uassert(g_sendQueue!=NULL);
  UDR=g_sendQueue->pop();
} // sendDataImpl()

char buf[128];
char *g_end=buf;
} // unnamed namespace


// USART RX completed interrupt
ISR(USART_RXC_vect)
{
  const uint8_t b=UDR;          // read form hardware ASAP
  uassert(g_recvQueue!=NULL);
  g_end=buf;
  *g_end=b;
  ++g_end;
  return;
  g_recvQueue->push(b);         // enqueue new byte
  g_recvQueue->clear();                     
}

// USART TX completed interrupt
ISR(USART_TXC_vect)
{
  uassert(g_sendQueue!=NULL);
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
  enable();
  // enable transciever
  UCSRB|= _BV(RXEN);    // RX enable
  UCSRB|= _BV(TXEN);    // TX enable

  // configure proper pins as in (RX) and out (TX)
  DDRD &=~_BV(PD0);     // RX as in
  PORTD|= _BV(PD0);     // RX high with pull-up
  DDRD |= _BV(PD1);     // TX as out

  // set global pointers
  setSendQueue(qSend);
  setRecvQueue(qRecv);
}


USART::~USART(void)
{
  g_recvQueue=NULL;
  g_sendQueue=NULL;
  disable();
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


void USART::enable(void)
{
  // enable interrupts
  UCSRB|= _BV(RXCIE);   // RX complete
  UCSRB|= _BV(TXCIE);   // TX complete
}


void USART::disable(void)
{
  // disable interrupts
  UCSRB&=~_BV(RXCIE);   // RX complete
  UCSRB&=~_BV(TXCIE);   // TX complete
}


void USART::setSendQueue(QueueSend &qSend)
{
  cli();
  g_sendQueue=&qSend;
  sei();
}


void USART::setRecvQueue(QueueRecv &qRecv)
{
  cli();
  g_recvQueue=&qRecv;
  sei();
}
