/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       xxxxxxxxxxxxxxx                                           */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition main file                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*KNOWN ERRORS
- none (noice)
*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;


//main.cpp variables and functions
double leftpower = 0;
double rightpower = 0;

bool CascadeRunning = false;
double CubeHeight = 0.0;
bool interrupted = false;
double holdTarget = 0;
int armAllIn = -20;
int armAllOut = 1000;
int armAllOutReleased = 1300;
bool IntakeAuto  = false;
bool rotright = true;
bool Bcooldown = true;

int aselection = -1; //auton selector
double maxHeightPrior = 0;

int sped = 100; //speed of arm motor in percent.
int armGoing = -1000;
bool StackerRunning = false;


//tracking WIP (DO NOT TOUCH)
double currentA = 0;
double currentX = 0;
double currentY = 0;



//functions
void cascadepos(); //cascade PID (cascadepos.cpp)

void screenflash(); //screen display (screenfunctions.cpp)

void drivestraight(int target); //drive PID (drivePID.cpp)
void driveturn(int ToAngle);

void testSCurve();

//autonomous functions (in corresponding files)
void Redrun();
void Bluerun();
void Red4();
void Blue4();

void BButtonCooldown (){
  Bcooldown = false;
  this_thread::sleep_for(500);
  Bcooldown = true;
}


/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  LiftMotor.setRotation(0, rev);
  ArmMotor.setRotation(0, rev);
  LeftQuad.setRotation(0, rev);
  RightQuad.setRotation(0, rev);
  ArmMotor.setVelocity(sped, pct);
  ArmMotor.setBrake(hold);
  while(aselection == -1){
    if(LiftMotor.rotation(rev)>maxHeightPrior){
      maxHeightPrior = LiftMotor.rotation(rev);
    }
    if(maxHeightPrior>.5 && LiftMotor.rotation(rev)<.1){
      Brain.Screen.printAt(200,0,"you have reached %f",LiftMotor.rotation(rev));
      aselection = (int)(maxHeightPrior * 2);
      Brain.Screen.printAt(200,100,"and have selected auton number %d",aselection);
      switch(aselection){
        case 0: Brain.Screen.printAt(200, 200, "none");
        case 1: Brain.Screen.printAt(200, 200,"Red Run");
        case 2: Brain.Screen.printAt(200, 200,"Blue Run");
        case 3: Brain.Screen.printAt(200, 200,"Red 4");
        case 4: Brain.Screen.printAt(200, 200,"Blue 4");
      }
    }
    Brain.Screen.printAt(100, 150, "%f",LiftMotor.rotation(rev));
    this_thread::sleep_for(50);
  }
  
  

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  if(aselection == 1){
    //red line snorting
    Redrun();
  }
  else if(aselection == 2){
    //blue line snorting
    Bluerun();
  }
  else if(aselection == 3){
    //red stack getting
    Red4();
  }
  else if(aselection == 4){
    //blue stack getting
    Blue4();
  }
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  thread(cascadepos).detach();
  thread(screenflash).detach();

  while(1) {
    // Drive Control
    if(Controller1.Axis3.value() + Controller1.Axis4.value() > 0) {
      leftpower = pow((Controller1.Axis3.value() + Controller1.Axis4.value()),1.67321)*.0384119;
    }
    else{
      leftpower = -1*(pow(abs(Controller1.Axis3.value() + Controller1.Axis4.value()),1.67321)*.0384119);
    }
    if(Controller1.Axis3.value() - Controller1.Axis4.value() > 0) {
      rightpower = pow((Controller1.Axis3.value() - Controller1.Axis4.value()),1.67321)*.0384119;
    }
    else{
      rightpower = -1*(pow(abs(Controller1.Axis3.value() - Controller1.Axis4.value()),1.67321)*.0384119);
    }

    LeftFrontMotor.spin(forward, leftpower, pct);
    LeftBackMotor.spin(forward, leftpower, pct);
    RightFrontMotor.spin(forward, rightpower, pct);
    RightBackMotor.spin(forward, rightpower, pct);


    // Lift Control
    // Set the other joystick's y axis to control the velocity value of the cascade lift's motor(s?)
    if(CascadeRunning == false){
      LiftMotor.spin(forward, (Controller1.Axis2.value()), pct);
    }

    // Arm Control
    // Use L1 and L2 buttons for directional actuation, if no button is pressed, turn off motor
    if (Controller1.ButtonL1.pressing()) {
      ArmMotor.spin(forward, 100, pct);
    }
    else if (Controller1.ButtonL2.pressing()) {
      ArmMotor.spin(forward, -100, pct);
    }
    else {
      ArmMotor.stop();
    }

/*
    if(buttonpressing){  //go to back
      armGo(-20);
    }

    if(buttonpressing2){  //go to front
      armGo(1000);
    }

    if(buttonpressing){  //release payload
      armGo(1500);
    }

*/


    // Intake Control
    // Always on unless manually interrupted with R1 and R2 buttons.  Restart auto-spin when both are pressed.
    if (Controller1.ButtonR1.pressing() && Controller1.ButtonR2.pressing()) {
      IntakeAuto = true;
    }
    else if (Controller1.ButtonR1.pressing()) {
      IntakeMotor.spin(forward, 100, pct);
      IntakeAuto = false;
    }
    else if (Controller1.ButtonR2.pressing()) {
      IntakeMotor.spin(forward, -100, pct);
      IntakeAuto = false;
    }
    else {
      if (IntakeAuto == false) {
        IntakeMotor.stop();
      }
      else{ //IntakeAuto is running, continue.
        IntakeMotor.spin(forward, 100, pct);
      }
    }


    // Sleep the task for a short amount of time to prevent wasted resources.
    this_thread::sleep_for(20); 
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
