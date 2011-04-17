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
  /** \brief value held inside. */
  typedef T value_type;

  /** \brief access operator - const version.
   *  \param s servo's number to get entry for.
   *  \return servo's entry.
   */
  const T get(const uint8_t s) const
  {
    uassert(s<N);
    return t_[s];
  }

  /** \brief access operator - non-const version.
   *  \param s servo's number to get entry for.
   *  \param v new value.
   *  \return servo's entry.
   */
  void set(const uint8_t s, const uint16_t v)
  {
    uassert(s<N);
    t_[s]=v;
  }

private:
  T t_[N];
}; // class Table

#endif
