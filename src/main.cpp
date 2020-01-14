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

#include "ButtonClass.h"

// A global instance of competition
competition Competition;
motor_group LiftMotors(LiftMotor,LiftMotor2);


//main.cpp variables and functions
double leftpower = 0;
double rightpower = 0;

bool CascadeRunning = false;
double CubeHeight = 0.0;
bool interrupted = false;
double holdTarget = 0;
bool IntakeAuto  = false;
bool rotright = true;
bool Bcooldown = true;


int aselection = -1; //auton selector
bool unconfirmed = true;

int sped = 100; //speed of arm motor in percent.
int armGoing = -1000;
int armout = 160;
int armrelease = 240;
int armin = 0;
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
void RedTower();
void BlueTower();

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
int xplace = 70;
int ColorChosen = 1;

lcdButton redButton(120, 22, 230, 35, "RED", "#252525","#FF2525",2);
lcdButton blueButton(480-120, 22, 230, 35, "BLUE", "#252525", "#2525FF", 2);

lcdButton confirm(300,150,100,60, "CONFIRM","#14c40e", "#0f990b", 4);
lcdButton none(xplace,90,120,40, "None", "#252525");

lcdButton redrun(xplace,135,120,40, "Red Run", "#FF2525", "#FFFFFF", 2);//, color(255, 33, 33));
lcdButton red4(xplace,180,120,40, "Red Tower", "#FF2525", "#FFFFFF", 2);//, color(255, 33, 33));
lcdButton bluerun(1000,135,120,40, "Blue Run", "#2525FF", "#FFFFFF", 2); //figure out color scheme
lcdButton blue4(1000,180,120,40, "Blue Tower", "#2525FF", "#FFFFFF", 2);


void drawTonomous(){
  Brain.Screen.clearScreen("black");
  Brain.Screen.setPenColor(white);
  Brain.Screen.setPenWidth(2);
  Brain.Screen.drawLine(0,60,500,60);
  redButton.draw();
  blueButton.draw();
  confirm.draw();
  none.draw();
  redrun.draw();
  red4.draw();
  bluerun.draw();
  blue4.draw();
}


void pre_auton(void) {
  int aselection = -1;
  bool unconfirmed = true;

  Brain.Screen.clearScreen("black");
  confirm.setPenColor("#0f990b");
  drawTonomous();

  //use coordinate presser for exact spacing

  while(unconfirmed){
    if(Brain.Screen.pressing()){
      if(redButton.pressing()){
        ColorChosen = 1;
        redrun.moveTo(xplace,redrun.yPos);
        red4.moveTo(xplace,red4.yPos);
        bluerun.moveTo(1000,bluerun.yPos);
        blue4.moveTo(1000,blue4.yPos);
        drawTonomous();
      }
      else if (blueButton.pressing()){
        ColorChosen = 2;
        redrun.moveTo(1000,redrun.yPos);
        red4.moveTo(1000,red4.yPos);
        bluerun.moveTo(xplace,bluerun.yPos);
        blue4.moveTo(xplace,blue4.yPos);
        drawTonomous();
      }
      else if (ColorChosen == 1){
        if(redrun.pressing()){
          aselection = 1;
          drawTonomous();
        }
        else if (red4.pressing()){
          aselection = 3;
          drawTonomous();
        }
      }
      else if (ColorChosen == 2){
        if(bluerun.pressing()){
          aselection = 2;
          drawTonomous();
        }
        else if (blue4.pressing()){
          aselection = 4;
          drawTonomous();
        }
      }
      if (none.pressing()){
        aselection = 0;
        drawTonomous();
      }
    }


    if(aselection >-1){  
      switch(aselection){
        case 0: Brain.Screen.printAt(200, 200, "none");
          break;
        case 1: Brain.Screen.printAt(200, 200, "Red Run");
          break;
        case 2: Brain.Screen.printAt(200, 200, "Blue Run");
         break;
        case 3: Brain.Screen.printAt(200, 200, "Red Tower");
         break;
        case 4: Brain.Screen.printAt(200, 200, "Blue Tower");
          break;
      }
      if(confirm.pressing()){
        unconfirmed = false;
      }
    }
    else{
      if(confirm.pressing()){
        Brain.Screen.printAt(150, 220, "Please select an option");
      }
    }
    this_thread::sleep_for(50);
  }
  Brain.Screen.clearScreen();
  
  

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
    RedTower();
  }
  else if(aselection == 4){
    //blue stack getting
    BlueTower();
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
    // Set the other joystick's y axis to control the velocity value of the cascade lift's motors
    if(CascadeRunning == false){
      LiftMotors.spin(forward, (Controller1.Axis2.value()), pct);
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
