using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor br;
extern motor bl;
extern motor ml;
extern motor mr;
extern motor fl;
extern motor fr;
extern motor cata;
extern motor claw;
extern inertial InertialSensor;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );