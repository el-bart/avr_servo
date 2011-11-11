#ifndef INCLUDE_SERVOCTRL_COMMDEVICE_HPP_FILE
#define INCLUDE_SERVOCTRL_COMMDEVICE_HPP_FILE

#include <mutex>
#include <string>
#include <fstream>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>

#include "ServoCtrl/Exception.hpp"
#include "ServoCtrl/SharedPtrNotNull.hpp"

namespace ServoCtrl
{

class CommDevice
{
public:
  struct ExceptionDevice: public Exception
  {
    explicit ExceptionDevice(const boost::filesystem::path &devPath):
      Exception{ Strm{}<<"error while opening device: "<<devPath.native() }
    {
    }
  };

  struct ExceptionIO: public Exception
  {
    explicit ExceptionIO(std::string details):
      Exception{ Strm{}<<"I/O error: "<<std::move(details) }
    {
    }
  };

  struct ExceptionProtocolError: public Exception
  {
    explicit ExceptionProtocolError(std::string message):
      Exception{ Strm{}<<"protocol reported error: "<<std::move(message) }
    {
    }
  };


  explicit CommDevice(boost::filesystem::path devPath);

  void send(std::string cmd);

private:
  std::mutex                 m_;
  boost::filesystem::fstream dev_;
}; // class CommDevice


typedef SharedPtrNotNull<CommDevice> CommDevicePtrNN;

} // namespace ServoCtrl

#endif
