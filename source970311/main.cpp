/*
 *	Project:	Oddballs
 *	File:		main.cpp
 *	Description:the "main" loop
 */


#include "oddballs.h"
#include "actors.h"
#include "personal.h"
#include "hand.h"
#include "cam.h"
#include "platforms.h"
#include "enemies.h"
#include "actors.h"
#include "nmebehaviour.h"
#include "collectables.h"
#include "speccase.h"
#include "collision.h"
#include "timegates.h"
#include "wind.h"
#include "catapults.h"


char message[256];
char controlMethod = CONTROL_HAND;
ACTORLIST	actorList;

TIMER_TYPE	timerList[10];

int frameCount = 0;

PLAYER_TYPE player;
LPDIRECT3DRMVISUAL lpShadowVisual = NULL;

/*	--------------------------------------------------------------------------------
	Function 	: MainLoop
	Purpose 	: this calls all the game routines
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void MainLoop()
{
	ACTORLISTENTRY		*actorPtr;
	ACTORLISTENTRY		*actor2Ptr;
	PLATFORMLISTENTRY	*platformPtr,*ptr1,*ptr2;
	PLATFORM			*platform;
	VECTOR				tempVect;
	PLANE				*plane;
	ACTOR				*actor;
	int					x,numChecked = 0;
	double				temp;
	double				scale;
	int					dist;
	
	if(handControl.inputDisabledTimer == 0)
	{
		ReadJoystick();
		ProcessInput();
	}
	else
		joystick.xPos = joystick.yPos = joystick.magnitude = joystick.button[0] = joystick.button[1] = joystick.button[2] = joystick.button[3] = joystick.button[4] = joystick.button[5] = 0;

	CheckSpecialCases();

	UpdateTimeGates();
	TransformTimeGates();

	if(player.messageTimer > 0)
	{
		player.messageTimer--;
		if(player.messageTimer == 0)
		{
			userSurface[LIVESGONE_ICON].display = FALSE;
			userSurface[TIMEOUT_ICON].display = FALSE;
			userSurface[GAMEWON_ICON].display = FALSE;

			userSurface[LIVESGONE_ICON].position.cx = (640 - 128) /2 - 400;
			userSurface[TIMEOUT_ICON].position.cx = (640 - 148) /2 + 400;
			userSurface[GAMEWON_ICON].position.cx = (640 - 156) /2 + 400;
		}
		else if(userSurface[LIVESGONE_ICON].display == TRUE)
		{
			if(player.messageTimer > 10)
			{
				dist = abs(userSurface[LIVESGONE_ICON].position.cx - (640 - 128)/2);
				dist = min(dist / 2, 64);
				userSurface[LIVESGONE_ICON].position.cx += dist;
			}
			else
				userSurface[LIVESGONE_ICON].position.cx += (11 - player.messageTimer) * 6;
		}
		else if(userSurface[TIMEOUT_ICON].display == TRUE)
		{
			if(player.messageTimer > 10)
			{
				dist = abs(userSurface[TIMEOUT_ICON].position.cx - (640 - 148)/2);
				dist = min(dist / 2, 64);
				userSurface[TIMEOUT_ICON].position.cx -= dist;
			}
			else
				userSurface[TIMEOUT_ICON].position.cx -= (11 - player.messageTimer) * 6;
		}
		else if(userSurface[GAMEWON_ICON].display == TRUE)
		{
			if(player.messageTimer > 10)
			{
				dist = abs(userSurface[GAMEWON_ICON].position.cx - (640 - 156)/2);
				dist = min(dist / 2, 64);
				userSurface[GAMEWON_ICON].position.cx -= dist;
			}
			else
				userSurface[GAMEWON_ICON].position.cx += (11 - player.messageTimer) * 6;
		}
	}

	if((player.timer > 0) && (frameCount & 1))
	{
		player.timer--;
		if(player.timer == 0)
		{
			player.messageTimer = 100;
			userSurface[TIMEOUT_ICON].display = TRUE;
			userSurface[LIVESGONE_ICON].display = FALSE;
			userSurface[GAMEWON_ICON].display = FALSE;
		}
	}
/*
	for(x = 0; x < numberOfEnemies; x++)
	{
		if(enemies[x].actor.status != INACTIVE)
			ControlEnemy(x);
	}
*/	

	StartTiming(TIMER__UPDATE_ACTOR);

	for(actorPtr = actorList.head.next;actorPtr != &actorList.head;actorPtr = actorPtr->next)
		actorPtr->actor->flags &= -1 - BEING_BLOWN;

	for(platformPtr = platformList.head.next;platformPtr != &platformList.head;platformPtr = platformPtr->next)
	{
		if(platformPtr->platform->fan)
			UpdateFan(platformPtr->platform);
	}

	for(actorPtr = actorList.head.next;actorPtr != &actorList.head;actorPtr = actor2Ptr)
	{
		actor2Ptr = actorPtr->next;
		UpdateActor(actorPtr->actor);
	}

	EndTiming(TIMER__UPDATE_ACTOR);


	StartTiming(TIMER__UPDATE_PLATFORM);
	for(ptr1 = platformList.head.next;numChecked < platformList.numEntries;ptr1 = ptr2)
	{
		numChecked++;
		if(ptr1->platform->flags & DESTROYED)
		{
			CreateDustCircle(&ptr1->platform->pos,8,0);
			KillPlatform(ptr1->platform);
			ptr2 = platformList.head.next;
			numChecked = 0;
		}
		else
			ptr2 = ptr1->next;
	}
	for(platformPtr = platformList.head.next;platformPtr != &platformList.head;platformPtr = platformPtr->next)
	{
		//only transform coords for platforms that are visible
		SubVector(&tempVect, &camera.pos, &platformPtr->platform->pos);
		if(MagnitudeSquared(&tempVect) < 40000)
			TransformPlatform(platformPtr->platform);
	}
	for(platformPtr = platformList.head.next;platformPtr != &platformList.head;platformPtr = platformPtr->next)
	{
		UpdatePlatform(platformPtr->platform);
	}

	UpdateRopeBridges();

	EndTiming(TIMER__UPDATE_PLATFORM);


	UpdateActorPositionDependencies();

	StartTiming(TIMER__UPDATE_SHADOWS);
	for(actorPtr = actorList.head.next;actorPtr != &actorList.head;actorPtr = actorPtr->next)
	{
		if(actorPtr->actor->frame != NULL)
			TransformActor(actorPtr->actor);
		if(actorPtr->actor->enemyNum >= 0)
		{
			TransformEnemySpheres(&enemies[actorPtr->actor->enemyNum]);
		}
	}
	EndTiming(TIMER__UPDATE_SHADOWS);



//work out shadows

 	for(actorPtr = actorList.head.next;actorPtr != &actorList.head;actorPtr = actorPtr->next)
	{
		actor = actorPtr->actor;
		if(actor->flags & SHADOW)
		{
/*			if(actor->onGround)
			{
				temp = actor->radius;
				plane = actor->plane[0];
			}
			else
*/				temp = Altitude(actor,&plane,&platform);
			if(actor == &ball)
				ballControl.altitude = temp;
			if(actor == &hand)
			{
				handControl.altitude = temp;
				if((IsHandHoldingBall()) || (hand.action == HAND_CLIMBDOWN))
					temp = 10000;
				if((hand.action == HAND_CLIMBDOWN) && (platform != handControl.sitPlat))
					ApplyAction(HAND_DROPDOWN);
			}

			if(temp < 0)
				temp = 10000;
//			if(temp >= 0)
			{
				SetVector(&tempVect, &plane->normal);
				if(tempVect.v[Y] < 0.6)
				{	
					tempVect.v[Y] = 0.6;
					MakeUnit(&tempVect);
				}
					
				TRY_MYD3DRM(actor->shadowFrame->DeleteVisual(actor->Shadow));
				TRY_MYD3DRM(g_lpD3DRM->CreateShadow(lpShadowVisual, dummyLight, actor->pos.v[X],
							actor->pos.v[Y] - temp + 0.2, actor->pos.v[Z],plane->normal.v[X], tempVect.v[Y], plane->normal.v[Z], &actor->Shadow));
							TRY_MYD3DRM(actor->shadowFrame->AddVisual(actor->Shadow));
				temp -= actor->radius;
				scale = 1 - (Min(0.7, temp / 12));
				actor->shadowFrame->AddScale(D3DRMCOMBINE_REPLACE, scale, scale, scale);
			}
		}
	}

   
	for(x = 0; x < numberOfEnemies; x++)
	{
		NMEProcessActorScript(&enemies[x]);
	}
   
   
   	ProcessPersonalLoop();
	UpdateWinds();

	frameCount++;
}




/*	--------------------------------------------------------------------------------
	Function 	: InitShadow
	Purpose 	: creates an actors 'shadowframe' and adds shadow to it.
	Parameters 	: ACTOR *
	Returns 	: none
	Info 		:
*/
void InitShadow(ACTOR *actor)
{
	int x;
	LPDIRECT3DRMMESHBUILDER mb;
	char temp[256];

	if(lpShadowVisual == NULL)
	{
		TRY_MYD3DRM(g_lpD3DRM->CreateMeshBuilder(&mb));
		sprintf(temp, "%sgraphics\\shadows\\shadow.x", rootPath);
		TRY_MYD3DRM(mb->Load(temp, NULL, D3DRMLOAD_FROMFILE, LoadTextures, NULL));
		lpShadowVisual = (LPDIRECT3DRMVISUAL)mb;
	}

	RELEASE(actor->shadowFrame);
	actor->Shadow = NULL;

    TRY_MYD3DRM(g_lpD3DRM->CreateFrame(actor->frame, &(actor->shadowFrame)));

	TRY_MYD3DRM(g_lpD3DRM->CreateShadow(lpShadowVisual, dummyLight, actor->pos.v[X],
	actor->pos.v[Y], actor->pos.v[Z],D3DVALUE(0), D3DVALUE (1), D3DVALUE (0), &(actor->Shadow)));
	TRY_MYD3DRM(actor->shadowFrame->AddVisual(actor->Shadow));
	x = 1;
}

/*	--------------------------------------------------------------------------------
	Function 	: InitTimerList
	Purpose 	: initialises the procedure timer list
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void InitTimerList()
{
	int x;

	for(x = 0; x < 10; x++)
		timerList[x].time = timerList[x].count.QuadPart = timerList[x].active = 0;

	timerList[TIMER__RENDER_LOOP].time = 0;
	timerList[TIMER__RENDER_LOOP].count.QuadPart = 0;
	timerList[TIMER__RENDER_LOOP].active = YES;
	sprintf(timerList[TIMER__RENDER_LOOP].procName, "RenderLoop");

	timerList[TIMER__UPDATE_ACTOR].time = 0;
	timerList[TIMER__UPDATE_ACTOR].count.QuadPart = 0;
	timerList[TIMER__UPDATE_ACTOR].active = YES;
	sprintf(timerList[TIMER__UPDATE_ACTOR].procName, "UpdateActor");

	timerList[TIMER__UPDATE_PLATFORM].time = 0;
	timerList[TIMER__UPDATE_PLATFORM].count.QuadPart = 0;
	timerList[TIMER__UPDATE_PLATFORM].active = YES;
	sprintf(timerList[TIMER__UPDATE_PLATFORM].procName, "UpdatePlatforms");

	timerList[TIMER__UPDATE_SHADOWS].time = 0;
	timerList[TIMER__UPDATE_SHADOWS].count.QuadPart = 0;
	timerList[TIMER__UPDATE_SHADOWS].active = YES;
	sprintf(timerList[TIMER__UPDATE_SHADOWS].procName, "UpdateShadows");

	timerList[TIMER__FRAMETIME].time = 0;
	timerList[TIMER__FRAMETIME].count.QuadPart = 0;
	timerList[TIMER__FRAMETIME].active = YES;
	sprintf(timerList[TIMER__FRAMETIME].procName, "FrameTime");


 //	timerList[TIMER__UPDATE_ACTOR].
 //	timerList[TIMER__UPDATE_PLATFORM].
}

void StartTiming(char entryNum)
{
	QueryPerformanceCounter(&timerList[entryNum].count);
}

void EndTiming(char entryNum)
{
	LARGE_INTEGER	temp;
	QueryPerformanceCounter(&temp);
	timerList[entryNum].time = temp.QuadPart - timerList[entryNum].count.QuadPart;
}