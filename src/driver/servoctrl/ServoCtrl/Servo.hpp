#ifndef INCLUDE_SERVOCTRL_SERVO_HPP_FILE
#define INCLUDE_SERVOCTRL_SERVO_HPP_FILE

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
      Exception{ Strm{}<<"invalid byte '"<<int{byte}<<"' when converting to hex: "<<details }
    {
    }
  }; // struct ExceptionInvalidHex

  Servo(ServoName name, CommDevicePtrNN dev_):
    name_{ std::move(name) },
    dev_{ std::move(dev_) }
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
};

} // namespace ServoCtrl

#endif
