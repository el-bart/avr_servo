/*
 * AVR_servo
 *
 * servo controller driver, written for AVR uC.
 *
 */
#include "config.hpp"       // this file must be included as first one!

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "USART.hpp"
#include "Timer1.hpp"
#include "Timer2.hpp"
#include "Watchdog.hpp"
#include "PowerSave.hpp"
#include "ChronoTable.hpp"
#include "CommProtocol.hpp"
#include "PhaseGenerator.hpp"
#include "uassert.hpp"


// handle unknown interruption(s)
ISR(BADISR_vect)
{
  uassert(!"unhandled interrupt");
}


namespace
{
inline Positions getDefaultPositions(const PersistentSettings &s)
{
  Positions out;
  for(uint8_t i=0; i<SERVO_COUNT; ++i)
    out[i]=s.posDef().read(i);
  return out;
} // getDefaultPositions()
} // unnamed namespace


//
// MAIN
//
int main(void)
{
#if 0
  PersistentSettings settings;                                      // persistent settings
  ChronoTable        chronoTable( getDefaultPositions(settings) );  // chronology table
  QueueSend          qSend;                                         // output queue
  QueueRecv          qRecv;                                         // input queue
  USART              usart(qSend, qRecv);                           // prepare USART to work
  Timer1             t1;                                            // configure T1
  Timer2             t2;                                            // configure T2
  PhaseGenerator     phaseGen;                                      // controling facility
  CommProtocol       proto(qSend, qRecv, settings);                 // protocol parser
  sei();                                                            // allow interrupts globally

  /*
  for(int i=0; i<SERVO_COUNT; ++i)
    chronoTable.currentPos()[i]=256/2;
    */

  uint8_t lastStep=0xFF;
  while(true)
  {

    // wait in idle mode while nothing happens
    const uint8_t step=t2.currentStep();
    if(step==lastStep)
    {
      PowerSave::idle();
      continue;
    }
    lastStep=step;

    // something has to be done - process next stage
    switch(step)
    {
      // initial 'high' state
      case 0:
           phaseGen.rise();
           chronoTable.update();
           break;

      // PWM generation for servos
      case 1:
           phaseGen.generate( t1, chronoTable.currentEntries() );
           break;

      // 'wait' in 'low' state untile the end of the cycle
      default:
           if(step<20)
           {
             proto.process( chronoTable.currentPos() );
             usart.sendData();
             break;
           }
           t2.resetStep();      // reset step counter (i.e. start new cycle)
           break;
    } // switch(current_step)

  } // while(true)
#endif

  Watchdog           wdg;                                           // initialize watchdog (disable by default)
  PersistentSettings settings;                                      // persistent settings
  ChronoTable        chronoTable( getDefaultPositions(settings) );  // chronology table
  QueueSend          qSend;                                         // output queue
  QueueRecv          qRecv;                                         // input queue
  USART              usart(qSend, qRecv);                           // prepare USART to work
  Timer1             t1;                                            // configure T1
  Timer2             t2;                                            // configure T2
  PhaseGenerator     phaseGen;                                      // controling facility
  CommProtocol       proto(qSend, qRecv, settings);                 // protocol parser
  wdg.enable();                                                     // enable watchdog
  sei();                                                            // allow interrupts globally

  uint8_t lastStep=0xFF;
  while(true)
  {

    // wait in idle mode while nothing happens
    const uint8_t step=t2.currentStep();
    if(step==lastStep)
    {
      PowerSave::idle();
      continue;
    }
    lastStep=step;

    // something has to be done - process next stage
    switch(step)
    {
      // initial 'high' state
      case 0:
           phaseGen.rise();
           chronoTable.update();
           break;

      // PWM generation for servos
      case 1:
           phaseGen.generate( t1, chronoTable.currentEntries() );
           phaseGen.fall();
           break;

      // 'wait' in 'low' state untile cycle ends
      default:
           if(step<20)
           {
             proto.process( chronoTable.currentPos() );
             usart.sendData();
             break;
           }
           wdg.reset();         // signal watchdog system is working fine
           t2.resetStep();      // reset step counter (i.e. start new cycle)
           break;
    } // switch(current_step)

  } // while(true)

  // code never reaches here
  return 0;
} // main()

