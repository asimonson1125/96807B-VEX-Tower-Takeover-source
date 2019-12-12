#include "vex.h"

using namespace vex;

#include "robot-config.h"
#include "common.h"

//do stuff with the arm motor, it'll be good.


void armGo(int targetti){
  if(!ArmMotor.isSpinning()){
    ArmMotor.spinTo(targetti, deg, false);
    while(ArmMotor.isSpinning() || armGoing != targetti){
      this_thread::sleep_for(100);
    }
    if(armGoing == targetti){
      ArmMotor.stop();
    }
  }
}
