/*
 * ChronoTable.cpp
 *
 */
#include "config.hpp"
#include "Hardware/io.hpp"
#include "Hardware/interrupt.hpp"
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
inline void bubbleSort(uint8_t (&idxs)[8], const Table<uint16_t, 8> &c)
{
  // find proper order of elements
  for(uint8_t i=0; i<8; ++i)
  {
    bool changed=false;
    for(uint8_t j=1; j<8; ++j)
    {
      if( c.get(idxs[j-1]) > c.get(idxs[j]) )
      {
        // swap
        const uint8_t tmp=idxs[j-1];
        idxs[j-1]=idxs[j];
        idxs[j]  =tmp;
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
  cli();                                // disable interrupts for a moment of copying
  const Table<uint16_t, 8> cur=cur_;    // make local copy (original may change in interrupts)
  sei();                                // interrupts are welcome now
  uint8_t idxs[8]={0,1,2,3,4,5,6,7};    // sorted indexes
  bubbleSort(idxs, cur);                // find proper order of elements

  uint8_t idxOut=0;
  for(uint8_t i=0; i<8; ++i)
  {
    // make default mask for this element
    uint8_t mask=0xFF;
    mask&=~_BV(idxs[i]);
    // try packing multiple entries into one, if possible
    while( i<8-1 && cur.get(idxs[i])==cur.get(idxs[i+1]) )
    {
      ++i;
      mask&=~_BV(idxs[i]);
    }
    // save this entry
    const Entry tmp={ cur.get(idxs[i]), mask };
    e_.set(idxOut, tmp);
    ++idxOut;
  }

  // fill up missing elements with zeros
  const Entry zero={0, 0x00};
  for(uint8_t i=idxOut; i<8; ++i)
    e_.set(i, zero);
}
