#include "ServoCtrl/CommDevice.hpp"

namespace ServoCtrl
{

typedef std::unique_lock<std::mutex> Lock;


CommDevice::CommDevice(boost::filesystem::path devPath)
{
  dev_.open(devPath);
  if( !dev_.is_open() )
    throw ExceptionDevice{devPath};
}

void CommDevice::send(std::string cmd)
{
  Lock lock{m_};
  // send command
  dev_<<cmd;
  dev_.flush();
  // read the response
  std::string ret;
  dev_>>ret;
  if( ret.find("ERROR")!=std::string::npos )
    throw ExceptionProtocolError{ret};
  // look like all's ok
}

} // namespace ServoCtrl
