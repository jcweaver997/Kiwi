/** \file
 * Definitions of class to control the drive train.
 *
 * This classes is derived from the standard Component base class and includes
 * definitions for the devices used to control the pallet jack wheels.
 */

#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include <pthread.h>
//#include <unistd.h>

//Robot
#include "WPILib.h"

#include "ComponentBase.h"			//For ComponentBase class
#include "ADXRS453Z.h"

class Drivetrain : public ComponentBase
{
public:
	Drivetrain();
	~Drivetrain();
	static void *StartTask(void *pThis)
	{
		((Drivetrain *)pThis)->DoWork();
		return(NULL);
	}

	bool GetGyroAngle();
private:

	CANTalon* leftMotor;
	CANTalon* rightMotor;
	CANTalon* bottomMotor;
	ADXRS453Z *gyro;
	BuiltInAccelerometer accelerometer;
	//Timer *pAutoTimer; //watches autonomous time and disables it if needed.IN COMPONENT BASE
	//stores motor values during autonomous

	// All rotation values in radians.
	float left = 0.0f; // power for the left most motor. +60*s
	float right = 0.0f; // power for the left most motor. -60*
	float bottom = 0.0f; // power for the bottom motor. 0*
	float targetRot = 0.0f; // the rotation the robot is trying to get to.
	float rotationGain = .85f; // how fast the controller adds to the target pos.
	float rotationPower = .8f; // how fast the motors go to get to the target pos.
	float maxPower = 1.0f; // the max power given to the motors.
	float angleThreshold = 0.35f; // the max angle offset before activating ball bearing mode.
	float bbTarget = 0.0f;// the target angle for ball bearing mode
	float rotationPriority = 0.5f; // Percentage of the power used for turning
	bool goingAngle = false;
	bool enableBB = false;
	//diameter*pi/encoder_resolution : 1.875 * 3.14 / 256

	void OnStateChange();
	void Run();
	void Put();//for SmartDashboard
	void KiwiDrive(float x, float y, float rot);

};

#endif			//DRIVETRAIN_H
