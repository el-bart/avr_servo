/*
 * Timer0.hpp
 *
 */
#ifndef INCLUDE_TIMER0_HPP_FILE
#define INCLUDE_TIMER0_HPP_FILE

#include "Noncopyable.hpp"

/** \brief timer T0 used for accurate impulses generation.
 */
class Timer0: private Noncopyable
{
public:
  /** \brief initialize timer.
   */
  Timer0(void);
  /** \brief deinitilize object (timer is still running though).
   */
  ~Timer0(void);

  /** \brief clear flag signalling interruption.
   */
  void clearInterruptFlag(void);
  /** \brief checks if interruption flag has been set.
   *  \return true, if interruption has arrived, false otherwise.
   */
  bool interruptCame(void) const;
}; // class Timer0

#endif
