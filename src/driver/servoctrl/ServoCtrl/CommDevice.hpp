#ifndef INCLUDE_SERVOCTRL_COMMDEVICE_HPP_FILE
#define INCLUDE_SERVOCTRL_COMMDEVICE_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>
#include <boost/filesystem/path.hpp>

#include "ServoCtrl/SharedPtrNotNull.hpp"
#include "ServoCtrl/SerialPort.hpp"
#include "ServoCtrl/Exception.hpp"

namespace ServoCtrl
{

class CommDevice: private boost::noncopyable
{
public:
  struct ExceptionProtocolError: public Exception
  {
    explicit ExceptionProtocolError(std::string message):
      Exception{ Strm{}<<"protocol reported error: "<<std::move(message) }
    {
    }
  };

  explicit CommDevice(const boost::filesystem::path &devPath);

  std::string run(std::string cmd);
  void runFast(std::string cmd);

private:
  SerialPortPtrNN rd_;
  SerialPortPtrNN wr_;
}; // class CommDevice


typedef SharedPtrNotNull<CommDevice> CommDevicePtrNN;

} // namespace ServoCtrl

#endif
