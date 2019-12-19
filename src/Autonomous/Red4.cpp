#include "vex.h"

using namespace vex;

#include "robot-config.h"
#include "common.h"

double inchesToDeg(double inches);
void alwaysStack();
void drivestraight(int target); //in degrees
void cascadeHold();


void RedTower(){
  thread(alwaysStack).detach();

  //rotate arm out and open passive
  //drivestraight(inchesToDeg(32.8));
  this_thread::sleep_for(80);
  //close passive and rotate arm back in

  //rotate 90+45 degrees to point at last cube and goal zone
  //drive into cube and to goal zone while lifting lift just a little bit and rotating arm in to pick up internally
  interrupted = false;
  holdTarget = 0;
  thread(cascadeHold).detach();
  while(LiftMotor.position(deg) > 25){
    this_thread::sleep_for(20);
  }
  interrupted = true;
  this_thread::sleep_for(20);
  interrupted = false;
  //rotate arm out and place cubes
  //drivestraight(inchesToDeg(-10));
}