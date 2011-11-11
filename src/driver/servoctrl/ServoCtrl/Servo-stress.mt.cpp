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
  Servo           servo{ ServoName{argv[2][0]}, dev, false };

  uint8_t pos=50;
  uint8_t dp =1;
  while(true)
  {
    usleep(10*1000);
    cout<<"sending pos "<<int{pos}<<"..."<<endl;
    try
    {
      servo.setPos(pos);
    }
    catch(const std::exception &ex)
    {
      cerr<<argv[0]<<": @"<<int{pos}<<" ("<<typeid(ex).name()<<"): "<<ex.what()<<endl;
    }
    pos+=dp;
    if(pos>200)
      dp=-1;
    if(pos<50)
      dp=+1;
    continue;

    cout<<"wait a while for servos to setup..."<<endl;
    usleep(750*1000);
    cout<<endl;
  } // while(true)

return 0;
}
