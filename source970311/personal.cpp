/*
 *	Project:		Oddballs
 *	File:			personal.cpp
 *	Description:	personal flags etc
 */

#include "oddballs.h"
#include "personal.h"
#include "anim.h"
#include "scene.h"
#include "actors.h"
#include "collision.h"
#include "hand.h"
#include "maths.h"
#include "nmebehaviour.h"
#include "cam.h"
#include "enemies.h"
#include "rmstats.h"
#include "platforms.h"
#include "sound.h"
#include "collectables.h"
#include "spikes.h"
#include "puzzles.h"

int		camMode = 1;
long	detailFlags = DF_TEXTURED + DF_STEVE + DF_GOURAUD + DF_CHEAT + DF_NOSOUND;
LPDIRECT3DRMTEXTURE	sphereTexture[10];

HELPER	helper;
VECTOR	ballStart,handStart;

animation handanims[MAX_HANDANIMS];
animation ballanims[MAX_HANDANIMS];
//animation batanims[20];

char outputMessage[256];

#ifdef _FIXES
#ifdef DEBUG
char rootPath[] = {"..\\..\\..\\"};
#else
char rootPath[] = {".\\"};
#endif
#else
char rootPath[] = {"\\pc\\oddballs\\"};
#endif
//char levelToLoad[] = {"level1\\level1"};
//char levelToLoad[] = {"dock\\dock"};
char levelToLoad[] = {"level1\\dockwoody"};
double handAlt;

VECTOR mapRes = {1, 1, 1};

ACTOR balls[50];

LPDIRECT3DRMTEXTURE	testTexture;
LPDIRECT3DRMTEXTURE	testTexture2;
LPDIRECT3DRMTEXTURE	backgroundTexture;


/*	--------------------------------------------------------------------------------
	Function 	: InitScene()
	Purpose 	: personal scene initialisation
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
/*	--------------------------------------------------------------------------------
	Function 	: InitScene()
	Purpose 	: personal scene initialisation
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void InitScene(LPDIRECT3DRMFRAME scene)
{
	int j,k,x;
	char temp[256];

	AddActor(&camera);
	AddActor(&hand);
	AddActor(&ball);
	
//initialise the hand
	ResetHand();
	InitActor(&hand);

	CalcMaxActorRadius(&hand);

	LoadActor(&hand, scene, "graphics\\mr_palm\\mesh.x");
	

	if(LoadAnimation(&handanims[HANDANIM_TIPTOE], 12, 1 * SCALINGFACTOR, "graphics\\mr_palm\\sidle.x", &hand) == FALSE)
		dprintf("Animation Load Failed walk\n");
	if(LoadAnimation(&handanims[HANDANIM_WALK], 8, 1 * SCALINGFACTOR, "graphics\\mr_palm\\walk.x", &hand) == FALSE)
		dprintf("Animation Load Failed walk\n");
	if(LoadAnimation(&handanims[HANDANIM_RUN], 8, 1 * SCALINGFACTOR, "graphics\\mr_palm\\run.x", &hand) == FALSE)
		dprintf("Animation Load Failed walk\n");
	if(LoadAnimation(&handanims[HANDANIM_STAND], 10, 1 * SCALINGFACTOR, "graphics\\mr_palm\\wait.x", &hand) == FALSE)
		dprintf("Animation Load Failed mesh\n");
	if(LoadAnimation(&handanims[HANDANIM_SKID], 6, 1 * SCALINGFACTOR, "graphics\\mr_palm\\skid.x", &hand) == FALSE)
		dprintf("Animation Load Failed mesh\n");

	if(LoadAnimation(&handanims[HANDANIM_ST2DUCK], 3, 1 * SCALINGFACTOR, "graphics\\mr_palm\\st_duck.x", &hand) == FALSE)
		dprintf("Animation Load Failed mesh\n");
	if(LoadAnimation(&handanims[HANDANIM_DUCK2ST], 3, 1 * SCALINGFACTOR, "graphics\\mr_palm\\duck_st.x", &hand) == FALSE)
		dprintf("Animation Load Failed mesh\n");

	if(LoadAnimation(&handanims[HANDANIM_ROLL], 8, 1 * SCALINGFACTOR, "graphics\\mr_palm\\roll.x", &hand) == FALSE)
		dprintf("Animation Load Failed mesh\n");
	if(LoadAnimation(&handanims[HANDANIM_DUCK2ROLL], 2, 1 * SCALINGFACTOR, "graphics\\mr_palm\\duk-roll.x", &hand) == FALSE)
		dprintf("Animation Load Failed mesh\n");
	if(LoadAnimation(&handanims[HANDANIM_ROLL2DUCK], 2, 1 * SCALINGFACTOR, "graphics\\mr_palm\\roll-duk.x", &hand) == FALSE)
		dprintf("Animation Load Failed mesh\n");




	if(LoadAnimation(&handanims[HANDANIM_IDLE1], 2, 0.7 * SCALINGFACTOR, "graphics\\mr_palm\\st_idle.x", &hand) == FALSE)
		dprintf("Animation Load Failed mesh\n");
	if(LoadAnimation(&handanims[HANDANIM_IDLE2], 6, 1 * SCALINGFACTOR, "graphics\\mr_palm\\idle.x", &hand) == FALSE)
		dprintf("Animation Load Failed mesh\n");
	if(LoadAnimation(&handanims[HANDANIM_IDLE3], 2, 0.6 * SCALINGFACTOR, "graphics\\mr_palm\\idle_st.x", &hand) == FALSE)
		dprintf("Animation Load Failed mesh\n");

	if(LoadAnimation(&handanims[HANDANIM_ST2CUP], 6, 1 * SCALINGFACTOR, "graphics\\mr_palm\\st_cup.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");
	if(LoadAnimation(&handanims[HANDANIM_CUP2ST], 10, 1 * SCALINGFACTOR, "graphics\\mr_palm\\cup_st.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");

	if(LoadAnimation(&handanims[HANDANIM_BNCE2THROWAIM], 4, 1 * SCALINGFACTOR, "graphics\\mr_palm\\bc_throw.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");
	if(LoadAnimation(&handanims[HANDANIM_THROWAIM], 6, 1 * SCALINGFACTOR, "graphics\\mr_palm\\throw1.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");
	if(LoadAnimation(&handanims[HANDANIM_THROW], 12, 1 * SCALINGFACTOR, "graphics\\mr_palm\\throw2.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");
	if(LoadAnimation(&handanims[HANDANIM_THROW2CUP], 5, 1 * SCALINGFACTOR, "graphics\\mr_palm\\thrw_cup.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");

	if(LoadAnimation(&handanims[HANDANIM_CUP2BOUNCE], 5, 1 * SCALINGFACTOR, "graphics\\mr_palm\\cup_bc.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");
	if(LoadAnimation(&handanims[HANDANIM_BOUNCE2CUP], 5, 1 * SCALINGFACTOR, "graphics\\mr_palm\\bc_cup.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");

	if(LoadAnimation(&handanims[HANDANIM_BOUNCE], 3, 1 * SCALINGFACTOR, "graphics\\mr_palm\\bouncey1.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");
	if(LoadAnimation(&handanims[HANDANIM_BOUNCE2], 5, 1 * SCALINGFACTOR, "graphics\\mr_palm\\bouncey2.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");
	if(LoadAnimation(&handanims[HANDANIM_BOUNCE3], 3, 1 * SCALINGFACTOR, "graphics\\mr_palm\\bouncey3.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");

	if(LoadAnimation(&handanims[HANDANIM_JUMP], 5, 1 * SCALINGFACTOR, "graphics\\mr_palm\\jump.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");
	if(LoadAnimation(&handanims[HANDANIM_LONGJUMP], 12, 1.1 * SCALINGFACTOR, "graphics\\mr_palm\\longjump.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");
	if(LoadAnimation(&handanims[HANDANIM_THROW2LAND], 9, 0.8 * SCALINGFACTOR, "graphics\\mr_palm\\land.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");

	if(LoadAnimation(&handanims[HANDANIM_CARTWHEEL], 10, 0.7 * SCALINGFACTOR, "graphics\\mr_palm\\cartwhel.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");


	if(LoadAnimation(&handanims[HANDANIM_CLING], 12, 1.2 * SCALINGFACTOR, "graphics\\mr_palm\\rollup.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");
	if(LoadAnimation(&handanims[HANDANIM_CLIMBDOWN], 18, 1.6 * SCALINGFACTOR, "graphics\\mr_palm\\lower.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");
	if(LoadAnimation(&handanims[HANDANIM_CLIMBDOWNIDLE], 22, 1 * SCALINGFACTOR, "graphics\\mr_palm\\idler.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");
	if(LoadAnimation(&handanims[HANDANIM_CLIMBUP], 13, 1 * SCALINGFACTOR, "graphics\\mr_palm\\spin.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");






	if(LoadAnimation(&handanims[HANDANIM_SLAM], 3, 1 * SCALINGFACTOR, "graphics\\mr_palm\\j_fslam.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");
	if(LoadAnimation(&handanims[HANDANIM_SLAM2ST], 4, 0.8 * SCALINGFACTOR, "graphics\\mr_palm\\fslam_st.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");

	if(LoadAnimation(&handanims[HANDANIM_WHACK1], 4, 0.2 * SCALINGFACTOR, "graphics\\mr_palm\\whack1.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");
	if(LoadAnimation(&handanims[HANDANIM_WHACK2], 4, 0.6 * SCALINGFACTOR, "graphics\\mr_palm\\whack2.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");

/*	if(LoadAnimation(&handanims[HANDANIM_PUNCH], 3, 1, "graphics\\mr_palm\\punch1.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");
	if(LoadAnimation(&handanims[HANDANIM_PUNCH2ST], 5, 1, "graphics\\mr_palm\\punch2.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");
*/
	if(LoadAnimation(&handanims[HANDANIM_SLAP], 12, 1 * SCALINGFACTOR, "graphics\\mr_palm\\slap.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");
/*
	if(LoadAnimation(&handanims[HANDANIM_PAT], 10, 0.6, "graphics\\mr_palm\\pat.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");
*/
/*
	if(LoadAnimation(&handanims[HANDANIM_SPECIAL1], 16, 0.8 * SCALINGFACTOR, "graphics\\mr_palm\\idle.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");
	if(LoadAnimation(&handanims[HANDANIM_SPECIAL2], 18, 0.6 * SCALINGFACTOR, "graphics\\mr_palm\\thumbsup.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");
*/
	if(LoadAnimation(&handanims[HANDANIM_STUN], 12, 0.5 * SCALINGFACTOR, "graphics\\mr_palm\\dazed.x", &hand) == FALSE)
		dprintf("Animation Load Failed\n");


	AttachAnimations(&hand, handanims);



	ResetBall(&ball);
	InitActor(&ball);
	CalcMaxActorRadius(&ball);

	LoadActor(&ball, scene, "graphics\\ball\\ball.x");
	if(LoadAnimation(&ballanims[BALLANIM_BOUNCE], 4, 1, "graphics\\ball\\squash2.x", &ball) == FALSE)
		dprintf("Animation Load Failed\n");
	if(LoadAnimation(&ballanims[BALLANIM_BURST], 12, 1, "graphics\\ball\\burst.x", &ball) == FALSE)
		dprintf("Animation Load Failed\n");
	AttachAnimations(&ball, ballanims);

/*
	k = 50;
	for(j = 0;j < k;j++)
	{
		AddActor(&balls[j]);

		ResetBall(&balls[j]);
		InitActor(&balls[j]);
		balls[j].frame = LoadMesh(scene, "graphics\\ball\\ball.x");
		balls[j].pos.v[X] += 10;
		balls[j].pos.v[Z] -= 20;
		balls[j].pos.v[Y] = 6.5499987310759;
		balls[j].pos.v[Y] += (k-1-j)*BALL_NORMAL_RADIUS*2.1;
		SetVector(&balls[j].oldpos,&balls[j].pos);
		InitShadow(&balls[j]);
	}
*/	

	AddActor(&cameraInfo.garib);
	InitActor(&cameraInfo.garib);
	g_lpD3DRM->CreateFrame(scene, &cameraInfo.garib.frame);
//	cameraInfo.garib.frame = LoadMesh(g_lpScene, "graphics\\ball\\ball.x");

	AnimateActor(&hand,HANDANIM_STAND, YES, NO, 0);
	AnimateActor(&ball,BALLANIM_BOUNCE, NO, NO, 0);


//set the background colour
	scene->SetSceneBackground(RGB_MAKE(115, 115, 255));


	cameraInfo.positionToTrack = &hand.pos;
	cameraInfo.velToTrack = &hand.vel;
	InitPlayer();

	float sphereTexSizes[] = {0.8, 1.7, 2.2};

	for(x = 0; ; x++)
	{
		sprintf(temp, "%s\\graphics\\enemies\\spheres\\sphere%d.ppm", rootPath, x+1);
		if(FileExists(temp))
		{		
			TRY_MYD3DRM(g_lpD3DRM->LoadTexture(temp, &sphereTexture[x]));
			sphereTexture[x]->SetDecalTransparency(TRUE);
			sphereTexture[x]->SetDecalTransparentColor(RGB_MAKE(0,0,0));
			sphereTexture[x]->SetDecalSize(sphereTexSizes[x], sphereTexSizes[x]);
			sphereTexture[x]->SetDecalOrigin(16, 16);
		}
		else 
			break;
	}


}

/*	--------------------------------------------------------------------------------
	Function 	: ResetHand()
	Purpose 	: resets the hand positoins
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void ResetHand()
{
	memset(&hand, 0, sizeof(ACTOR));

	hand.type = HAND;

	SetVector(&hand.pos,&handStart);

	hand.velInertia = HAND_INERTIA;
	hand.rotInertia = HAND_INERTIA;
	hand.rAim.v[Y] = ON;
	hand.rAim.v[X] = ON;
	hand.flags = COLLISION + GRAVITY + DECELERATE + SHADOW + BLOWN_BY_WIND;
	hand.radius = hand.radiusAim = 1.5;
	hand.turnSpeed = 0.8;
	hand.mass = 10;
	hand.stopSpeed = 0.12;
	hand.slideLimit = 0.8;
	hand.action = HAND_IDLE;
	hand.lastAction = HAND_IDLE;
	hand.normInertia = hand.velInertia;
	hand.status = ACTIVE;

	hand.accel = HAND_ACCELERATION;

	memset(&handControl, 0, sizeof(HAND_CONTROL_TYPE));
	handControl.status = HAND_NORMAL;
	handControl.lastTimeGate = -1;

	hand.maxSpeed = DEFAULT_HAND_MAXSPEED*5;
	hand.maxAccelSpeed = DEFAULT_HAND_MAXSPEED;

	AddActor(&handControl.verticalEdgeChecker);
	memset(&handControl.verticalEdgeChecker, 0, sizeof(ACTOR));
	InitActor(&handControl.verticalEdgeChecker);
//	handControl.verticalEdgeChecker.flags = /*COLLISION + GRAVITY + DECELERATE + */SHADOW;
	handControl.verticalEdgeChecker.radiusAim = handControl.verticalEdgeChecker.radius = 0.2;
	handControl.verticalEdgeChecker.maxSpeed = DEFAULT_HAND_MAXSPEED * 2;
	handControl.verticalEdgeChecker.maxAccelSpeed = DEFAULT_HAND_MAXSPEED;
//	handControl.verticalEdgeChecker.frame = LoadMesh(g_lpWorld, "graphics\\marker.x");
	InitShadow(&handControl.verticalEdgeChecker);

}

/*	--------------------------------------------------------------------------------
	Function 	: RestartHand()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void RestartHand()
{
	VECTOR tempVect;

	if(handControl.safePlat)
	{
//		RotateVector(&tempVect,&handControl.safePos,handControl.safePlat->rot.v[Y]);
//		AddVector(&hand.pos,&handControl.safePlat->pos,&tempVect);
		AddVector(&hand.pos,&handControl.safePlat->pos,&handControl.safePos);
	}
	else
		SetVector(&hand.pos,&handControl.safePos);

	SetVector(&hand.vel, &zero);
	hand.flags = COLLISION + GRAVITY + DECELERATE + SHADOW + BLOWN_BY_WIND;
	hand.maxSpeed = DEFAULT_HAND_MAXSPEED*5;
	hand.maxAccelSpeed = DEFAULT_HAND_MAXSPEED;
	hand.accel = HAND_ACCELERATION;

	ApplyAction(HAND_IDLE);

	handControl.status = HAND_NORMAL;
	handControl.whackStrength = 0;
	handControl.inputDisabledTimer = 0;
	cameraInfo.positionToTrack = &hand.pos;
	cameraInfo.velToTrack = &hand.vel;

}


/*	--------------------------------------------------------------------------------
	Function 	: ResetCamera()
	Purpose 	: resets the camera positoins
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void ResetCamera()
{
	memset(&camera, 0, sizeof(ACTOR));
	camera.type = CAMERA;

	camera.pos.v[X] = 57;
	camera.pos.v[Y] = 20;
	camera.pos.v[Z] = -48;

	camera.rot.v[X] = 0.3;
	camera.rot.v[Y] = 2.43;

	camera.velInertia = CAMERA_INERTIA / 3;
	camera.rotInertia = CAMERA_INERTIA;
	camera.normInertia = camera.velInertia;
	camera.maxSpeed = 2;

	camera.status = ACTIVE;
//	camera.flags = COLLISION;
//	camera.radius = camera.radiusAim = camera.maxRadius = 10;
 
 	cameraInfo.distanceFromHand = 20;
//	cameraInfo.rotationBias = PI;
//	cameraInfo.swingBehindSpeed = 0.4;
	cameraInfo.positionBias.v[Y] = 0;
	cameraInfo.type = 0;		
	memset(&cameraInfo.garib, 0, sizeof(ACTOR));
	cameraInfo.garib.pos.v[X] = 33;
	cameraInfo.garib.pos.v[Y] = 30;
	cameraInfo.garib.pos.v[Z] = -48;
	cameraInfo.garib.velInertia = HAND_INERTIA / 4;
	cameraInfo.garib.rotInertia = HAND_INERTIA;
	cameraInfo.garib.rAim.v[Y] = ON;
	cameraInfo.garib.rAim.v[X] = ON;
	cameraInfo.garib.maxSpeed = 3;
	cameraInfo.garib.normInertia = cameraInfo.garib.velInertia;


}

/*	--------------------------------------------------------------------------------
	Function 	: RestartCmaera()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void RestartCamera()
{
	return;

	SetVector(&cameraInfo.garib.pos, &hand.pos);
	SetVector(&camera.pos, &hand.pos);
	camera.pos.v[Z] += 20;
}

/*	--------------------------------------------------------------------------------
	Function 	: ResetBall()
	Purpose 	: resets the camera positoins
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void ResetBall(ACTOR *ball)
{
	memset(ball, 0, sizeof(ACTOR));
	ball->type = BALL;

	SetVector(&ball->pos,&ballStart);

	ball->rAim.v[Y] = ON;
	ball->velInertia = 0.9;
	ball->rotInertia = HAND_INERTIA;
	ball->flags = GRAVITY + COLLISION + ROLLING + SHADOW + DESTROYER + BLOWN_BY_WIND + MAGNETIC;
	ball->radius = ball->radiusAim = BALL_NORMAL_RADIUS;
	ball->bounce = 0.8;
	ball->maxSpeed = 2;
	ball->mass = 10;
	ball->turnSpeed = 1;
	ball->stopSpeed = 0.05;
	ball->slideLimit = 0.99;
	ball->normInertia = ball->velInertia;

	ball->accel = BALL_ACCELERATION;
	ball->status = ACTIVE;
	ballControl.status = BALL_NORMAL;
	player.numLives = 3;
//	InitShadow(&ball);
//	ball.owner = &hand;
}

/*	--------------------------------------------------------------------------------
	Function 	: RestartBall()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void RestartBall()
{
	SetVector(&ball.pos, &hand.pos);
	ball.pos.v[Y] += 4;

	SetVector(&ball.vel, &zero);
	ball.flags = GRAVITY + COLLISION + ROLLING + SHADOW + DESTROYER + BLOWN_BY_WIND + MAGNETIC;
	ball.radius = BALL_NORMAL_RADIUS;
	ball.bounce = 0.8;
	ball.accel = BALL_ACCELERATION;

	ballControl.status = BALL_NORMAL;
//	ballControl.numLives = 3;
	AnimateActor(&ball,BALLANIM_BOUNCE, NO, NO, 0);
//	ball.owner = &hand;

	cameraInfo.positionToTrack = &hand.pos;
	cameraInfo.velToTrack = &hand.vel;
}

/*	--------------------------------------------------------------------------------
	Function 	: ProcessPersonalKeys()
	Purpose 	: special keys just for me
	Parameters 	: WPARAM key.
	Returns 	: none
	Info 		:
*/
void ProcessPersonalKeys(WPARAM key)
{
	VECTOR	vect;
	PLATFORMLISTENTRY *ptr;
	static PLATFORM *platform = NULL;
	ACTORLISTENTRY	*aptr;
	ACTOR	*actor;
	int	numNonDummies,foundHim;
	double	scale;

	switch(key)
	{
		case VK_0:
			if(hand.platform)
			{
				platform = NULL;
				KillPlatform(hand.platform);
			}
			break;

		case VK_1:	
			cameraInfo.distanceFromHand = 10;
			break;

		case VK_2:	
			cameraInfo.distanceFromHand = 20;
			break;

		case VK_3:	
			cameraInfo.distanceFromHand = 30;
			break;

		case VK_4:	
			cameraInfo.distanceFromHand = 40;
			break;

		case VK_C:
			RestartCamera();
//			cameraInfo.type = 1 - cameraInfo.type;
			break;

		case VK_R:	
			RestartHand();
			RestartBall();
			break;

		case VK_TAB:
			cameraFollow = 1 - cameraFollow;
			break;

		case VK_O:
			vect.v[X] = sin(camera.rot.v[Y] - PI/2)*CAMERA_ACCELERATION*4;
			vect.v[Y] = 0;
			vect.v[Z] = cos(camera.rot.v[Y] - PI/2)*CAMERA_ACCELERATION*4;
			ActorAddVelocity(&camera,&vect);
			break;

		case VK_P:
			vect.v[X] = sin(camera.rot.v[Y] + PI/2)*CAMERA_ACCELERATION*4;
			vect.v[Y] = 0;
			vect.v[Z] = cos(camera.rot.v[Y] + PI/2)*CAMERA_ACCELERATION*4;
			ActorAddVelocity(&camera,&vect);
			break;

		case VK_I:
			inertia = 1-inertia;
			break;

		case VK_L:
			numNonDummies = 0;
			foundHim = NO;
			for(ptr = platformList.head.next;ptr != &platformList.head;ptr = ptr->next)
			{
				if((ptr->platform->flags & DUMMY) == 0)
					numNonDummies++;
				if(ptr->platform == platform)
					foundHim = YES;
			}

			if(numNonDummies == 0)
				return;
			if(foundHim)
				for(ptr = platformList.head.next;ptr->platform != platform;ptr = ptr->next);
	  		while((ptr->platform == 0) || (ptr->platform->flags & DUMMY) || ((ptr->platform == platform) && (numNonDummies > 1)))
	  			ptr = ptr->next;
			if(hand.action == HAND_JOINED)
				actor = &ball;
			else
				actor = &hand;
			platform = ptr->platform;
			SetVector(&actor->pos,&platform->pos);
			scale = max(platform->initScale.v[Y],1);
			actor->pos.v[Y] += actor->radius + (platform->size.v[Y]*platform->scale.v[Y])/scale;
			SetVector(&actor->oldpos,&actor->pos);
			SetVector(&actor->vel,&zero);
			actor->vel.v[Y] -= gravity;
			actor->platform = actor->oldPlatform = platform;
			break;

		case VK_K:
			numNonDummies = 0;
			foundHim = NO;
			for(ptr = platformList.head.next;ptr != &platformList.head;ptr = ptr->next)
			{
				if((ptr->platform->flags & DUMMY) == 0)
					numNonDummies++;
				if(ptr->platform == platform)
					foundHim = YES;
			}

			if(numNonDummies == 0)
				return;
			if(foundHim)
				for(ptr = platformList.head.next;ptr->platform != platform;ptr = ptr->prev);
	  		while((ptr->platform == 0) || (ptr->platform->flags & DUMMY) || ((ptr->platform == platform) && (numNonDummies > 1)))
//	  		while((ptr->platform == 0) || (((ptr->platform == platform) || (ptr->platform->flags & DUMMY))/* && (numNonDummies > 1)*/))
					ptr = ptr->prev;
			if(hand.action == HAND_JOINED)
				actor = &ball;
			else
				actor = &hand;
			platform = ptr->platform;
			SetVector(&actor->pos,&platform->pos);
			scale = max(platform->initScale.v[Y],1);
			actor->pos.v[Y] += actor->radius + (platform->size.v[Y]*platform->scale.v[Y])/scale;
			SetVector(&actor->oldpos,&actor->pos);
			SetVector(&actor->vel,&zero);
			actor->vel.v[Y] -= gravity;
			actor->platform = actor->oldPlatform = platform;
			break;

/*		case VK_M:
			hand.mass++;
			break;

		case VK_N:
			hand.mass = max(0.1,hand.mass - 1);
			break;
*/
		case VK_F:
			ball.radius += 0.1;
			ball.radiusAim = ball.radius;
			break;

		case VK_D:
			ball.radius = max(0.1,ball.radius - 0.1);
			ball.radiusAim = ball.radius;
			break;

		case VK_B:
			SetVector(&ball.pos,&hand.pos);
			ball.pos.v[Y] += 1 + ball.radius + hand.radius;
			SetVector(&ball.vel,&zero);
			ApplyAction(HAND_JOINED);
			break;

//		case VK_V:
//			hand.bounce = max(0.1,hand.bounce - 0.1);
//			break;

		case VK_V:
			camMode = 1 - camMode;
			break;

		case VK_H:
			ball.bounce += 0.1;
			break;

		case VK_G:
			ball.bounce = max(0.1,ball.bounce - 0.1);
			break;

		case	VK_Y:	
			for(aptr = actorList.head.next; (aptr->actor != NULL) && (&aptr->actor->pos != cameraInfo.positionToTrack); aptr = aptr->next);
			while((aptr->next->actor == 0) || (aptr->next->actor->type == DEBRIS) || (aptr->next->actor == &camera ) || (aptr->next->actor == &cameraInfo.garib))
				aptr = aptr->next;
			cameraInfo.positionToTrack = &aptr->next->actor->pos;
			cameraInfo.velToTrack = &aptr->next->actor->vel;
			SetVector(&cameraInfo.garib.pos, &aptr->next->actor->pos);
			break;

		case	VK_U:
			cameraInfo.overrideCounter = 100;
			cameraInfo.lastPositionToTrack = cameraInfo.positionToTrack;
			cameraInfo.lastVelToTrack = cameraInfo.velToTrack;
			cameraInfo.positionToTrack = &platformList.head.next->platform->pos;
			cameraInfo.velToTrack = &platformList.head.next->platform->vel;
			break;



	}
}


/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: sort out my stuff
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void ProcessPersonalLoop()
{
	VECTOR temp;

	MoveHand();
	MoveBall();
	MoveCam();
	ProcessCollectables();
	UpdateDebris();
	UpdateSpikes();
	TranslateSpikes();
	CheckPuzzles();

//	sprintf(outputMessage,"H_MASS: %f H_BOUNCE: %f  B_MASS: %f B_BOUNCE: %f",(float)hand.mass,(float)hand.bounce,(float)ball.mass,(float)ball.bounce);
//	sprintf(outputMessage,"TIME: %d   SCORE: %d",player.timer,player.score);
	sprintf(outputMessage,"%f  %f  %f",(float)hand.pos.v[X],(float)hand.pos.v[Y],(float)hand.pos.v[Z]);

	if(frameCount == 0)
		dprintf("\nACTOR structure = %d bytes\nPLATFORM structure = %d bytes\nPLANE structure = %d bytes\n\n",sizeof(ACTOR),sizeof(PLATFORM),sizeof(PLANE));

	SetVector(&handControl.verticalEdgeChecker.vel, &zero);
	SetVector(&temp, &zero);
	temp.v[Z] = -0.3;
	RotateVector2D(&temp, &temp, hand.rot.v[Y]);
	AddVector(&handControl.verticalEdgeChecker.pos, &hand.pos, &temp);
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void InitPlayer()
{
	memset(&player,0,sizeof(PLAYER_TYPE));

	player.numLives = 3;
	player.timer = 500000;
	handControl.idleTime = 0;
}
