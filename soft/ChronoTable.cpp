/*
 * ChronoTable.cpp
 *
 */
#include "ChronoTable.hpp"

ChronoTable::ChronoTable(void):
  min_( (uint16_t*)(0x01+0*8) ),
  max_( (uint16_t*)(0x01+1*8) ),
  start_( (uint16_t*)(0x01+2*8) )
{
  // if caules couldn't be read from EEPROM create default entries
  if( !readFromEEPROM() )
  {
    // create some initial values
    for(uint8_t i=0; i<8; ++i)
    {
      min_.write(i,    8*1000);
      max_.write(i,   16*1000);
      start_.write(i, 12*1000);
    }
    writeToEEPROM();
  }

  // initially start with default startup values
  for(uint8_t i=0; i<8; ++i)
    cur_[i]=start_.read(i);

  // compute table for given entries
  compute();
}

bool ChronoTable::readFromEEPROM(void)
{
  // TODO: min, max, start
  return false;
}

void ChronoTable::writeToEEPROM(void)
{
  // TODO: min, max, start
}

void ChronoTable::compute(void)
{
  // first normalize entry to match min/max
  for(uint8_t i=0; i<8; ++i)
  {
    const uint16_t min=min_.read(i);
    if(cur_[i]<min)
      cur_[i]=min;
    else
    {
      const uint16_t max=max_.read(i);
      if(cur_[i]>max)
        cur_[i]=max;
    }
  }

  // sort entries
  // TODO
}
