#ifndef INCLUDE_SERVOCTRL_SERIALPORT_HPP_FILE
#define INCLUDE_SERVOCTRL_SERIALPORT_HPP_FILE

/* public header */

#include <mutex>
#include <boost/noncopyable.hpp>
#include <boost/filesystem/path.hpp>

#include "ServoCtrl/Exception.hpp"
#include "ServoCtrl/SharedPtrNotNull.hpp"

namespace ServoCtrl
{

class SerialPort: private boost::noncopyable
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

  struct ExceptionTimeout: public Exception
  {
    explicit ExceptionTimeout(std::string details):
      Exception{ Strm{}<<"timeout while reading data: "<<std::move(details) }
    {
    }
  };

  enum Direction
  {
    READ,
    WRITE
  };


  SerialPort(const boost::filesystem::path &devPath, Direction dir);

  /** \brief sends given line.
   *  \param line lien to be sent.
   */
  void writeLine(std::string line);
  /** \brief reads line withing a given timeout.
   *  \param timeout [ms] to wait.
   *  \return line that has been read.
   */
  std::string readLine(unsigned int timeout);

private:
  void configure(void);
  void writeData(std::string cmd);
  std::string recvData(unsigned int timeout);
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
}; // class SerialPort


typedef SharedPtrNotNull<SerialPort> SerialPortPtrNN;

} // namespace ServoCtrl

#endif
