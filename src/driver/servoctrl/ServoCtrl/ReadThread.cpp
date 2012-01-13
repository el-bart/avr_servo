#include "ServoCtrl/ReadThread.hpp"

namespace ServoCtrl
{

ReadThread::ReadThread(SerialPortPtrNN rd):
  quit_{false},
  rd_{rd},
  th_{ [&]{ threadBody(); } }
{
}


ReadThread::~ReadThread(void)
{
  quit_=true;
  try
  {
    // add something to queue
    q_.push( ServoName{ ServoName::first() } );
    // wait for thread to end
    if( th_.joinable() )
      th_.join();
  }
  catch(...)
  {
    // nothing more can be done...
    th_.detach();
  }
}


Response ReadThread::enqueue(ServoName s)
{
  return Response{ q_.push(s) };
}


void ReadThread::threadBody(void)
{
  while(!quit_)
  {
    try
    {
      PromisesQueue::Element e=q_.pop();
      try
      {
        // TODO: compute deadlien basing on t_
        std::string ret=rd_->readLine(40);
        // fulfill promise
        e.p_.set_value( std::move(ret) );
      }
      catch(...)
      {
        e.p_.set_exception( std::current_exception() );
      }
    }
    catch(const std::exception &)
    {
      // TODO: what to do with this error?
    }
  } // while(!quit_)
}

} // namespace ServoCtrl
