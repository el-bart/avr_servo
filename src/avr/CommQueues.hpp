#ifndef INCLUDE_COMMQUEUES_HPP_FILE
#define INCLUDE_COMMQUEUES_HPP_FILE

#include "config.hpp"
#include "FastQueue.hpp"

typedef FastQueue<QUEUE_RECV_SIZE> QueueSend;
typedef FastQueue<QUEUE_SEND_SIZE> QueueRecv;

#endif
