using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern controller Controller2;
extern motor LeftBackMotor;
extern motor RightBackMotor;
extern motor LeftFrontMotor;
extern motor RightFrontMotor;
extern motor LiftMotor;
extern motor ArmMotor;
extern motor IntakeMotor;
extern motor StackerMotor;
extern bumper StackBumper;
extern pot BarPot;
extern encoder RightQuad;
extern encoder LeftQuad;
extern encoder BackQuad;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );