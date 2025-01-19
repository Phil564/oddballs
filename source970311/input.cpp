/*
 *	Project:	oddballs
 *	File:		input.c
 *	Description:handles all input
 */

#include "oddballs.h"
#include "input.h"
#include "actors.h"
#include "maths.h"
#include "hand.h"
#include "personal.h"
#include "sound.h"
#include "collectables.h"
#include "collision.h"

#define DEADZONE 8000

JOYINFOEX	joystickinfo;
joystickType joystick;

int JS_BUTTON_A	= 0;
int	JS_BUTTON_B	= 1;
int JS_BUTTON_Z	= 5;

BOOL	keyboardShift = FALSE;

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

	JOYCAPS		jc;


	joyGetDevCaps(JOYSTICKID1, &jc,sizeof(JOYCAPS));
	joystick.numButtons = jc.wNumButtons;

	if(joystick.numButtons != 6)
		JS_BUTTON_Z	= 2;

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
										res = joyGetPosEx(JOYSTICKID1, &joystickinfo);

										if(joystickinfo.dwFlags & JOY_RETURNX)
											joystick.xOrig += joystickinfo.dwXpos;

 										if(joystickinfo.dwFlags & JOY_RETURNY)
											joystick.yOrig += joystickinfo.dwYpos;


									}

									for(x = 0;x < 6;x++)
										joystick.timeSincePress[x] = 999999;
		
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
void ProcessKeys(WPARAM key)
{
	VECTOR	vect;
 
	if(key == VK_M)
	{
		musicPlaying = 1 - musicPlaying;
		if(musicPlaying)
			playCDTrack(g_hWnd,2);
		else
			StopCDTrack();
	}

	if(controlMethod == CONTROL_HAND)
	{
		switch(key)
		{		
			case VK_T:
				if(detailFlags & DF_CHEAT)
				{
					controlMethod = CONTROL_CAMERA;
				}
				break;

			case VK_Q:		
				if(detailFlags & DF_CHEAT)
				{
					vect.v[X] = vect.v[Z] = 0;
					vect.v[Y] = 0.2;
					if(IsHandHoldingBall() == TRUE)
						ActorAddVelocity(&ball,&vect);
					else
						ActorAddVelocity(&hand,&vect);
				}
				break;

			case VK_A:		
				if(detailFlags & DF_CHEAT)
				{
			   		vect.v[X] = vect.v[Z] = 0;
					vect.v[Y] = -0.4;
					if(IsHandHoldingBall() == TRUE)
						ActorAddVelocity(&ball,&vect);
					else
						ActorAddVelocity(&hand,&vect);
				}
 				break;
			case VK_W:		
				if(detailFlags & DF_CHEAT)
				{
					vect.v[X] = 0.04;
					vect.v[Y] = vect.v[Z] = 0;
					RotateActor(&hand,&vect);
				}
				break;

			case VK_S:		
				if(detailFlags & DF_CHEAT)
				{
					vect.v[X] = -0.04;
					vect.v[Y] = vect.v[Z] = 0;
					RotateActor(&hand,&vect);
				}
				break;

			case VK_UP:	 
				if(keyboardShift == TRUE)
					helper.box.size.v[Z] += 1;
				else
					helper.box.pos.v[Z] += 1;
				break;

			case VK_DOWN:	
				if(keyboardShift == TRUE)
				{
					if(helper.box.size.v[Z] > 1)
						helper.box.size.v[Z] -= 1;
				}
				else
					helper.box.pos.v[Z] -= 1;
				break;

			case VK_LEFT:	
				if(keyboardShift == TRUE)
				{
					if(helper.box.size.v[X] > 1)
						helper.box.size.v[X] -= 1;
				}
				else
					helper.box.pos.v[X] -= 1;
				break;

			case VK_RIGHT:	
				if(keyboardShift == TRUE)
					helper.box.size.v[X] += 1;
				else
					helper.box.pos.v[X] += 1;
				break;
			case VK_P:	
				if(keyboardShift == TRUE)
				{
					if(helper.box.size.v[Y] > 1)
						helper.box.size.v[Y] -= 1;
				}
				else
					helper.box.pos.v[Y] -= 1;
				break;
			case VK_O:	
				if(keyboardShift == TRUE)
					helper.box.size.v[Y] += 1;
				else
					helper.box.pos.v[Y] += 1;
				break;
			case VK_F1:
				SetVector(&helper.box.pos, &hand.pos);
				break;


		}
	}
	else if(controlMethod == CONTROL_CAMERA)
 	{
		switch(key)
		{		
			if(detailFlags & DF_CHEAT)
			{
				case VK_T:
					controlMethod = CONTROL_HAND;
					break;

				case VK_Q:		
					vect.v[X] = vect.v[Z] = 0;
					vect.v[Y] = CAMERA_ACCELERATION;
					ActorAddVelocity(&camera,&vect);
					break;

				case VK_A:
					vect.v[X] = vect.v[Z] = 0;
					vect.v[Y] = -CAMERA_ACCELERATION;
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

			case VK_UP:		
				AccelerateActor2D(&camera,CAMERA_ACCELERATION);
				break;

			case VK_DOWN:	
				AccelerateActor2D(&camera,-CAMERA_ACCELERATION);
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

		}
	}
}

#define JOY_STR 40000

void ProcessInput()
{
	float	accel,maxSpeed,speed;
	VECTOR vect;

	if(controlMethod == CONTROL_CAMERA)
	{
		vect.v[Y] = -joystick.xPos / 500000;
		vect.v[X] = vect.v[Z] = 0;
		AccelerateActor2D(&camera, joystick.yPos/10000);
		RotateActor(&camera, &vect);
	}
	if((controlMethod == CONTROL_HAND))
	{
		if(joystick.magnitude > 2000)
//		if(((detailFlags & DF_STEVE) == 0) && ((joystick.xPos != 0) || (joystick.yPos != 0)))
		{
			if((hand.action != HAND_DUCK) && (hand.action != HAND_CLIMBDOWN))
				hand.rotaim.v[Y] = Aabs(joystick.direction + camera.rot.v[Y] + PI);
			hand.velInertia = hand.normInertia;
		}
																				  
		if((hand.action == HAND_WALK) || (hand.action == HAND_JUMP) || (hand.action == HAND_PUSH))// || (hand.onGround == FALSE) && (IsHandHoldingBall() == FALSE))
		{
			vect.v[Y] = 0;
			vect.v[X] = hand.accel*10/hand.mass*(sin(camera.rot.v[Y])*joystick.yPos/JOY_STR - sin(camera.rot.v[Y]+PI/2)*joystick.xPos/JOY_STR);
			vect.v[Z] = hand.accel*10/hand.mass*(cos(camera.rot.v[Y])*joystick.yPos/JOY_STR - cos(camera.rot.v[Y]+PI/2)*joystick.xPos/JOY_STR);
//			MakeUnit(&vect);
//			vect.v[X] *= 0.6;
//			vect.v[Z] *= 0.6;

			if(hand.plane[0])
			{
				if(terrain[hand.plane[0]->terrain].slidy > 1)
				{
					vect.v[X] /= terrain[hand.plane[0]->terrain].slidy/2;
					vect.v[Z] /= terrain[hand.plane[0]->terrain].slidy/2;
					maxSpeed = hand.maxAccelSpeed * terrain[hand.plane[0]->terrain].slidy/3;
				}
				else
					maxSpeed = hand.maxAccelSpeed * terrain[hand.plane[0]->terrain].slidy;
			}
			else
				maxSpeed = hand.maxAccelSpeed;

			float oldSpeed = Magnitude2D(&hand.vel);

			ActorAddVelocity(&hand,&vect);
			SetVector(&handControl.handAccel,&vect);
			if(Magnitude2D(&vect))
				hand.velInertia = hand.normInertia;

			if(oldSpeed <= maxSpeed)
			{
			 	float speed = Magnitude2D(&hand.vel);
				
				if(speed > maxSpeed)
				{
					hand.vel.v[X] *= maxSpeed/speed;
					hand.vel.v[Z] *= maxSpeed/speed;
				}
			}


		}
		else if(hand.action == HAND_CRAWL)
		{
			vect.v[Y] = 0;
			vect.v[X] = 0;
			vect.v[Z] = 1;

			float oldSpeed = Magnitude2D(&hand.vel);

			if((joystick.yPos != 0) || (joystick.xPos != 0))
			{
				if(handControl.currentActionTime > 4)
				{
					if(handControl.currentActionTime == 5)
						accel = 0.7;
					else
						accel = 0.2;
				}
				else
					accel = -0.05;
			}
			if(accel)
			{
				if(hand.plane[0])
				{
					if(terrain[hand.plane[0]->terrain].slidy > 1)
					{
						accel /= terrain[hand.plane[0]->terrain].slidy/2;
						maxSpeed = hand.maxAccelSpeed * terrain[hand.plane[0]->terrain].slidy/3;
					}
					else
						maxSpeed = hand.maxAccelSpeed * terrain[hand.plane[0]->terrain].slidy;
				}
				else
					maxSpeed = hand.maxAccelSpeed;

				SetVector2D(&handControl.handAccel,&hand.vel);

				speed = -accel;
  				hand.vel.v[X] += speed * sin(hand.rot.v[Y]);
				hand.vel.v[Z] += speed * cos(hand.rot.v[Y]);
				hand.speed = Magnitude(&hand.vel);
				SubVector(&handControl.handAccel,&hand.vel,&handControl.handAccel);
				speed = Magnitude2D(&hand.vel);
				if(speed > hand.maxSpeed)
				{
					hand.vel.v[X] *= hand.maxSpeed/speed;
					hand.vel.v[Z] *= hand.maxSpeed/speed;
				}
//				AccelerateActor2D(&hand, -accel);

				if(Magnitude2D(&vect))
					hand.velInertia = hand.normInertia;

				if(oldSpeed <= maxSpeed)
				{
			 		float speed = Magnitude2D(&hand.vel);
					
					if(speed > maxSpeed)
					{
						hand.vel.v[X] *= maxSpeed/speed;
						hand.vel.v[Z] *= maxSpeed/speed;
					}
				}
			}
		
		}
		else if((hand.action == HAND_JOINED) || (hand.action == HAND_BOUNCE))
		{
			vect.v[Y] = 0;
			vect.v[X] = hand.accel/HAND_ACCELERATION*ball.accel*10/ball.mass*(sin(camera.rot.v[Y])*joystick.yPos/JOY_STR - sin(camera.rot.v[Y]+PI/2)*joystick.xPos/JOY_STR);
			vect.v[Z] = hand.accel/HAND_ACCELERATION*ball.accel*10/ball.mass*(cos(camera.rot.v[Y])*joystick.yPos/JOY_STR - cos(camera.rot.v[Y]+PI/2)*joystick.xPos/JOY_STR);
			vect.v[X] *= 0.32;
			vect.v[Z] *= 0.32;

			float oldSpeed = Magnitude2D(&ball.vel);

			if(ball.plane[0])
			{
				if(terrain[ball.plane[0]->terrain].slidy > 1)
				{
					vect.v[X] /= terrain[ball.plane[0]->terrain].slidy/2;
					vect.v[Z] /= terrain[ball.plane[0]->terrain].slidy/2;
					maxSpeed = ball.maxAccelSpeed * terrain[ball.plane[0]->terrain].slidy/3;
				}
				else
					maxSpeed = ball.maxAccelSpeed * terrain[ball.plane[0]->terrain].slidy;
			}
			else
				maxSpeed = ball.maxAccelSpeed;

			ActorAddVelocity(&ball,&vect);
			SetVector(&handControl.handAccel,&vect);
			if(Magnitude2D(&vect))
				ball.velInertia = ball.normInertia;

			if(oldSpeed <= maxSpeed)
			{
				float speed = Magnitude2D(&ball.vel);
				if(speed > maxSpeed)
				{
					ball.vel.v[X] *= maxSpeed/speed;
					ball.vel.v[Z] *= maxSpeed/speed;
				}
			}
		}
		else if(hand.action == HAND_BALLWALK)
		{
			vect.v[Y] = 0;
			vect.v[X] = hand.accel/HAND_ACCELERATION*ball.accel*10/ball.mass*(sin(camera.rot.v[Y])*joystick.yPos/JOY_STR - sin(camera.rot.v[Y]+PI/2)*joystick.xPos/JOY_STR);
			vect.v[Z] = hand.accel/HAND_ACCELERATION*ball.accel*10/ball.mass*(cos(camera.rot.v[Y])*joystick.yPos/JOY_STR - cos(camera.rot.v[Y]+PI/2)*joystick.xPos/JOY_STR);
			vect.v[X] *= 0.22;
			vect.v[Z] *= 0.22;
			FlipVector(&vect);

			float oldSpeed = Magnitude2D(&ball.vel);

			ActorAddVelocity(&ball,&vect);
			if(Magnitude2D(&vect))
				ball.velInertia = ball.normInertia;

			if(oldSpeed <= ball.maxAccelSpeed)
			{
				float speed = Magnitude2D(&ball.vel);
				if(speed > ball.maxAccelSpeed)
				{
					ball.vel.v[X] *= ball.maxAccelSpeed/speed;
					ball.vel.v[Z] *= ball.maxAccelSpeed/speed;
				}
			}
		}


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

	int x;
	HRESULT res;
	long	deadzone = 0;
 //	char	lastButton[2];

	joystick.lastXPos = joystick.xPos;
	joystick.lastYPos = joystick.yPos;


	joystickinfo.dwFlags = JOY_RETURNALL;
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

	joystick.magnitude = sqrt(joystick.xPos*joystick.xPos + joystick.yPos*joystick.yPos);
	if(joystick.magnitude > JOYSTICK_MAX)
	{
		joystick.xPos *= JOYSTICK_MAX/joystick.magnitude;
		joystick.yPos *= JOYSTICK_MAX/joystick.magnitude;
		joystick.magnitude = JOYSTICK_MAX;
	}

/*	if(joystick.xPos >= 0)
		joystick.xPos = Min(joystick.xPos, JOYSTICK_MAX);
	else
		joystick.xPos = Max(joystick.xPos, -JOYSTICK_MAX);
	if(joystick.yPos >= 0)
		joystick.yPos = Min(joystick.yPos, JOYSTICK_MAX);
	else
		joystick.yPos = Max(joystick.yPos, -JOYSTICK_MAX);
*/

//check buttons

	for(x = 0; x < joystick.numButtons; x++)
	{
		joystick.lastButton[x] = joystick.button[x];
		joystick.button[x] = joystick.debounce[x] = 0;

		if(joystickinfo.dwButtons & (1 << x))
			joystick.button[x] = 1;

		if((joystick.button[x]) && (!joystick.lastButton[x]))
			joystick.debounce[x] = 1;

		if(joystick.lastButton[x] == joystick.button[x])
			joystick.buttonStateTime[x]++;
		else
		{
			joystick.buttonLastStateTime[x] = joystick.buttonStateTime[x];
			joystick.buttonStateTime[x] = 0;
		}

		if(joystick.debounce[x])
			joystick.timeSincePress[x] = 0;
		else
			joystick.timeSincePress[x]++;

	}


	if((joystick.xPos != 0) || (joystick.yPos != 0))
	{
		joystick.direction = atan2(joystick.yPos, joystick.xPos) - (PI / 2);
		joystick.direction = Aabs(joystick.direction);
  	}
}