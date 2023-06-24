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
// br                   motor         1               
// bl                   motor         10              
// ml                   motor         5               
// mr                   motor         15              
// fl                   motor         20              
// fr                   motor         11              
// cata                 motor         6               
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

motor_group leftDrive (bl, ml, fl);
motor_group rightDrive (br, mr, fr);
drivetrain Flingy(leftDrive, rightDrive, 12.56,10,9,distanceUnits::in);

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

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  leftDrive.setStopping(coast);
  rightDrive.setStopping(coast);
  cata.setStopping(hold);
  cata.setVelocity(25,percent);
  cata.setPosition(0,degrees); //set catapult starting point as 0 degree
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
    rightDrive.spin(fwd);
    leftDrive.spin(fwd);
    rightDrive.setVelocity(Controller1.Axis2.position()-Controller1.Axis1.position(),pct);
    leftDrive.setVelocity(Controller1.Axis2.position()+Controller1.Axis1.position(),pct);

    if(Controller1.ButtonR1.pressing()){ //fire catapult
      cata.spinToPosition(-80,degrees);
      /*l.spinToPosition(70, degrees);
      grabby.driveFor(fwd, 2, inches);
      wait(0.5,sec);*/
      //c.set(false);
    }

    if(Controller1.ButtonB.pressing()){ //fire catapult
      cata.spinToPosition(-30,degrees);
     // cata.spinToPosition(
    }

   
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
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
