#include "config.hpp"
#include "PersistentSettings.hpp"

PersistentSettings::PersistentSettings(void):
  posMin_(0*SERVO_COUNT),
  posDef_(1*SERVO_COUNT),
  posMax_(2*SERVO_COUNT),
  flags_(3*SERVO_COUNT)
{
  if(flags_.read(0)==flags_.read(1))
  {
    writeDefaultValues();
    flags_.write(0, 0x42);      // just mark that something has changed
  }
}

void PersistentSettings::writeDefaultValues(void)
{
  for(int i=0; i<SERVO_COUNT; ++i)
  {
    posMin_.write(i, 0x00);
    posDef_.write(i, 0xFF/2);
    posMax_.write(i, 0xFF);
  }
}
