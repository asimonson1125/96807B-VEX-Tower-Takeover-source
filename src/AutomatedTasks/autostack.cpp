#include "vex.h"

using namespace vex;

#include "robot-config.h"
#include "common.h"

//autonomous calls void alwaysStack

void cascadeHold();

void alwaysStack(){
  while(!Controller1.ButtonX.pressing() || Controller1.Axis2.value() < -15 || Controller1.Axis2.value() > 15){
    holdTarget = 1.5;
    interrupted = false;
    thread(cascadeHold).detach();
    if (StackBumper.pressing() == true) {
      StackerRunning = true;
      interrupted = true;
      this_thread::sleep_for(20);
      //do a stack
      holdTarget = 0;
      interrupted = false;
      thread(cascadeHold).detach();
      while(LiftMotor.position(rev)>.1){
        this_thread::sleep_for(20);
      }
      interrupted = true;
      StackerRunning = false;
    }
    this_thread::sleep_for(20);
  }
}