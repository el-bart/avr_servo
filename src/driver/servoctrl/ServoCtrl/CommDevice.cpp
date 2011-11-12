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

typedef std::unique_lock<std::mutex> Lock;

CommDevice::AutoDescriptor::AutoDescriptor(const int fd):
  fd_(fd)
{
}

CommDevice::AutoDescriptor::~AutoDescriptor(void)
{
  if(fd_!=-1)
    close(fd_);
}


CommDevice::CommDevice(const boost::filesystem::path &devPath):
  fd_( open(devPath.native().c_str(), O_RDWR|O_NOCTTY|O_NDELAY) )
{
  if(fd_.get()==-1)
    throw ExceptionDevice{devPath};
  configure();
}

std::string CommDevice::run(std::string cmd)
{
  Lock lock{m_};
  // ensure no data is waiting int the buffer
  discardContent();

  // send command
  sendData(cmd);
  // get the response - if it does not match given command, proceed with next
  while(true)
  {
    const std::string ret=recvData();
    // skip responses that are not for our command
    if(ret.length()>0 && cmd.length()>0 && ret[0]!=cmd[0])
      continue;
    // report errors
    if( ret.find("ERR")!=std::string::npos )
      throw ExceptionProtocolError{ret};
    // look like all's ok
    return ret;
  }

  assert(!"code never reaches here");
  return "?? code never reaches here ??";
}


void CommDevice::runFast(std::string cmd)
{
  Lock lock{m_};
  sendData(cmd);
}


void CommDevice::configure(void)
{
  // ??
  if( fcntl(fd_.get(), F_SETFL, 0)==-1 )
    throw ExceptionIO{"fcntl() failed"};

  // initial settings
  struct termios opts;
  memset(&opts, 0, sizeof(opts));

  // set baud rate
  const speed_t speed=B38400;
  cfsetispeed(&opts, speed);
  cfsetospeed(&opts, speed);
  // 8N1 (8 data bits, no parity, 1 stop bit)
  opts.c_cflag &= ~PARENB;
  opts.c_cflag &= ~CSTOPB;
  opts.c_cflag &= ~CSIZE;
  opts.c_cflag |=  CS8;

  // apply these settings
  if( tcsetattr(fd_.get(), TCSANOW, &opts)!=0 )
    throw ExceptionIO{"tcsetattr(): setting new settings failed"};
}


void CommDevice::sendData(const std::string &cmd)
{
  const int len=cmd.length();
  if( write(fd_.get(), cmd.c_str(), len)!=len )
    throw ExceptionIO{"write(): could not write command"};
}


std::string CommDevice::recvData(void)
{

  constexpr size_t  bufSize=16;     // TODO: magic value
  char              buf[bufSize];   // input buffer
  char             *ptr =buf;       // start from
  size_t            left=bufSize-1; // max more bytes to read


  while(true)   // note: will stop on timeout from select(), on data read
  {
    // wait for data or timeout
    fd_set rdfd;
    FD_ZERO(&rdfd);
    FD_SET(fd_.get(), &rdfd);
    struct timeval timeout={0, 20*1000};        // 20[ms] TODO: magic value
    const int ret=select(fd_.get()+1, &rdfd, NULL, NULL, &timeout);
    if(ret<0)
      throw ExceptionIO{"select() failed for some reason"};
    if(ret==0)
      throw ExceptionProtocolError{"timeout while waiting for response"};

    // read new data
    const int n=read(fd_.get(), ptr, 1);
    if(n<0)
      throw ExceptionIO{"read() failed for some reason"};

    // check fo rend of line
    bool eol=false;
    std::for_each(ptr, ptr+n, [&eol](const char c){ if(c=='\r' || c=='\n') eol=true; });
    if(eol)
    {
      ptr[n]=0;
      assert( std::string{buf}.length()<bufSize );
      return std::string{buf};
    }

    // ok - something more to be read...
    ptr +=n;
    left-=n;

    // check for buffer end
    if(left==0)
      throw ExceptionProtocolError{"buffer full of junk - discarding content"};
  } // while(true)

  assert(!"code never reaches here");
  return "?? code never reaches here ??";
}


void CommDevice::discardContent(void)
{
  long int dt=0;
  while(true)
  {
    struct timeval timeout={0, dt};
    fd_set rdfd;
    FD_ZERO(&rdfd);
    FD_SET(fd_.get(), &rdfd);
    const int ret=select(fd_.get()+1, &rdfd, NULL, NULL, &timeout);
    if(ret<=0)
      return;

    char tmp[128];
    if( read(fd_.get(), tmp, sizeof(tmp)-1)==-1 )
      throw ExceptionIO{"read(): unable to dispose of garbage in the buffers"};
    // if has some data already, wait a bit - maby there's more to come?
    dt=2*1000;      // 2[ms] // TODO: magic value
  }
}

} // namespace ServoCtrl
