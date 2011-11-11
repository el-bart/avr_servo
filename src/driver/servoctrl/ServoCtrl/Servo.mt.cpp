#include <iostream>

#include "ServoCtrl/Servo.hpp"

using namespace std;
using namespace ServoCtrl;

int main(int argc, char **argv)
{
  if(argc!=1+2)
  {
    cerr<<argv[0]<<" <dev> <servo>"<<endl;
    return 1;
  }

  CommDevicePtrNN dev{ new CommDevice{argv[1]} };
  Servo           servo{ ServoName{argv[2][0]}, dev, true };

  for(uint8_t pos=50; pos<=200; pos+=20)
  {
    cout<<"sending pos "<<int{pos}<<"..."<<endl;
    servo.setPos(pos);
    cout<<"wait a while for servos to setup..."<<endl;
    usleep(750*1000);
    cout<<endl;
  }

  return 0;
}
