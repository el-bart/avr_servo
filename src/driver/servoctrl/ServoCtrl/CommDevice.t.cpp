#include <tut/tut.hpp>
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
    tut::ensure("unable to create fifi", mkfifo(devPath_)==0 );
  }

  ~TestClass(void)
  {
    unlink(devPath_);
  }

  const char *devPath_;
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
  // TODO
}

// 
template<>
template<>
void testObj::test<3>(void)
{
}

// 
template<>
template<>
void testObj::test<4>(void)
{
}

} // namespace tut
