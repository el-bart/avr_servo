#ifndef INCLUDE_HARDWARE_TIMER1_HPP_FILE
#define INCLUDE_HARDWARE_TIMER1_HPP_FILE

#include "Hardware/io.hpp"

namespace Hardware
{
struct Timer1
{
  static uint16_t getValue(void)
  {
    return TCNT1;
  }

  static void init(void)
  {
    TCCR1A&=~_BV(COM1A1); // disconnect OC1A
    TCCR1A&=~_BV(COM1A0); // ...
    TCCR1A&=~_BV(COM1B1); // disconnect OC1B
    TCCR1A&=~_BV(COM1B0); // ...
    TCCR1B&=~_BV(WGM13);  // normal opration mode
    TCCR1B&=~_BV(WGM12);  // ...
    TCCR1A&=~_BV(WGM11);  // ...
    TCCR1A&=~_BV(WGM10);  // ...
    //TCCR1C&=~_BV(FOC1A);  // disable force output compare
    //TCCR1C&=~_BV(FOC1B);  // disable force output compare
  }

  static void start(void)
  {
    TCNT1=0x0000;         // zero counter before start
    TCCR1B&=~_BV(CS12);   // set no prescaller
    TCCR1B&=~_BV(CS11);   // ...
    TCCR1B|= _BV(CS10);   // ...
  }

  static void stop(void)
  {
    TCCR1B&=~_BV(CS12);   // disable counter
    TCCR1B&=~_BV(CS11);   // ...
    TCCR1B&=~_BV(CS10);   // ...
    TCNT1=0x0000;         // zero counter
  }
}; // struct Timer1
} // namespace Hardware

#endif
