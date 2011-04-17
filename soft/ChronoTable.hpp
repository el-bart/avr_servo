/*
 * ChronoTable.hpp
 *
 */
#ifndef INCLUDE_CHRONOTABLE_HPP_FILE
#define INCLUDE_CHRONOTABLE_HPP_FILE

#include "config.hpp"
#include <inttypes.h>
#include "PositionsTable.hpp"
#include "Table.hpp"
#include "TableEEPROM.hpp"

class ChronoTable
{
public:
  struct Entry
  {
    uint16_t time_;
    uint8_t  mask_;
  }; // struct Entry

  ChronoTable(void);

  void writeToEEPROM(void);

  TableEEPROM &min(void)
  {
    return min_;
  }
  TableEEPROM &max(void)
  {
    return max_;
  }
  TableEEPROM &startPositions(void)
  {
    return start_;
  }

  void compute(void);

  const Entry operator[](const uint8_t p) const
  {
    return e_[p];
  }

private:
  bool readFromEEPROM(void);

  // input data
  TableEEPROM     min_;     // minimum positions
  TableEEPROM     max_;     // maximum positions
  TableEEPROM     start_;   // start-up positions
  PositionsTable  cur_;     // current positions
  Table<Entry, 8> e_;       // processed entries
}; // class ChronoTable

#endif
