#include "config.hpp"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ChronoTable.hpp"


ChronoTable::ChronoTable(const Positions defaultPositions)
{
  // initially start with default values
  cur_=defaultPositions;
  // compute and update table for given entries
  update();
}


namespace
{
inline void bubbleSort(uint8_t (&idxs)[SERVO_COUNT], const ChronoTable::Positions &c)
{
  // find proper order of elements
  for(uint8_t i=0; i<SERVO_COUNT; ++i)
  {
    bool changed=false;
    for(uint8_t j=1; j<SERVO_COUNT; ++j)
    {
      if( c[idxs[j-1]] > c[idxs[j]] )
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


void servoNoToMask(const uint8_t servoNo, Entry &out)
{
  if(servoNo<6) // PORTB?
  {
    const uint8_t tmp=servoNo;
    out.maskPB_&=~_BV(tmp);
    return;
  }
  if(servoNo<12) // PORTC?
  {
    const uint8_t tmp=servoNo-6;
    out.maskPC_&=~_BV(tmp);
    return;
  }
  if(servoNo<18) // PORTD?
  {
    const uint8_t tmp=servoNo-12+2;
    out.maskPD_&=~_BV(tmp);
    return;
  }
  // if we're here, it means invalid servo number. entry will be skipped...
} // servoNoToMask()
} // unnamed namespace


void ChronoTable::update(void)
{
  uint8_t idxs[SERVO_COUNT]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};  // sorted indexes
  bubbleSort(idxs, cur_);                                                   // find proper order of elements

  uint8_t idxOut=0;
  for(uint8_t i=0; i<SERVO_COUNT; ++i)
  {
    // make default mask for this element
    Entry entry={cur_[idxs[i]], 0xFF, 0xFF, 0xFF};
    servoNoToMask(idxs[i], entry);
    // try packing multiple entries into one, if possible
    while( i<SERVO_COUNT-1 && cur_[idxs[i]]==cur_[idxs[i+1]] )
    {
      ++i;
      servoNoToMask(idxs[i], entry);
    }
    // save this entry
    e_[idxOut]=entry;
    ++idxOut;
  }

  // fill up missing elements with zeros
  const Entry zero={0, 0x00, 0x00, 0x00};
  for(uint8_t i=idxOut; i<SERVO_COUNT; ++i)
    e_[i]=zero;
}
