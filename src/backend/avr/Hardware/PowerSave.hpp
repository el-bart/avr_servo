#ifndef INCLUDE_HARDWARE_POWERSAVE_HPP_FILE
#define INCLUDE_HARDWARE_POWERSAVE_HPP_FILE

#include "Hardware/io.hpp"
#include <avr/sleep.h>

namespace Hardware
{
struct PowerSave
{
  static void enterIdleMode(void)
  {
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_mode();
  }
}; // struct PowerSave
} // namespace Hardware

#endif
