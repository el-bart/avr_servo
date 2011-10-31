#ifndef INCLUDE_USART_HPP_FILE
#define INCLUDE_USART_HPP_FILE

#include <inttypes.h>
#include "Noncopyable.hpp"
#include "Queue.hpp"

/** \brief set of operations on serial interface.
 */
class USART: private Noncopyable
{
public:
  typedef Queue<QUEUE_RECV_SIZE> QueueSend;
  typedef Queue<QUEUE_SEND_SIZE> QueueRecv;

  /** \brief initialize USART.
   */
  USART(QueueSend &qSend, QueueRecv &qRecv);
  ~USART(void);

  /** \brief signal begining of the sending data.
   */
  void sendData(void);
}; // class USART

#endif
