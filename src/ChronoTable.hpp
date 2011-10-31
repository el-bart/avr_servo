#ifndef INCLUDE_CHRONOTABLE_HPP_FILE
#define INCLUDE_CHRONOTABLE_HPP_FILE

#include "config.hpp"
#include <inttypes.h>

#include "Table.hpp"
#include "EntryTable.hpp"
#include "Noncopyable.hpp"
#include "PersistentSettings.hpp"

/** \brief class gathering and parsing positions, providing
 *         fast-to-process output table.
 */
class ChronoTable: private Noncopyable
{
public:
  typedef Table<uint8_t, SERVO_COUNT> Positions;

  /** \brief initialize data.
   */
  explicit ChronoTable(const Positions defaultPositions);

  /** \brief return reference to table of current positions.
   *  \return reference fo current positions to be changed.
   */
  Positions &currentPos(void)
  {
    return cur_;
  }

  /** \brief compute new entry table basing on new settings.
   */
  void update(void);

  /** \brief get preprocessed entries, so that it can be applied to the ports.
   */
  const EntryTable &currentEntries(void) const
  {
    return e_;
  }

  /** \brief get entry of a given number.
   *  \param p entry number to get.
   *  \return requested entry.
   */
  const Entry get(const uint8_t p) const
  {
    return e_[p];
  }

private:
  Positions          cur_;          // current positions
  EntryTable         e_;            // processed entries
  const uint8_t      defMaskPB_;    // mask to apply to all entries of port B
  const uint8_t      defMaskPC_;    // mask to apply to all entries of port C
  const uint8_t      defMaskPD_;    // mask to apply to all entries of port D
  PersistentSettings settings_;     // <min;default;max> settings for each servo
}; // class ChronoTable

#endif
