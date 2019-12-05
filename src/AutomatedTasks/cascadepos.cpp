#include "vex.h"

using namespace vex;

#include "robot-config.h"
#include "common.h"

//cascadepos is for usercontrol
//cascadeHold is for autonomous (uses holdTarget)

double LiftTarget;

void alwaysStack();

void cascadepos() {
  double kp = 500;
  double error = 0;
  double integral = 0;
  double ki = 0;
  double derivative = 0;
  double kd = 0;
  double lastError = 0;
  LiftMotor.setPosition(0,degrees);
  while(true){
    this_thread::sleep_for(20);
    //find the target
    if(Controller1.ButtonLeft.pressing()){
     LiftTarget = 1.5;
    }
    else if (Controller1.ButtonRight.pressing()){
      LiftTarget = 2;
    }
    else if (Controller1.ButtonUp.pressing()){
      LiftTarget = 2.5;
    }
    else if (Controller1.ButtonDown.pressing()){
      LiftTarget = -2.0;
    }
    else{
      LiftTarget = -1.0;
    }
    //P loop
    if(LiftTarget > -1.0){
      CascadeRunning = true;
      while(!Controller1.ButtonX.pressing() || Controller1.Axis2.value() < -15 || Controller1.Axis2.value() > 15){ //go until stopped with the X button or joystick interrupt
        error = LiftTarget - LiftMotor.position(turns);
        derivative = error - lastError;
        integral += error;
        LiftMotor.spin(forward, (error * kp)+ (integral * ki) + (derivative * kd), pct);
        this_thread::sleep_for(20);
      }
      LiftMotor.stop();
      CascadeRunning = false;
      interrupted = false;
    }
    if(LiftTarget == -2.0){
      thread t2(alwaysStack);
      while(!Controller1.ButtonX.pressing() && Controller1.Axis2.value() > -15 && Controller1.Axis2.value() < 15){
        this_thread::sleep_for(100);
      }
      //thread(alwaysStack).interrupt();
      // OR t2.interrupt();
      //If above doesn't work add the while interrupt conditions back in, they were stupid wrong last time.
      //Try to get this running first for consistent termination
      interrupted = true;
    }
    if(LiftTarget == -2.0){
      this_thread::sleep_for(20);
      CascadeRunning = false;
      interrupted = false;
    }
  }
}

void cascadeHold(){
  double kp = 500;
  double error = 0;
  double integral = 0;
  double ki = 0;
  double derivative = 0;
  double kd = 0;
  double lastError = 0;
  CascadeRunning = true;
   while(!interrupted){
     error = holdTarget - LiftMotor.position(turns);
     derivative = error - lastError;
     integral += error;
     LiftMotor.spin(forward, (error * kp)+ (integral * ki) + (derivative * kd), pct);
     this_thread::sleep_for(20);
   }
   LiftMotor.stop();
   CascadeRunning = false;
}
