#ifndef INCLUDE_SERVOCTRL_RESPONSE_HPP_FILE
#define INCLUDE_SERVOCTRL_RESPONSE_HPP_FILE

/* public header */

#include <string>
#include <future>

#include "ServoCtrl/Exception.hpp"

// TODO: doxygen
// TODO: tests

namespace ServoCtrl
{

class Response
{
private:
  typedef std::future<std::string> FutureValue;

public:
  explicit Response(FutureValue fv);

  bool ok(void);
  bool error(void);

private:
  FutureValue fv_;
}; // class Response

} // namespace ServoCtrl

#endif