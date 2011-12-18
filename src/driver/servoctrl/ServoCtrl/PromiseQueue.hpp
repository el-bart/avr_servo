#ifndef INCLUDE_SERVOCTRL_PROMISEQUEUE_HPP_FILE
#define INCLUDE_SERVOCTRL_PROMISEQUEUE_HPP_FILE

/* public header */

#include <deque>
#include <mutex>
#include <boost/noncopyable.hpp>

#include "ServoCtrl/TimerRT.hpp"
#include "ServoCtrl/Response.hpp"
#include "ServoCtrl/ServoName.hpp"
#include "ServoCtrl/Exception.hpp"

// TODO: doxygen
// TODO: tests

namespace ServoCtrl
{

class PromiseQueue: private boost::noncopyable
{
public:
  struct Element
  {
    ServoName         s_;
    TimerRT           t_;
    Response::Promise p_;
  }; // struct Element

  Response::Future push(ServoName s);
  Element pop(void);

private:
  typedef std::deque<Element> Promises;

  std::mutex m_;
  Promises   queue_;
}; // class Response

} // namespace ServoCtrl

#endif
