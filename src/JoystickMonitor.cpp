#include "JoystickMonitor.h"
#include "RobotParams.h"
#include "WPILib.h"

#include <cmath>

JoystickMonitor::JoystickMonitor(Joystick *j) {
	stick = j;
	axisTolerance = .0001;

	for (int i = 0; i < JOYSTICK_BUTTON_COUNT; i++)
	{
		buttonsDown.push_back(stick->GetRawButton(i + 1));
	}
	for (int i = 0; i < JOYSTICK_AXIS_COUNT; i++)
	{
		//the ID is 1 greater than the location in the vector
		axisValues.push_back(stick->GetRawAxis(i + 1));
	}
}
JoystickMonitor::~JoystickMonitor() {

}

/**
 * Updates the stored "previous" values of each button and axis.
 * Be sure to call this at the END of the run function so that you have these values during the next cycle.
 */
void JoystickMonitor::FinalUpdate() {
	for (unsigned int i = 0; i < buttonsDown.size(); i++)
	{
		buttonsDown[i] = stick->GetRawButton(i + 1);
	}
	for (unsigned int i = 0; i < axisValues.size(); i++)
	{
		axisValues[i] = stick->GetRawAxis(i + 1);
	}
}

///Returns true if the target button was up in the previous cycle but is down in the current cycle.
bool JoystickMonitor::ButtonPressed(unsigned int button) {
	if (button > 0 && button <= buttonsDown.size()
			&& stick->GetRawButton(button) && !buttonsDown[button - 1])
	{
		return true;
	}
	return false;
}

///Returns true if the target button was down in the previous cycle but is up in the current cycle.
bool JoystickMonitor::ButtonReleased(unsigned int button) {
	if (button > 0 && button <= buttonsDown.size()
			&& !stick->GetRawButton(button) && buttonsDown[button - 1])
	{
		return true;
	}
	return false;
}

///Returns true if given axis moved at least the tolerated distance between the cycles.
bool JoystickMonitor::AxisMoved(unsigned int axis) {
	if (axis > 0 && axis <= axisValues.size()
			&& std::abs(stick->GetRawAxis(axis) - axisValues[axis - 1])
					> axisTolerance)
	{
		return true;
	}
	return false;
}

void JoystickMonitor::SetAxisTolerance(float tolerance) {
	axisTolerance = tolerance;
}

float JoystickMonitor::GetAxisTolerance() {
	return axisTolerance;
}
