#ifndef INCLUDE_HARDWARE_TIMER1_HPP_FILE
#define INCLUDE_HARDWARE_TIMER1_HPP_FILE

#include "Hardware/io.hpp"
#include <inttypes.h>
#include <cassert>

namespace Hardware
{
struct Timer1
{
  static uint16_t getValue(void)
  {
    assert( initialized() );
    return value();
  }

  static void init(void)
  {
    assert( !initialized() );
    value()=true;
  }

  static void start(void)
  {
    assert( initialized() );
  }

  static void stop(void)
  {
    assert( initialized() );
  }


  // TEST CALLS:
  static uint16_t &value(void)
  {
    static uint16_t v=666;
    return v;
  }
  static bool &initialized(void)
  {
    static bool v=false;
    return v;
  }
}; // struct Timer1
} // namespace Hardware

#endif
