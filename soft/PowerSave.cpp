/*
 * PowerSave.cpp
 *
 */
#include "config.hpp"
#include <avr/sleep.h>
#include "PowerSave.hpp"

void PowerSave::idle(void)
{
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_mode();
}
