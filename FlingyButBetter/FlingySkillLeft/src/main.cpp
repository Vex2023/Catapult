#include "vex.h"

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// br                   motor         20              
// bl                   motor         11              
// ml                   motor         16              
// mr                   motor         15              
// fl                   motor         1               
// fr                   motor         10              
// cata                 motor         19              
// claw                 motor         9               
// InertialSensor       inertial      18              
// ---- END VEXCODE CONFIGURED DEVICES ----


using namespace vex;

// A global instance of competition
competition Competition;

inertial Inertial = inertial(PORT18);
motor_group leftDrive (bl, ml, fl);
motor_group rightDrive (br, mr, fr);
smartdrive Flingy(leftDrive, rightDrive, Inertial, 12.56,10,9,distanceUnits::in);

/*---------------------------------------------------------------------------*/
/*                             VEXcode Config                                */
/*                                                                           */
/*  Before you do anything else, start by configuring your motors and        */
/*  sensors using the V5 port icon in the top right of the screen. Doing     */
/*  so will update robot-config.cpp and robot-config.h automatically, so     */
/*  you don't have to. Ensure that your motors are reversed properly. For    */
/*  the drive, spinning all motors forward should drive the robot forward.   */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                             JAR-Template Config                           */
/*                                                                           */
/*  Where all the magic happens. Follow the instructions below to input      */
/*  all the physical constants and values for your robot. You should         */
/*  already have configured your robot manually with the sidebar configurer. */
/*---------------------------------------------------------------------------*/

Drive chassis(

//Specify your drive setup below. There are seven options:
//ZERO_TRACKER_NO_ODOM, ZERO_TRACKER_ODOM, TANK_ONE_ENCODER, TANK_ONE_ROTATION, TANK_TWO_ENCODER, TANK_TWO_ROTATION, HOLONOMIC_TWO_ENCODER, and HOLONOMIC_TWO_ROTATION
//For example, if you are not using odometry, put ZERO_TRACKER_NO_ODOM below:
ZERO_TRACKER_NO_ODOM,

//Add the names of your Drive motors into the motor groups below, separated by commas, i.e. motor_group(Motor1,Motor2,Motor3).
//You will input whatever motor names you chose when you configured your robot using the sidebar configurer, they don't have to be "Motor1" and "Motor2".

//Left Motors:
motor_group(bl, ml, fl),

//Right Motors:
motor_group(br, mr, fr),

//Specify the PORT NUMBER of your inertial sensor, in PORT format (i.e. "PORT1", not simply "1"):
PORT18,

//Input your wheel diameter. (4" omnis are actually closer to 4.125"):
3.25,

//External ratio, must be in decimal, in the format of input teeth/output teeth.
//If your motor has an 84-tooth gear and your wheel has a 60-tooth gear, this value will be 1.4.
//If the motor drives the wheel directly, this value is 1:
1.67,

//Gyro scale, this is what your gyro reads when you spin the robot 360 degrees.
//For most cases 360 will do fine here, but this scale factor can be very helpful when precision is necessary.
360,

/*---------------------------------------------------------------------------*/
/*                                  PAUSE!                                   */
/*                                                                           */
/*  The rest of the drive constructor is for robots using POSITION TRACKING. */
/*  If you are not using position tracking, leave the rest of the values as  */
/*  they are.                                                                */
/*---------------------------------------------------------------------------*/

//If you are using ZERO_TRACKER_ODOM, you ONLY need to adjust the FORWARD TRACKER CENTER DISTANCE.

//FOR HOLONOMIC DRIVES ONLY: Input your drive motors by position. This is only necessary for holonomic drives, otherwise this section can be left alone.
//LF:      //RF:    
PORT1,     -PORT2,

//LB:      //RB: 
PORT3,     -PORT4,

//If you are using position tracking, this is the Forward Tracker port (the tracker which runs parallel to the direction of the chassis).
//If this is a rotation sensor, enter it in "PORT1" format, inputting the port below.
//If this is an encoder, enter the port as an integer. Triport A will be a "1", Triport B will be a "2", etc.
3,

//Input the Forward Tracker diameter (reverse it to make the direction switch):
2.75,

//Input Forward Tracker center distance (a positive distance corresponds to a tracker on the right side of the robot, negative is left.)
//For a zero tracker tank drive with odom, put the positive distance from the center of the robot to the right side of the drive.
//This distance is in inches:
-2,

//Input the Sideways Tracker Port, following the same steps as the Forward Tracker Port:
1,

//Sideways tracker diameter (reverse to make the direction switch):
-2.75,

//Sideways tracker center distance (positive distance is behind the center of the robot, negative is in front):
5.5

);

int current_auton_selection = 0;
bool auto_started = false; //for competition
//bool auto_started = true; //for driver practice
bool clawClosed = true;
bool cataStop = true;

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

void goLoadingPos() //make cata go down to loaded position
{
  double current_position = cata.position(degrees);
  //cata.spinToPosition(30,degrees);
  cata.spinFor(reverse, 120, degrees, 40, velocityUnits::pct,true);
  current_position = cata.position(degrees);
  // cata.spinToPosition()
}

void shootFromLoadingPos() // make cata shoot from loaded position
{
  double current_position = cata.position(degrees);
  //cata.spinToPosition(30,degrees);
  cata.spinFor(reverse, 240, degrees, 40, velocityUnits::pct);
  current_position = cata.position(degrees);
  // cata.spinToPosition()
}

void continuousShoot(){
  cataStop = false;
  float current_position = cata.position(degrees);

  float degreeToSpin = (155-(int)current_position)%360;
  cata.spinFor(reverse, degreeToSpin,deg);
  //cata.spinToPosition(-155, degrees);
  //cata.spinToPosition(205, degrees);
  current_position = cata.position(degrees);
  cout<< "load position: " << current_position<< endl;
  wait(0.3,sec);
  while(true)
  {
    cata.spinFor(reverse, 360, degrees, 90, velocityUnits::pct);
    
    current_position = cata.position(degrees);
    //if(Controller1.ButtonL1.pressing()){
    cout<<"cataStop="<<cataStop<<endl;
    if(cataStop){
      //cata.spinFor(reverse, 205-current_position, degrees);
      break;
    }
    wait(0.3,sec);
  }
}

void stopShoot() {
  cout<< "stopped"<<endl;
  cataStop=true;
}

void toggleClaw()
{
  if(!clawClosed)
  {
    claw.spinToPosition(0,degrees);
    clawClosed = true;
  }
  else if(clawClosed)
  {
    claw.spinToPosition(78, degrees);
    // claw.spinFor(reverse, 70, degrees, 30, velocityUnits::pct);
    clawClosed = false;
  }
}

void skill(){
  Flingy.driveFor(fwd, 8, inches); //move drivetrain to approx. loading position
  Flingy.turnToHeading(315, deg); //align both wheels
  Flingy.driveFor(fwd, 2, inches); //both wheels touch loading bar, more stable
  Flingy.drive(fwd, 3, velocityUnits::pct); //prevents wheels from bouncing off bar, pushes against bar
  for(int iter = 0; iter < 43; ++iter){
    
    cout<< "iter= "<<iter<<"\n"; //print number of times fired
    goLoadingPos(); //fires catapult
    shootFromLoadingPos();
  }
}

/*********************************************************************/
//autonomous for the goal side that puts the alliance triball preloaded in claw into the goal and puts one green triball into goal

void goalSideTwoAcorns() {
  Flingy.setDriveVelocity(40,velocityUnits::pct);
  toggleClaw();//open claw
  Flingy.driveFor(reverse, 2, inches, 40, velocityUnits::pct); //shake claw
  Flingy.driveFor(fwd, 2, inches, 40, velocityUnits::pct); //shake claw
  toggleClaw();//close claw
  chassis.drive_distance(45);
  chassis.turn_to_angle(80);//turn to position for pushing alliance acorn into goal
  chassis.drive_distance(8);//push alliance acorn into goal
  toggleClaw(); //open claw
  chassis.drive_distance(-12.5);//get out of goal
  chassis.turn_to_angle(241);//turn into position to grab green acorn touching long barrier
  chassis.drive_distance(13);//drive forward so that acorn gets into claw
  claw.setTimeout(500, msec);
  toggleClaw(); //close claw
  chassis.drive_distance(-5);//get away from long barrier
  chassis.turn_to_angle(75);//turn into position to push green acorn into goal
  chassis.drive_distance(21);//push acorn into goal
  toggleClaw(); //open claw
  chassis.drive_distance(-23);
} //goalSideTwoAcorns


/*******************************************************************/
//autonomous for the goal side that puts the alliance triball preloaded in claw into the goal and touches elevation bar

void goalSideTouchElevation() {
  Flingy.setDriveVelocity(40,velocityUnits::pct);
  claw.setTimeout(500, msec);
  toggleClaw();//open claw
  Flingy.driveFor(reverse, 2, inches, 40, velocityUnits::pct); //shake claw
  Flingy.driveFor(fwd, 2, inches, 40, velocityUnits::pct); //shake claw
  toggleClaw();//close claw
  chassis.drive_distance(44);
  chassis.turn_to_angle(80);//turn to position for pushing alliance acorn into goal
  chassis.drive_distance(7.5);//push alliance acorn into goal
  toggleClaw(); //open claw
  chassis.drive_distance(-14.5);//get out of goal
  chassis.turn_to_angle(180);//turn towards short barrier
  chassis.drive_distance(17);//drive towards short barrier
  chassis.turn_to_angle(250);//turn towards long barrier
  chassis.drive_distance(10);//drive towards long barrier
  chassis.drive_distance(4);//drive forward more so zip tie can touch elevation bar
  chassis.turn_to_angle(246);
} //goalSideTouchElevation
/*******************************************************************/

/**************************Match Load Side Auton********************/
void matchLoadSide(){
  toggleClaw(); //open claw
  cata.spinFor(reverse, 120, degrees, 40, velocityUnits::pct); //load catapult
  Flingy.driveFor(reverse, 2, inches, 20, velocityUnits::pct); //shake claw
  cata.spinFor(reverse, 240, degrees, 40, velocityUnits::pct); //shoot catapult
  Flingy.driveFor(fwd, 2, inches, 10, velocityUnits::pct);// move into to touch load zone
  claw.setTimeout(1, sec);
  toggleClaw(); //grab acorn
  chassis.drive_distance(-5);
  chassis.turn_to_angle(260);
  chassis.drive_distance(11);
  chassis.turn_to_angle(225);
  toggleClaw();
  chassis.drive_distance(-5);
  chassis.drive_distance(33,225,12,6);//drive forward for 33 inches with heading 225 degree, drive voltage 12V, turn voltage 6V
  chassis.drive_distance(-4);
 /* Flingy.driveFor(reverse, 5, inches, 20, velocityUnits::pct); //take acorn out of match load zone
  Flingy.setTurnVelocity(10, velocityUnits::pct);
  Flingy.turnToHeading(260,degrees); // turn to diagnal
  Flingy.driveFor(fwd, 11, inches, 30, velocityUnits::pct);


  Flingy.turnToHeading(225,degrees); //facing horizontal bar
  toggleClaw(); //open claw
  Flingy.driveFor(fwd, 16, inches, 50, velocityUnits::pct); // push acorn to other side
  */

}
/*******************************************************************/





/**********************************************************************/
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
void frontBrake()
{
  fl.stop(brake); 
  fr.stop(brake);
  ml.stop(brake);
  mr.stop(brake);
  bl.stop(coast);
  br.stop(coast);
}
void rearBrake()
{
  fl.stop(coast); 
  fr.stop(coast);
  ml.stop(brake);
  mr.stop(brake);
  bl.stop(brake);
  br.stop(brake);
}
void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  Brain.Screen.setFont(vex::fontType::mono30);
  
  vexcodeInit();
  Inertial.calibrate();
  default_constants();
  cata.setStopping(hold);
  claw.setStopping(hold);
  claw.setPosition(0,degrees);
  cata.setPosition(0,degrees); //set catapult starting point as 0 degree


  while(auto_started == false){            //Changing the names below will only change their names on the
    Brain.Screen.clearScreen();
           //brain screen for auton selection.
    switch(current_auton_selection){       //Tap the brain screen to cycle through autons.
      case 0:
        Brain.Screen.printAt(50, 50, "Goal Side Touch Elevation");
        break;
      case 1:
        Brain.Screen.printAt(50, 50, "Goal Side Two Acorns");
        break;
      case 2:
        Brain.Screen.printAt(50, 50, "Match Load Side");
        break;
      case 3:
        Brain.Screen.printAt(50, 50, "Skills");
        break;
      case 4:
        Brain.Screen.printAt(50, 50, "turn Test");
        break;
    }
    if(Brain.Screen.pressing()){
      while(Brain.Screen.pressing()) {}
      current_auton_selection ++;
    } else if (current_auton_selection == 8){
      current_auton_selection = 0;
    }

    
    task::sleep(10);
  }
}

void autonomous(void) {
  auto_started = true;
  switch(current_auton_selection){  
    case 0:
      goalSideTouchElevation();  //This is the default auton, if you don't select from the brain.
      break;        //Change these to be your own auton functions in order to use the auton selector.
    case 1:         //Tap the screen to cycle through autons.
      goalSideTwoAcorns();
      break;
    case 2:
      matchLoadSide();
      break;
    case 3:
      skill();
      break;
    case 4:
      turn_test();
      break;
 }
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

//
// Main will set up the competition functions and callbacks.
//
int main() {
  pre_auton();
  Controller1.ButtonR1.pressed(toggleClaw);
  Controller1.ButtonY.pressed(printTemps);
  Controller1.ButtonB.pressed(goLoadingPos);
  Controller1.ButtonX.pressed(shootFromLoadingPos);
  Controller1.ButtonL1.pressed(continuousShoot);
  Controller1.ButtonL2.pressed(stopShoot);
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
