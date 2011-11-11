#include "ServoCtrl/Servo.hpp"

namespace ServoCtrl
{

void Servo::setPos(uint8_t pos)
{
  send('s', pos);
}

void Servo::setMinPos(uint8_t pos)
{
  send('l', pos);
}

void Servo::setMaxPos(uint8_t pos)
{
  send('h', pos);
}

void Servo::setDefaultPos(uint8_t pos)
{
  send('d', pos);
}


namespace
{
uint8_t fromHex(const char h)
{
  if('0'<=h && h<'9')
    return h-'0';
  if('a'<=h && h<='f')
    return 10+(h-'a');
  throw Servo::ExceptionInvalidHex{h, "unexpected char"};
} // fromHex()

char toHex(const uint8_t num)
{
  if(num>0x0F)
    throw Servo::ExceptionInvalidHex{num, "number out of range"};
  const char lut[]="01234567890abcdef";
  return lut[num];
} // fromHex()

char computeChecksum(const char servoName, const char mode, const char posH, const char posL)
{
  // compute XOR of all fields
  uint8_t out=0x00;
  out^=servoName;
  out^=mode;
  out^=posH;
  out^=posL;
  // compute xor of half-bytes
  out=((out>>4)&0x0F)^(out&0x0F);
  // return as printable hex
  return toHex(out);
} // computeChecksum()
} // unnamed namespace


void Servo::send(char cmd, uint8_t pos)
{
  // create command
  char buf[1+1+2+1+1+1];
  buf[0]=toHex(name_.getName());
  buf[1]=toHex(cmd);
  buf[2]=toHex((pos&0xF0)>>4);
  buf[3]=toHex((pos&0x0F)>>0);
  buf[4]=computeChecksum(buf[0], buf[1], buf[2], buf[3]);
  buf[5]='\n';
  buf[6]=0;
  // send it
  dev_->send(buf);
  // if we're here, it means it worked
}

} // namespace ServoCtrl
