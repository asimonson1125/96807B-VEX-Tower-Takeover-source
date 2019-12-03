#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;
controller Controller1 = controller(primary);
controller Controller2 = controller(partner);
motor LeftBackMotor = motor(PORT1, ratio18_1, false);
motor RightBackMotor = motor(PORT2, ratio18_1, true);
motor LeftFrontMotor = motor(PORT3, ratio18_1, false);
motor RightFrontMotor = motor(PORT4, ratio18_1, true);
motor LiftMotor = motor(PORT5, ratio18_1, true);
motor ArmMotor = motor(PORT6, ratio18_1, false);
motor IntakeMotor = motor(PORT7, ratio18_1, false);
motor StackerMotor = motor(PORT8, ratio18_1, false);
encoder LeftQuad = encoder(Brain.ThreeWirePort.A);
encoder RightQuad = encoder(Brain.ThreeWirePort.C);
encoder BackQuad = encoder(Brain.ThreeWirePort.E);
bumper StackBumper = bumper(Brain.ThreeWirePort.G);
pot BarPot = pot(Brain.ThreeWirePort.H);


/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}