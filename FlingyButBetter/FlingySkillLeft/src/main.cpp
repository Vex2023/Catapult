/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// br                   motor         20              
// bl                   motor         11              
// ml                   motor         5               
// mr                   motor         15              
// fl                   motor         1               
// fr                   motor         10              
// cata                 motor         19              
// Controller1          controller                    
// InertialSensor       inertial      18              
// claw                 motor         9               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <iostream>


using namespace vex;
using namespace std;

// A global instance of competition
competition Competition;

inertial Inertial = inertial(PORT2);
motor_group leftDrive (bl, ml, fl);
motor_group rightDrive (br, mr, fr);
smartdrive Flingy(leftDrive, rightDrive, Inertial, 12.56,10,9,distanceUnits::in);
//for opening and closing claw
bool clawClosed = false;
// define your global instances of motors and other devices here

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
  Inertial.calibrate();
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  leftDrive.setStopping(coast);
  rightDrive.setStopping(coast);
  cata.setStopping(hold);
  claw.setStopping(hold);
  claw.setPosition(0,degrees);
  cata.setPosition(0,degrees); //set catapult starting point as 0 degree
}

/*---------------------------------------------------------------------------*/
/*                          custom defined Functions                         */
/*                                                                           */
                          
/*---------------------------------------------------------------------------*/

/**********************************/
void fireCata(){ //fires catapult
  double current_position = cata.position(degrees);
  cout<<"pos before fire="<<current_position<<"\n";
  cata.spinFor(reverse, 120, degrees, 50, velocityUnits::pct); //loading
  current_position = cata.position(degrees);
  cout<<"loading position="<<current_position<<"\n";
  //wait(0.3,sec);
  cata.spinFor(reverse, 240, degrees, 40, velocityUnits::pct); //fire and back to original position

}

/**********************************/
void runChassis (int leftSpeed, int rightSpeed)
{
  //set the wheel motor torque to max
  fl.setMaxTorque(100,pct);
  ml.setMaxTorque(100,pct);
  bl.setMaxTorque(100,pct);
  fr.setMaxTorque(100,pct);
  mr.setMaxTorque(100,pct);
  br.setMaxTorque(100,pct);

  leftDrive.spin(fwd, leftSpeed, pct);
  rightDrive.spin(fwd, rightSpeed, pct);
}


/*************************************/
void frontBrake()
{
  fl.stop(brake); 
  fr.stop(brake);
  ml.stop(brake);
  mr.stop(brake);
  bl.stop(coast);
  br.stop(coast);
}

/*************************************/
void rearBrake()
{
  fl.stop(coast); 
  fr.stop(coast);
  ml.stop(brake);
  mr.stop(brake);
  bl.stop(brake);
  br.stop(brake);
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
void closeClaw()
{
  if(!clawClosed)
  {
    claw.spinToPosition(0,degrees);
    clawClosed = true;
  }
  else if(clawClosed)
  {
    claw.spinToPosition(-78, degrees);
    double current_position = claw.position(degrees);
    cout<<"closed claw pos="<<current_position<<"\n";
    // claw.spinFor(reverse, 70, degrees, 30, velocityUnits::pct);
    clawClosed = false;
  }
  cout << clawClosed << "R1 pressing" << endl;
}
void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................
    /*rightDrive.spin(fwd);
    leftDrive.spin(fwd);
    rightDrive.setVelocity(Controller1.Axis2.position()-(0.5*Controller1.Axis1.position()),pct);
    leftDrive.setVelocity(Controller1.Axis2.position()+(0.5*Controller1.Axis1.position()),pct);*/

    int a1 = Controller1.Axis4.position();
    int a2 = Controller1.Axis3.position();
   // cout<<"a1="<<a1<<" a2="<<a2<<"\n";
    if (abs(a1)>10 || abs(a2)>10) //to avoid joystick drift and random touch
    {
      int leftSpeed = a2 + a1*0.6; //0.6 is used to reduce to the turn speed
      int rightSpeed = a2 - a1*0.6; //0.6 is used to reduce to the turn speed
      runChassis(leftSpeed,rightSpeed);
    }
    else
    {
      if(bl.direction()==fwd || br.direction()==fwd) 
        frontBrake();
      else rearBrake();
    }


    
    if(Controller1.ButtonL1.pressing()){ //fire catapult
      //cata.spinToPosition(-80,degrees);0
      //cata.setTimeout(2,seconds);

      cata.spinFor(reverse, 360, degrees, 90, velocityUnits::pct);
      double current_position = cata.position(degrees);
      
      cout<<"pos after fire="<<current_position<<"\n";
      /*l.spinToPosition(70, degrees);
      grabby.driveFor(fwd, 2, inches);
      wait(0.5,sec);*/
      //c.set(false);
    } 

    if(Controller1.ButtonB.pressing()){ //fire catapult
      
      double current_position = cata.position(degrees);
      cout<<"start cata pos="<<current_position<<"\n";
      //cata.spinToPosition(30,degrees);
      cata.spinFor(reverse, 120, degrees, 40, velocityUnits::pct);
      current_position = cata.position(degrees);
      cout<<"cata loading pos="<<current_position<<"\n";
     // cata.spinToPosition()
    }

    if(Controller1.ButtonY.pressing()){//print all motor temperatures
      Brain.Screen.clearScreen();
      Brain.Screen.setCursor(1,0);
      Brain.Screen.print(" blT=");
      Brain.Screen.print(bl.temperature(celsius));
      Brain.Screen.print(" mlT=");
      Brain.Screen.print(ml.temperature(celsius));
      Brain.Screen.print(" flT=");
      Brain.Screen.print(fl.temperature(celsius));
      Brain.Screen.newLine();
      Brain.Screen.print("brT=");
      Brain.Screen.print(br.temperature(celsius));
      Brain.Screen.print(" mrT=");
      Brain.Screen.print(mr.temperature(celsius));
      Brain.Screen.print(" frT=");
      Brain.Screen.print(fr.temperature(celsius));
      Brain.Screen.newLine();
      Brain.Screen.print("CataT=");
      Brain.Screen.print(cata.temperature(celsius));
    }
    Controller1.ButtonR1.pressed(closeClaw);
   /* if(Controller1.ButtonR1.pressing())
    {
      if(!clawClosed)
      {
        claw.spinToPosition(0,degrees);
        clawClosed = true;
      }
      else if(clawClosed)
      {
        claw.spinToPosition(-78, degrees);
        double current_position = claw.position(degrees);
        cout<<"closed claw pos="<<current_position<<"\n";
        // claw.spinFor(reverse, 70, degrees, 30, velocityUnits::pct);
        clawClosed = false;
      }
      cout << clawClosed << "R1 pressing" << endl;
      wait(200,msec);
    }*/


    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}




void skill(){
  //Flingy.driveFor(fwd, 8, inches); //move drivetrain to approx. loading position
  //Flingy.turnToHeading(315, deg); //align both wheels
  //Flingy.driveFor(fwd, 2, inches); //both wheels touch loading bar, more stable
  Flingy.drive(fwd, 1, velocityUnits::pct); //prevents wheels from bouncing off bar, pushes against bar
  for(int iter = 0; iter < 43; ++iter){
    
    cout<< "iter= "<<iter<<"\n"; //print number of times fired
    fireCata(); //fires catapult
  }
}

/***********************************************************************/
/*Auton program for goal side
  Starts with a preload in the fork
  Places preload into the goal
  Then, gets the green acorn in the middle into the goal*/
void goalSideAuton(){
  /*Pushes the preload into the goal*/
  Flingy.driveFor(fwd, 44, inches, 50, velocityUnits::pct);
  Flingy.setTurnVelocity(15, velocityUnits::pct);
  Flingy.turnToHeading(90, degrees);
  Flingy.setTimeout(2, sec);
  Flingy.driveFor(fwd, 14, inches);

  /*Get the middle green acorn and pushes it into the goal*/
  Flingy.driveFor(reverse, 35, inches, 50, velocityUnits::pct);
  Flingy.turnFor(vex::left, 35, degrees);
  Flingy.driveFor(fwd, 15, inches, 50, velocityUnits::pct);
  Flingy.turnFor(vex::right, 35, degrees);
  Flingy.driveFor(fwd, 23, inches, 50, velocityUnits::pct); 
}

/***********************************************************************/
/*Auton program for match load side
  Starts with a preload in the catapult
  Shoots preload
  Rams green triball under the bar
  */
void matchLoadSideAuton(){
  fireCata();
  Flingy.driveFor(reverse, 24, inches, 90, velocityUnits::pct);
}
//
// Main will set up the competition functions and callbacks.
//
int main() {
  pre_auton();
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(skill); //slot 1
  //Competition.autonomous(goalSideAuton); //slot 2
  //Competition.autonomous(matchLoadSideAuton); //slot 3
  //Competition.autonomous(fireCata); //slot 3

  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
