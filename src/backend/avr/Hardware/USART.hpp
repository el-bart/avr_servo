#ifndef INCLUDE_HARDWARE_USART_HPP_FILE
#define INCLUDE_HARDWARE_USART_HPP_FILE

#include "Hardware/io.hpp"


#define USART_UBRR(baud,f) ( ((f)/((baud)*16L)) -1 )
// TODO: add test for baud-rate error and show error if value is over 1%


namespace Hardware
{
struct USART
{
  static void init(void)
  {
    // clock divider register (computed from baud rate and oscilator frequency)
    UBRRH=(uint8_t)( (USART_UBRR(USART_BAUD, F_CPU)>>8) & 0x00FF );
    UBRRL=(uint8_t)( (USART_UBRR(USART_BAUD, F_CPU)>>0) & 0x00FF );

    // enable interrupts
    UCSRB|= _BV(RXCIE);   // RX complete
    UCSRB|= _BV(TXCIE);   // TX complete
    // enable transciever
    UCSRB|= _BV(RXEN);    // RX enable
    UCSRB|= _BV(TXEN);    // TX enable

    // configure proper pins as in (RX) and out (TX)
    DDRD &=~_BV(PD0);     // RX as in
    PORTD|= _BV(PD0);     // RX high with pull-up
    DDRD |= _BV(PD1);     // TX as out
  }

  static void send(uint8_t b)
  {
    UDR=b;
  }

  static uint8_t receive(void)
  {
    return UDR;
  }

  static void signalOnDataEmpty(void)
  {
    // signal on data registry empty. if transmition has not yet started this will
    // send initial (first) byte as soon as USART is ready
    UCSRB|=_BV(UDRIE);
  }

  static void onDataRegisterEmpty(void)
  {
    // disable einterrupt when data register is empty
    UCSRB&=~_BV(UDRIE);
  }
}; // struct Timer1
} // namespace Hardware

#endif
