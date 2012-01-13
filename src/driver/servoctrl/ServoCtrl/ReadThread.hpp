#ifndef INCLUDE_SERVOCTRL_READTHREAD_HPP_FILE
#define INCLUDE_SERVOCTRL_READTHREAD_HPP_FILE

/* public header */

#include <thread>
//#include <boost/noncopyable.hpp>

#include "ServoCtrl/PromisesQueue.hpp"
#include "ServoCtrl/SerialPort.hpp"
//#include "ServoCtrl/Exception.hpp"

namespace ServoCtrl
{

class ReadThread
{
public:
  explicit ReadThread(SerialPortPtrNN rd);
  ~ReadThread(void);

  Response enqueue(ServoName s);

private:
  void threadBody(void);

  bool            quit_;
  SerialPortPtrNN rd_;
  PromisesQueue   q_;
  std::thread     th_;
}; // class ReadThread

} // namespace ServoCtrl

#endif
