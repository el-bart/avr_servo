/*
 * TimeTable.hpp
 *
 */
#ifndef INCLUDE_TIMETABLE_HPP_FILE
#define INCLUDE_TIMETABLE_HPP_FILE

#include "config.hpp"

#include <inttypes.h>

/** \brief table of times to activate servos at.
 */
class TimeTable
{
public:
  const uint16_t operator[](const uint8_t pos) const
  {
    uassert(pos<8);
    return tab_[pos];
  }

  uint16_t &operator[](const uint8_t pos)
  {
    uassert(pos<8);
    return tab_[pos];
  }

private:
  uint8_t tab_[8];
}; // class TimeTable

#endif
