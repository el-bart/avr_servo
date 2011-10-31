#include "config.hpp"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "uassert.hpp"
#include "USART.hpp"
#include "Queue.hpp"

#define USART_UBRR(baud,f) ( ((f)/((baud)*16L)) -1 )

// TODO: add test for baud-rate error and show error if value is over 1%

//
// I/O queues
//
namespace
{
ChronoTable *g_chronoTable=NULL;

// input queue
Queue<QUEUE_RECV_SIZE> g_inQueue;
// output queue
Queue<QUEUE_SEND_SIZE> g_outQueue;


void parseInQueue(void)
{
  if( g_inQueue.size()<4 )
    return;

  // get servo number
  const uint8_t servo=g_inQueue.pop();
  // get servo position
  const uint8_t posH =g_inQueue.peep(0);
  const uint8_t posL =g_inQueue.peep(1);
  // compute sum
  const uint8_t sum  =servo^posH^posL;

  // put together position
  const uint16_t pos=(posH<<8)|posL;

  // check if checksum is valid
  if( sum!=g_inQueue.peep(2) )
    return;
  // if sum is fine, clear the queue
  g_inQueue.clear();

  // check if servo number is valid
  if(servo>7)
    return;
  // check if position is valid
  if(pos>24000)
    return;
  // invalid pointer to table?
  if( g_chronoTable==NULL )
    return;

  // we have valid data, so we apply it
  g_chronoTable->currentPos()[servo]=pos;

  // and we send the response
  USART::send(servo);
  USART::send(sum);
} // parseInQueue()
} // unnamed namespace


static inline void sendDataImpl(void)
{
  uassert( UCSRA & _BV(UDRE) );
  UDR=g_outQueue.pop();
}

extern volatile int bytes_;              

// USART RX completed interrupt
ISR(USART_RXC_vect)
{
  const uint8_t c=UDR;          // read form hardware ASAP

g_inQueue.push(c);          
//return;

  ++bytes_;         
  if(bytes_>10)     
    bytes_=0;       
  return;           

  g_inQueue.push(c);            // enqueue new byte
  parseInQueue();               // try processing input queue
}

// USART TX completed interrupt
ISR(USART_TXC_vect)
{
  if( g_outQueue.size()>0 )     // if have something to send
    sendDataImpl();
}

// USART data register is empty interrupt
ISR(USART_UDRE_vect)
{
  UCSRB&=~_BV(UDRIE);           // data registry empty - disable interrupt
  if( g_outQueue.size()>0 )     // if data register is empty and we have data to send
    sendDataImpl();             // we can send it now
}


void USART::init(ChronoTable &ct)
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

  // set global pointer
  g_chronoTable=&ct;
}

void USART::send(uint8_t b)
{
  g_outQueue.push(b);               // enqueue next char to send
  if( g_outQueue.size()==1 )        // this might be first char to send
    UCSRB|=_BV(UDRIE);              // signal on data registry empty.
                                    // if transmition has not yet started this will
                                    // send initial (first) byte as soon as USART is ready
}

uint8_t USART::recv(void)
{
  while( g_inQueue.size()==0 )
  {
  }
  return g_inQueue.pop();
}
