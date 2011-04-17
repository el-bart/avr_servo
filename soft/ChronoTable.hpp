/*
 * ChronoTable.hpp
 *
 */
#ifndef INCLUDE_CHRONOTABLE_HPP_FILE
#define INCLUDE_CHRONOTABLE_HPP_FILE

#include "config.hpp"
#include <inttypes.h>
#include "Table.hpp"

class ChronoTable
{
public:
  struct Entry
  {
    uint16_t time_;
    uint8_t  mask_;
  }; // struct Entry

  ChronoTable(void);

  Table<int16_t, 8> &currentPos(void)
  {
    return cur_;
  }

  void compute(void);

  const Entry get(const uint8_t p) const
  {
    return e_.get(p);
  }

private:
  // input data
  Table<int16_t, 8> cur_;   // current positions
  Table<Entry, 8>   e_;     // processed entries
}; // class ChronoTable

#endif
