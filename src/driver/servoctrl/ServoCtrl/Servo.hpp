#ifndef INCLUDE_SERVOCTRL_SERVO_HPP_FILE
#define INCLUDE_SERVOCTRL_SERVO_HPP_FILE

/* public header */

#include "ServoCtrl/CommDevice.hpp"
#include "ServoCtrl/ServoName.hpp"
#include "ServoCtrl/Exception.hpp"

namespace ServoCtrl
{

class Servo
{
public:
  struct ExceptionInvalidHex: public Exception
  {
    explicit ExceptionInvalidHex(const uint8_t byte, std::string details):
      Exception{ Strm{}<<"invalid byte '"<<int{byte}<<"' ("<<byte<<") when converting to hex: "<<std::move(details) }
    {
    }
  }; // struct ExceptionInvalidHex

  Servo(ServoName name, CommDevicePtrNN dev_, bool fast):
    name_{ std::move(name) },
    dev_{ std::move(dev_) },
    fast_{fast}
  {
  }

  void setPos(uint8_t pos);
  void setMinPos(uint8_t pos);
  void setMaxPos(uint8_t pos);
  void setDefaultPos(uint8_t pos);

private:
  void send(char cmd, uint8_t pos);

  ServoName       name_;
  CommDevicePtrNN dev_;
  bool            fast_;
};

} // namespace ServoCtrl

#endif
