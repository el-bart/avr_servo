#ifndef INCLUDE_HARDWARE_USART_HPP_FILE
#define INCLUDE_HARDWARE_USART_HPP_FILE

#include "Hardware/io.hpp"
#include <inttypes.h>
#include <cassert>

namespace Hardware
{
struct USART
{
  static void init(void)
  {
    assert( !initialized() );
    initialized()=true;
  }

  static void send(uint8_t b)
  {
    assert( initialized() );
    data()=b;
  }

  static uint8_t receive(void)
  {
    assert( initialized() );
    return data();
  }

  static void signalOnDataEmpty(void)
  {
    assert( initialized() );
  }

  static void onDataRegisterEmpty(void)
  {
    assert( initialized() );
  }


  // TEST CALLS:
  static uint8_t &data(void)
  {
    static uint8_t d=42;
    return d;
  }
  static bool &initialized(void)
  {
    static bool v=false;
    return v;
  }
}; // struct Timer1
} // namespace Hardware

#endif
