#ifndef INCLUDE_SERVOCTRL_RESPONSE_HPP_FILE
#define INCLUDE_SERVOCTRL_RESPONSE_HPP_FILE

/* public header */

#include <string>
#include <future>

#include "ServoCtrl/Exception.hpp"

// TODO: tests

namespace ServoCtrl
{

/** \brief response object, representing driver's response on given command.
 */
class Response
{
public:
  /** \brief future object - short name. */
  typedef std::future<std::string>  Future;
  /** \brief promise object - short name. */
  typedef std::promise<std::string> Promise;

  /** \brief creates object from a given future.
   *  \param fut future to associate this object with.
   */
  explicit Response(Future fut);

  /** \brief checks if command succeeded.
   *  \return true if command was confirmed, false otherwise.
   */
  bool ok(void);
  /** \brief checks if command failed.
   *  \return true, if command returned an error, false otherwise.
   */
  bool error(void);

private:
  Future fut_;
}; // class Response

} // namespace ServoCtrl

#endif
