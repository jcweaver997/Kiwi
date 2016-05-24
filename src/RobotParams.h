/** \file
 *  Defines task parameters, hardware assignments and controller button/axis assignment.
 *
 * This header contains basic parameters for the robot. All parameters must be constants with internal
 * linkage, otherwise the One Definition Rule will be violated.
 */

#ifndef ROBOT_PARAMS_H
#define ROBOT_PARAMS_H

//Robot
#include "JoystickLayouts.h"			//For joystick layouts

//Robot Params
const char* const ROBOT_NAME =		"TRICERATOPS";	//Formal name
const char* const ROBOT_NICKNAME =   "Mr. Kiwi";		//Nickname
const char* const ROBOT_VERSION =	"1.0";						//Version

//Robot Mode Macros - used to tell what mode the robot is in
#define ISAUTO			RobotBase::getInstance().IsAutonomous()
#define ISTELEOPERATED	RobotBase::getInstance().IsOperatorControl()
#define ISTEST			RobotBase::getInstance().IsTest()
#define ISENABLED		RobotBase::getInstance().IsEnabled()
#define ISDISABLED		RobotBase::getInstance().IsDisabled()

//Utility Functions - Define commonly used operations here
#define ABLIMIT(a,b)		if(a > b) a = b; else if(a < -b) a = -b;
#define TRUNC_THOU(a)		((int)(1000 * a)) * .001
#define TRUNC_HUND(a)		((int)(100 * a)) * .01
#define PRINTAUTOERROR		printf("Early Death! %s %i \n", __FILE__, __LINE__);

//Task Params - Defines component task priorites relative to the default priority.
//EXAMPLE: const int DRIVETRAIN_PRIORITY = DEFAULT_PRIORITY -2;
const int DEFAULT_PRIORITY = 150;
const int COMPONENT_PRIORITY 	= DEFAULT_PRIORITY;
const int DRIVETRAIN_PRIORITY 	= DEFAULT_PRIORITY;
const int AUTONOMOUS_PRIORITY 	= DEFAULT_PRIORITY;
const int AUTOEXEC_PRIORITY 	= DEFAULT_PRIORITY;
const int AUTOPARSER_PRIORITY 	= DEFAULT_PRIORITY;

//Task Names - Used when you view the task list but used by the operating system
//EXAMPLE: const char* DRIVETRAIN_TASKNAME = "tDrive";
const char* const COMPONENT_TASKNAME	= "tComponent";
const char* const DRIVETRAIN_TASKNAME	= "tDrive";
const char* const AUTONOMOUS_TASKNAME	= "tAuto";
const char* const AUTOEXEC_TASKNAME		= "tAutoEx";
const char* const AUTOPARSER_TASKNAME	= "tParse";

const int COMPONENT_STACKSIZE	= 0x10000;
const int DRIVETRAIN_STACKSIZE	= 0x10000;
const int AUTONOMOUS_STACKSIZE	= 0x10000;
const int AUTOEXEC_STACKSIZE	= 0x10000;
const int AUTOPARSER_STACKSIZE	= 0x10000;

//TODO change these variables throughout the code to PIPE or whatever instead  of QUEUE
//Queue Names - Used when you want to open the message queue for any task
//NOTE: 2015 - we use pipes instead of queues
//EXAMPLE: const char* DRIVETRAIN_TASKNAME = "tDrive";
const char* const COMPONENT_QUEUE 	= "/tmp/qComp";
const char* const DRIVETRAIN_QUEUE 	= "/tmp/qDrive";
const char* const AUTONOMOUS_QUEUE 	= "/tmp/qAuto";
const char* const AUTOPARSER_QUEUE 	= "/tmp/qParse";

//PWM Channels - Assigns names to PWM ports 1-10 on the Roborio
//EXAMPLE: const int PWM_DRIVETRAIN_FRONT_LEFT_MOTOR = 1;
const int PWM_DRIVETRAIN_LEFT_MOTOR = 1;
const int PWM_DRIVETRAIN_RIGHT_MOTOR = 0;
const int PWM_DRIVETRAIN_BOTTOM_MOTOR = 2;

const int CAN_PDB = 0;
const int CAN_DRIVETRAIN_LEFT_MOTOR = 2;
const int CAN_DRIVETRAIN_RIGHT_MOTOR = 8;
const int CAN_DRIVETRAIN_BOTTOM_MOTOR = 1;

//Relay Channels - Assigns names to Relay ports 1-8 on the Roborio
//EXAMPLE: const int RLY_COMPRESSOR = 1;

//Digital I/O - Assigns names to Digital I/O ports 1-14 on the Roborio
//EXAMPLE: const int DIO_DRIVETRAIN_BEAM_BREAK = 0;

//Solenoid - Assigns names to Solenoid ports 1-8 on the 9403
//EXAMPLE: const int SOL_DRIVETRAIN_SOLENOID_SHIFT_IN = 1;

//I2C - Assigns names to I2C ports 1-2 on the Roborio
//EXAMPLE: const int IO2C_AUTO_ACCEL = 1;

//Analog I/O - Assigns names to Analog I/O ports 1-8 on Anal;og Breakout Module
//EXAMPLE: const int AIO_BATTERY = 8;

//Joystick Input Device Counts - used by the listener to watch buttons and axis
const int JOYSTICK_BUTTON_COUNT = 10;
const int JOYSTICK_AXIS_COUNT = 5;

//POV IDs - Assign names to the 9 POV positions: -1 to 7
//EXAMPLE: const int POV_STILL = -1;
const int POV_STILL = -1;

//Primary Controller Mapping - Assigns action to buttons or axes on the first joystick
#undef	USE_X3D_FOR_CONTROLLER_1
#undef	USE_XBOX_FOR_CONTROLLER_1
#define	USE_L310_FOR_CONTROLLER_1

//Secondary Controller Mapping - Assigns action to buttons or axes on the second joystick
#undef	USE_X3D_FOR_CONTROLLER_2
#undef 	USE_XBOX_FOR_CONTROLLER_2
#define USE_L310_FOR_CONTROLLER_2

#ifdef USE_XBOX_FOR_CONTROLLER_1
#endif

#ifdef USE_L310_FOR_CONTROLLER_1
//ID numbers for various buttons and axis

#define KIWI_DRIVE_X				Controller_1->GetRawAxis(L310_THUMBSTICK_LEFT_X)
#define KIWI_DRIVE_Y				-Controller_1->GetRawAxis(L310_THUMBSTICK_LEFT_Y)
#define KIWI_DRIVE_R				Controller_1->GetRawAxis(L310_TRIGGER_RIGHT)-Controller_1->GetRawAxis(L310_TRIGGER_LEFT);
#endif // USE_L310_FOR_CONTROLLER_1

#ifdef USE_X3D_FOR_CONTROLLER_2
#endif // USE_X3D_FOR_CONTROLLER_2

#ifdef USE_XBOX_FOR_CONTROLLER_2
#endif // USE_XBOX_FOR_CONTROLLER_2

#ifdef USE_L310_FOR_CONTROLLER_2
//ID numbers for various buttons and axis

#endif // USE_L310_FOR_CONTROLLER_2

#endif //ROBOT_PARAMS_H
