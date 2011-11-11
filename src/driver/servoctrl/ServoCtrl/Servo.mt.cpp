#include <iostream>

#include "ServoCtrl/Servo.hpp"

using namespace std;
using namespace ServoCtrl;

int main(int argc, char **argv)
{
  if(argc!=1+1+2)
  {
    cerr<<argv[0]<<" <dev> <x-servo> <y-servo>"<<endl;
    return 1;
  }

  CommDevicePtrNN dev{ new CommDevice{argv[1]} };
  Servo           servoX{ ServoName{argv[2][0]}, dev };
  Servo           servoY{ ServoName{argv[3][0]}, dev };

  cout<<"sedninf 1\n";
  servoX.setPos(50);
  usleep(1*1000*1000);

  cout<<"sedninf 2\n";
  servoX.setPos(200);

  return 0;
}
