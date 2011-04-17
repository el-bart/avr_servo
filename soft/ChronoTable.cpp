/*
 * ChronoTable.cpp
 *
 */
#include "ChronoTable.hpp"

ChronoTable::ChronoTable(void)
{
  // initially start with default startup values
  for(uint8_t i=0; i<8; ++i)
    cur_.set(i, 12000);
  // compute table for given entries
  compute();
}

namespace
{
inline void bubbleSort(uint8_t (&idxs)[8], const Table<ChronoTable::Entry, 8> &e)
{
  // find proper order of elements
  for(uint8_t i=0; i<8; ++i)
  {
    bool changed=false;
    for(uint8_t j=i+1; j<8; ++j)
    {
      if( e.get(idxs[j-1]).time_ > e.get(idxs[j]).time_ )
      {
        // swap
        const uint8_t tmp=idxs[i];
        idxs[i]=idxs[j];
        idxs[j]=tmp;
        // mark some change
        changed=true;
      }
    }
    // already sorted?
    if(!changed)
      break;
  }
} // bubbleSort()
} // unnamed namespace

void ChronoTable::compute(void)
{
  uint8_t idxs[8]={0,1,2,3,4,5,6,7};    // sorted indexes
  bubbleSort(idxs, e_);

  // find proper order of elements
  for(uint8_t i=0; i<8; ++i)
  {
  }
  // TODO
}
