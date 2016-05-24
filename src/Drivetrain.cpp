
/** \file
 * Implementation of class to drive the pallet jack.
 *
 * This class is derived from the standard Component base class and includes
 * initialization for the devices used to control the pallet jack's wheels.
 *
 * The task receives messages form the main robot class and runs the wheels.
 * Special commands use a gyro and quadrature encoder to drive straight X feet
 * or to turn X degrees.
 *
 * Motor orientations:
 * left +
 * right -
 */

#include "Drivetrain.h"			//For the local header file

#include <math.h>
#include <assert.h>

#include <string>
#include <iostream>
#include <algorithm>

#include "ComponentBase.h"
#include "RobotParams.h"
using namespace std;

Drivetrain::Drivetrain() :
		ComponentBase(DRIVETRAIN_TASKNAME, DRIVETRAIN_QUEUE,
				DRIVETRAIN_PRIORITY) {

	leftMotor = new CANTalon(CAN_DRIVETRAIN_LEFT_MOTOR);
	rightMotor = new CANTalon(CAN_DRIVETRAIN_RIGHT_MOTOR);
	bottomMotor = new CANTalon(CAN_DRIVETRAIN_BOTTOM_MOTOR);

	wpi_assert(leftMotor && rightMotor && bottomMotor);

	leftMotor->SetControlMode(CANSpeedController::kPercentVbus);
	rightMotor->SetControlMode(CANSpeedController::kPercentVbus);
	bottomMotor->SetControlMode(CANSpeedController::kPercentVbus);
	leftMotor->SetVoltageRampRate(120.0);
	rightMotor->SetVoltageRampRate(120.0);
	bottomMotor->SetVoltageRampRate(120.0);
	//leftMotor->SetSafetyEnabled(true);
	//rightMotor->SetSafetyEnabled(true);
	//bottomMotor->SetSafetyEnabled(true);

	wpi_assert(leftMotor->IsAlive());
	wpi_assert(rightMotor->IsAlive());
	wpi_assert(bottomMotor->IsAlive());
	//Ah, ah, ah, ah, stayin' alive, stayin' alive

	gyro = new ADXRS453Z;
	wpi_assert(gyro);
	gyro->Start();

	pTask = new Task(DRIVETRAIN_TASKNAME, (FUNCPTR) &Drivetrain::StartTask,
			DRIVETRAIN_PRIORITY, DRIVETRAIN_STACKSIZE);
	wpi_assert(pTask);
	pTask->Start((int) this);
}

Drivetrain::~Drivetrain()			//Destructor
{
	delete (pTask);
	delete leftMotor;
	delete rightMotor;
	delete bottomMotor;
	delete gyro;
	//delete encoder;
}

void Drivetrain::OnStateChange()			//Handles state changes
{
	switch(localMessage.command) {
	case COMMAND_ROBOT_STATE_AUTONOMOUS:
		//restore motor values
		leftMotor->Set(left);
		rightMotor->Set(right);
		bottomMotor->Set(bottom);
		targetRot = 0;
		gyro->Zero();
		//encoder->Reset();
		//gyro should be reset by a message from autonomous
		break;

	case COMMAND_ROBOT_STATE_TEST:
		leftMotor->Set(0.0);
		rightMotor->Set(0.0);
		break;

	case COMMAND_ROBOT_STATE_TELEOPERATED:
		leftMotor->Set(0.0);
		rightMotor->Set(0.0);
		gyro->Zero();
		targetRot = 0;
		break;

	case COMMAND_ROBOT_STATE_DISABLED:
		leftMotor->Set(0.0);
		rightMotor->Set(0.0);
		gyro->Zero();
		targetRot = 0;
		break;

	case COMMAND_ROBOT_STATE_UNKNOWN:
		leftMotor->Set(0.0);
		rightMotor->Set(0.0);
		gyro->Zero();
		targetRot = 0;
		break;

	default:
		leftMotor->Set(0.0);
		rightMotor->Set(0.0);
		gyro->Zero();
		targetRot = 0;
		break;
	}
}

///left + , right -
void Drivetrain::Run() {
	switch(localMessage.command) {

	case COMMAND_AUTONOMOUS_RUN:	//when auto starts
		//SmartDashboard::PutString("Drivetrain CMD", "AUTONOMOUS_RUN");
		//reset stored values
		left = 0;
		right = 0;
		bottom = 0;
		pAutoTimer->Reset();
		gyro->Zero();
		break;

	case COMMAND_AUTONOMOUS_COMPLETE:
		//SmartDashboard::PutString("Drivetrain CMD", "AUTONOMOUS_COMPLETE");
		//reset all auto variables
		left = 0;
		right = 0;
		bottom = 0;
		leftMotor->Set(left);
		rightMotor->Set(right);
		bottomMotor->Set(bottom);
		gyro->Zero();
	break;

	case COMMAND_DRIVETRAIN_STOP:
		//SmartDashboard::PutString("Drivetrain CMD", "DRIVETRAIN_STOP");
		//reset all auto variables
		left = 0.0;
		right = 0.0;
		bottom = 0.0;
		leftMotor->Set(left);
		rightMotor->Set(right);
		bottomMotor->Set(bottom);
		gyro->Zero();
		break;

	case COMMAND_SYSTEM_MSGTIMEOUT:
		//SmartDashboard::PutString("Drivetrain CMD", "SYSTEM_MSGTIMEOUT");
		break;
	case COMMAND_DRIVETRAIN_DRIVE_KIWI:
		KiwiDrive(
				localMessage.params.kiwiDrive.x,
				localMessage.params.kiwiDrive.y,
				localMessage.params.kiwiDrive.r);
		break;
	default:
		break;
	}

	//Put out information
	if (pRemoteUpdateTimer->Get() > 0.2)
	{
		pRemoteUpdateTimer->Reset();
		//SmartDashboard::PutBoolean("Tote Detector", toteSensor->Get());
		//gyro reading is truncated for the sake of the CSV file.
		SmartDashboard::PutNumber("Gyro Angle", TRUNC_THOU(gyro->GetAngle()));
	}
}
void Drivetrain::KiwiDrive(float x, float y, float rot){


	// Assuming gyro rotates counter-clockwise and in degrees
	// and that sin and cos are in radians


	float gangle = gyro->GetAngle();
	float grangle = gangle/180*3.1415926535;

	if (abs(rot)<.1){// DEADZONE
		rot = 0;
	}else{
		targetRot = grangle+rot*rotationGain;
	}
	float rotationAmount = (targetRot-grangle)*rotationPower;

	// < ball bearing
	if(enableBB){
	if(rot == 0 && abs(targetRot-grangle) < angleThreshold){
		goingAngle = false;
	}else if(rot != 0){
		goingAngle = true;
		if(bbTarget != 0){
			bbTarget = 0;
			targetRot = grangle+rot*rotationGain;
		}
	}else if(goingAngle == false && abs(targetRot-grangle) > angleThreshold){
		if(bbTarget == 0){
			if(targetRot-grangle>0){
				bbTarget = targetRot-(3.1415926535*2);
			}else{
				bbTarget = targetRot+(3.1415926535*2);
			}
			rotationAmount = (bbTarget-grangle)*rotationPower;
		}else if(abs(bbTarget-grangle) < angleThreshold){
			targetRot = bbTarget;
			bbTarget=0;
		}else{
			rotationAmount = (bbTarget-grangle)*rotationPower;
		}

	}
	}
	// ball bearing >

	float polarmag = (pow(x,2)+pow(y,2));
	ABLIMIT(rotationAmount, maxPower);
	ABLIMIT(polarmag, (maxPower-abs(rotationAmount*rotationPriority)));
	if(polarmag>maxPower){
		rotationAmount*=rotationPriority;
	}
	float polarang = 3.1415926535/2;
	if(x == 0){
		polarang *= (y>0 ? 1 : -1);
	}else{
		polarang = (atan(y/x));
	}
	if(x < 0){
		polarang = 3.1415926535-(atan(y/-x));

	}
	float angle = polarang + grangle;
	float nx = polarmag*cos(angle);
	float ny = polarmag*sin(angle);




	left = -.5*nx - sqrt(3)/2*ny + rotationAmount;
	right = -.5*nx + sqrt(3)/2*ny + rotationAmount;
	bottom = nx + rotationAmount;


	leftMotor->Set(-left);
	rightMotor->Set(-right);
	bottomMotor->Set(-bottom);

	SmartDashboard::PutNumber("Max power", maxPower);
	SmartDashboard::PutNumber("Gyro angle", gangle);
	SmartDashboard::PutNumber("Gyro angle rad", grangle);
	SmartDashboard::PutNumber("Polar magnitude", polarmag);
	SmartDashboard::PutNumber("Polar angle", polarang);
	SmartDashboard::PutNumber("Target angle", targetRot);
	SmartDashboard::PutNumber("Rotation amount", rotationAmount);
	SmartDashboard::PutNumber("Left motor", left);
	SmartDashboard::PutNumber("Right motor", right);
	SmartDashboard::PutNumber("Bottom motor", bottom);

}
