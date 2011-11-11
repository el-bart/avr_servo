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
  const char lut[]="0123456789abcdef";
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
  // TODO: fix this: 
  const uint8_t posNum[2]={ uint8_t{(pos&0xF0)>>4}, uint8_t{(pos&0x0F)>>0} };
  // create command
  char buf[1+1+2+1+1+1];
  buf[0]=name_.getName();
  buf[1]=cmd;
  buf[2]=toHex(posNum[0]);
  buf[3]=toHex(posNum[1]);
  //buf[4]='?';      // if checksums are not needed ;)
  buf[4]=computeChecksum(buf[0], buf[1], buf[2], buf[3]);
  buf[5]='\n';
  buf[6]=0;

  // send it
  if(fast_)
    dev_->runFast(buf);
  else
  {
    const std::string ret=dev_->run(buf);
    // TODO: is this ret needed at all?
  }
}

} // namespace ServoCtrl
