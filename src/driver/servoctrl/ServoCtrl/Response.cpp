#include "ServoCtrl/Response.hpp"

namespace ServoCtrl
{

Response::Response(Future fut):
  fut_( std::move(fut) )
{
}


bool Response::ok(void)
{
  return !error();
}


bool Response::error(void)
{
  try
  {
    const std::string r=fut_.get();
    return r.find("ERR")!=std::string::npos;
  }
  catch(...)
  {
    // errors are propagated as codes here
  }
  return true;
}

} // namespace ServoCtrl
