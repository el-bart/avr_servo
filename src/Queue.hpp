#ifndef INCLUDE_QUEUE_HPP_FILE
#define INCLUDE_QUEUE_HPP_FILE

template<uint8_t N>
class Queue
{
public:
  Queue(void):
    begin_(0),
    size_(0)
  {
  }

  void push(uint8_t b)
  {
    const uint8_t idx=(begin_+size())%N;
    d_[idx]=b;
    if(size()!=N)
      ++size_;
    else
      ++begin_;
  }

  uint8_t peep(uint8_t pos) const
  {
    const uint8_t idx=(begin_+pos)%N;
    return d_[idx];
  }

  uint8_t pop(void)
  {
    const uint8_t tmp=d_[begin_];
    remove(1);
    return tmp;
  }

  void remove(const uint8_t count)
  {
    if(count>=size())
    {
      clear();
      return;
    }
    begin_=(begin_+count)%N;
    size_-=count;
  }

  void clear(void)
  {
    size_ =0;
    begin_=0;
  }

  uint8_t size(void) const
  {
    return size_;
  }

private:
  volatile uint8_t begin_;
  volatile uint8_t size_;
  volatile uint8_t d_[N];
}; // class Queue

#endif
