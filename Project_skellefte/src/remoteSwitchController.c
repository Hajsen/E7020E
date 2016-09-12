/*
 * remoteSwitchController.c
 *
 *  Created on: Mar 22, 2016
 *      Author: davbom-2
 */
#include "remoteSwitchController.h"
#include "transceiver.h"
#include "usart.h"

int remoteSwitchState = OFF;
char *homecode = "2263FB";
void setRemoteSwitchONState()
{
	remoteSwitchState = ON;
}
void setRemoteSwitchOFFState()
{
	remoteSwitchState = OFF;
}
int getRemoteSwitchState()
{
	return remoteSwitchState;
}

//ID e.g D, E, F
void OrderTransceiver(char * ID, int Status)
{
	char sequence[9];
	strcpy(sequence,homecode);

	if(Status == ON)
	{
		strcat(sequence,"9");
	}
	else if(Status == OFF)
	{
		strcat(sequence,"8");
	}
	strcat(sequence,ID);
	sendSequence(sequence);

}
void setMotionRemoteSwitch(char * ID, int Status)
{
	if(Status == ON)
	{
		if(getRemoteSwitchState()==OFF)
		{
			setRemoteSwitchONState();
			OrderTransceiver(ID, Status);
		}
		if(getMotionLogStatus()==1)
		{
			send_USART_String("Motion detected remote Switch is On \n \r");
		}

		//console output ON e.g Motion detected
	}
	else if(Status == OFF)
	{
		if(getRemoteSwitchState()==ON)
		{
			setRemoteSwitchOFFState();
			OrderTransceiver(ID, Status);
		}
		if(getMotionLogStatus()==1)
		{
			send_USART_String("Motion not detected for 10s. Remote Switch is Off \n \r");
		}

		// console output OFF Motion not detected last 10seconds turn off remote switch
	}
}
