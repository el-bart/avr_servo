/*
 * Timer0.hpp
 *
 */
#ifndef INCLUDE_TIMER0_HPP_FILE
#define INCLUDE_TIMER0_HPP_FILE

#include "Noncopyable.hpp"

class Timer0: private Noncopyable
{
public:
  Timer0(void);
  ~Timer0(void);

  void clearInterruptFlag(void);
  bool interruptCame(void) const;
}; // class Timer0

#endif
