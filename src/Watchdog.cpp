#include "config.hpp"
#include <avr/wdt.h>

#include "Watchdog.hpp"

Watchdog::Watchdog(void)
{
  disable();
}

Watchdog::~Watchdog(void)
{
  disable();
}

void Watchdog::enable(void)
{
  // since whole cycle is 5ms, it means loosing 12 cycles. if it happens,
  // it can be safely assumed something went VERY wrong.
  wdt_enable(WDTO_60MS);
}

void Watchdog::reset(void)
{
  wdt_reset();
}

void Watchdog::disable(void)
{
  wdt_disable();
}
