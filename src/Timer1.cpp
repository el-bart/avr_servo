/*
 * Timer1.cpp
 *
 */
#include "Timer1.hpp"
#include "uassert.hpp"

namespace
{
uint8_t g_initCount=0;
} // unnamed namespace


Timer1::Timer1(void)
{
  // set timer 1
  Hardware::Timer1::init();
  // keep stopped by default
  stop();
  // mark initialized element
  ++g_initCount;
  uassert( g_initCount==1 );
}

Timer1::~Timer1(void)
{
  --g_initCount;
}

void Timer1::start(void)
{
  Hardware::Timer1::start();
}

void Timer1::stop(void)
{
  Hardware::Timer1::stop();
}
