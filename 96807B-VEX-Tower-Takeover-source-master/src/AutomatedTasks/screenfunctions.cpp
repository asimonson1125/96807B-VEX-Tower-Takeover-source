#include "vex.h"

using namespace vex;

#include "robot-config.h"
#include "common.h"

//main.cpp calls the 'screenflash' function, which is defined at the bottom.
//This calls all of the other functions in this file


void basicPlot(){
  //three segments, cascade position (flash if auto running), segment 2, and stacker position (flashing if stacking).
  Brain.Screen.setPenColor(white);
  Brain.Screen.setPenWidth(2);
  Brain.Screen.drawLine(150, 0, 150, 400);
  Brain.Screen.drawLine(325, 0, 325, 400);

  Brain.Screen.setFont(fontType::mono30) ;
  Brain.Screen.printAt(15, 50, "Cascade");
  Brain.Screen.printAt(175,50,"Release");
  Brain.Screen.printAt(345,50,"Stacker");
    
  Brain.Screen.setFont(fontType::mono20);
  Brain.Screen.printAt(15, 225, "%f",LiftMotor.position(turns));
  Brain.Screen.printAt(200,225,"%f",LeftQuad.rotation(deg));
  Brain.Screen.printAt(350,225,"%f",RightQuad.rotation(deg));
}

void screenflash(){
  Brain.Screen.setPenColor(white);
  Brain.Screen.setPenWidth(2);
  while(1){
    if(CascadeRunning){
      Brain.Screen.drawRectangle(0, 0, 147, 400, red);
    }
    if(ArmMotor.rotation(deg)>200){
      Brain.Screen.drawRectangle(153, 0, 147, 400,red);
    }
    if(StackerRunning){
      Brain.Screen.drawRectangle(328, 0, 147, 400,red);
    }
    basicPlot();
    this_thread::sleep_for(100);
    Brain.Screen.clearScreen();
    basicPlot();
    this_thread::sleep_for(100);
  }
}
