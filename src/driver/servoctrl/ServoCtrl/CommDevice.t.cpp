#include <tut/tut.hpp>
#include <thread>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ServoCtrl/CommDevice.hpp"

using namespace ServoCtrl;

namespace
{
struct TestClass
{
  TestClass(void):
    devPath_("test-fifo")
  {
    unlink(devPath_);
    tut::ensure("unable to create fifo", mkfifo(devPath_, 0600)==0 );
  }

  ~TestClass(void)
  {
    unlink(devPath_);
  }

  const char  *devPath_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("ServoCtrl/CommDevice");
} // unnamed namespace


namespace tut
{

// try opening USART 0
template<>
template<>
void testObj::test<1>(void)
{
  CommDevice cd("/dev/ttyS0");
}

// open whatever and try sending something
template<>
template<>
void testObj::test<2>(void)
{
  CommDevice cd(devPath_);
  cd.send("hello\n");
}

// test error condition
template<>
template<>
void testObj::test<3>(void)
{
  CommDevice cd(devPath_);
  try
  {
    cd.send("oops-ERROR-found\n");
    fail("no exception on error");
  }
  catch(const CommDevice::ExceptionProtocolError&)
  {
    // this is expected
  }
}

// test error when opening non-existing file
template<>
template<>
void testObj::test<4>(void)
{
  try
  {
    CommDevice cd("/non/exisitng/element");
    fail("no exception on error");
  }
  catch(const CommDevice::ExceptionDevice&)
  {
    // this is expected
  }
}

} // namespace tut
