/*
 * USART.hpp
 *
 */
#ifndef INCLUDE_USART_HPP_FILE
#define INCLUDE_USART_HPP_FILE

#include <stdio.h>
#include <inttypes.h>
#include "ChronoTable.hpp"

/** \brief set of operations on serial interface.
 */
struct USART
{
  /** \brief initialize USART.
   */
  static void init(ChronoTable &ct);

  /** \brief send singe byte.
   *  \param b byte to send.
   */
  static void send(uint8_t b);

  static uint8_t recv(void);
}; // struct USART

#endif
