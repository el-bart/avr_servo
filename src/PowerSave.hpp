/*
 * PowerSave.hpp
 *
 */
#ifndef INCLUDE_POWERSAVE_HPP_FILE
#define INCLUDE_POWERSAVE_HPP_FILE

#include "config.hpp"
#include "Hardware/PowerSave.hpp"

/** \brief power-consumption reducing calls.
 */
struct PowerSave
{
  /** \brief enter idle mode.
   */
  static void idle(void)
  {
    Hardware::PowerSave::enterIdleMode();
  }
}; // struct PowerSave

#endif
