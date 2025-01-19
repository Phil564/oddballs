/*
 *	Project:	oddballs
 *	File:		hand.cpp
 *	Description:handy work tm
 */

#include "oddballs.h"
#include "hand.h"
#include "anim.h"
#include "maths.h"
#include "collision.h"
#include "actors.h"
#include "personal.h"
#include "collectables.h"
#include "enemies.h"
#include "nmebehaviour.h"
#include "cam.h"
#include "sound.h"
#include "bullets.h"
#include "platforms.h"

#define BALL_PROXIMITY 1.2
#define MAX_FIRE_COUNT 10
#define BALL_WHACK_DELAY 2

#define DUCK_DELAY_COUNT		4
#define THROW_DELAY_COUNT		5
#define WHACK_DELAY_COUNT		2
#define LONGJUMP_DELAY_COUNT	7
#define SLAM2ST_DELAY_COUNT		10

#define START_SKIDDING_SPEED	0.06

#define CLIMBDOWN_SLOPE_LIMIT	0.92

#define WAIT_FOR_ANIMEND 1000
#define WAIT_FOR_ONGROUND 1001

HAND_CONTROL_TYPE handControl;
BALL_CONTROL_TYPE ballControl;

/*	--------------------------------------------------------------------------------
	Function 	: MoveHand()
	Purpose 	: handles control of the hand
	Parameters 	: none
	Returns 	: none
	Info 		:
*/

char handActionValidityTable[MAX_HAND_ACTIONS][MAX_HAND_ACTIONS] = 
{


//				IDLE | WALK | JUMP | JOIN | THROW | PWHACK | WHACK | SKID | DUCK | REL  | GETUP | T-AIM | T-rel | FALL | BOUNCE | BNCE2CUP | AIRWHCK | THRWLND | SLAM | SLM2ST | PUNCH | PNCH2ST | SLAP | BALLWALK | CRAWL | CART | CLIMBU | CLIMBD | CLING | DROP | HURT | PUSH
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*IDLE,*/		 0,		1,		1,	   1,	  0,	  0,	   0,	   1,	  1,	 0,		0,		0,		0,	   0,		0,	     0,         0,       0,		   0,	  0,	   0,       0,		1,		1,		  0,	  1,      0,	   1,	    0,		0,     0,	 1,	  /*IDLE,*/		 
/*WALK,*/		 1,		0,		1,	   1,	  0,	  0,	   0,	   1,	  1,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 0,		   0,	  0,  	   0,		0,		1,		1,		  0,	  1,	  0,	   1,		0,		0, 	   0,	 1,	  /*WALK,*/		 
/*JUMP,*/		 0,		0,		0,	   0,	  0,	  0,	   0,	   0,	  0,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 1,		   1,	  0,  	   0,		0,		0,		1,		  0,	  0,	  0,	   1,		0,		0, 	   0,	 0,	  /*JUMP,*/		 
/*JOINED,*/		 0,		0,		0,	   0,	  0,	  1,	   0,	   0,	  0,	 0,		0,		1,		0,	   0,		1,		 0,			0,		 0,		   0,	  0,  	   0,		0,		0,		0,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*JOINED,*/		 
/*THROW,*/		 0, 	0,		0,	   0,	  0,	  0,	   0,	   0,	  0,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 1,		   0,	  0,  	   0,		0,		0,		1,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*THROW,*/		 
/*PREWHACK*/	 0,		0,		0,	   0,	  0,	  0,	   1,	   0,	  0,	 1,		0,		0,		0,	   0,		0,		 0,			0,		 0,		   0,	  0,  	   0,		0,		0,		0,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*PREWHACK*/	 
/*WHACK,*/		 1,		1,		0,	   0,	  0,	  0,	   0,	   0,	  0,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 0,		   0,	  0,  	   0,		0,		0,		0,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*WHACK,*/		 
/*SKIDDING,*/	 1,		1,		1,	   1,	  0,	  0,	   0,	   0,	  1,	 0,		0,		0,		0,	   1,		0,		 0,			0,		 0,		   0,	  0,  	   0,		0,		1,		0,		  0,	  1,	  0,	   1,		0,		0, 	   0,	 0,	  /*SKIDDING,*/	 
/*DUCK,*/		 0,		0,		0,	   0,	  0,	  0,	   0,	   0,	  0,	 0,		1,		0,		0,	   0,		0,		 0,			0,		 0,		   0,	  0,  	   0,		0,		0,		0,		  1,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*DUCK,*/		 
/*RELEASE*/ 	 1,		1,		0,	   0,	  0,	  0,	   0,	   0,	  0,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 0,		   0,	  0,  	   0,		0,		0,		0,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*RELEASE*/ 	 
/*GET-UP */ 	 1,		1,		0,	   0,	  0,	  0,	   0,	   0,	  0,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 0,		   0,	  0,  	   0,		0,		0,		0,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*GET-UP */ 	 
/*throwaim*/ 	 0,		0,		0,	   0,	  1,	  0,	   0,	   0,	  0,	 0,		0,		0,		1,	   0,		0,		 0,			0,		 0,		   0,	  0,  	   0,		0,		0,		0,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*throwaim*/ 	 
/*thrw2cup*/ 	 0,		0,		0,	   1,	  0,	  0,	   0,	   0,	  0,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 0,		   0,	  0,  	   0,		0,		0,		0,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*thrw2cup*/ 	 
/*falling*/ 	 1,		1,		0,	   0,	  0,	  0,	   0,	   1,	  0,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 0,		   0,	  0,  	   0,		0,		0,		1,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*falling*/ 	 
/*bouncing*/ 	 0,		0,		0,	   0,	  0,	  0,	   0,	   0,	  0,	 0,		0,		1,		0,	   0,		0,		 1,			1,		 0,		   0,	  0,  	   0,		0,		0,		0,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*bouncing*/ 	 
/*bnc2cup*/ 	 0,		0,		0,	   1,	  0,	  0,	   0,	   0,	  0,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 0,		   0,	  0,  	   0,		0,		0,		0,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*bnc2cup*/ 	 
/*airwhack*/ 	 1,		1,		0,	   0,	  0,	  0,	   0,	   0,	  0,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 1,		   0,	  0,  	   0,		0,		0,		0,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*airwhack*/ 	 
/*thrwland*/ 	 1,		1,		0,	   0,	  0,	  0,	   0,	   0,	  0,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 0,		   0,	  0,  	   0,		0,		0,		1,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*thrwland*/ 	 
/*slam   */ 	 0,		0,		0,	   0,	  0,	  0,	   0,	   0,	  0,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 0,		   0,	  1,  	   0,		0,		0,		0,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*slam   */ 	 
/*slam2st*/ 	 1,		1,		0,	   0,	  0,	  0,	   0,	   0,	  0,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 0,		   0,	  0,	   0,		0,		0,		0,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*slam2st*/ 	 
/*punch*/	 	 0,		0,		0,	   0,	  0,	  0,	   0,	   0,	  0,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 0,		   0,	  0,	   0,		1,		0,		0,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*punch*/	 	 
/*punch2st*/ 	 1,		1,		0,	   0,	  0,	  0,	   0,	   0,	  0,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 0,		   0,	  0,	   0,		0,		0,		0,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*punch2st*/ 	 
/*slap		*/ 	 1,		1,		0,	   0,	  0,	  0,	   0,	   0,	  0,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 0,		   0,	  0,	   0,		0,		0,		0,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*slap		*/ 	 
/*ballwalk	*/ 	 0,		0,		1,	   0,	  0,	  0,	   0,	   0,	  0,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 0,		   0,	  0,	   0,		0,		0,		0,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*ballwalk	*/ 	 
/*crawl		*/ 	 0,		0,		0,	   0,	  0,	  0,	   0,	   0,	  1,	 0,		1,		0,		0,	   0,		0,		 0,			0,		 0,		   0,	  0,	   0,		0,		0,		0,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*crawl		*/ 	 
/*cartwheel */ 	 1,		1,		0,	   0,	  0,	  0,	   0,	   0,	  0,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 0,		   0,	  0,	   0,		0,		0,		0,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*cartwheel */ 	 
/*climb up*/ 	 1,		1,		0,	   0,	  0,	  0,	   0,	   0,	  0,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 0,		   0,	  0,	   0,		0,		0,		0,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*climb up*/ 	 
/*climb down*/ 	 0,		0,		0,	   0,	  0,	  0,	   0,	   0,	  0,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 0,		   0,	  0,	   0,		0,		0,		0,		  0,	  0,	  1,	   0,		0,		1, 	   0,	 0,	  /*climb down*/ 	 
/*cling*/		 0,		0,		0,	   0,	  0,	  0,	   0,	   0,	  0,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 0,		   0,	  0,	   0,		0,		0,		0,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*cling*/		 
/*dropdown*/	 1,		1,		0,	   0,	  0,	  0,	   0,	   0,	  0,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 0,		   0,	  0,	   0,		0,		0,		0,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*dropdown*/	 
/*hurt		*/	 1,		1,		0,	   0,	  0,	  0,	   0,	   0,	  0,	 0,		0,		0,		0,	   0,		0,		 0,			0,		 1,		   0,	  0,	   0,		0,		0,		0,		  0,	  0,	  0,	   0,		0,		0, 	   0,	 0,	  /*hurt		*/	 
/*push*/         1,     1,      1,     1,     0,      0,       0,      0,     1,     0,     0,      0,      0,     0,       0,       0,         0,       0,        0,     0,       0,       0,      1,      1,        0,      1,      0,       0,       0,      0,     0,    0,   /*push*/         

};



/*	--------------------------------------------------------------------------------
	Function 	: ActionAllowed
	Purpose 	: determine if this action is allowed from the current action
	Parameters 	: hand action, requested action
	Returns 	: bool
	Info 		: looks in a table to determine result
*/
BOOL ActionAllowed(int handAction, int requestedAction)
{
	return (handActionValidityTable[handAction][requestedAction]);
}

/*	--------------------------------------------------------------------------------
	Function 	: ConditionsMet
	Purpose 	: determine if the requirements are met to perform the specified action
	Parameters 	: requested action
	Returns 	: bool
	Info 		:
*/
BOOL ConditionsMet(int requestedAction)
{

	VECTOR	vel;
	double	temp;
	float	handAlt, tempAlt;
	PLANE	*plane = NULL;
	int		x,j;
	VECTOR	tempVect;
	PLATFORM	*platform;

	SubVector(&vel,&hand.pos,&hand.oldpos);

	switch(requestedAction)
	{
		case HAND_IDLE:
			if((joystick.magnitude == 0) && (Magnitude2D(&vel) < 0.01) && (hand.onGround))
				return TRUE;
			break;

		case HAND_WALK:
			if((joystick.magnitude > 2000) && ((hand.flags & BEING_BLOWN) || (hand.onGround)))
			{
				if(hand.action == HAND_PUSH)
				{
					SetVector(&vel,&handControl.handAccel);
					MakeUnit(&vel);
					for(j = 0;j < NUMACTORPLANES;j++)
					{
						if((hand.plane[j]) && (hand.plane[j]->normal.v[Y] < 0.5))
						{
							if(DotProduct2D(&vel,&hand.plane[j]->normal) < -0.4)
								return FALSE;
						}
					}
					return TRUE;
				}
				else
					return TRUE;
			}
			break;

		case HAND_JUMP:
			if((joystick.button[JS_BUTTON_A]) && ((hand.onGround) || (hand.action == HAND_BALLWALK)))
				return TRUE;
			break;

		case HAND_JOINED:
			if(hand.action == HAND_BOUNCE2CUP)
				return TRUE;
			if(handControl.currentActionTime <= 1)
				break;

			if((ball.teleport) || (hand.teleport))
				break;

			if(ballControl.status == BALL_HELD)
				break;
			if(fabs(hand.vel.v[Y]) > 0.3)
				break;
			if((temp = DistanceBetweenPoints(&hand.pos,&ball.pos)) < ball.radius + hand.radius + 0.2)
			{
				if((temp < 0.0001) || (IsActorFacingActor(&hand, &ball, 0.6)))
//				if((IsActorFacingActor(&hand, &ball, 1)))
				{
					if((ballControl.status != BALL_HELD) && (ballControl.status != BALL_BURST))
				 		return TRUE;
				}
			}
			break;

		case HAND_THROWAIM:
			if((joystick.button[JS_BUTTON_B]) && (joystick.buttonStateTime[JS_BUTTON_B] > THROW_DELAY_COUNT))
			{
				if(fabs(hand.pos.v[Y] - ball.pos.v[Y]) < ball.radius*0.8 + 0.3)
					return TRUE;
			}
			break;

		case HAND_THROW2CUP:
			if(!joystick.button[JS_BUTTON_B])
				return TRUE;
			break;

		case HAND_THROW:
			if((!joystick.button[JS_BUTTON_B]) && (handControl.whackStrength > 1))
				return TRUE;
			break;

		case HAND_SKIDDING:
			if(hand.action == HAND_IDLE)
			{
				if((abs(joystick.xPos) < 4000) && (abs(joystick.yPos) < 4000) && (hand.plane[0]) && (hand.plane[0]->normal.v[Y] <= 1 - ((1 - hand.slideLimit)/(terrain[hand.plane[0]->terrain].slidy*terrain[hand.plane[0]->terrain].slidy))) && (Magnitude2D(&hand.vel) > START_SKIDDING_SPEED))
					return TRUE;
			}
			else if((abs(joystick.xPos) < 4000) && (abs(joystick.yPos) < 4000) && ((handControl.lastWalkMode == HANDWALK_RUN) || ((hand.plane[0]) && (terrain[hand.plane[0]->terrain].slidy > 1) && (Magnitude2D(&hand.vel) > 0.15))) && (hand.onGround))
				return TRUE;
			break;

		case HAND_CRAWL:
			if(((joystick.xPos != 0) || (joystick.yPos != 0)) && (handControl.currentActionTime > DUCK_DELAY_COUNT))
				return TRUE;
			break;

		case HAND_DUCK:
			if((hand.onGround) && (joystick.button[JS_BUTTON_Z]) && (joystick.buttonStateTime[JS_BUTTON_Z] > DUCK_DELAY_COUNT)
			   && (hand.action != HAND_CRAWL))
				return TRUE;
			if(hand.action == HAND_CRAWL)
			{
				if((joystick.xPos == 0) && (joystick.yPos == 0))
				{
					return TRUE;
				}
			}
			break;

		case HAND_GETUP:
			if((!joystick.button[JS_BUTTON_Z]) && (handControl.inputDisabledTimer == 0))
				return TRUE;
			break;

		case HAND_PREWHACK:
			if(joystick.button[JS_BUTTON_A])
				return TRUE;
			break;

		case HAND_WHACK:
			if(((!joystick.button[JS_BUTTON_A]) && (handControl.whackStrength > WHACK_DELAY_COUNT)))
			{
				handControl.whackStrength -= WHACK_DELAY_COUNT;
				return TRUE;
			}
			break;

		case HAND_RELEASE:
			if((!joystick.button[JS_BUTTON_A]) && (handControl.whackStrength <= WHACK_DELAY_COUNT))
				return TRUE;
			break;

		case HAND_FALLING:
//			if(!hand.onGround)
			if(handControl.fallCounter > 2)
				return TRUE;
			break;

		case HAND_BOUNCE:
			if((!joystick.button[JS_BUTTON_B]) && (joystick.timeSincePress[JS_BUTTON_B] <= THROW_DELAY_COUNT + 1))
				return TRUE;
			break;							

		case HAND_BOUNCE2CUP:
			if((ball.onGround == TRUE) && (fabs(ball.vel.v[Y]) < 0.1))
				return TRUE;
			break;							

		case HAND_AIRBOURNEWHACK:
			if(joystick.button[JS_BUTTON_A])
				return TRUE;
			break;							

		case HAND_THROW2LAND:
			if((hand.onGround == TRUE) && (handControl.currentActionTime > 1) && (handControl.catPlat == NULL)) 
				return TRUE;
			break;

		case HAND_SLAM:
			if(joystick.debounce[JS_BUTTON_Z])
				return TRUE;
			break;

		case HAND_SLAM2ST:
			if(hand.onGround == TRUE)
				return TRUE;
			break;

		case HAND_PUNCH:
			if((joystick.debounce[JS_BUTTON_A]) && (handControl.currentActionTime > 1))
				return TRUE;
			break;

		case HAND_PUNCH2ST:
			if(hand.onGround == TRUE)
				return TRUE;
			break;

		case HAND_SLAP:
			if(joystick.debounce[JS_BUTTON_B])
				return TRUE;
			break;

		case HAND_BALLWALK:
			if(handControl.currentActionTime <= 1)
				break;
			if((ball.teleport) || (hand.teleport))
				break;
			if(ballControl.status == BALL_HELD)
				break;
			if(hand.vel.v[Y] > 0)
				break;
			if(ball.onGround == 0)
				break;
			if(DistanceBetweenPoints2D(&hand.pos, &ball.pos) < ball.radius * 0.8)
			{
				float temp1, temp2;
				temp1 = hand.pos.v[Y] - ball.pos.v[Y];
				temp2 = hand.radius + ball.radius + 0.3;
				if(fabs(temp1 - temp2) < 0.5)
					return TRUE;

			}
  			break;

		case HAND_CARTWHEEL:
			if((!joystick.button[JS_BUTTON_Z]) && (joystick.timeSincePress[JS_BUTTON_Z] <= DUCK_DELAY_COUNT + 1))
				return TRUE;
			break;

		case HAND_CLIMBDOWN:
			if((hand.onGround) && (hand.speed > 0) && (hand.speed < 0.3) && (joystick.magnitude < JOYSTICK_MAX/2))// && ((hand.platform == NULL) || ((hand.platform->flags & TILTING) == 0)))
			{
				tempAlt = Altitude(&handControl.verticalEdgeChecker,&plane,&platform);
				handAlt = Altitude(&hand,&plane,&platform);
				if((plane) && (plane->normal.v[Y] > CLIMBDOWN_SLOPE_LIMIT) && ((tempAlt - handAlt > 4) || (tempAlt < 0)))
				{
					if(HandOverstepsEdge(&handControl.verticalEdgeChecker, plane))
					{
						handControl.altPlane = plane;
						handControl.sitPlat = platform;
						return TRUE;
					}
				}
			}
			break;

		case HAND_CLING:
			if((hand.onGround == FALSE))
			{
				if((handControl.altitude > 3) || (handControl.altitude < 0))
				{
					for(x = 0; x < 2; x++)
					{
						tempAlt = Altitude(&handControl.frontProbe[x], &plane,&platform);
						if((tempAlt < 1.2) && (tempAlt > 0))
						{
							handControl.frontProbeAltitude[x] = tempAlt;
							if(HandOverstepsEdge(&handControl.frontProbe[x], plane))
							{
								return TRUE;
							}
						}
						else handControl.frontProbeAltitude[x] = -1;
					}
				}
			}
			break;


		case HAND_CLIMBUP:
			if(joystick.debounce[JS_BUTTON_A])
				return TRUE;
			if((joystick.xPos != 0) || (joystick.yPos != 0))
			{
//				if(FindShortestAngle(hand.rot.v[Y], Aabs(joystick.direction + PI + camera.rot.v[Y])) < PI/2)
				if(FindShortestAngle(hand.rot.v[Y], Aabs(joystick.direction + PI + camera.rot.v[Y])) > PI/2)
					return TRUE;
			}
			break;

		case HAND_DROPDOWN:
			if((joystick.xPos != 0) || (joystick.yPos != 0))
			{
//				if(FindShortestAngle(hand.rot.v[Y], Aabs(joystick.direction + PI + camera.rot.v[Y])) > PI/2)
				if(FindShortestAngle(hand.rot.v[Y], Aabs(joystick.direction + PI + camera.rot.v[Y])) < PI/2)
					return TRUE;
			}
			break;

		case HAND_PUSH:
			if((joystick.magnitude) && (hand.onGround))
			{
				SetVector(&vel,&handControl.handAccel);
				MakeUnit(&vel);
				for(j = 0;j < NUMACTORPLANES;j++)
				{
					if((hand.plane[j]) && (hand.plane[j]->normal.v[Y] < 0.5))
					{
						if(DotProduct2D(&vel,&hand.plane[j]->normal) < -0.4)
						{
							if(handControl.pushCount++ > 2)
								return TRUE;
							else
								break;
						}
					}
				}
				if(j >= NUMACTORPLANES)
					handControl.pushCount = 0;
			}
			break;
	}


	return FALSE;
}

/*
#define HAND_NORMAL_RADIUS		((detailFlags & DF_STEVE) ? 0.9 : 1.4)
#define HAND_JOINED_RADIUS		((detailFlags & DF_STEVE) ? 0.9 : 1.3)
#define HAND_DUCKED_RADIUS		((detailFlags & DF_STEVE) ? 0.9 : 0.9)
#define HAND_THROWAIM_RADIUS	((detailFlags & DF_STEVE) ? 0.9 : 2.0)
#define HAND_WHACK_RADIUS		((detailFlags & DF_STEVE) ? 0.9 : 0.8)
#define HAND_BOUNCE_RADIUS		((detailFlags & DF_STEVE) ? 0.9 : 1.2)
#define HAND_SLAM_RADIUS		((detailFlags & DF_STEVE) ? 0.9 : 0.8)
#define HAND_PUNCH_RADIUS		((detailFlags & DF_STEVE) ? 0.9 : 0.8)
*/
#define HAND_NORMAL_RADIUS		(1.4)
#define HAND_JOINED_RADIUS		(ball.radius + 0.05)
#define HAND_DUCKED_RADIUS		(1.3)
#define HAND_THROWAIM_RADIUS	(2.0)
#define HAND_WHACK_RADIUS		(0.8)
#define HAND_BOUNCE_RADIUS		(1.2)
#define HAND_SLAM_RADIUS		(0.8)
#define HAND_PUNCH_RADIUS		(0.8)
#define HAND_CLING_RADIUS		(0.6)
#define HAND_CARTWHEEL_RADIUS	(0.6)


/*	--------------------------------------------------------------------------------
	Function 	: ApplyAction
	Purpose 	: do all the stuff needed for new action
	Parameters 	: new action
	Returns 	: none
	Info 		:
*/
void ApplyAction(int action)
{
	int lastActionTime = handControl.currentActionTime;
	VECTOR	tempVect;

	hand.lastAction = hand.action;
	hand.action = action;
	ball.maxSpeed = NORMALBALL_MAXSPEED * 2;
	ball.maxAccelSpeed = NORMALBALL_MAXSPEED;
//	ball.maxSpeed = 2.5;
//	hand.maxSpeed = 1;
	handControl.currentActionTime = 0;
//	handControl.inputDisabled = FALSE;

	handControl.bounceAnimate = NO;
	hand.flags |= COLLISION;
	hand.numExtraCollisionSpheres = 0;

	switch(action)
	{
		case HAND_IDLE:
			if(handControl.idleTime > 200)
			{
				AnimateActor(&hand,HANDANIM_IDLE3, NO, NO, 0);
				AnimateActor(&hand,HANDANIM_STAND, YES, YES, 0);
			}
			else
				AnimateActor(&hand,HANDANIM_STAND, YES, NO, 0);
			hand.radiusAim = HAND_NORMAL_RADIUS;
			break;

		case HAND_WALK:
			if(handControl.idleTime > 200)
			{
				AnimateActor(&hand,HANDANIM_IDLE3, NO, NO, 0);
				AnimateActor(&hand,HANDANIM_TIPTOE, YES, YES, 0);
			}
			else
				AnimateActor(&hand,HANDANIM_TIPTOE, YES, NO, 0);
			hand.radiusAim = HAND_NORMAL_RADIUS;
			handControl.walkMode = -1;
			break;

		case HAND_SKIDDING:
			DisableControl(4);
			handControl.newActionDelay = 4;
			dprintf("skid\n");
			AnimateActor(&hand,HANDANIM_SKID, NO, NO, 0);
//			SetVector(&hand.vel, &zero);
			hand.radiusAim = HAND_NORMAL_RADIUS;
			hand.rotaim.v[Y] = Aabs(atan2(hand.vel.v[X],hand.vel.v[Z]) + PI);
			break;

		case HAND_JUMP:
			handControl.longJump = NO;
			if(handControl.idleTime > 200)
			{
				AnimateActor(&hand,HANDANIM_IDLE3, NO, NO, 0);
				AnimateActor(&hand,HANDANIM_JUMP, NO, YES, 0);
			}
			else
				AnimateActor(&hand,HANDANIM_JUMP, NO, NO, 0);
			hand.vel.v[Y] = 0;
			hand.vel.v[Y] += 1.3 * SCALINGFACTOR;
			hand.radiusAim = HAND_NORMAL_RADIUS;
			PlaySample(SFX_HANDJUMP, 0, &hand.pos, 100);
			handControl.newActionDelay = 1;
			hand.velInertia = hand.normInertia;

			if(handControl.actionLastFrame == HAND_BALLWALK)
			{
				hand.vel.v[X] = hand.vel.v[Z] = 0;
				hand.vel.v[Y] += 0.08 * (float)handControl.jumpStrength;
//				AnimateActor(&ball,BALLANIM_BOUNCE, NO, NO, 0);
				if(handControl.jumpStrength < 6)
					handControl.jumpStrength++;
			}
			break;

		case HAND_JOINED:
			if((hand.lastAction != HAND_THROW2CUP) && (hand.lastAction != HAND_BOUNCE2CUP))
			{
				MoveHandToBall();
				SetVector(&hand.vel, &ball.vel);
				AnimateActor(&hand,HANDANIM_ST2CUP, NO, NO, 0);
				hand.radiusAim = HAND_JOINED_RADIUS;
				handControl.newActionDelay = 4;
				ball.owner = &hand;
			}
/*			hand.numExtraCollisionSpheres = 2;
			hand.collisionSphere[0].offset.v[X] = 0.6;
			hand.collisionSphere[0].offset.v[Y] = 0;
			hand.collisionSphere[0].offset.v[Z] = 0.6;
			hand.collisionSphere[0].radius = 0.8;
			hand.collisionSphere[1].offset.v[X] = 1;
			hand.collisionSphere[1].offset.v[Y] = 0;
			hand.collisionSphere[1].offset.v[Z] = 1;
			hand.collisionSphere[1].radius = 0.7;
*/			break;

		case HAND_PREWHACK:
			AnimateActor(&hand,HANDANIM_WHACK1, NO, NO, 0);
			handControl.whackStrength = 0;
			hand.radiusAim = HAND_WHACK_RADIUS;
			break;

		case HAND_WHACK:
			AnimateActor(&hand,HANDANIM_WHACK2, NO, NO, 0);
			AnimateActor(&hand,HANDANIM_STAND, YES, YES, 0);
			handControl.newActionDelay = 8;
			SetVector(&ball.rot, &hand.rot);
			handControl.whackDelay = BALL_WHACK_DELAY;
			hand.radiusAim = HAND_NORMAL_RADIUS;
			break;

		case HAND_RELEASE:
			AnimateActor(&hand,HANDANIM_CUP2ST, NO, NO, 0);
			AnimateActor(&hand,HANDANIM_STAND, YES, YES, 0);
			handControl.newActionDelay = 10;
			AccelerateActor2D(&hand, 0.7);
			hand.vel.v[Y] += 0.2;
			hand.radiusAim = HAND_NORMAL_RADIUS;
			if(cameraInfo.positionToTrack == &ball.pos)
			{
				cameraInfo.positionToTrack = &hand.pos;
				cameraInfo.velToTrack = &hand.vel;
			}
			handControl.releaseTimer = 6;
			hand.velInertia = hand.normInertia;
			break;

		case HAND_DUCK:
			if(hand.lastAction != HAND_CRAWL)
			{
				AnimateActor(&hand,HANDANIM_ST2DUCK, NO, NO, 0);
				hand.radiusAim = HAND_DUCKED_RADIUS;
				handControl.newActionDelay = WAIT_FOR_ANIMEND;
				DisableControl(8);
				hand.maxAccelSpeed = CRAWL_HAND_MAXSPEED;
			}
			else
				AnimateActor(&hand,HANDANIM_ROLL2DUCK, NO, NO, 0);
//			hand.turnSpeed = 0.8 * SCALINGFACTOR;
			break;
		case HAND_CRAWL:
			AnimateActor(&hand,HANDANIM_DUCK2ROLL, NO, NO, 0);
			AnimateActor(&hand,HANDANIM_ROLL, YES, YES, 0);
			hand.normInertia = hand.velInertia = 0.99;
//			hand.turnSpeed = 0.1;
			break;
		case HAND_GETUP:
			if((hand.lastAction == HAND_DUCK) && (lastActionTime < DUCK_DELAY_COUNT));
			else
			{			
				AnimateActor(&hand,HANDANIM_DUCK2ST, NO, NO, 0);
				AnimateActor(&hand,HANDANIM_STAND, YES, YES, 0);
				handControl.newActionDelay = 4;
				hand.radius = 1.0;
				hand.radiusAim = HAND_NORMAL_RADIUS;
			}
			hand.maxAccelSpeed = DEFAULT_HAND_MAXSPEED;
			hand.normInertia = hand.velInertia = HAND_INERTIA;
			break;

		case HAND_THROW2CUP:
			hand.rot.v[X] = hand.rotaim.v[X] = 0;
			AnimateActor(&hand,HANDANIM_THROW2CUP, NO, NO, 0);
			handControl.newActionDelay = 5;
			hand.radiusAim = HAND_JOINED_RADIUS;
			hand.flags &= -1 - LO_GRAVITY;
			break;

		case HAND_THROWAIM:
			if(hand.lastAction == HAND_JOINED)
				AnimateActor(&hand,HANDANIM_THROWAIM, NO, YES, 0);
			else
				AnimateActor(&hand,HANDANIM_BNCE2THROWAIM, NO, YES, 0);
			hand.rot.v[X] = hand.rotaim.v[X] = 5.8;
			handControl.newActionDelay = 4;
			hand.radiusAim = HAND_THROWAIM_RADIUS;
			handControl.whackStrength = 0;
			break;

		case HAND_THROW:
			hand.rot.v[X] = hand.rotaim.v[X] = 0;
			handControl.newActionDelay = 10;
			handControl.whackDelay = 1;
			break;

		case HAND_FALLING:
			AnimateActor(&hand,HANDANIM_JUMP, NO, NO, 0);
			hand.radiusAim = HAND_NORMAL_RADIUS;
			break;

		case HAND_BOUNCE2CUP:
			AnimateActor(&hand,HANDANIM_BOUNCE2CUP, NO, NO, 0);
			handControl.newActionDelay = WAIT_FOR_ANIMEND;
			hand.radiusAim = HAND_JOINED_RADIUS;
			hand.flags &= -1 - LO_GRAVITY;
			break;

		case HAND_BOUNCE:
			AnimateActor(&hand,HANDANIM_CUP2BOUNCE, NO, NO, 0);
			hand.flags |= LO_GRAVITY;
			hand.flags &= -1 - COLLISION;
			hand.platform = hand.oldPlatform = hand.touchPlatform = hand.oldTouchPlatform = NULL;
			hand.onGround = FALSE;
			hand.plane[0] = NULL;
   			ball.vel.v[Y] += 1 * SCALINGFACTOR;
			ball.maxAccelSpeed = BallBounceMaxSpeed();
			hand.radiusAim = HAND_BOUNCE_RADIUS;
			break;
 
 		case HAND_AIRBOURNEWHACK:
			hand.flags &= -1 - LO_GRAVITY;
			AnimateActor(&hand,HANDANIM_WHACK2, NO, NO, 0);
			AnimateActor(&hand,HANDANIM_STAND, YES, YES, 0);
			handControl.newActionDelay = 9;
			SetVector(&ball.rot, &hand.rot);
			handControl.whackDelay = BALL_WHACK_DELAY;
			hand.radiusAim = HAND_NORMAL_RADIUS;
			handControl.whackStrength = 4;
			break;

		case HAND_THROW2LAND:
			VECTOR tempVect;

			AnimateActor(&hand,HANDANIM_THROW2LAND, NO, NO, 0);
			AnimateActor(&hand,HANDANIM_STAND, YES, YES, 0);
			handControl.newActionDelay = 4;
//							SetVector(&tempVect, &hand.pos);
//							tempVect.v[Y] -= 1;
//							CreateDustCircle(&tempVect, 4, 0);
			ScaleVector2D(&hand.vel, 0.7);
			break;

		case HAND_SLAM:
			PlaySample(SFX_SWISH, 0, &hand.pos, 100);
			AnimateActor(&hand, HANDANIM_SLAM, NO, NO, 0);
			handControl.newActionDelay = 4;
			hand.radiusAim = HAND_SLAM_RADIUS;
			break;

		case HAND_SLAM2ST:
			handControl.newActionDelay = 8 + SLAM2ST_DELAY_COUNT;
			break;

		case HAND_PUNCH:
			AnimateActor(&hand, HANDANIM_PUNCH, NO, NO, 0);
			ScaleVector(&hand.vel, 0.5);
			hand.radiusAim = HAND_PUNCH_RADIUS;
			break;

		case HAND_PUNCH2ST:
			handControl.newActionDelay = 10;
			break;

		case HAND_SLAP:
			handControl.newActionDelay = WAIT_FOR_ANIMEND;
			DisableControl(8);
			AnimateActor(&hand, HANDANIM_SLAP, NO, NO, 0);
			AnimateActor(&hand, HANDANIM_STAND, YES, YES, 0);
//			FireBulletFromActor(&hand);
			FireSlapBullet();
			break;

		case HAND_BALLWALK:
			AnimateActor(&hand, HANDANIM_THROW2LAND, NO, NO, 0);
			ball.owner = &hand;
			handControl.walkMode = -1;
			AddVector2D(&ball.vel, &ball.vel, &hand.vel);
			break;

		case HAND_CARTWHEEL:
			AnimateActor(&hand, HANDANIM_CARTWHEEL, NO, NO, 0);
			handControl.newActionDelay = WAIT_FOR_ANIMEND;
			hand.radiusAim = HAND_CARTWHEEL_RADIUS;

			hand.collisionSphere[0].offset.v[X] = 0;
			hand.collisionSphere[0].offset.v[Y] = HAND_NORMAL_RADIUS - HAND_CARTWHEEL_RADIUS;
			hand.collisionSphere[0].offset.v[Z] = 0;
			hand.collisionSphere[0].radius = HAND_CARTWHEEL_RADIUS;

			hand.collisionSphere[1].offset.v[X] = 0;
			hand.collisionSphere[1].offset.v[Y] = -(HAND_NORMAL_RADIUS - HAND_CARTWHEEL_RADIUS);
			hand.collisionSphere[1].offset.v[Z] = 0;
			hand.collisionSphere[1].radius = HAND_CARTWHEEL_RADIUS;

			hand.numExtraCollisionSpheres = 2;
			break;

		case HAND_CLIMBDOWN:
			dprintf("Climb down\n");
			hand.radius = hand.radiusAim = HAND_CLING_RADIUS;
//			hand.flags &= -1 - GRAVITY;// - COLLISION;
//			hand.mass = 100000;
			SubVector2D(&tempVect, &handControl.verticalEdgeChecker.pos, &hand.pos);
			ScaleVector2D(&tempVect, 0.1);
			AddVector2D(&hand.pos, &hand.pos, &tempVect);
//			hand.pos.v[Y] -= 1.4;
			SetVector(&hand.vel, &zero);
			handControl.newActionDelay = 20;
			AnimateActor(&hand, HANDANIM_CLIMBDOWN, NO, NO, 0);
			AnimateActor(&hand, HANDANIM_CLIMBDOWNIDLE, NO, YES, 0);
			hand.rot.v[Y] = hand.rotaim.v[Y] = Aabs(hand.rot.v[Y] + PI);
			break;

		case HAND_CLING:
			dprintf("Cling\n");
			
			AnimateActor(&hand, HANDANIM_CLING, NO, NO, 0);
			hand.radius = hand.radiusAim = HAND_CLING_RADIUS;
			hand.rot.v[Y] = hand.rotaim.v[Y] = Aabs(hand.rot.v[Y] + PI);
			hand.flags &= -1 - GRAVITY;// - COLLISION;
//			SubVector2D(&tempVect, &handControl.verticalEdgeChecker.pos, &hand.pos);
//			ScaleVector2D(&tempVect, 2);
//			AddVector2D(&hand.pos, &hand.pos, &tempVect);
//			hand.pos.v[Y] += 0.4;
			SetVector(&hand.vel, &zero);
			handControl.newActionDelay = 20;

//			MaintainAction();
			break;


		case HAND_CLIMBUP:
			AnimateActor(&hand, HANDANIM_CLIMBUP, NO, NO, 0);
//			hand.rot.v[Y] = hand.rotaim.v[Y] = Aabs(hand.rot.v[Y] + PI);
			hand.radius = hand.radiusAim = HAND_NORMAL_RADIUS;
			hand.flags |= GRAVITY;// - COLLISION;
			handControl.newActionDelay = 20;
			DisableControl(20);
			break;

		case HAND_DROPDOWN:
//			hand.radiusAim = HAND_NORMAL_RADIUS;
//			hand.radius = hand.radiusAim = HAND_NORMAL_RADIUS;
			hand.flags |= GRAVITY;// - COLLISION;
			AccelerateActor2D(&hand, -0.4);
			break;

		case HAND_HURT:
/*
			if(IsHandHoldingBall() == TRUE)
			{
				ApplyAction(HAND_RELEASE);
				DisableControl(20);
			}
*/
//			handControl.newActionDelay = 20;
			handControl.newActionDelay = WAIT_FOR_ONGROUND + 3;
			AnimateActor(&hand, HANDANIM_LONGJUMP, NO, NO, 0);
			break;

		case HAND_PUSH:
			AnimateActor(&hand,HANDANIM_ST2DUCK, NO, NO, 0);
//			hand.radiusAim = HAND_DUCKED_RADIUS;
			handControl.newActionDelay = WAIT_FOR_ANIMEND;
//			DisableControl(8);
			break;
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: MaintainAction
	Purpose 	: update relevant things for current action
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void MaintainAction()
{
	int		y, x;
	int		numStars = 8;
	double	angle;
	double	angleStep = (2*PI) / numStars;
	double	burstRadius = 0.4;
	float val;
	float	bestSlope = 0.5;
	VECTOR	tempVect, tempVect2, vect;
	float	temp, temp2;
	PLANE	*plane = NULL;
	ACTOR	*actor;
	hand.mass = 10;

//-------------------------------------------------
//make hand face up slopes
	x = 0;
	if(hand.action == HAND_JOINED)
		actor = &ball;
	else
		actor = &hand;
	if(((hand.onGround) || (hand.action == HAND_JOINED)) && (hand.action != HAND_THROWAIM))
	{
		while(actor->plane[x] != NULL)
		{
			val = 1 - actor->plane[x]->normal.v[Y];
			if(val < bestSlope)
			{
				bestSlope = val;
				plane = actor->plane[x];
			}
			x++;
		}
		if(plane)
		{
				float perfectDist, actualDist;
				VECTOR tempVect, tempVect2;
				VECTOR normalVect;

				tempVect2.v[X] = 0;
				tempVect2.v[Y] = 0;
				tempVect2.v[Z] = 1;
				RotateVector2D(&tempVect2, &tempVect2, hand.rot.v[Y]);
				AddVector(&tempVect2, &tempVect2, &actor->pos);

				//test distance to point exactly above hand
				normalVect.v[X] = normalVect.v[Z] = 0;
				normalVect.v[Y] = 1;
				AddVector(&tempVect, &normalVect, &actor->pos);
				perfectDist = DistanceBetweenPoints(&tempVect, &tempVect2);

				//test distance to point projected from face normal
				AddVector(&tempVect, &plane->normal, &actor->pos);
				actualDist = DistanceBetweenPoints(&tempVect, &tempVect2);
				
				val = (actualDist - perfectDist) * 12;

				if(val >= 0)
					val = Min(val, 6);
				else
					val = Max(val, -6);

				if(hand.action != HAND_JOINED)
					val *= 0.5;

				hand.rotaim.v[X] = -(val / 10);


		}
	}
	else if(hand.action != HAND_THROWAIM)
		hand.rotaim.v[X] = 0;

//-------------------------------------------------







//reset jumpstrength to zero when hand actually lands (this is used for jumping on the ball)
	if(hand.onGround == TRUE)
	{
		handControl.jumpStrength = 0;
		handControl.longJump = NO;
	}

// this is used for when the ball is to be released (eg after a whack) but needs time to move first
	if((handControl.releaseTimer) && (ball.owner == &hand))
	{
		if(--handControl.releaseTimer == 0)
			ball.owner = NULL;
	}
	else
		handControl.releaseTimer = 0;

	switch(hand.action)
	{
		case HAND_JOINED:
			ball.maxAccelSpeed = hand.maxAccelSpeed;
			break;
			

		case HAND_IDLE:
			if(handControl.idleTime > 400)
			{
				if(hand.currentAnimation == HANDANIM_STAND)
				{
					AnimateActor(&hand, HANDANIM_IDLE1, NO, NO, 0);
					AnimateActor(&hand, HANDANIM_IDLE2, NO, YES, 0);
				}
				if(!(frameCount % 10))
				{
					if(rand()%10 < 3)
					{
						AnimateActor(&hand, HANDANIM_IDLE2, NO, YES, 0);
						AnimateActor(&hand, HANDANIM_IDLE2, NO, YES, 0);
					}
				}


			}
			break;
		
		case HAND_WALK:	
		case HAND_PUSH:
			y = handControl.walkMode;
			if(y == HANDWALK_ONBALL)
				break;
			temp = Magnitude2D(&hand.vel);

			if((hand.onGround == FALSE) && (hand.flags & BEING_BLOWN))
				hand.maxAccelSpeed = DEFAULT_HAND_MAXSPEED/5;
			else
				hand.maxAccelSpeed = DEFAULT_HAND_MAXSPEED;

			if(temp == 0)
				handControl.walkMode = -1;
			else
			if(temp	< 0.2 * SCALINGFACTOR)
			{
				handControl.lastRunTimer++;
				handControl.walkMode = HANDWALK_TIPTOE;
				temp = Min(2, hand.speed * 4);
			}
			else
				if(temp < 0.5 * SCALINGFACTOR)
				{
					handControl.lastRunTimer++;
					handControl.walkMode = HANDWALK_WALK;
					temp = Min(1, hand.speed * 2);
				}
				else
				{
					handControl.lastRunTimer = 0;
					handControl.walkMode = HANDWALK_RUN;
					temp = 0.8 * SCALINGFACTOR;
				}

			if(hand.action != HAND_PUSH)
			{
				if(y != handControl.walkMode)
				{
					switch(handControl.walkMode)
					{
						case HANDWALK_TIPTOE:
								AnimateActor(&hand, HANDANIM_TIPTOE, YES, NO, 0);
								break;
						case HANDWALK_WALK:
								AnimateActor(&hand, HANDANIM_WALK, YES, NO, 0);
								break;
						case HANDWALK_RUN:
								AnimateActor(&hand, HANDANIM_RUN, YES, NO, 0);
								break;
					}
				}
				SetAnimationSpeed(&hand, temp);
			}
			break;

		case HAND_DUCK:
			if(Magnitude2D(&hand.vel) > 0.5)
				CreateDustCircle(&hand.pos, 2, 0);
			ScaleVector(&hand.vel, 0.8);
			break;

		case HAND_WHACK:
		case HAND_AIRBOURNEWHACK:
			if(handControl.whackDelay)
			{
				handControl.whackDelay--;
				if(handControl.whackDelay == 0)
				{
					if(handControl.whackStrength > 9)
						handControl.whackStrength = 9;
					if(handControl.whackStrength < 3)
  						handControl.whackStrength = 3;
					temp = 2*handControl.whackStrength;
					temp /= ball.mass;
					AccelerateActorFromActor(&hand, &ball, -temp*SCALINGFACTOR);
					if(hand.action == HAND_WHACK)
					{
						temp = 5/ball.mass;
						temp *= (handControl.whackStrength) / 7;
						ball.vel.v[Y] += temp;
					}
					else
					{
						temp = 12/ball.mass;
						ball.vel.v[Y] = temp * SCALINGFACTOR;
					}
//					handControl.whackStrength = 0;
					PlaySample(SFX_HANDWHACK, 0, &hand.pos, 100);
					if(cameraInfo.positionToTrack == &ball.pos)
					{
						cameraInfo.positionToTrack = &hand.pos;
						cameraInfo.velToTrack = &hand.vel;
					}
					handControl.releaseTimer = 6;
				}
			}
			break;

		case HAND_PREWHACK:
			handControl.whackStrength += 0.4;
			MoveHandToBall();
			SetVector(&hand.vel, &zero);
			break;

		case HAND_THROWAIM:
			if(hand.reachedEndOfAnimation)
			{
				handControl.whackStrength += 0.25;
				if(hand.rot.v[X] > 5)
					hand.rotaim.v[X] = Aabs(hand.rotaim.v[X] + 0.03);
			}
			if(handControl.whackStrength > 5)
				handControl.whackStrength = 5;
			break;

		case HAND_THROW:
			if(handControl.whackDelay)
			{
				handControl.whackDelay--;
				if(handControl.whackDelay == 0)
				{
					AnimateActor(&hand,HANDANIM_THROW, NO, NO, 0);
					AnimateActor(&hand,HANDANIM_STAND, YES, YES, 0);
					handControl.newActionDelay = 12;
					temp = 0.12 * ((handControl.whackStrength/2) + 5);
					temp *= 10/ball.mass;
					AccelerateActorFromActor(&hand, &ball, -(temp * SCALINGFACTOR));
					ball.vel.v[Y] += (temp * 2) * SCALINGFACTOR;
					hand.vel.v[Y] += 1.2 * SCALINGFACTOR;
					hand.radiusAim = HAND_NORMAL_RADIUS;
					hand.radius = HAND_NORMAL_RADIUS;
					hand.flags &= -1 - LO_GRAVITY;
					PlaySample(SFX_SWISH, 0, &hand.pos, 100);
					if(cameraInfo.positionToTrack == &ball.pos)
					{
						cameraInfo.positionToTrack = &hand.pos;
						cameraInfo.velToTrack = &hand.vel;
					}
					handControl.releaseTimer = 6;

				}
			}
			break;
  
  		case HAND_SKIDDING:
			SetVector(&tempVect, &hand.pos);
			tempVect.v[Y] -= 1;
			if(handControl.currentActionTime <= 3)
				CreateDustCircle(&tempVect, 2, 0);
			if((hand.plane[0] == NULL) || (terrain[hand.plane[0]->terrain].slidy <= 1))
				ScaleVector(&hand.vel, 0.8);
//			if(Magnitude2D(&hand.vel) > START_SKIDDING_SPEED/2)
//				hand.rotaim.v[Y] = Aabs(atan2(hand.vel.v[X],hand.vel.v[Z]) + PI);
			break;

		case HAND_BOUNCE:
			BOOL reoa;
			reoa = hand.reachedEndOfAnimation;
			temp = DistanceBetweenPoints(&hand.pos, &ball.pos);

			if(temp < ball.radius + 0.1)
			{
				if(reoa == TRUE)
				{
					if(hand.currentAnimation != HANDANIM_BOUNCE)
						AnimateActor(&hand,HANDANIM_BOUNCE, NO, NO, 0);
				}

			}
			else
			{
				if(reoa == TRUE)
				{
					if(hand.currentAnimation != HANDANIM_BOUNCE3)
						AnimateActor(&hand,HANDANIM_BOUNCE3, NO, NO, 0);
				}
			}

			if(handControl.bounceMe)
			{
				if(temp < ball.radius + 0.7)
				{
					if((ball.vel.v[Y] < -0.4) && (ball.vel.v[Y] > -0.6))
					{
						handControl.bounceMe = NO;
						ball.vel.v[Y] -= 1 * SCALINGFACTOR;
						if(handControl.actionLastFrame == HAND_BOUNCE)
						{
							AnimateActor(&hand,HANDANIM_BOUNCE2, NO, NO, 0);
						}
					}
				}
				
			}
			if((!joystick.debounce[JS_BUTTON_B]) && (joystick.timeSincePress[JS_BUTTON_B] == 2))
			{
				handControl.bounceMe = YES;
				if((hand.reachedEndOfAnimation == TRUE) && (handControl.bounceAnimate == 0))
				{
					handControl.bounceAnimate = YES;
				}
			}
			break;

		case HAND_SLAM:
			if(handControl.currentActionTime < 5)
				hand.vel.v[Y] = 0;
			else if(handControl.currentActionTime == 5)
				hand.vel.v[Y] = -2 * SCALINGFACTOR;

			hand.vel.v[X] /= 2;
			hand.vel.v[Z] /= 2;
			break;

		case HAND_SLAM2ST:
			if(handControl.currentActionTime == 0)
			{
				angle = 0;
				for(y = 0; y < numStars; y++)
				{
					SetVector(&tempVect, &zero);
					tempVect.v[X] = burstRadius;
					RotateVector2D(&tempVect, &tempVect, angle);
					angle += angleStep;
					AddDebris(DUST, &hand.pos, &tempVect);
				}
			}
			if(handControl.currentActionTime == SLAM2ST_DELAY_COUNT)
			{
				AnimateActor(&hand, HANDANIM_SLAM2ST, NO, NO, 0);
				AnimateActor(&hand,HANDANIM_STAND, YES, YES, 0);
				handControl.newActionDelay = 6;
				hand.vel.v[Y] += 0.8 * SCALINGFACTOR;
				hand.radiusAim = HAND_NORMAL_RADIUS;
			}
			break;
										
		case HAND_JUMP:	
			if(handControl.currentActionTime > 1)
			{
				if(ball.owner == &hand)
					ball.owner = NULL;
			}

			if((handControl.currentActionTime > 2) && (handControl.currentActionTime < 7))
			{
				if(joystick.button[JS_BUTTON_A])
					hand.vel.v[Y] += 0.08;
			}
//			if((joystick.buttonStateTime[0] == LONGJUMP_DELAY_COUNT) && (Magnitude2D(&hand.vel) > 0.38) && (joystick.button[0]))
//				AnimateActor(&hand,HANDANIM_LONGJUMP, NO, NO, 0);
			if((handControl.currentActionTime > 1) && (joystick.debounce[JS_BUTTON_A]) && (handControl.longJump == NO))
			{
				if(hand.vel.v[Y] >= -0.5)
				{
					AnimateActor(&hand,HANDANIM_LONGJUMP, NO, NO, 0);
					AccelerateActor2D(&hand, Magnitude2D(&hand.vel));
					
					hand.vel.v[Y] = 1;
	//				hand.vel.v[Y] += 1;
					handControl.longJump = YES;
				}
			}

			break;

		case HAND_PUNCH:
			if((joystick.button[JS_BUTTON_A]) && (handControl.currentActionTime < 10))
			{
				SetVector(&hand.vel, &zero);
				AccelerateActor2D(&hand, 0.2);
			}
			else
			{
				hand.maxSpeed = 2;
				AccelerateActor2D(&hand, -2);
			}
			break;

		case HAND_PUNCH2ST:
			if(handControl.currentActionTime < 5)
				CreateDustCircle(&hand.pos, 1, 0.2);
			if(handControl.currentActionTime == 5)
			{
				AnimateActor(&hand, HANDANIM_PUNCH2ST, NO, NO, 0);
				AnimateActor(&hand,HANDANIM_STAND, YES, YES, 0);
				hand.radiusAim = HAND_NORMAL_RADIUS;

			}
			break;

		case HAND_BALLWALK:
			if(!joystick.button[JS_BUTTON_A])
				handControl.jumpStrength = 0;

			// fall off if ball is moving too fast
//			if(ball.speed > 0.6)
			if(((Magnitude2D(&ball.vel) > max(0.3,ball.radius/7.5)) && (handControl.currentActionTime > 6)) || (ball.vel.v[Y] < -gravity * 6))
			{
				ball.maxAccelSpeed = NORMALBALL_MAXSPEED;
				ApplyAction(HAND_WALK);
				SetVector(&hand.vel, &ball.vel);
				FlipVector(&hand.vel);
				ScaleVector(&hand.vel, 2);
				hand.vel.v[Y] = 0.2;
				ball.owner = NULL;
			}

			// fall off if hand is too far above ball
			if((hand.pos.v[Y] - ball.pos.v[Y] > (ball.radius + hand.radius) + 1) || (ball.vel.v[Y] > gravity * 3))
			{
				ball.maxAccelSpeed = NORMALBALL_MAXSPEED;
				ApplyAction(HAND_JUMP);
			}

			if((joystick.xPos == 0) && (joystick.yPos == 0))
			{
				if(ball.speed > 0.01)
				{
					hand.rotaim.v[Y] = ball.rotaim.v[Y];
				}
			}

			hand.onGround = TRUE;
			y = handControl.walkMode;
			if(y == HANDWALK_ONBALL)
				break;
			temp = Magnitude2D(&ball.vel);
			if(temp	< 0.01 * SCALINGFACTOR)
			{
				if(handControl.walkMode != -1)
					AnimateActor(&hand, HANDANIM_STAND, YES, NO, 0);
				handControl.walkMode = -1;
				break;
			}
			else if(temp < 0.2 * SCALINGFACTOR)
			{
				handControl.walkMode = HANDWALK_TIPTOE;
				temp = Min(2, ball.speed * 4);
			}
			else if(temp < 0.5 * SCALINGFACTOR)
			{
				handControl.walkMode = HANDWALK_WALK;
				temp = Min(1, ball.speed * 2);
			}
			else
			{
				handControl.walkMode = HANDWALK_RUN;
				temp = 0.8 * SCALINGFACTOR;
			}

			if(y != handControl.walkMode)
			{
				switch(handControl.walkMode)
				{
					case HANDWALK_TIPTOE:
							AnimateActor(&hand, HANDANIM_TIPTOE, YES, NO, 0);
							break;
					case HANDWALK_WALK:
							AnimateActor(&hand, HANDANIM_WALK, YES, NO, 0);
							break;
					case HANDWALK_RUN:
							AnimateActor(&hand, HANDANIM_RUN, YES, NO, 0);
							break;
				}
			}
			SetAnimationSpeed(&hand, temp);
			break;
  
  
		case HAND_CARTWHEEL:
			if(handControl.currentActionTime > 2)
			{
				hand.maxSpeed = DEFAULT_HAND_MAXSPEED*2;
				AccelerateActor2D(&hand, -1);
				hand.maxSpeed = DEFAULT_HAND_MAXSPEED*5;
			}
			if(handControl.currentActionTime == 3)
				hand.vel.v[Y] += 0.5;
			if(hand.reachedEndOfAnimation == TRUE)
			{
				if(hand.onGround)
				{
					ApplyAction(HAND_SKIDDING);
					hand.radius = hand.radiusAim = HAND_NORMAL_RADIUS;
				}
			}
			break;

		case HAND_CLIMBUP:
			AccelerateActor2D(&hand, 0.02);//move hand away from the ledge
			if(hand.reachedEndOfAnimation == TRUE)
			{	
				hand.rot.v[Y] = hand.rotaim.v[Y] = Aabs(hand.rot.v[Y] + PI);
				TransformActor(&hand);
				ApplyAction(HAND_IDLE);
			}
			break;

		case HAND_CLIMBDOWN:
//			hand.mass = 100000;
			SetVector(&hand.vel, &zero);						
			if(handControl.currentActionTime > 20)
			{
//				if(!(frameCount % 10))
				{
					if(rand()%20 == 1)
					{
						if(hand.reachedEndOfAnimation == TRUE)
							AnimateActor(&hand, HANDANIM_CLIMBDOWNIDLE, NO, YES, 0);
					}
				}
			}
			if(handControl.altPlane->normal.v[Y] <= CLIMBDOWN_SLOPE_LIMIT)
				ApplyAction(HAND_DROPDOWN);

			break;

		case HAND_CLING:
			SetVector(&hand.vel, &zero);

			temp = handControl.frontProbeAltitude[0] > 0 ? 
			handControl.frontProbeAltitude[0] : handControl.frontProbeAltitude[1];
			temp -= 0.18;
			hand.pos.v[Y] -= temp / 12;
			
			DisableControl(2);

			if(hand.reachedEndOfAnimation == TRUE)
			{
				hand.pos.v[Y] += 2.8;
				SubVector2D(&tempVect, &handControl.verticalEdgeChecker.pos, &hand.pos);
				ScaleVector2D(&tempVect, 4);
				AddVector2D(&hand.pos, &hand.pos, &tempVect);
				SetVector(&hand.vel, &zero);
				hand.radius = hand.radiusAim = HAND_NORMAL_RADIUS;
				hand.flags |= GRAVITY;// - COLLISION;

				TransformActor(&hand);
				ApplyAction(HAND_IDLE);

				cameraInfo.slowGaribTimer = 7;
				cameraInfo.slowGaribSpeed = 0.3;
			}
			break;

		case HAND_THROW2LAND:
			if(hand.reachedEndOfAnimation == TRUE)
				ApplyAction(HAND_IDLE);
			break;		  

		case HAND_DROPDOWN:
			if(handControl.currentActionTime == 5)
				hand.pos.v[Y] -= HAND_NORMAL_RADIUS - HAND_CLING_RADIUS;
			else if(handControl.currentActionTime == 6)
			{
				hand.radiusAim = HAND_NORMAL_RADIUS;
				AnimateActor(&hand,HANDANIM_JUMP,NO,NO,0);
			}
			break;
  	}



	if(player.pickupTimer[SPEEDUP] > 0)
	{
		SetVector(&tempVect, &hand.pos);
		temp = (rand() % 30) - 15;
		temp /= 10;
		tempVect.v[X] += temp;
		temp = (rand() % 30) - 15;
		temp /= 10;
		tempVect.v[Y] += temp;
		temp = (rand() % 30) - 15;
		temp /= 10;
		tempVect.v[Z] += temp;
		AddDebris(MAGIC, &tempVect, &zero);
	}

	if(handControl.status == HAND_TELEPORTING)
	{
		SetVector(&tempVect, &hand.pos);
		temp = (rand() % 30) - 15;
		temp /= 10;
		tempVect.v[X] += temp;
		temp = (rand() % 30) - 15;
		temp /= 10;
		tempVect.v[Y] += temp;
		temp = (rand() % 30) - 15;
		temp /= 10;
		tempVect.v[Z] += temp;
		AddDebris(SPARK, &tempVect, &zero);
		if(--hand.lifetime == 0)
		{
			handControl.status = HAND_NORMAL;
		}

	}







//---------------------------------------------------------------------------
//===============handle dust clouds, splashes etc============================
	if((hand.onGround == TRUE) && (handControl.lastOnGround == FALSE) && (hand.oldVel.v[Y] < -1))
	{
		if((hand.vel.v[Y] == -TERMINAL_VELOCITY) && (IsHandHoldingBall() == NO))
		{
			PlaySample(SFX_HEAVYLAND, 0, &hand.pos, 100);
//			DxlPlaySampleInstance(&sounds[SFX_HEAVYLAND],0);
			AnimateActor(&hand,HANDANIM_STUN, NO, NO, 0);
			AnimateActor(&hand,HANDANIM_STAND, YES, YES, 0);
			handControl.newActionDelay = 12;
		}
		if((hand.pos.v[X] >= 36) && (hand.pos.v[X] <= 64) && (hand.pos.v[Z] >= -99) && (hand.pos.v[Z] <= -69.6))
		{
			if(hand.pos.v[Y] <= 7.93)
			{
				SetVector(&tempVect, &hand.pos);
				tempVect.v[Y] = 7.5;
				CreateSplashCircle(&tempVect, 4, 0);
				PlaySample(SFX_SPLASH1, 0, &hand.pos, 100);
			}
		}
		else
		{
			SetVector(&tempVect, &hand.pos);
			tempVect.v[Y] -= 1;
			CreateDustCircle(&tempVect, 4, 0);
			PlaySample(SFX_HANDLAND, 0, &hand.pos, 100);
		}
	}

	if((ball.onGround == TRUE) && (ballControl.lastOnGround == FALSE) && (ball.oldVel.v[Y] < -1))
	{
		AnimateActor(&ball,BALLANIM_BOUNCE, NO, NO, 0);
		if((ball.pos.v[X] >= 36) && (ball.pos.v[X] <= 64) && (ball.pos.v[Z] >= -99) && (ball.pos.v[Z] <= -69.6))
		{
			if(ball.pos.v[Y] <= 7.93)
			{
				SetVector(&tempVect, &ball.pos);
				tempVect.v[Y] = 7.5;
				CreateSplashCircle(&tempVect, 4, 0);
				PlaySample(SFX_SPLASH1, 0, &hand.pos, 100);
			}
		}
		else
		{
			SetVector(&tempVect, &ball.pos);
			AddDebris(DUST, &tempVect, &zero);
		}
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: MoveHand
	Purpose 	: determines what actions are available and if they are possible
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void MoveHand()
{
	int x;
//	VECTOR	tempVect;
//	PLATFORM	*platform;

	if((hand.action != HAND_CLIMBDOWN) && (hand.action != HAND_CLIMBUP))
		handControl.sitPlat = NULL;

	MaintainAction();

	CheckHandCatapult();
	CheckHandPlatform();

	if(handControl.newActionDelay == WAIT_FOR_ANIMEND)
	{
		if(hand.reachedEndOfAnimation == TRUE)
			handControl.newActionDelay = 0;
	}
	else if(handControl.newActionDelay > WAIT_FOR_ONGROUND)
	{
		handControl.newActionDelay--;		
	}
	else if(handControl.newActionDelay == WAIT_FOR_ONGROUND)
	{
		if(hand.onGround == TRUE)
			handControl.newActionDelay = 0;
	}
	else
	{
		if(handControl.newActionDelay != 0)
			handControl.newActionDelay--;
	}

	if(handControl.newActionDelay == 0)
	{ 
 		for(x = 0; x < MAX_HAND_ACTIONS; x++)
		{
			if(ActionAllowed(hand.action, x) && ConditionsMet(x))
			{
				ApplyAction(x);
				break;
			}
		}
	}
//	else
//		handControl.newActionDelay--;


//	keep a record of what the hand was doing last frame..
	if((hand.onGround) && (joystick.magnitude == 0) && (!joystick.button[JS_BUTTON_A]) && (!joystick.button[JS_BUTTON_B]))
	{
		handControl.idleTime++;
	}
	else
		handControl.idleTime = 0;
 
	handControl.lastWalkMode = handControl.walkMode;
 	handControl.actionLastFrame = hand.action;
	handControl.lastOnGround = hand.onGround;
	ballControl.lastOnGround = ball.onGround;
	handControl.currentActionTime++;

	if(handControl.inputDisabledTimer)
		handControl.inputDisabledTimer--;

	if(hand.onGround == FALSE)
		handControl.fallCounter++;
	else
		handControl.fallCounter = 0;
}

/*	--------------------------------------------------------------------------------
	Function 	: CheckHandForEnemyCollision
	Purpose 	: kill enemies and all that
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void CheckHandForEnemyCollision()
{
	int x;
	VECTOR tempVect;
		
	for(x = 0; x < numberOfEnemies; x++)
	{
		if((enemies[x].actor.status == INACTIVE) || (enemies[x].actor.status == DEAD))
			continue;

		if(AreActorsTouching(&enemies[x].actor, &hand) == TRUE)
		{
			if(enemies[x].flags & HURT_BY_SLAM)
			{
				if(/*(hand.action == HAND_SLAM) || */(hand.action == HAND_SLAM2ST))
				{
					AnimateActor(&enemies[x].actor, NMEANIM_DEATH, NO, NO, 0);
					player.score += 100;
					enemies[x].actor.status = DEAD;
//					CreateStarCircle(&enemies[x].actor.pos, 6);
				}
			}
		}
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: CheckBallForEnemyCollision
	Purpose 	: guess?
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void CheckBallForEnemyCollision()
{
	int x;
	VECTOR tempVect;
	VECTOR tempVect2;
		
	for(x = 0; x < numberOfEnemies; x++)
	{
		if((enemies[x].actor.status == INACTIVE) || (enemies[x].actor.status == DEAD))
			continue;

		if(enemies[x].flags & DEADLY_TO_BALL)
		{
			if(AreActorsTouching(&enemies[x].actor, &ball) == TRUE)
			{
				BurstBall();
			}
		}
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: CheckHandCatapult()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void CheckHandCatapult()
{
	PLATFORM *platform;
	VECTOR	tempVect;

	if(hand.onGround)
		hand.flags &= -1 - CATAPULTING;
	else if(hand.flags & CATAPULTING)
	{
		hand.velInertia = hand.normInertia;
		DisableControl(2);
	}

	if(handControl.catPlat)
	{
		platform = handControl.catPlat;
		hand.velInertia = hand.normInertia;
		if(platform->catapult->delayCounter > 0)
		{
			platform->catapult->delayCounter--;
			if((platform->catapult->delayCounter == 0) && (hand.platform == platform) && (platform->catapult->catCounter == 0))
			{
				hand.platform->rotvel.v[X] = -platform->catapult->rotSpeed;
				platform->catapult->catCounter = platform->catapult->rotTime;
			}
			if(hand.action == HAND_JOINED)
			{
				SetVector2D(&ball.vel, &zero);
				MoveHandToBall();
				if(hand.platform)
					ball.rotaim.v[Y] = hand.platform->rot.v[Y];
			}
			else
				SetVector2D(&hand.vel, &zero);
		}
		if(platform->catapult->catCounter > 0)
		{
			if(--platform->catapult->catCounter == 0)
			{
//				platform->flags &= -1 - BACKGROUND;
				platform->rotvel.v[X] = -platform->rotvel.v[X]/4;
				platform->catapult->catCounter = -1;
	
				hand.flags |= CATAPULTING;

				if(hand.action == HAND_JOINED)
				{
					SetVector(&ball.vel,&zero);
					AccelerateActor2D(&ball, -platform->catapult->strengthXZ);
					ball.vel.v[Y] = platform->catapult->strengthY;
				}
				else
				{
					SetVector(&hand.vel,&zero);
					ApplyAction(HAND_JUMP);
//					AnimateActor(&hand, HANDANIM_JUMP, NO, NO, 0);
					AccelerateActor2D(&hand, -handControl.catPlat->catapult->strengthXZ);
					hand.vel.v[Y] = platform->catapult->strengthY;
				}
			}
		}
		else if(platform->catapult->catCounter)
		{
			if(--platform->catapult->catCounter == -(platform->catapult->rotTime-1)*4)
			{
				handControl.catPlat = NULL;
				platform->rotvel.v[X] = 0;
				platform->rot.v[X] = 0;
				platform->catapult->catCounter = 0;
			}
		}
	}			

	if((hand.platform) && (hand.platform->catapult) && (hand.platform->catapult->status) && (hand.platform->catapult->delayCounter == 0) && (hand.platform->catapult->catCounter == 0))
	{
		SetVector(&tempVect, &hand.platform->catapult->activateOffset);
		RotateVector2D(&tempVect, &tempVect, hand.platform->rot.v[Y]);
		AddVector(&tempVect, &hand.platform->pos, &tempVect);
		scenicItems[0].frame->SetPosition(g_lpWorld, tempVect.v[X], tempVect.v[Y], tempVect.v[Z]);
		if(DistanceBetweenPoints(&hand.pos, &tempVect) < 1)
		{
//			hand.platform->catapult->active = TRUE;
			dprintf("catapult activated\n");
			handControl.catPlat = hand.platform;
			handControl.catPlat->catapult->delayCounter = 20;
			handControl.catPlat->catapult->durationCounter = 0;
			handControl.newActionDelay = 21 + handControl.catPlat->catapult->rotTime;
			DisableControl(handControl.newActionDelay);
			hand.rotaim.v[Y] = hand.platform->rot.v[Y];
			if(hand.action == HAND_JOINED)
			{
				ball.rotaim.v[Y] = hand.platform->rot.v[Y];
//				ball.maxSpeed = DEFAULT_HAND_MAXSPEED * 3;
			}
		}
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: DisableControl
	Purpose 	: disables control of hand for specified time
	Parameters 	: short
	Returns 	: none
	Info 		:
*/
void DisableControl(short time)
{
	if(handControl.inputDisabledTimer < time)
		handControl.inputDisabledTimer = time;
}


/*	--------------------------------------------------------------------------------
	Function 	: HandOverstepsEdge
	Purpose 	: determine whether the hand has stepped over the edge of a face.
	Parameters 	: none
	Returns 	: T/F
	Info 		:
*/
BOOL HandOverstepsEdge(ACTOR *probe, PLANE *plane)
{
//	PLANE *plane;
//	 = hand.plane[0];
	VECTOR tempVect,tempVect2;
	int x,y;
	float val1, val2;

	if((probe == &handControl.verticalEdgeChecker) && (hand.onGround == FALSE))
		return FALSE;

	for(x = 0; x < 3; x++)
	{
		y = (x + 1) MOD 3;

		tempVect.v[X] = -plane->vertex[x].v[Z] + plane->vertex[y].v[Z];
		tempVect.v[Z] = plane->vertex[x].v[X] - plane->vertex[y].v[X];

		tempVect2.v[X] = hand.pos.v[X] - plane->vertex[y].v[X];
		tempVect2.v[Z] = hand.pos.v[Z] - plane->vertex[y].v[Z];
		val1 = DotProduct2D(&tempVect2,&tempVect);

		tempVect2.v[X] = probe->pos.v[X] - plane->vertex[y].v[X];
		tempVect2.v[Z] = probe->pos.v[Z] - plane->vertex[y].v[Z];
		val2 = DotProduct2D(&tempVect2,&tempVect);
		if((val1 * val2) < 0)
		{
			hand.rotaim.v[Y] = Aabs(atan2(tempVect.v[X], tempVect.v[Z]) + PI);
//			if(probe == &handControl.verticalEdgeChecker)
			{
				if(FindShortestAngle(hand.rotaim.v[Y], hand.rot.v[Y]) < PI / 2)
				{
					dprintf("adjusting angle\n");
					hand.rot.v[Y] = hand.rotaim.v[Y] = hand.rotaim.v[Y] + PI;
//					hand.rotaim.v[Y] += PI;
				}
				else
				{
					dprintf("not adjusting angle\n");
					hand.rot.v[Y] = hand.rotaim.v[Y];
				}
			}
			return TRUE;
		}
 	}

	return FALSE; 
}

/*	--------------------------------------------------------------------------------
	Function 	: IsHandHoldingBall()
	Purpose 	: determins if the hand is holding the ball
	Parameters 	: none
	Returns 	: true/false
	Info 		:
*/
BOOL IsHandHoldingBall()
{

	if(
		(hand.action == HAND_JOINED) ||
		(hand.action == HAND_PREWHACK) ||
		(hand.action == HAND_THROWAIM) ||
		(hand.action == HAND_THROW2CUP) ||
		(hand.action == HAND_BOUNCE) ||
		(hand.action == HAND_BOUNCE2CUP) || 
		(hand.action == HAND_AIRBOURNEWHACK)
	  )return TRUE;

	return FALSE;
}

/*	--------------------------------------------------------------------------------
	Function 	: DropBall
	Purpose 	: releases the ball from an actor
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void DropBall()
{
	ballControl.status = BALL_NORMAL;

	ballControl.holdingActor = NULL;
	SetVector(&ball.vel, &zero);
	SetVector(&ballControl.holdingOffset, &zero);
	ball.owner = NULL;
}

/*	--------------------------------------------------------------------------------
	Function 	: DropHand
	Purpose 	: releases the hand from an actor
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void DropHand()
{
	handControl.status = HAND_NORMAL;

	handControl.holdingActor = NULL;
	SetVector(&hand.vel, &zero);
	SetVector(&handControl.holdingOffset, &zero);
	hand.owner = NULL;
	ApplyAction(HAND_JUMP);
	DisableControl(20);	
}



/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void MoveBall()
{
	VECTOR tempVect;

	if(ball.reachedEndOfAnimation == FALSE)
	{
		SetVector(&ball.rot, &zero);
		SetVector(&ball.rotaim, &zero);

	}

	if(ballControl.status == BALL_BURST)
	{
		if(ballControl.timer > 0)
		{
			if(!(ballControl.timer % 4))
			{
				tempVect.v[X] = rand()%50;
				tempVect.v[X] -= 25;
				tempVect.v[X] /= 5;
				tempVect.v[Z] = rand()%50;
				tempVect.v[Z] -= 25;
				tempVect.v[Z] /= 5;
				tempVect.v[Y] = rand()%50;
				tempVect.v[Y] /= 30;
				SetVector(&ball.vel, &tempVect);
			}


			ballControl.timer--;
			if(ballControl.timer == 0)
			{
				RestartBall();
				ballControl.status = BALL_NORMAL;
				SetVector(&ball.pos, &hand.pos);
				ball.pos.v[Y] += 3;
				SetVector(&ball.vel, &zero);
			}
		}


	}

	CheckBallForEnemyCollision();
 }

/*	--------------------------------------------------------------------------------
	Function 	: BurstBall()
	Purpose 	: set the ball to be bursting
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void BurstBall()
{
	if(ballControl.status == BALL_BURST)
		return;

	LoseLife();
	ballControl.status = BALL_BURST;
	ballControl.timer = 50;	
	ball.vel.v[Y] += 0.5;
	ball.owner = NULL;
	AnimateActor(&ball,BALLANIM_BURST, YES, NO, 0);

	PlaySample(SFX_BALLBURST, 0, &ball.pos, 100);

	if(IsHandHoldingBall() == TRUE)
	{
		handControl.newActionDelay = 10;
		ApplyAction(HAND_RELEASE);
		hand.flags &= -1 - LO_GRAVITY;
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: HurtHand()
	Purpose 	: makes hand jump and let go of ball
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void HurtHand(int type)
{
	float temp,scale = 1.3;
	VECTOR	tempVect;

	if(hand.action != HAND_HURT)
	{
		if(IsHandHoldingBall() == TRUE)
		{
			ApplyAction(HAND_RELEASE);
			SetVector(&hand.vel,&zero);
			DisableControl(20);
		}
		ApplyAction(HAND_HURT);

		hand.vel.v[Y] = 0.9;
		switch(type)
		{
			case HURT_GO_TO_SAFE:
				if(handControl.safePlat == NULL)
					SubVector2D(&tempVect,&hand.pos,&handControl.safePos);
				else
				{
					SubVector2D(&tempVect,&hand.pos,&handControl.safePlat->pos);
					AddVector2D(&tempVect,&tempVect,&handControl.safePos);
				}
					
				MakeUnit2D(&tempVect);
				ScaleVector2D(&tempVect,scale);

				if(Magnitude2D(&tempVect))
					SubVector2D(&hand.vel,&hand.vel,&tempVect);
				else
					AccelerateActor2D(&hand,scale);
				break;
		
			case HURT_GO_BACKWARDS:
				AccelerateActor2D(&hand,scale);
				break;
		}
	}


}
/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void LoseLife()
{
	player.numLives--;
	if(player.numLives < 0)
	{
		dprintf("lives up matey!\n");
		player.messageTimer = 100;
//		userSurface[LIVESGONE_ICON].display = TRUE;
		userSurface[TIMEOUT_ICON].display = FALSE;
		userSurface[GAMEWON_ICON].display = FALSE;
 	}
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
double BallBounceMaxSpeed()
{
	switch(ballControl.ballType)
	{
		case BALL_NORMAL:
			return 0.5;
			break;
		case HEAVYBALL:
			return 0.3;
			break;
		case LIGHTBALL:
			return 0.6;
			break;
	}

	return 0;
}

/*	--------------------------------------------------------------------------------
	Function 	: CheckHandPlatform()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void CheckHandPlatform()
{
	PLATFORMLISTENTRY	*platformPtr;
	PLATFORM			*platform;

	if((hand.platform != NULL) && (hand.platform->counter == -1))
	{
		if(MagnitudeSquared(&hand.platform->targetSpeed) != 0)
		{
			platformPtr = FindTaggedPlatform(&platformList.head,hand.platform->targetTag);
			while((platformPtr != NULL) && (platformPtr->platform))
			{
				platform = platformPtr->platform;
				SetVector(&platform->vel,&hand.platform->targetSpeed);
				AddVector(&platform->vel,&platform->vel,&platform->moveBySpeed);
				platformPtr = FindTaggedPlatform(platformPtr,hand.platform->targetTag);
			}
		}					
		else
			hand.platform->counter = 1;
	}
	else if((hand.oldPlatform != NULL) && (hand.oldPlatform->counter == -1))
	{
		if(MagnitudeSquared(&hand.oldPlatform->targetSpeed) != 0)
		{
			platformPtr = FindTaggedPlatform(&platformList.head,hand.oldPlatform->targetTag);
			while((platformPtr != NULL) && (platformPtr->platform))
			{
				platform = platformPtr->platform;
				SetVector(&platform->vel,&zero);
				platformPtr = FindTaggedPlatform(platformPtr,hand.oldPlatform->targetTag);
			}
		}					
	}
}
