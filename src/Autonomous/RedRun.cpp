#include "vex.h"

using namespace vex;

#include "robot-config.h"
#include "common.h"

double inchesToDeg(double inches);
void alwaysStack();
void drivestraight(int target); //in motor degrees
void driveturn(int ToAngle); //in net rotation degrees
void cascadeHold();


void Redrun(){
  thread(alwaysStack).detach();
  this_thread::sleep_for(500); //pause to allow flipout
  drivestraight(inchesToDeg(32.8));
  //you now have intaked 4 cubes
  this_thread::sleep_for(100);
  ArmMotor.spinToPosition(armout, deg, false);
  drivestraight(inchesToDeg(-28));
  driveturn(100);
  drivestraight(inchesToDeg(10));
  LiftMotor.spinTo(0, rev, 50, velocityUnits::pct, true);
  ArmMotor.spinTo(armrelease, deg);
  this_thread::sleep_for(100);
  drivestraight(inchesToDeg(-30));
  driveturn(-100);
}