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
   *  counter returns current timer setting. it gives 1000 ticks every 1ms.
   */
  uint16_t get(void) const
  {
    // interruptions must be disabled, to prevent races when reading 16-bit register.
    cli();
    const uint16_t tmp=TCNT1;
    sei();
    return tmp;
  }
}; // class Timer1

#endif
