#ifndef INCLUDE_HARDWARE_INTERRUPT_HPP_FILE
#define INCLUDE_HARDWARE_INTERRUPT_HPP_FILE

#include "Hardware/io.hpp"

#define ISR(name) void name(void)


namespace Hardware
{
inline bool &interruptsEnabled(void)
{
  static bool e=false;
  return e;
}
} // namespace Hardware


inline void cli(void)
{
  Hardware::interruptsEnabled()=false;
}

inline void sei(void)
{
  Hardware::interruptsEnabled()=true;
}

#endif
