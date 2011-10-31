#ifndef INCLUDE_CHRONOTABLE_HPP_FILE
#define INCLUDE_CHRONOTABLE_HPP_FILE

#include "config.hpp"
#include <inttypes.h>
#include "Table.hpp"

/** \brief class gathering and parsing positions, providing
 *         fast-to-process output table.
 */
class ChronoTable
{
public:
  /** \brief entry of a single processing data unit.
   */
  struct Entry
  {
    uint16_t time_;         ///< time to switch pin(s) off
    uint8_t  mask_;         ///< mask of pins to be turned off
  }; // struct Entry

  /** \brief initialize data.
   */
  ChronoTable(void);

  /** \brief return reference to table of current positions.
   *  \return reference fo current positions to be changed.
   */
  Table<uint16_t, 8> &currentPos(void)
  {
    return cur_;
  }

  /** \brief compute new entry table basing on new settings.
   */
  void compute(void);

  /** \brief get entry of a given number.
   *  \param p entry number to get.
   *  \return requested entry.
   */
  const Entry get(const uint8_t p) const
  {
    return e_.get(p);
  }

private:
  Table<uint16_t, 8> cur_;  // current positions
  Table<Entry, 8>   e_;     // processed entries
}; // class ChronoTable

#endif
