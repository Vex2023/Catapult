#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor br = motor(PORT20, ratio18_1, false);
motor bl = motor(PORT11, ratio18_1, true);
motor ml = motor(PORT16, ratio18_1, true);
motor mr = motor(PORT15, ratio18_1, false);
motor fl = motor(PORT1, ratio18_1, true);
motor fr = motor(PORT10, ratio18_1, false);
motor cata = motor(PORT19, ratio36_1, false);
motor claw = motor(PORT9, ratio36_1, false);
inertial InertialSensor = inertial(PORT18);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}