#include "vex.h"

using namespace vex;

#include "robot-config.h"
#include "common.h"


//includes drivestraight and driveturn plus conversions

double inchesToDeg(double inches){
  //3.75" diameter wheels
  return (inches/11.780972450961723)*360;
}

double degToInches(double deg){
  //3.75" diameter wheels
  return((deg/360)*11.780972450961723);
}


void trackme(){
  double leftCurrent;
  double rightCurrent;
  double backCurrent;

  double leftChange;
  double rightChange;
  double backChange;

  double leftPrevious = 0;
  double rightPrevious = 0;
  double backPrevious = 0;

  double leftToCenter = 8.5;
  double rightToCenter = 8.5;
  double backToCenter = 8.5;

  double h; // The hypotenuse of the triangle formed by the middle of the robot on the starting position and ending position and the middle of the circle it travels around
	double i; // Half on the angle that I've traveled
	double h2; // The same as h but using the back instead of the side wheels
  double a; // The angle that I've traveled


  while(1){
    //get new values
    leftCurrent = LeftQuad.position(deg);
    rightCurrent = RightQuad.position(deg);
    backCurrent = BackQuad.position(deg);

    //get new change
    leftChange = degToInches(leftCurrent - leftPrevious);
    rightChange = degToInches(rightCurrent - rightPrevious);
    backChange = degToInches(backCurrent - backPrevious);

    //new values are now old
    leftPrevious  = leftCurrent;
    rightPrevious = rightCurrent;
    backPrevious = backCurrent;


	a = (leftChange - rightChange) / (leftToCenter + rightToCenter); 
	if (a)
	{
		double r = rightChange / a; // The radius of the circle the robot travel's around with the right side of the robot
		i = a / 2.0;
		double sinI = sin(i);
		h = ((r + rightToCenter) * sinI) * 2.0;

		double r2 = backChange / a; // The radius of the circle the robot travel's around with the back of the robot
		h2 = ((r2 + backToCenter) * sinI) * 2.0;
	}
	else
	{
		h = rightChange;
		i = 0;

		h2 = backChange;
	}
	double p = i + currentA; // The global ending angle of the robot
	double cosP = cos(p);
	double sinP = sin(p);

	// Update the global position
	currentY += h * cosP;
	currentX += h * sinP;

	currentY += h2 * -sinP; // -sin(x) = sin(-x)
	currentX += h2 * cosP; // cos(x) = cos(-x)

	currentA += a;

    this_thread::sleep_for(5);
  }

}



void testSCurve(){
  double Kp = .4;
  LeftQuad.resetRotation();
  RightQuad.resetRotation();
  LeftFrontMotor.spin(reverse, 60, pct);
  LeftBackMotor.spin(reverse, 60, pct);
  while(LeftQuad.rotation(deg) < inchesToDeg(33.51032164)){
    RightFrontMotor.spin(reverse,(LeftQuad.rotation(deg)-((RightQuad.rotation(deg)*2.13333333))*Kp)+60,pct);
    RightBackMotor.spin(reverse,(LeftQuad.rotation(deg)-((RightQuad.rotation(deg)*2.13333333))*Kp)+60,pct);
    this_thread::sleep_for(5);
  }
  LeftQuad.resetRotation();
  RightQuad.resetRotation();
  RightFrontMotor.spin(reverse, 60, pct);
  RightBackMotor.spin(reverse, 60, pct);
  while(RightQuad.rotation(deg) < inchesToDeg(33.51032164)){
    LeftFrontMotor.spin(reverse,(RightQuad.rotation(deg)-((LeftQuad.rotation(deg)*2.13333333))*Kp)+60,pct);
    LeftBackMotor.spin(reverse,(RightQuad.rotation(deg)-((LeftQuad.rotation(deg)*2.13333333))*Kp)+60,pct);
    this_thread::sleep_for(5);
  }
}



void drivestraight(int target) {
  target *= 360;
  double kp = .4;
  double error = 0;
  double integral = 0;
  double ki = 0;
  double derivative = 0;
  double kd = 0;
  double lastError = 0;
  target += LeftQuad.position(degrees);
  while(LeftQuad.position(degrees) < target){
    double lpower = (target- LeftQuad.position(degrees))*.4;
    if (lpower > 55){
      lpower = 55;
    }
    error = LeftQuad.position(degrees) - RightQuad.position(degrees);
    integral += error;
    derivative = error - lastError;
    LeftFrontMotor.spin(forward, lpower, pct);
    LeftBackMotor.spin(forward, lpower, pct);

    RightFrontMotor.spin(forward, lpower + (error * kp) + (integral * ki) + (derivative * kd), pct);
    RightBackMotor.spin(forward, lpower + (error * kp) + (integral * ki) + (derivative * kd), pct);
    lastError = error;
    this_thread::sleep_for(20);
  }
  LeftFrontMotor.stop();
  LeftBackMotor.stop();
  RightFrontMotor.stop();
  RightBackMotor.stop();
}



void driveturn(int ToAngle){
  //egg
}

