/** \file
 * Class for our autonomous behaviours
 *
 *  This file contains our autonomous algorithms.  It should detect if we are in
 *  autonomous mode or not, select an algorithm based upon switch settings at
 *  the driver station and implement the behaviours till autonomous mode ends.
 */

#include "Autonomous.h"
#include "WPILib.h"
#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

//Robot
#include "ComponentBase.h"
#include "RobotParams.h"
#include "AutoParser.h"

using namespace std;

extern "C" {
}

bool Autonomous::CommandResponse(const char *szQueueName) {
	int iPipeXmt;
	bool bReturn = true;

	iPipeXmt = open(szQueueName, O_WRONLY);
	wpi_assert(iPipeXmt > 0);

	Message.replyQ = AUTONOMOUS_QUEUE;
	write(iPipeXmt, (char*) &Message, sizeof(RobotMessage));
	close(iPipeXmt);

	bReceivedCommandResponse = false;

	while (!bReceivedCommandResponse)
	{
		//purposefully empty
	}

	if(iAutoDebugMode)
	{
		printf("%0.3lf Response received\n", pDebugTimer->Get());
	}

	if (ReceivedCommand == COMMAND_AUTONOMOUS_RESPONSE_OK)
	{
		SmartDashboard::PutString("Auto Status","auto ok");
		bReturn = true;
	}
	else if (ReceivedCommand == COMMAND_AUTONOMOUS_RESPONSE_ERROR)
	{
		SmartDashboard::PutString("Auto Status","EARLY DEATH!");
		PRINTAUTOERROR;
		bReturn = false;
	}

	return bReturn;
}


//UNTESTED
//USAGE: MultiCommandResponse({DRIVETRAIN_QUEUE, CONVEYOR_QUEUE}, {COMMAND_DRIVETRAIN_STRAIGHT, COMMAND_CONVEYOR_SEEK_TOTE});
bool Autonomous::MultiCommandResponse(vector<char*> szQueueNames, vector<MessageCommand> commands) {
	//wait for several commands at once
	//check that queue list is as long as command list
	if(szQueueNames.size() != commands.size())
	{
		SmartDashboard::PutString("Auto Status","MULTICOMMAND error!");
		return false;
	}
	bool bReturn = true;
	int iPipeXmt;
	uResponseCount = 0;
	//vector<int> iPipesXmt = new vector<int>();
	//send messages to each component
	for (unsigned int i = 0; i < szQueueNames.size(); i++)
	{
		iPipeXmt = open(szQueueNames[i], O_WRONLY);
		wpi_assert(iPipeXmt > 0);

		Message.replyQ = AUTONOMOUS_QUEUE;
		Message.command = commands[i];
		write(iPipeXmt, (char*) &Message, sizeof(RobotMessage));
		close(iPipeXmt);
	}

	bReceivedCommandResponse = false;

	while (uResponseCount < szQueueNames.size())
	{
		while (!bReceivedCommandResponse)
		{
			//purposefully empty
		}

		if(iAutoDebugMode)
		{
			printf("%0.3lf Response received\n", pDebugTimer->Get());
		}

		if (ReceivedCommand == COMMAND_AUTONOMOUS_RESPONSE_OK)
		{
			SmartDashboard::PutString("Auto Status", "auto ok");
			bReturn = true;
		}
		else if (ReceivedCommand == COMMAND_AUTONOMOUS_RESPONSE_ERROR)
		{
			SmartDashboard::PutString("Auto Status", "EARLY DEATH!");
			bReturn = false;
		}
	}
	return bReturn;
}

bool Autonomous::CommandNoResponse(const char *szQueueName) {
	int iPipeXmt;

	iPipeXmt = open(szQueueName, O_WRONLY);
	wpi_assert(iPipeXmt > 0);

	write(iPipeXmt, (char*) &Message, sizeof(RobotMessage));
	close(iPipeXmt);
	return (true);
}

void Autonomous::Delay(float delayTime)
{
	//breaks the delay into little bits to prevent issues in the event of disabling
	for (double fWait = 0.0; fWait < delayTime; fWait += 0.01)
	{
		// if we are paused break the delay into pieces

		while (bPauseAutoMode)
		{
			Wait(0.02);
		}

		Wait(0.01);
	}
}

bool Autonomous::Start()
{
	//TODO write Autonomous::Start()
	return true;
}
bool Autonomous::Finish()
{
	//TODO write Autonomous::Finish()
	return true;
}

bool Autonomous::Begin(char *pCurrLinePos)
{
	//tell all the components who may need to know that auto is beginning
	Message.command = COMMAND_AUTONOMOUS_RUN;
	return (CommandNoResponse(DRIVETRAIN_QUEUE));
}

bool Autonomous::End(char *pCurrLinePos)
{
	//tell all the components who may need to know that auto is beginning
	Message.command = COMMAND_AUTONOMOUS_COMPLETE;
	CommandNoResponse(DRIVETRAIN_QUEUE);
	return (true);
}

bool Autonomous::Stop(char *pCurrLinePos) {
	//tell those who need to know that the autonomous behavior is over - reset variables
	Message.command = COMMAND_DRIVETRAIN_STOP;
	return (CommandNoResponse(DRIVETRAIN_QUEUE));
}

