/*
 * ChronoTable.cpp
 *
 */
#include "ChronoTable.hpp"

ChronoTable::ChronoTable(void)
{
  // initially start with default startup values
  for(uint8_t i=0; i<8; ++i)
    cur_[i]=8000+4000;
  // compute table for given entries
  compute();
}

void ChronoTable::compute(void)
{
  // sort entries
  // TODO
}
