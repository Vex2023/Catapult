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
// ml                   motor         16              
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

inertial Inertial = inertial(PORT18);
motor_group leftDrive (bl, ml, fl);
motor_group rightDrive (br, mr, fr);
smartdrive Flingy(leftDrive, rightDrive, Inertial, 12.56,10,9,distanceUnits::in);
//for opening and closing claw
bool clawClosed = true;
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
void toggleClaw()
{
  cout<<"button presseed\n";
  if(!clawClosed)
  {
     cout<<" before close\n";
    claw.spinToPosition(0,degrees);
     cout<<" after close\n";

    double current_position = claw.position(degrees);
    cout<<"closed claw pos="<<current_position<<"\n";
    clawClosed = true;
  }
  else if(clawClosed)
  {
     cout<<" before open\n";
        claw.spinToPosition(78, degrees);
        cout<<"after open\n";
    double current_position = claw.position(degrees);
    cout<<"open claw pos="<<current_position<<"\n";
    // claw.spinFor(reverse, 70, degrees, 30, velocityUnits::pct);
    clawClosed = false;
  }
  cout << clawClosed << "R1 pressing" << endl;
}
void goLoadingPos() //make cata go down to loaded position
{
  double current_position = cata.position(degrees);
  cout<<"start cata pos="<<current_position<<"\n";
  //cata.spinToPosition(30,degrees);
  cata.spinFor(reverse, 120, degrees, 40, velocityUnits::pct);
  current_position = cata.position(degrees);
  cout<<"cata loading pos="<<current_position<<"\n";
  // cata.spinToPosition()
}
void shootFromLoadingPos() // make cata shoot from loaded position
{
  double current_position = cata.position(degrees);
  cout<<"loaded cata pos ="<<current_position<<"\n";
  //cata.spinToPosition(30,degrees);
  cata.spinFor(reverse, 240, degrees, 40, velocityUnits::pct);
  current_position = cata.position(degrees);
  cout<<"cata after fire pos="<<current_position<<"\n";
  // cata.spinToPosition()
}
void continuousShoot(){
  wait(0.2,sec);
  while(true)
  {
    if(Controller1.ButtonL1.pressing()){
      break;
    }
    else
    {
      cata.spinFor(reverse, 360, degrees, 90, velocityUnits::pct);
      double current_position = cata.position(degrees);
      cout<<"pos after fire="<<current_position<<"\n";
    }
  }
  wait(1.5,sec);
}
void printTemps()
{
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
      Brain.Screen.newLine();
      Brain.Screen.print("ClawT=");
      Brain.Screen.print(claw.temperature(celsius));
      Brain.Screen.print("Before");
      Brain.Screen.print("After");
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
    if (Controller1.ButtonA.pressing())
    {
      Flingy.driveFor(40, inches, 50, velocityUnits::pct);
    }

    
    /*if(Controller1.ButtonL1.pressing()){ //fire catapult
      //cata.spinToPosition(-80,degrees);0
      //cata.setTimeout(2,seconds);

      cata.spinFor(reverse, 360, degrees, 90, velocityUnits::pct);
      double current_position = cata.position(degrees);
      
      cout<<"pos after fire="<<current_position<<"\n";
      //l.spinToPosition(70, degrees);
      //grabby.driveFor(fwd, 2, inches);
      //wait(0.5,sec);
      //c.set(false);
    } */


   
   // Controller1.ButtonR1.pressed(toggleClaw); //this should not be in the while loop
 
    //if(Controller1.ButtonR1.pressing())
    //{ toggleClaw();}
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

/***********************************************************************/
/*Auton program for match load side
  Starts with a preload in the catapult
  Shoots preload
  Rams green triball under the bar
  */
void matchLoadSideAuton_1(){
  cata.spinFor(reverse, 120, degrees, 40, velocityUnits::pct);
  Flingy.driveFor(reverse, 3, inches);
  Flingy.driveFor(fwd, 8, inches);
  cata.spinFor(reverse, 240, degrees, 40, velocityUnits::pct);
  Flingy.turnFor(vex::right, 40, degrees);
  Flingy.driveFor(reverse, 25, inches);
  //Flingy.driveFor(fwd, 2, inches, 20, velocityUnits::pct);
  //wait (1, sec);
  //fireCata();
  //Flingy.driveFor(reverse, 26, inches, 35, velocityUnits::pct);
}
/***********************************************************************/
/*
autonomous to shoot the alliance acorn and remove acorn from match load zone, 
push match load acorn and acorn under elevation bar to other side.
touches elevation bar
  */
void matchLoadSideAuton_2(){
  toggleClaw(); //open claw
  cata.spinFor(reverse, 120, degrees, 40, velocityUnits::pct); //load catapult
  Flingy.driveFor(reverse, 2, inches, 20, velocityUnits::pct); //shake claw
  cata.spinFor(reverse, 240, degrees, 40, velocityUnits::pct); //shoot catapult
  Flingy.driveFor(fwd, 2, inches, 10, velocityUnits::pct);// move into to touch load zone
  claw.setTimeout(1, sec);
  toggleClaw(); //grab acorn
  Flingy.driveFor(reverse, 5, inches, 20, velocityUnits::pct); //take acorn out of match load zone
  Flingy.setTurnVelocity(10, velocityUnits::pct);
  Flingy.turnToHeading(260,degrees); // turn to diagnal
  Flingy.driveFor(fwd, 11, inches, 30, velocityUnits::pct);


  Flingy.turnToHeading(225,degrees); //facing horizontal bar
  toggleClaw(); //open claw
  Flingy.driveFor(fwd, 16, inches, 50, velocityUnits::pct); // push acorn to other side
}

void goalSideAuton() {
  Flingy.setTurnVelocity(15, velocityUnits::pct);
  toggleClaw();//open claw
  Flingy.driveFor(reverse, 2, inches, 40, velocityUnits::pct); //shake claw
  Flingy.driveFor(fwd, 2, inches, 40, velocityUnits::pct);
  toggleClaw();//close claw
  Flingy.driveFor(fwd, 32, inches, 40, velocityUnits::pct);
  Flingy.turnToHeading(80, deg);
  Flingy.driveFor(fwd, 6, inches, 50, velocityUnits::pct);
  toggleClaw();
  Flingy.driveFor(reverse, 10.5, inches, 40, velocityUnits::pct);
  Flingy.turnToHeading(235, deg);
  wait(0.5,sec);
  Flingy.driveFor(fwd, 7, inches, 40, velocityUnits::pct);
  claw.setTimeout(0.5, sec);
  toggleClaw();
  Flingy.driveFor(reverse, 5, inches, 40, velocityUnits::pct);
  Flingy.turnToHeading(90, deg);
  Flingy.driveFor(fwd, 12, inches, 40, velocityUnits::pct);
  toggleClaw();
  Flingy.driveFor(reverse, 12, inches, 40, velocityUnits::pct);
 // Flingy.driveFor(fwd, 6, inches, 40, velocityUnits::pct);
  //toggleClaw();
  //Flingy.driveFor(reverse, 5, inches, 40, velocityUnits::pct);
  //Flingy.turnToHeading(10, deg);
  //Flingy.driveFor(reverse, 5, inches, 40, velocityUnits::pct);
 // Flingy.turnToHeading(90, deg);
  //toggleClaw();
  //Flingy.driveFor(reverse, 10, inches, 40, velocityUnits::pct);
  //Flingy.turnToHeading(270, deg);
  //Flingy.driveFor(reverse, 20, inches, 50, velocityUnits::pct);
}
// Main will set up the competition functions and callbacks.
//
int main() {
  pre_auton();
  // Set up callbacks for autonomous and driver control periods.
  Controller1.ButtonR1.pressed(toggleClaw);
  Controller1.ButtonY.pressed(printTemps);
  Controller1.ButtonB.pressed(goLoadingPos);
  Controller1.ButtonX.pressed(shootFromLoadingPos);
  Controller1.ButtonL1.pressed(continuousShoot);
  //Competition.autonomous(skill); //slot 1
  //Competition.autonomous(goalSideAuton); //slot 2
  Competition.autonomous(matchLoadSideAuton_1); //slot 3
  //Competition.autonomous(matchLoadSideAuton_2); //slot 4

  //Competition.autonomous(fireCata); //slot 3
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
