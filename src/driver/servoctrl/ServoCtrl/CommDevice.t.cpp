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
    devPath_("/dev/ttyUSB0")
  {
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
  CommDevice cd(devPath_);
}

// open whatever and try sending something
template<>
template<>
void testObj::test<2>(void)
{
  CommDevice cd(devPath_);
  ensure_equals("invalid response", cd.run("as7f?\n"), "a-ok\n");
}

// test error condition
template<>
template<>
void testObj::test<3>(void)
{
  CommDevice cd(devPath_);
  try
  {
    cd.run("wtf?\n");
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

// try sending multiple commands
template<>
template<>
void testObj::test<5>(void)
{
  CommDevice cd(devPath_);
  ensure_equals("invalid response 1", cd.run("as30?\n"), "a-ok\n");
  ensure_equals("invalid response 2", cd.run("asa0?\n"), "a-ok\n");
}

// try sending multiple commands: error, ok - it should recover
template<>
template<>
void testObj::test<6>(void)
{
  CommDevice cd(devPath_);
  try
  {
    cd.run("wtf?\n");
    fail("no expcetion on error message");
  }
  catch(const CommDevice::ExceptionProtocolError&)
  {
    // this is expected
  }
  ensure_equals("invalid response", cd.run("asa0?\n"), "a-ok\n");
}

} // namespace tut
