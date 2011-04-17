/*
 * PositionsTable.hpp
 *
 */
#ifndef INCLUDE_POSITIONSTABLE_HPP_FILE
#define INCLUDE_POSITIONSTABLE_HPP_FILE

#include "config.hpp"

#include <inttypes.h>

#include "uassert.hpp"

/** \brief table of positions for servos.
 */
class PositionsTable
{
public:
  /** \brief access operator - const version.
   *  \param s servo's number to get position for.
   *  \return servo's position.
   */
  const uint16_t operator[](const uint8_t s) const
  {
    uassert( s < sizeof(pos_)/sizeof(pos_[0]) );
    return pos_[s];
  }

  /** \brief access operator - non-const version.
   *  \param s servo's number to get position for.
   *  \return servo's position.
   */
  uint16_t &operator[](const uint8_t s)
  {
    uassert( s < sizeof(pos_)/sizeof(pos_[0]) );
    return pos_[s];
  }

private:
  uint16_t pos_[8];
}; // class PositionsTable

#endif
