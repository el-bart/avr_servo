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
public:
  typedef std::future<std::string>  Future;
  typedef std::promise<std::string> Promise;

  explicit Response(Future fut);

  bool ok(void);
  bool error(void);

private:
  Future fut_;
}; // class Response

} // namespace ServoCtrl

#endif
