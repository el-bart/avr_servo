#ifndef INCLUDE_SERVOCTRL_EXCEPTION_HPP_FILE
#define INCLUDE_SERVOCTRL_EXCEPTION_HPP_FILE

#include <string>
#include <sstream>
#include <stdexcept>

namespace ServoCtrl
{

struct Exception: public std::runtime_error
{
  explicit Exception(std::string message):
    std::runtime_error{message}
  {
  }

protected:
  struct Strm
  {
  public:
    template<typename T>
    Strm &operator<<(const T &t)
    {
      ss_<<t;
      return *this;
    }

    operator std::string(void) const
    {
      return ss_.str();
    }

  private:
    std::stringstream ss_;
  }; // struct Strm
}; // struct Exception

} // namespace ServoCtrl

#endif
