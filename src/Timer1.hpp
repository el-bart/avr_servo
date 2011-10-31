/*
 * Timer1.hpp
 *
 */
#ifndef INCLUDE_TIMER1_HPP_FILE
#define INCLUDE_TIMER1_HPP_FILE

#include "config.hpp"
#include <avr/io.h>
#include "Noncopyable.hpp"

/** \brief 16-bit timer1 wrapper.
 */
class Timer1: private Noncopyable
{
public:
  /** \brief initialize T1.
   *  \note timer starts as stopped.
   */
  Timer1(void);

  /** \brief start timer (resets counter to 0).
   */
  void start(void);
  /** \brief stops counter (resets counter to 0).
   */
  void stop(void);

  /** \brief get current counter value.
   *  \return counter value.
   *
   *  this returns number of micro seconds since started. this means
   *  8000 is 1ms, thus making typical servo to work in range
   *  8000:16000, with center at 12000, though this may warry between
   *  different devices.
   */
  uint16_t get(void) const
  {
    cli();
    const uint16_t tmp=TCNT1;
    sei();
    return tmp;
  }
}; // class Timer1

#endif
