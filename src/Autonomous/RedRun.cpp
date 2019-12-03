#include "vex.h"

using namespace vex;

#include "robot-config.h"
#include "common.h"

double inchesToDeg(double inches);
void alwaysStack();
void drivestraight(int target); //in degrees
void cascadeHold();


void Redrun(){
  thread(alwaysStack).detach();
  //drivestraight(inchesToDeg(32.8));
  //you now have intaked 4 cubes

  //rotate 45+90 deg towards goal zone
  //begin rotating arm out and drive toward the goal zone
  //release cubes
  //drive back
}