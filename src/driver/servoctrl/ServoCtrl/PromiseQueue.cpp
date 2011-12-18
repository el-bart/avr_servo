#include "ServoCtrl/PromiseQueue.hpp"

namespace ServoCtrl
{

typedef std::unique_lock<std::mutex> Lock;


Response::Future PromiseQueue::push(ServoName s)
{
  Element          e={ std::move(s), TimerRT{}, Response::Promise{} };
  Response::Future f=e.p_.get_future();
  {
    Lock lock(m_);
    queue_.push_back( std::move(e) );
  }
  return f;
}


PromiseQueue::Element PromiseQueue::pop(void)
{
  Lock lock(m_);
  Element e=std::move( queue_.front() );
  queue_.pop_front();
  return std::move(e);
}

} // namespace ServoCtrl
