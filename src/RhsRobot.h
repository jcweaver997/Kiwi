/** \file
 * Main robot class.
 *
 * The RhsRobot class is the main robot class. It inherits from RhsRobotBase and MUST define the Init() function, the Run() function, and
 * the OnStateChange() function.  Messages from the DS are processed and commands.
 */
#ifndef RHS_ROBOT_H
#define RHS_ROBOT_H

#include "WPILib.h"

#include "Autonomous.h"
#include "Drivetrain.h"
#include "RhsRobotBase.h"
#include "JoystickMonitor.h"

class RhsRobot : public RhsRobotBase
{
public:
	RhsRobot();
	virtual ~RhsRobot();

private:
	const float fDriveMax = 1;

	Joystick* Controller_1;
	JoystickMonitor* Monitor_1;
	Drivetrain* drivetrain;
	Autonomous* autonomous;

	std::vector <ComponentBase *> ComponentSet;
	
	void Init();
	void OnStateChange();
	void Run();
	bool CheckButtonPressed(bool, bool);
	bool CheckButtonReleased(bool, bool);


	int iLoop;
};

#endif //RHS_ROBOT_H
