#ifndef INCLUDE_SERVOCTRL_COMMDEVICE_HPP_FILE
#define INCLUDE_SERVOCTRL_COMMDEVICE_HPP_FILE

#include <mutex>
#include <boost/noncopyable.hpp>
#include <boost/filesystem/path.hpp>

#include "ServoCtrl/Exception.hpp"
#include "ServoCtrl/SharedPtrNotNull.hpp"

namespace ServoCtrl
{

class CommDevice: private boost::noncopyable
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


  explicit CommDevice(const boost::filesystem::path &devPath);

  std::string run(std::string cmd);

private:
  void configure(void);
  void sendData(const std::string &cmd);
  std::string recvData(void);
  void discardContent(void);

  class AutoDescriptor: private boost::noncopyable
  {
  public:
    explicit AutoDescriptor(const int fd);
    ~AutoDescriptor(void);
    int get(void)
    {
      return fd_;
    }

  private:
    int fd_;
  }; // struct AutoDescriptor

  std::mutex     m_;
  AutoDescriptor fd_;
}; // class CommDevice


typedef SharedPtrNotNull<CommDevice> CommDevicePtrNN;

} // namespace ServoCtrl

#endif
