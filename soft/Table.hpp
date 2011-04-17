/*
 * Table.hpp
 *
 */
#ifndef INCLUDE_TABLE_HPP_FILE
#define INCLUDE_TABLE_HPP_FILE

#include "config.hpp"
#include <inttypes.h>
#include "uassert.hpp"

/** \brief table of given elements
 */
template<typename T, uint8_t N>
class Table
{
public:
  /** \brief access operator - const version.
   *  \param s servo's number to get entry for.
   *  \return servo's entry.
   */
  const T operator[](const uint8_t s) const
  {
    uassert(s<N);
    return t_[s];
  }

  /** \brief access operator - non-const version.
   *  \param s servo's number to get entry for.
   *  \return servo's entry.
   */
  uint16_t &operator[](const uint8_t s)
  {
    uassert(s<N);
    return t_[s];
  }

private:
  T t_[N];
}; // class Table

#endif
