/*
 * USART.cpp
 *
 */
#include "config.hpp"

#include "Hardware/io.hpp"
#include "Hardware/interrupt.hpp"
#include "Hardware/USART.hpp"

#include "uassert.hpp"
#include "USART.hpp"

//
// I/O queues
//
namespace
{
ChronoTable *g_chronoTable=NULL;


template<uint8_t N>
class Queue
{
public:
  Queue(void):
    size_(0)
  {
  }

  void push(uint8_t b)
  {
    if(size_==N)
      pop();
    d_[size_]=b;
    ++size_;
  }

  uint8_t peep(uint8_t pos)
  {
    return d_[pos];
  }

  uint8_t pop(void)
  {
    const uint8_t tmp=d_[0];
    for(uint8_t i=1; i<size_; ++i)
      d_[i-1]=d_[i];
    --size_;
    return tmp;
  }

  void clear(void)
  {
    size_=0;
  }

  uint8_t size(void)
  {
    return size_;
  }

private:
  volatile uint8_t size_;
  volatile uint8_t d_[N];
}; // class Queue

// input queue
Queue<4> g_inQueue;
// output queue
Queue<2> g_outQueue;


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
  g_chronoTable->currentPos().set(servo, pos);

  // and we send the response
  USART::send(servo);
  USART::send(sum);
} // parseInQueue()
} // unnamed namespace


static inline void sendDataImpl(void)
{
  //uassert( UCSRA & _BV(UDRE) );
  Hardware::USART::send( g_outQueue.pop() );
}

extern volatile int bytes_;              

// USART RX completed interrupt
ISR(USART_RXC_vect)
{
  const uint8_t c=Hardware::USART::receive();   // read form hardware ASAP

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
  Hardware::USART::onDataRegisterEmpty();   // data registry empty - disable interrupt
  if( g_outQueue.size()>0 )                 // if data register is empty and we have data to send
    sendDataImpl();                         // we can send it now
}


void USART::init(ChronoTable &ct)
{
  Hardware::USART::init();
  // set global pointer
  g_chronoTable=&ct;
}

void USART::send(uint8_t b)
{
  g_outQueue.push(b);                       // enqueue next char to send
  if( g_outQueue.size()==1 )                // this might be first char to send
    Hardware::USART::signalOnDataEmpty();   // signal on data registry empty.
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