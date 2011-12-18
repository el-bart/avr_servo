#include <algorithm>
#include <cstring>
#include <cassert>
#include <sys/select.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "ServoCtrl/CommDevice.hpp"

namespace ServoCtrl
{

CommDevice::CommDevice(const boost::filesystem::path &devPath):
  rd_{ new SerialPort{devPath, SerialPort::READ} },
  wr_{ new SerialPort{devPath, SerialPort::WRITE} }
{
}

std::string CommDevice::run(std::string cmd)
{
  wr_->writeLine( std::move(cmd) );
  std::string ret=rd_->readLine(40);
  if( ret.find("ERR")!=std::string::npos )
    throw ExceptionProtocolError{ret};
  return ret;
}


void CommDevice::runFast(std::string cmd)
{
  wr_->writeLine( std::move(cmd) );
}

} // namespace ServoCtrl
