#include "ServoCtrl/Response.hpp"

namespace ServoCtrl
{

Response::Response(FutureValue fv):
  fv_( std::move(fv) )
{
}


bool Response::ok(void)
{
  return !error();
}


bool Response::error(void)
{
  const std::string r=fv_.get();
  return r.find("ERR")!=std::string::npos;
}

} // namespace ServoCtrl
