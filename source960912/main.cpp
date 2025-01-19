/*
 *	Project:	Oddballs
 *	File:		main.cpp
 *	Description:the "main" loop
 */


#include "oddballs.h"

char message[256];
char controlMethod = CONTROL_CAMERA;

/*	--------------------------------------------------------------------------------
	Function 	: MainLoop
	Purpose 	: this calls all the game routines
	Parameters 	: none
	Returns 	: none
	Info 		:
*/

void MainLoop()
{

	switch(controlMethod)
	{
		case CONTROL_HAND:	
							sprintf(message, "Hand Control");
							break;
		case CONTROL_CAMERA:
							sprintf(message, "Camera Control");
							break;
	}
	WriteInfoBuffer(message, 0, 0);


	ReadJoystick();
	UpdateActor(&camera);
	UpdateActor(&hand);

}