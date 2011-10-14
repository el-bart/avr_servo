#ifndef INCLUDE_HARDWARE_POWERSAVE_HPP_FILE
#define INCLUDE_HARDWARE_POWERSAVE_HPP_FILE

#include "Hardware/io.hpp"

namespace Hardware
{
struct PowerSave
{
  static void enterIdleMode(void)
  {
    sleepMode()=true;
  }


  // TEST CALLS:
  static bool &sleepMode(void)
  {
    static bool sm=false;
    return sm;
  }
}; // struct PowerSave
} // namespace Hardware

#endif
