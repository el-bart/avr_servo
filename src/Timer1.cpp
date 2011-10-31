/*
 * Timer1.cpp
 *
 */
#include "config.hpp"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer1.hpp"


Timer1::Timer1(void)
{
  // set timer 1
  //TCCR1A=0x00;          // all features disable, operate in normal mode

  /*
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
  */
  // keep stopped by default
  stop();
}

void Timer1::start(void)
{
  TCNT1=0x0000;         // zero counter before start
  TCCR1B&=~_BV(CS12);   // set no prescaller
  TCCR1B&=~_BV(CS11);   // ...
  TCCR1B|= _BV(CS10);   // ...
}

void Timer1::stop(void)
{
  TCCR1B&=~_BV(CS12);   // disable counter
  TCCR1B&=~_BV(CS11);   // ...
  TCCR1B&=~_BV(CS10);   // ...
}
