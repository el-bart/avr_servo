#ifndef INCLUDE_PERSISTENTSETTINGS_HPP_FILE
#define INCLUDE_PERSISTENTSETTINGS_HPP_FILE

#include "config.hpp"
#include "PersistentTable.hpp"

class PersistentSettings
{
private:
  typedef PersistentTable<SERVO_COUNT> ServoTable;

public:
  PersistentSettings(void);

  ServoTable &posMin(void)
  {
    return posMin_;
  }
  const ServoTable &posMin(void) const
  {
    return posMin_;
  }

  ServoTable &posMax(void)
  {
    return posMax_;
  }
  const ServoTable &posMax(void) const
  {
    return posMax_;
  }

  ServoTable &posDef(void)
  {
    return posDef_;
  }
  const ServoTable &posDef(void) const
  {
    return posDef_;
  }

private:
  ServoTable posMin_;
  ServoTable posDef_;
  ServoTable posMax_;
}; // class PersistentSettings

#endif
