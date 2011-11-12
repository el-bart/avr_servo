#ifndef INCLUDE_FASTQUEUE_HPP_FILE
#define INCLUDE_FASTQUEUE_HPP_FILE

#include "config.hpp"
#include <inttypes.h>


template<uint8_t N>
class FastQueue
{
public:
  FastQueue(void):
    size_(0)
  {
  }

  void push(uint8_t b)
  {
    d_[size_]=b;
    ++size_;
  }

  uint8_t peek(uint8_t pos) const
  {
    return d_[pos];
  }

  /*
  uint8_t pop(void)
  {
    const uint8_t tmp=d_[0];
    // TODO: for?
    return tmp;
  }
  */

  void clear(void)
  {
    size_=0;
  }

  uint8_t size(void) const
  {
    return size_;
  }

private:
  volatile uint8_t size_;
  volatile uint8_t d_[N];
}; // class Queue

#endif
