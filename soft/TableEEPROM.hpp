/*
 * TableEEPROM.hpp
 *
 */
#ifndef INCLUDE_TABLEEEPROM_HPP_FILE
#define INCLUDE_TABLEEEPROM_HPP_FILE

#include "config.hpp"
#include <inttypes.h>
#include <avr/eeprom.h>
#include "uassert.hpp"

/** \brief table of given elements, kept in EEPROM
 */
class TableEEPROM
{
public:
  explicit TableEEPROM(uint16_t *eeptr):
    eeptr_(eeptr)
  {
  }

  /** \brief access operator - const version.
   *  \param s servo's number to get entry for.
   *  \return servo's entry.
   */
  const uint16_t read(const uint8_t s) const
  {
    uassert(s<N);
    return eeprom_read_word(eeptr_+s);
  }

  /** \brief access operator - non-const version.
   *  \param s servo's number to get entry for.
   *  \param e entry to be written.
   */
  void write(const uint8_t s, const uint16_t e)
  {
    uassert(s<N);
    eeprom_update_word(eeptr_+s, e);
  }

private:
  uint16_t *eeptr_;
}; // class TableEEPROM

#endif
