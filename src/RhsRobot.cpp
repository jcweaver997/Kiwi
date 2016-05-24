/** \file
 * Main robot class.
 *
 * The RhsRobot class is the main robot class. It inherits from RhsRobotBase and MUST define the Init() function, the Run() function, and
 * the OnStateChange() function.  Messages from the DS are processed and commands sent to the subsystems
 * that implement behaviors for each part for the robot.
 */

#include "RhsRobot.h"
#include "WPILib.h"

//Robot
#include "ComponentBase.h"
#include "RobotParams.h"

RhsRobot::RhsRobot() {
	Controller_1 = NULL;
	Monitor_1 = NULL;
	drivetrain = NULL;
	autonomous = NULL;

	iLoop = 0;
}

RhsRobot::~RhsRobot() {
	std::vector<ComponentBase *>::iterator nextComponent = ComponentSet.begin();

	for(; nextComponent != ComponentSet.end(); ++nextComponent)
	{
		delete (*nextComponent);
	}

	delete Controller_1;
	delete Monitor_1;
}

void RhsRobot::Init() {
	/* 
	 * Set all pointers to null and then allocate memory and construct objects
	 * EXAMPLE:	drivetrain = NULL; (in constructor)
	 * 			drivetrain = new Drivetrain(); (in RhsRobot::Init())
	 */
	Controller_1 = new Joystick(0);
	Monitor_1 = new JoystickMonitor(Controller_1);
	Monitor_1->SetAxisTolerance(.05);
	drivetrain = new Drivetrain();
	autonomous = new Autonomous();

	std::vector<ComponentBase *>::iterator nextComponent = ComponentSet.begin();

	if(drivetrain)
	{
		nextComponent = ComponentSet.insert(nextComponent, drivetrain);
	}

	if(autonomous)
	{
		nextComponent = ComponentSet.insert(nextComponent, autonomous);
	}
}

void RhsRobot::OnStateChange() {
	std::vector<ComponentBase *>::iterator nextComponent;

	for(nextComponent = ComponentSet.begin();
			nextComponent != ComponentSet.end(); ++nextComponent)
	{
		(*nextComponent)->SendMessage(&robotMessage);
	}
}

void RhsRobot::Run() {
	/* Poll for control data and send messages to each subsystem. Surround blocks with if(component) so entire components can be disabled
	 * by commenting out their construction.
	 * EXAMPLE: if(drivetrain) 
	 * 			{ 
	 * 				//Check joysticks and send messages 
	 * 			}
	 */

	if(autonomous)
	{
		if(GetCurrentRobotState() == ROBOT_STATE_AUTONOMOUS)
		{
			// all messages to components will come from the autonomous task
			return;
		}
	}

	if (drivetrain)
	{
		//for keepalign tests: comment out everything to the sendMessage
		//also comment out the if (ISAUTO) at the bottom of Drivetrain::Run()
		//robotMessage.command = COMMAND_DRIVETRAIN_START_KEEPALIGN;

		robotMessage.command = COMMAND_DRIVETRAIN_DRIVE_KIWI;
		robotMessage.params.kiwiDrive.x = KIWI_DRIVE_X;
		robotMessage.params.kiwiDrive.y = KIWI_DRIVE_Y;
		robotMessage.params.kiwiDrive.r = KIWI_DRIVE_R;
		drivetrain->SendMessage(&robotMessage);
	}

	Monitor_1->FinalUpdate();
	iLoop++;
}

START_ROBOT_CLASS(RhsRobot)
