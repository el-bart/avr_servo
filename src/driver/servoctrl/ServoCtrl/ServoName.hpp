#ifndef INCLUDE_SERVOCTRL_SERVONAME_HPP_FILE
#define INCLUDE_SERVOCTRL_SERVONAME_HPP_FILE

/* public header */

#include "ServoCtrl/Exception.hpp"


namespace ServoCtrl
{

class ServoName
{
public:
  struct ExceptionInvalidServo: public Exception
  {
    explicit ExceptionInvalidServo(const char name):
      Exception{ Strm{}<<"invalid servo name '"<<name<<"'" }
    {
    }
  }; // struct ExceptionInvalidServo


  explicit ServoName(const char name):
    name_{ check(name) }
  {
  }

  char getName(void) const
  {
    return name_;
  }

  static constexpr char first(void)
  {
    return 'a';
  }
  static constexpr char last(void)
  {
    return 'q';
  }

private:
  char check(const char name)
  {
    if(name<first() || last()<name)
      throw ExceptionInvalidServo(name);
    return name;
  }

  char name_;
}; // class ServoName

} // namespace ServoCtrl

#endif
