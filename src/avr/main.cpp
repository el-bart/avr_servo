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
  for(;;) {};   // watchdog will do the rest
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
#ifdef NDEBUG
  // watchdog should be enabled only in release modes
  wdg.enable();                                                     // enable watchdog
#endif
  sei();                                                            // allow interrupts globally

  const unsigned int cycleLen=20*2;
  uint8_t            lastStep=0xFF;
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
           usart.disable();
           phaseGen.rise();
           break;

      // PWM generation for servos
      case 1:
           phaseGen.generate( t1, chronoTable.currentEntries() );
           phaseGen.fall();
           usart.enable();
           // NOTE: it is very important that chrono-table is re-calculated here, and not
           //       during the initial cycle, since for certain data sets this computations
           //       can take few ms!
           chronoTable.update();
           break;

      // 'wait' in 'low' state untile cycle ends
      default:
           // process commands until next-to-last cycle (the last one that can be used)
           if(step<cycleLen-2)
           {
             proto.process( chronoTable.currentPos() );
             usart.sendData();
             break;
           }
           // during last step disable usart, to ensure RT PWM generation
           if(step==cycleLen-1)
           {
             usart.disable();
             break;
           }
           // just wait until the end of the cycle, to ensure that no delays will be introduced
           if(step<cycleLen)
             break;
           wdg.reset();         // signal watchdog system is working fine
           t2.resetStep();      // reset step counter (i.e. start new cycle)
           break;
    } // switch(current_step)

  } // while(true)

  // code never reaches here
  return 0;
} // main()

