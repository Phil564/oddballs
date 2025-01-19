/*
 *	Project:	oddballs
 *	File:		input.c
 *	Description:handles all input
 */
#include "oddballs.h"


#define DEADZONE 2000

JOYINFOEX	joystickinfo;
joystickType joystick;



/*	--------------------------------------------------------------------------------
	Function 	: InitJoystick
	Purpose 	: initialises the joystick
	Parameters 	: none
	Returns 	: none
	Info 		:
*/

BOOL InitJoystick()
{

	HRESULT		res;
	int			x;


	memset(&joystickinfo, 0, sizeof(JOYINFOEX));
	joystickinfo.dwSize = sizeof(JOYINFOEX);

	res = joyGetPosEx(JOYSTICKID1, &joystickinfo);
	dprintf("Joystick test - ");
	switch(res)
	{

		case JOYERR_UNPLUGGED :		dprintf("JOYERR_UNPLUGGED \n");
									break;
		case MMSYSERR_BADDEVICEID :	dprintf("MMSYSERR_BADDEVICEID \n");
									break;
		case MMSYSERR_INVALPARAM :	dprintf("MMSYSERR_INVALPARAM \n");
									break;
		case MMSYSERR_NODRIVER :	dprintf("MMSYSERR_NODRIVER \n");
									break;
		case JOYERR_NOCANDO :		dprintf("JOYERR_NOCANDO \n");
									break;
		case JOYERR_PARMS:			dprintf("JOYERR_PARMS\n");
									break;

		case JOYERR_NOERROR:		dprintf("Joystick Detected \n");
									for(x = 0; x < 20; x++)
									{
										joystickinfo.dwFlags = JOY_RETURNX + JOY_RETURNY;
//										joystickinfo.dwFlags |= JOY_CAL_READXYONLY;
										res = joyGetPosEx(JOYSTICKID1, &joystickinfo);

										if(joystickinfo.dwFlags & JOY_RETURNX)
											joystick.xOrig += joystickinfo.dwXpos;

 										if(joystickinfo.dwFlags & JOY_RETURNY)
											joystick.yOrig += joystickinfo.dwYpos;


									}
		
									joystick.xOrig /= 20;
									joystick.yOrig /= 20;
									
									return TRUE;
									break;

		default:					dprintf("eh?[%d]\n", res);
									break;
	}

	return FALSE;	


}


/*	--------------------------------------------------------------------------------
	Function 	: ProcessKeys
	Purpose 	: translates keypresses to move camera / hand
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
#define HAND_VELOCITY 0.5
void ProcessKeys(WPARAM key)
{
	VECTOR	vect;
 
	if(controlMethod == CONTROL_HAND)
	{
		switch(key)
		{		
			case VK_UP:		
				vect.v[X] = sin(camera.rot.v[Y])*HAND_VELOCITY;
				vect.v[Y] = 0;
				vect.v[Z] = cos(camera.rot.v[Y])*HAND_VELOCITY;
				ActorAddVelocity(&hand,&vect);
				hand.rotaim.v[Y] = camera.rot.v[Y];
				break;

			case VK_DOWN:	
				vect.v[X] = -sin(camera.rot.v[Y])*HAND_VELOCITY;
				vect.v[Y] = 0;
				vect.v[Z] = -cos(camera.rot.v[Y])*HAND_VELOCITY;
				ActorAddVelocity(&hand,&vect);
				hand.rotaim.v[Y] = camera.rot.v[Y] + PI;
				break;

			case VK_LEFT:	
				vect.v[X] = sin(camera.rot.v[Y] - PI/2)*HAND_VELOCITY;
				vect.v[Y] = 0;
				vect.v[Z] = cos(camera.rot.v[Y] - PI/2)*HAND_VELOCITY;
				ActorAddVelocity(&hand,&vect);
				hand.rotaim.v[Y] = camera.rot.v[Y] + PI + PI / 2;
				break;

			case VK_RIGHT:	
				vect.v[X] = sin(camera.rot.v[Y] + PI/2)*HAND_VELOCITY;
				vect.v[Y] = 0;
				vect.v[Z] = cos(camera.rot.v[Y] + PI/2)*HAND_VELOCITY;
				ActorAddVelocity(&hand,&vect);
				hand.rotaim.v[Y] = camera.rot.v[Y] + PI / 2;
				break;

			case VK_Q:		
				vect.v[X] = vect.v[Z] = 0;
				vect.v[Y] = 0.4;
				ActorAddVelocity(&hand,&vect);
				break;
			case VK_A:		
				vect.v[X] = vect.v[Z] = 0;
				vect.v[Y] = -0.4;
				ActorAddVelocity(&hand,&vect);
 				break;
			case VK_W:		
				vect.v[X] = 0.04;
				vect.v[Y] = vect.v[Z] = 0;
				RotateActor(&hand,&vect);
				break;
			case VK_S:		
				vect.v[X] = -0.04;
				vect.v[Y] = vect.v[Z] = 0;
				RotateActor(&hand,&vect);
				break;
		}
	}

	if(controlMethod == CONTROL_CAMERA)
 	{
		switch(key)
		{		
			case VK_UP:		
				AccelerateActor(&camera,0.4);
				break;

			case VK_DOWN:	
				AccelerateActor(&camera,-0.4);
				break;

			case VK_LEFT:	
				vect.v[X] = vect.v[Z] = 0;
				vect.v[Y] = -0.04;
				RotateActor(&camera,&vect);
				break;

			case VK_RIGHT:	
				vect.v[X] = vect.v[Z] = 0;
				vect.v[Y] = 0.04;
				RotateActor(&camera,&vect);
				break;

			case VK_Q:		
				vect.v[X] = vect.v[Z] = 0;
				vect.v[Y] = 0.4;
				ActorAddVelocity(&camera,&vect);
				break;

			case VK_A:
				vect.v[X] = vect.v[Z] = 0;
				vect.v[Y] = -0.4;
				ActorAddVelocity(&camera,&vect);
				break;

			case VK_S:
				vect.v[X] = 0.04;
				vect.v[Y] = vect.v[Z] = 0;
				RotateActor(&camera,&vect);
				break;

			case VK_W:		
				vect.v[X] = -0.04;
				vect.v[Y] = vect.v[Z] = 0;
				RotateActor(&camera,&vect);
				break;
		}
	}
}


void ProcessInput()
{

	VECTOR vect;

	if(controlMethod == CONTROL_CAMERA)
	{
		vect.v[Y] = joystick.yPos / 50000;
		vect.v[X] = vect.v[Z] = 0;
		AccelerateActor(&camera, joystick.yPos/10000);
		RotateActor(&camera, &vect);
	}


}






/*	--------------------------------------------------------------------------------
	Function 	: ReadJoystick
	Purpose 	: erm...
	Parameters 	: 
	Returns 	: 
	Info 		:
*/

void ReadJoystick()
{

	HRESULT res;
	long	deadzone = 0;

	joystickinfo.dwFlags = JOY_RETURNX + JOY_RETURNY;
	res = joyGetPosEx(JOYSTICKID1, &joystickinfo);

	if(joystickinfo.dwFlags & JOY_RETURNX)
	{

		deadzone = joystick.xOrig - joystickinfo.dwXpos;
		if(abs(deadzone)> DEADZONE)
		{
			if(deadzone > 0)
				joystick.xPos = joystick.xOrig - joystickinfo.dwXpos - DEADZONE;
			else
				joystick.xPos = joystick.xOrig - joystickinfo.dwXpos + DEADZONE;

		}
		else joystick.xPos = 0;
	}
 	if(joystickinfo.dwFlags & JOY_RETURNY)
	{
		deadzone = joystick.yOrig - joystickinfo.dwYpos;
		if(abs(deadzone)> DEADZONE)
		{
			if(deadzone > 0)
				joystick.yPos = joystick.yOrig - joystickinfo.dwYpos - DEADZONE;
			else
				joystick.yPos = joystick.yOrig - joystickinfo.dwYpos + DEADZONE;
		}
		else joystick.yPos = 0;
	}

}