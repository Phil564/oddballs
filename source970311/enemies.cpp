/*
 *  Project:         oddballs
 *  File:            enemies.cpp
 *  Description:     handles all enemy thingies, loading etc
 *
 *  (c)1996 ISL.
 */

#include "oddballs.h"
#include "enemies.h"
#include "nmebehaviour.h"
#include "actors.h"
#include "anim.h"
#include "personal.h"
#include "bullets.h"


ENEMY_TYPE enemies[MAX_ENEMIES];
//ACTOR enemies[MAX_ENEMIES];
int numberOfEnemies = 0;
animation enemyAnims[MAX_ENEMIES][20];


/*	--------------------------------------------------------------------------------
	Function 	: LoadEnemy
	Purpose 	: loads an enemy, along with animations etc
	Parameters 	: type, pos, mass, flags
	Returns 	: 
	Info 		:
*/
void LoadEnemy(int enemyType, double xpos, double ypos, double zpos, double mass,int flags,int tag)
{
	static int		numEnemiesLoaded[MAX_ENEMIES] = {0,0,0,0,0,0,0,0,0,0};
	char			fileName[512],temp[512];
	ACTOR			*actor = &enemies[numberOfEnemies].actor;
	ENEMY_TYPE		*enemy = &enemies[numberOfEnemies];

	memset(actor, 0, sizeof(ACTOR));
//	store original position
	enemy->originalPosition.v[X] = xpos;
	enemy->originalPosition.v[Y] = ypos;
	enemy->originalPosition.v[Z] = zpos;
	enemy->flags |= flags;
	enemy->tag = tag;
	enemy->bankMultiplier = 0.8;

//set for 1 gun firing
	enemy->firingType = 1;
	enemy->whichGun = 0;
//	enemy->numGuns = 1;
		
	actor->type = enemyType;
	actor->pos.v[X] = xpos;
	actor->pos.v[Y] = ypos;
	actor->pos.v[Z] = zpos;
	actor->rAim.v[Y] = ON;
	actor->velInertia = 0.3;
	actor->rotInertia = HAND_INERTIA;
	actor->flags = GRAVITY + COLLISION + BLOWN_BY_WIND;
	actor->radius = actor->radiusAim = 2.9;
	actor->turnSpeed = 0.1;
	actor->bounce = 0.1;
	actor->maxAccelSpeed = 0.4;
	actor->maxSpeed = 2;
	actor->normInertia = actor->velInertia;
	actor->status = ACTIVE;
	actor->accel = 0.5;
	AddActor(actor);
	InitActor(actor);
	actor->enemyNum = numberOfEnemies;
	actor->mass = mass;
//	actor->flying = FALSE;


	numEnemiesLoaded[enemyType]++;
	
	switch(enemyType)
	{
		case CHEST:
//			enemies[numberOfEnemies].flags = DEADLY_TO_BALL;

			actor->radius = actor->radiusAim = 1.42;
			actor->turnSpeed = 0.6;
			actor->mass = 10000;
			actor->normInertia = actor->velInertia = 0.9;
			actor->maxSpeed = 2;
			actor->accel = 0.2;
			actor->flags &= -1 - GRAVITY;

			sprintf(fileName,"graphics\\enemies\\chest%d\\",numEnemiesLoaded[enemyType]);
			sprintf(temp,"%sstand.x",fileName);
			LoadActor(actor, g_lpScene,temp);

			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_IDLE], 1, 1,temp, actor) == FALSE)
				dprintf("Animation Load Failed\n");

			sprintf(temp,"%slunge.x",fileName);
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_ATTACK1], 9, 1,temp, actor) == FALSE)
				dprintf("Animation Load Failed\n");

			sprintf(temp,"%swaddle.x",fileName);
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_WALK], 8, 1.2,temp, actor) == FALSE)
				dprintf("Animation Load Failed\n");
			AttachAnimations(actor, enemyAnims[numberOfEnemies]);

//			SetVector(&(instruction+1)->pos,&actor->pos);

			break;

		case LOBSTER:
			enemies[numberOfEnemies].flags = DEADLY_TO_BALL + HURT_BY_SLAM;
			actor->radius = actor->radiusAim = 1.4;
			actor->turnSpeed = 0.25;
			actor->maxAccelSpeed = 0.3;
			actor->accel = 0.15;

			sprintf(fileName,"graphics\\enemies\\lobster%d\\",numEnemiesLoaded[enemyType]);
			sprintf(temp,"%slob_st.x",fileName);
			LoadActor(actor, g_lpScene,temp);

			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_IDLE], 1, 1,temp, actor) == FALSE)
				dprintf("Animation Load Failed\n");

			sprintf(temp,"%slob_wk.x",fileName);
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_WALK], 12, 1.6,temp, actor) == FALSE)
				dprintf("Animation Load Failed\n");

			sprintf(temp,"%slob_attk.x",fileName);
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_ATTACK1], 9, 1,temp, actor) == FALSE)
				dprintf("Animation Load Failed\n");

			sprintf(temp,"%slob_sqsh.x",fileName);
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_DEATH], 4, 1,temp, actor) == FALSE)
				dprintf("Animation Load Failed\n"); 
			AttachAnimations(actor, enemyAnims[numberOfEnemies]);
			break;

		case CUTLASS:
			actor->flags |= SHADOW;
			actor->radius = actor->radiusAim = 1.5;
			actor->turnSpeed = 0.25;
			actor->maxAccelSpeed = 0.3;
			actor->normInertia = actor->velInertia = 0.7;
			actor->slideLimit = 0.99;
			actor->accel = 0.1;

			sprintf(fileName,"graphics\\enemies\\cutlass%d\\",numEnemiesLoaded[enemyType]);
			sprintf(temp,"%smesh.x",fileName);
			LoadActor(actor, g_lpScene,temp);

//			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_IDLE], 1, 1,temp, actor) == FALSE)
//				dprintf("Animation Load Failed\n");

			sprintf(temp,"%swalk.x",fileName);
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_WALK], 8, 0.4,temp, actor) == FALSE)
				dprintf("Animation Load Failed\n");

			sprintf(temp,"%scharge.x",fileName);
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_RUN], 8, 1,temp, actor) == FALSE)
				dprintf("Animation Load Failed\n");

			sprintf(temp,"%shack.x",fileName);
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_ATTACK1], 10, 1,temp, actor) == FALSE)
				dprintf("Animation Load Failed\n");
			AttachAnimations(actor, enemyAnims[numberOfEnemies]);
			AnimateActor(actor,NMEANIM_WALK, YES, NO, 0);
			break;

		case PARROT:
			actor->flags |= SHADOW;
			actor->radius = actor->radiusAim = 1.7;
			actor->turnSpeed = 0.06;
			actor->rAim.v[X] = ON;
			actor->rAim.v[Z] = ON;
//			actor->turnSpeed = 0.25;
//			actor->maxAccelSpeed = 0.85;
			actor->maxAccelSpeed = 0.25;
			actor->accel = 0.05;
//			actor->accel = 0.1;
			actor->maxSpeed = 0.5;
			actor->rotaim.v[Y] = PI/2;
			actor->rotaim.v[Y] = 0;
			actor->flags &= -1 - GRAVITY;
			actor->flying = TRUE;
			LoadActor(actor, g_lpScene, "graphics\\enemies\\parrot\\parrot.x");
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_IDLE], 12, 1, "graphics\\enemies\\parrot\\sidle_l.x", actor) == FALSE)
				dprintf("Animation Load Failed\n");
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_WALK], 12, 1, "graphics\\enemies\\parrot\\sidle_r.x", actor) == FALSE)
				dprintf("Animation Load Failed\n");
//			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_TAKEOFF], 6, 0.7, "graphics\\enemies\\parrot\\takeoff.x", actor) == FALSE)
//				dprintf("Animation Load Failed\n");
//			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_FLY], 12, 1, "graphics\\enemies\\parrot\\fly.x", actor) == FALSE)
//				dprintf("Animation Load Failed\n");
			AttachAnimations(actor, enemyAnims[numberOfEnemies]);
//			AnimateActor(actor,NMEANIM_FLY, YES, NO, 0);
			break;

		case SHARKFIN:
			actor->turnSpeed = 0.25;
			actor->maxAccelSpeed = 0.35;
			actor->maxSpeed = 0.35;
			actor->flags &= -1 - GRAVITY;
			actor->flags &= -1 - COLLISION;
			SetVector(&actor->vel, &zero);
			actor->frame = LoadMesh(g_lpScene, "graphics\\enemies\\shark\\sharkfin.x");
			break;

		case BIGBALL:
			actor->radius = actor->radiusAim = 1.15;
			actor->flags |= ROLLING + SHADOW;
			actor->bounce = 0.8;
			actor->maxAccelSpeed = 1.5;
			actor->turnSpeed = 0.25;
			actor->normInertia = actor->velInertia = 0.92;
			actor->accel = 0.12;
			actor->frame = LoadMesh(g_lpScene, "graphics\\enemies\\bigball\\bigball.x");
			actor->rot.v[X] = actor->rot.v[Y] = actor->rotaim.v[Y] = PI;
			InitShadow(actor);
			break;

		case CYLON:
			SetVector(&actor->centreOfGravity,&zero);
			actor->collisionSphere[0].offset.v[X] = 0;
			actor->collisionSphere[0].offset.v[Y] = 2.5;
			actor->collisionSphere[0].offset.v[Z] = 0;
			actor->collisionSphere[0].radius = 1;
			actor->collisionSphere[0].spinVal = 0;

//			actor->collisionSphere[1].offset.v[X] = 0;
//			actor->collisionSphere[1].offset.v[Y] = 2.6;
//			actor->collisionSphere[1].offset.v[Z] = 0;
//			actor->collisionSphere[1].radius = 0.9;
//			actor->collisionSphere[1].spinVal = 0;

			actor->numExtraCollisionSpheres = 1;

			actor->flags |= SHADOW;
			actor->rAim.v[Z] = ON;
			actor->radius = actor->radiusAim = 0.9;
			actor->turnSpeed = 0.25;
			actor->maxAccelSpeed = 1.2;
			actor->normInertia = actor->velInertia = 0.8;
			actor->slideLimit = 0.99;
			actor->accel = 0.1;

			LoadActor(actor, g_lpScene, "graphics\\enemies\\cylon\\cy_phovr.x");
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_IDLE], 11, 0.8, "graphics\\enemies\\cylon\\c_phovr.x", actor) == FALSE)
				dprintf("Animation Load Failed\n");
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_WALK], 11, 0.8, "graphics\\enemies\\cylon\\c_phovr.x", actor) == FALSE)
				dprintf("Animation Load Failed\n");
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_RUN], 8, 0.8, "graphics\\enemies\\cylon\\c_run.x", actor) == FALSE)
				dprintf("Animation Load Failed\n");
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_START], 9, 0.7, "graphics\\enemies\\cylon\\c_pstrt.x", actor) == FALSE)
				dprintf("Animation Load Failed\n");
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_STOP], 21, 1, "graphics\\enemies\\cylon\\c_pstop.x", actor) == FALSE)
				dprintf("Animation Load Failed\n");
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_SPECIAL1], 20, 1, "graphics\\enemies\\cylon\\c_pleft.x", actor) == FALSE)
				dprintf("Animation Load Failed\n");
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_SPECIAL2], 20, 1, "graphics\\enemies\\cylon\\c_prght.x", actor) == FALSE)
				dprintf("Animation Load Failed\n");
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_SPECIAL3], 32, 2, "graphics\\enemies\\cylon\\c_surps.x", actor) == FALSE)
				dprintf("Animation Load Failed\n");
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_SPECIAL4], 30, 1, "graphics\\enemies\\cylon\\c_giveup.x", actor) == FALSE)
				dprintf("Animation Load Failed\n");
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_ATTACK1], 30, 1, "graphics\\enemies\\cylon\\c_throw.x", actor) == FALSE)
				dprintf("Animation Load Failed\n");

			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_ENDATTACK], 11, 0.8, "graphics\\enemies\\cylon\\c_phovr.x", actor) == FALSE)
				dprintf("Animation Load Failed\n");


			AttachAnimations(actor, enemyAnims[numberOfEnemies]);
			AnimateActor(actor, NMEANIM_IDLE, YES, NO, 0);

			enemy->sphere[0] = (SPHERE_TYPE*)MyCalloc(1, sizeof(SPHERE_TYPE));
		    TRY_MYD3DRM(g_lpD3DRM->CreateFrame(actor->frame, &(enemy->sphere[0]->frame)));
			TRY_MYD3DRM(enemy->sphere[0]->frame->AddVisual(sphereTexture[1]));
			enemy->sphere[0]->offset.v[X] = 0;
			enemy->sphere[0]->offset.v[Y] = 0;
			enemy->sphere[0]->offset.v[Z] = 0;
			enemy->flags |= HURT_BY_SLAM;
			break;

		case DROID:
			if(enemy->flags & SPECIFY_X)
				actor->rot.v[Y] = actor->rotaim.v[Y] = PI/2;
			else
				actor->rot.v[Y] = actor->rotaim.v[Y] = 0;

			actor->flags |= SHADOW;
			actor->rAim.v[Z] = ON;
			actor->radius = actor->radiusAim = 2;
			actor->turnSpeed = 0.25;
			actor->maxAccelSpeed = 1.5;
			actor->normInertia = actor->velInertia = 0.6;
			actor->slideLimit = 0.99;
			actor->accel = 0.4;
			actor->flying = TRUE;
			actor->flags &= -1 - GRAVITY;

			actor->frame = LoadMesh(g_lpWorld, "graphics\\enemies\\droid\\droid.x");
			InitShadow(actor);

			enemy->sphere[0] = (SPHERE_TYPE*)MyCalloc(1, sizeof(SPHERE_TYPE));
		    TRY_MYD3DRM(g_lpD3DRM->CreateFrame(actor->frame, &(enemy->sphere[0]->frame)));
			TRY_MYD3DRM(enemy->sphere[0]->frame->AddVisual(sphereTexture[2]));
			enemy->sphere[0]->offset.v[X] = 0;
			enemy->sphere[0]->offset.v[Y] = 0;
			enemy->sphere[0]->offset.v[Z] = 0;


			break;


		case FIGHTER:
			enemy->firingType = 1;
//			enemy->numGuns = 2;

			actor->flags |= SHADOW;
			actor->radius = actor->radiusAim = 1.7;
			actor->turnSpeed = 0.06;
			actor->rAim.v[X] = ON;
			actor->rAim.v[Z] = ON;
			actor->maxAccelSpeed = 0.8;
			actor->accel = 0.2;
			actor->maxSpeed = 1;
			actor->velInertia = 0.3;
			actor->rotInertia = 0.8;

			actor->rotaim.v[Y] = PI/2;
			actor->rotaim.v[Y] = 0;
			actor->flags &= -1 - GRAVITY;
			actor->flying = TRUE;
			actor->frame = LoadMesh(g_lpWorld, "graphics\\enemies\\fighter\\fighter.x");
			InitShadow(actor);

			enemy->sphere[0] = (SPHERE_TYPE*)MyCalloc(1, sizeof(SPHERE_TYPE));
		    TRY_MYD3DRM(g_lpD3DRM->CreateFrame(actor->frame, &(enemy->sphere[0]->frame)));
			TRY_MYD3DRM(enemy->sphere[0]->frame->AddVisual(sphereTexture[0]));
			enemy->sphere[0]->offset.v[X] = -1.64;
			enemy->sphere[0]->offset.v[Y] = -0.72;
			enemy->sphere[0]->offset.v[Z] = 0;

			enemy->sphere[1] = (SPHERE_TYPE*)MyCalloc(1, sizeof(SPHERE_TYPE));
		    TRY_MYD3DRM(g_lpD3DRM->CreateFrame(actor->frame, &(enemy->sphere[1]->frame)));
			TRY_MYD3DRM(enemy->sphere[1]->frame->AddVisual(sphereTexture[0]));
			enemy->sphere[1]->offset.v[X] = 1.56;
			enemy->sphere[1]->offset.v[Y] = -0.72;
			enemy->sphere[1]->offset.v[Z] = 0;
			break;


		case SNEEZY:
			enemies[numberOfEnemies].flags = DEADLY_TO_BALL + DEADLY_TO_HAND;
			enemy->firingType = 2;
//			enemy->numGuns = 6;

			actor->flags |= SHADOW;
			actor->radius = actor->radiusAim = 0.9;
			actor->turnSpeed = 0.08;
			actor->maxAccelSpeed = 0.3;
			actor->normInertia = actor->velInertia = 0.8;
			actor->slideLimit = 0.99;
			actor->accel = 0.02;

			LoadActor(actor, g_lpScene, "graphics\\enemies\\sneezy\\sn_walk.x");
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_IDLE], 60, 0.8, "graphics\\enemies\\sneezy\\hg_snuff.x", actor) == FALSE)
				dprintf("Animation Load Failed\n");
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_WALK], 11, 0.8, "graphics\\enemies\\sneezy\\hg_walk.x", actor) == FALSE)
				dprintf("Animation Load Failed\n");
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_SPECIAL1], 23, 0.8, "graphics\\enemies\\sneezy\\hg_sniff.x", actor) == FALSE)
				dprintf("Animation Load Failed\n");
			if(LoadAnimation(&enemyAnims[numberOfEnemies][NMEANIM_ATTACK1], 96, 0.8, "graphics\\enemies\\sneezy\\hg_sneez.x", actor) == FALSE)
				dprintf("Animation Load Failed\n");



			AttachAnimations(actor, enemyAnims[numberOfEnemies]);
			AnimateActor(actor, NMEANIM_WALK, YES, NO, 0);

			break;


	}

	CalcMaxActorRadius(actor);
	
	enemies[numberOfEnemies].script.numNormalInstructions = 0;
	enemies[numberOfEnemies].script.numConditionalInstructions = 0;
	enemies[numberOfEnemies].script.numAttackInstructions = 0;
	enemies[numberOfEnemies].script.currentInstructionNum = 0;
	enemies[numberOfEnemies].script.attacking = FALSE;
	enemies[numberOfEnemies].script.activeInstructionList = enemies[numberOfEnemies].script.instructionList;
	enemies[numberOfEnemies].script.hasBeenOverridden = FALSE;

	numberOfEnemies++;
}



//*********************************************************************************
//=================================================================================
//		This section is where all the enemy actions are defined
//
//=================================================================================



/*	--------------------------------------------------------------------------------
	Function 	: NMEActionMoveTo
	Purpose 	: moves an enemy to a specific point (used for flying enemies)
	Parameters 	: ACTOR*, VECTOR*, int
	Returns 	: If enemy has reached destination T/F
	Info 		:
*/
BOOL NMEActionMoveTo(ENEMY_TYPE *enemy, VECTOR *dest, float proximity, int flags)
{
	ACTOR *actor = &enemy->actor;
	VECTOR tempVect;
	float temp;
	float tempProx;

//	if((DistanceBetweenPoints(&actor->pos, dest) < proximity))
	if((DistanceBetweenPoints2D(&actor->pos, dest) < proximity) && (fabs(actor->pos.v[Y] - dest->v[Y]) < 0.6))
	{
//		actor->vel.v[Y] = 0;
		NMEActionWait(enemy);
		actor->rotaim.v[Z] = 0;
		return TRUE;
	}
	else
	{
		if(Magnitude(&actor->vel) >= actor->maxAccelSpeed)
			return FALSE;
		SubVector(&tempVect, &(actor->pos), dest);
		MakeUnit(&tempVect);
		ScaleVector(&tempVect, actor->accel);


		if(!(flags & NMEFLAGS_DONT_FACE_DIR))
			actor->rotaim.v[Y] = Aabs(atan2(tempVect.v[X], tempVect.v[Z]));


		if(flags & NMEFLAGS_DONT_SLOW_WHEN_NEAR)
		{
			if(FindShortestAngle(actor->rot.v[Y], actor->rotaim.v[Y]) > (PI/3))
			{
				ScaleVector(&actor->vel, 0.7);
			}
		}
		else//(!(flags & NMEFLAGS_DONT_SLOW_WHEN_NEAR)) 
		{

			if(FindShortestAngle(actor->rot.v[Y], actor->rotaim.v[Y]) > (PI/3))
			{
				ScaleVector(&actor->vel, 0.5);
			}


			temp = DistanceBetweenPoints(&actor->pos, dest);
			if((flags & NMEFLAGS_USE_Z_AXIS) || (flags & NMEFLAGS_USE_X_AXIS))
				tempProx = Max(1, proximity);
			else
				tempProx = Max(5, proximity);
			if(temp < tempProx)
			{
				ScaleVector(&actor->vel, temp / tempProx);
				ScaleVector(&tempVect, 0.25);
			}
//			else
//				ScaleVector(&tempVect, 0.2);
		}
//		else
//			ScaleVector(&tempVect, 0.2);
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


//make actor bob up and down
//		if(actor->type == DROID)
//			tempVect.v[Y] += SineWave(25,0) / 80;

//actually move actor
		if(flags & NMEFLAGS_MOVE_ACCEL)
		{
			AccelerateActor2D(actor, -Magnitude2D(&tempVect));
		}
		if(flags & NMEFLAGS_MOVE_VEL)
		{
			actor->vel.v[X] -= tempVect.v[X];
			actor->vel.v[Z] -= tempVect.v[Z];
		}
//		actor->vel.v[Y] *= 0.6;
		actor->vel.v[Y] *= 0.8;
		actor->vel.v[Y] -= tempVect.v[Y];


// make actor bank when turning	
		if(flags & NMEFLAGS_BANK)
		{
			temp = FindShortestAngleSigned(actor->rotaim.v[Y], actor->rot.v[Y]);
			if(fabs(temp * enemy->bankMultiplier) > 0.08)
			{
			//actor is turning
				actor->rotaim.v[Z] = temp * enemy->bankMultiplier;
			}
			else
				actor->rotaim.v[Z] = 0;
		}

		return FALSE;
	}

	return FALSE;
}


/*	--------------------------------------------------------------------------------
	Function 	: NMEActionMoveTo2D
	Purpose 	: moves an enemy to a specific 2D point 
	Parameters 	: ACTOR*, VECTOR*, int
	Returns 	: If enemy has reached destination T/F
	Info 		:
*/
BOOL NMEActionMoveTo2D(ENEMY_TYPE *enemy, VECTOR *dest, float proximity, int flags)
{
	VECTOR tempVect;
	ACTOR *actor = &enemy->actor;
	float temp;

	actor->rotaim.v[Z] = 0;

	if(DistanceBetweenPoints(&actor->pos, dest) < proximity)
//	if(DistanceBetweenPoints2D(&actor->pos, dest) < proximity)
	{
		ScaleVector2D(&actor->vel, 0.5);
		return TRUE;
	}
	else
	{
//enemy will turn towards the dest, moving forwards..
		if(flags & NMEFLAGS_MOVE_ACCEL)
		{
			SubVector(&tempVect, &(actor->pos), dest);
			tempVect.v[Y] = 0;
			MakeUnit(&tempVect);
			ScaleVector(&tempVect, actor->accel);

			if(!(flags & NMEFLAGS_DONT_FACE_DIR))
				actor->rotaim.v[Y] = atan2(tempVect.v[X], tempVect.v[Z]);
 			if(Magnitude2D(&actor->vel) >= actor->maxAccelSpeed)
				return FALSE;
			AccelerateActor2D(actor, -Magnitude2D(&tempVect));
		}
//enemy will simply move in the direction of the destintion
		if(flags & NMEFLAGS_MOVE_VEL)
		{
 			if(Magnitude2D(&actor->vel) >= actor->maxAccelSpeed)
				return FALSE;
			SubVector(&tempVect, &(actor->pos), dest);
			tempVect.v[Y] = 0;
			MakeUnit(&tempVect);
			ScaleVector(&tempVect, actor->accel);
			actor->vel.v[X] -= tempVect.v[X];
			actor->vel.v[Z] -= tempVect.v[Z];
		}


		if(flags & NMEFLAGS_BANK)
		{
			temp = FindShortestAngleSigned(actor->rotaim.v[Y], actor->rot.v[Y]);
			if(fabs(temp * enemy->bankMultiplier) > 0.08)
			{
			//actor is turning
				actor->rotaim.v[Z] = temp * enemy->bankMultiplier;
			}
			else
				actor->rotaim.v[Z] = 0;
		}

		return FALSE;



	}

	return FALSE;
}



/*	--------------------------------------------------------------------------------
	Function 	: NMEActionWait
	Purpose 	: do nothing for a while (if enemy if flying, he will need to bob up/down
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void NMEActionWait(ENEMY_TYPE *enemy)
{
	ACTOR *actor = &enemy->actor;

	if(enemy->script.currentInstructionTime == 1)
	{
		if(!(enemy->script.currentInstruction.flags & NMEFLAGS_DONTANIMATE))
			AnimateActor(&enemy->actor, NMEANIM_IDLE, NO, NO, 0);
	}

//	if(actor->flying == TRUE)
//		actor->vel.v[Y] -= SineWave(10,0) / 10;

}

/*	--------------------------------------------------------------------------------
	Function 	: NMEActionFaceTo
	Purpose 	: make an enemy face a point
	Parameters 	: ACTOR*, VECTOR*
	Returns 	: T/F
	Info 		:
*/
BOOL NMEActionFaceTo(ENEMY_TYPE *enemy, VECTOR *dest)
{
	ACTOR *actor = &enemy->actor;
	VECTOR tempVect;

/*	if(actor->flying == TRUE)
		actor->vel.v[Y] += SineWave(10,0) / 20;
*/
	SubVector(&tempVect, &actor->pos, dest);
	actor->rotaim.v[Y] = Aabs(atan2(tempVect.v[X], tempVect.v[Z]));
	if(fabs(actor->rotaim.v[Y] - actor->rot.v[Y]) < 0.1)
		return TRUE;

	return FALSE;
}

/*	--------------------------------------------------------------------------------
	Function 	: NMEActionRandomMove
	Purpose 	: makes an enemy move at random within a box
	Parameters 	: ACTOR*
	Returns 	: none
	Info 		:
*/
void NMEActionRandomMove(ENEMY_TYPE *enemy)
{
	ACTOR *actor = &enemy->actor;
//	VECTOR tempVect = enemy->script.currentInstruction.val1.v;
	VECTOR tempVect;
	VECTOR oldDest;
	BOX	box = enemy->script.currentInstruction.val1.b;

	float slop = enemy->script.currentInstruction.val2.f;
	int area;
	BOOL reachedDestination;

	if(enemy->script.currentInstructionTime > 0)
	{
		if(actor->flying == TRUE)
		 	reachedDestination = NMEActionMoveTo(enemy, &enemy->script.tempDest, 1, enemy->script.currentInstruction.flags);
		else
		 	reachedDestination = NMEActionMoveTo2D(enemy, &enemy->script.tempDest, 1, enemy->script.currentInstruction.flags);
	}
	else
	{
		AnimateActor(&enemy->actor, NMEANIM_WALK, YES, NO, 0);
	}	
	if((enemy->script.currentInstructionTime == 0) ||
	   (reachedDestination))
	{
		char t = 0;
		do
		{
			tempVect.v[X] = box.pos.v[X] + rand() % (int)box.size.v[X];
			tempVect.v[Z] = box.pos.v[Z] + rand() % (int)box.size.v[Z];
			t++;
			if(t > 1)dprintf("ooops new pos required\n");
		}while(DistanceBetweenPoints2D(&enemy->script.tempDest, &tempVect) < slop);
		tempVect.v[Y] = box.pos.v[Y];
				
		SetVector(&enemy->script.tempDest, &tempVect);
//		dprintf("New action:randommove %f,%f\n", enemy->script.tempDest.v[X], enemy->script.tempDest.v[Z]);
	}

}

/*	--------------------------------------------------------------------------------
	Function 	: NMEActionFacePlayer
	Purpose 	: make the enemy turn to face either hand, ball or cloeset
	Parameters 	: ENEMY_TYPE *
	Returns 	: T/F
	Info 		:
*/
BOOL NMEActionFacePlayer(ENEMY_TYPE *enemy, int flags)
{
	NME_INSTRUCTION	*instruction = &enemy->script.currentInstruction;
	VECTOR			*tempVectPtr;

	if(flags & NMEFLAGS_SPECIFY_HAND)
		tempVectPtr = &hand.pos;
	else if(flags & NMEFLAGS_SPECIFY_BALL)
		tempVectPtr = &ball.pos;
	else if(flags & NMEFLAGS_SPECIFY_CLOSEST)
	{
		if(DistanceBetweenPoints(&hand.pos, &enemy->actor.pos) < DistanceBetweenPoints(&ball.pos, &enemy->actor.pos))
			tempVectPtr = &hand.pos;
		else					
			tempVectPtr = &ball.pos;
	}
	else
		dprintf("NME_FACEPLAYER:error no player specified\n");

	return(NMEActionFaceTo(enemy, tempVectPtr));

}

/*	--------------------------------------------------------------------------------
	Function 	: NMEActionFollowPlayer
	Purpose 	: makes the enemy follow either hand, ball or closest
	Parameters 	: ENEMY_TYPE *
	Returns 	: T/F
	Info 		:
*/
BOOL NMEActionFollowPlayer(ENEMY_TYPE *enemy)
{
	NME_INSTRUCTION *instruction = &enemy->script.currentInstruction;
	VECTOR			*tempVectPtr;
	VECTOR			tempVect;
	BOOL			res;

	if(instruction->flags & NMEFLAGS_SPECIFY_HAND)
		tempVectPtr = &hand.pos;
	else if(instruction->flags & NMEFLAGS_SPECIFY_BALL)
		tempVectPtr = &ball.pos;
	else if(instruction->flags & NMEFLAGS_SPECIFY_CLOSEST)
	{
		if(DistanceBetweenPoints(&hand.pos, &enemy->actor.pos) < DistanceBetweenPoints(&ball.pos, &enemy->actor.pos))
			tempVectPtr = &hand.pos;
		else					
			tempVectPtr = &ball.pos;
	}
	else
		dprintf("NME_FOLLOWPLAYER:error no player specified\n");


	AddVector(&tempVect, tempVectPtr, &instruction->val1.v);
	if(enemy->actor.flying == TRUE)
	{
		res = NMEActionMoveTo(enemy, &tempVect, instruction->val2.f, instruction->flags);
	}
	else
	{
		res = NMEActionMoveTo2D(enemy, &tempVect, instruction->val2.f, instruction->flags);
//		if((res == TRUE) && (fabs(hand.pos.v[Y] - enemy->actor.pos.v[Y]) > instruction->val2.f))
//			res = FALSE;
	}

	return res;
}

/*	--------------------------------------------------------------------------------
	Function 	: NMEActionFleePlayer
	Purpose 	: makes the enemy flee either hand, ball or closest
	Parameters 	: ENEMY_TYPE *
	Returns 	: none
	Info 		:
*/
BOOL NMEActionFleePlayer(ENEMY_TYPE *enemy)
{
	NME_INSTRUCTION *instruction = &enemy->script.currentInstruction;
	VECTOR			*tempVectPtr;
	VECTOR			tempVect;

	if(instruction->flags & NMEFLAGS_SPECIFY_HAND)
		tempVectPtr = &hand.pos;
	else if(instruction->flags & NMEFLAGS_SPECIFY_BALL)
		tempVectPtr = &ball.pos;
	else if(instruction->flags & NMEFLAGS_SPECIFY_CLOSEST)
	{
		if(DistanceBetweenPoints(&hand.pos, &enemy->actor.pos) < DistanceBetweenPoints(&ball.pos, &enemy->actor.pos))
			tempVectPtr = &hand.pos;
		else					
			tempVectPtr = &ball.pos;
	}
	else
		dprintf("NME_FLEEPLAYER:error no player specified\n");
	
	SubVector(&tempVect, &enemy->actor.pos, tempVectPtr);
	ScaleVector(&tempVect, 256);
	AddVector(&tempVect, &enemy->actor.pos, &tempVect);
	NMEActionMoveTo2D(enemy, &tempVect, 2, instruction->flags);

	return FALSE;
}


/*	--------------------------------------------------------------------------------
	Function 	: NMEActionCharge
	Purpose 	: makes the enemy charge at the player for a certain length of time
	Parameters 	: ENEMY_TYPE *
	Returns 	: none
	Info 		:
*/
BOOL NMEActionCharge(ENEMY_TYPE *enemy)
{
	NME_INSTRUCTION *instruction = &enemy->script.currentInstruction;
	VECTOR			*tempVectPtr;
	VECTOR			tempVect;
	NME_SCRIPT		*script = &enemy->script;
	BOOL			res;
	
	if(instruction->flags & NMEFLAGS_SPECIFY_HAND)
		tempVectPtr = &hand.pos;
	else if(instruction->flags & NMEFLAGS_SPECIFY_BALL)
		tempVectPtr = &ball.pos;
	else if(instruction->flags & NMEFLAGS_SPECIFY_CLOSEST)
	{
		if(DistanceBetweenPoints(&hand.pos, &enemy->actor.pos) < DistanceBetweenPoints(&ball.pos, &enemy->actor.pos))
			tempVectPtr = &hand.pos;
		else					
			tempVectPtr = &ball.pos;
	}
	else
		dprintf("NME_CHARGE:error no player specified\n");


	if(script->currentInstructionTime == 0)
	{
		script->tempInt = 0;
	}


	if(script->tempInt == 0)
	{
		if((IsActorFacingPoint(&enemy->actor, tempVectPtr, 1) == FALSE) || (script->currentInstructionTime == 0))
		{
			//initialise direction to charge in
			SubVector(&tempVect, &enemy->actor.pos, tempVectPtr);
			ScaleVector(&tempVect, 256);
			SubVector(&tempVect, &enemy->actor.pos, &tempVect);
			SetVector(&script->tempDest, &tempVect);
		}
		else
			script->tempInt = 1;
	}


	if(enemy->actor.flying == TRUE)
		res = NMEActionMoveTo(enemy, &script->tempDest, 2, instruction->flags);
	else
		res = NMEActionMoveTo2D(enemy, &script->tempDest, 2, instruction->flags);

	return res;

}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
BOOL NMEActionCircle(ENEMY_TYPE *enemy)
{
	NME_INSTRUCTION *instruction = &enemy->script.currentInstruction;
	VECTOR			tempVect;
	NME_SCRIPT		*script = &enemy->script;
	float			tempFloat;
	char			axis = Y;
	int				tempFlags;
	float			rotationFactor = 1.12;

	if(enemy->actor.type == FIGHTER)
		rotationFactor = 0.7;

	if(script->currentInstructionTime == 0)
	{
		if(instruction->flags & NMEFLAGS_ANTICLOCKWISE)
			script->tempFloat = script->tempFloat2 = PI;
		else
			script->tempFloat = script->tempFloat2 = 0;
	}

	tempFlags = instruction->flags;

	if(DistanceBetweenPoints(&script->tempDest, &enemy->actor.pos) < 6)
	{
		ScaleVector(&enemy->actor.vel, 0.9);
		if(script->currentInstructionTime > 0)
		{		//1.12
			if(script->currentInstruction.flags & NMEFLAGS_ANTICLOCKWISE)
			{
				script->tempFloat = Aabs(script->tempFloat - ((enemy->actor.maxAccelSpeed * rotationFactor) / instruction->val2.f));
				script->tempFloat2 = Aabs(script->tempFloat2 + ((enemy->actor.maxAccelSpeed * rotationFactor) / instruction->val2.f));
			}
			else
			{
				script->tempFloat = Aabs(script->tempFloat + ((enemy->actor.maxAccelSpeed * rotationFactor) / instruction->val2.f));
				script->tempFloat2 = Aabs(script->tempFloat2 + ((enemy->actor.maxAccelSpeed * rotationFactor) / instruction->val2.f));
			}
		}



		if(instruction->flags & NMEFLAGS_USE_Z_AXIS)
		{
			if(instruction->flags & NMEFLAGS_ANTICLOCKWISE)
			{
				enemy->actor.rotaim.v[Y] = PI + PI/2;
				enemy->actor.rot.v[X] = Aabs(script->tempFloat2 + PI);
//				enemy->actor.rotaim.v[X] = Aabs(script->tempFloat2 + PI);
			}
			else
			{
				enemy->actor.rotaim.v[Y] = PI/2;
				enemy->actor.rot.v[X] = Aabs(script->tempFloat + PI);
//				enemy->actor.rotaim.v[X] = Aabs(script->tempFloat + PI);
			}
		}
		else if(instruction->flags & NMEFLAGS_USE_X_AXIS)
		{
			if(instruction->flags & NMEFLAGS_ANTICLOCKWISE)
			{
				enemy->actor.rotaim.v[Y] = 0;
				enemy->actor.rotaim.v[X] = Aabs(script->tempFloat2 + PI);
			}
			else
			{
				enemy->actor.rotaim.v[Y] = PI;
				enemy->actor.rotaim.v[X] = Aabs(script->tempFloat + PI);
			}
		}
	}
	else
	{
		//if we are not close to out target pos, dont decrease timer, and may still need to turn
		enemy->actor.rotaim.v[X] = 0;
		if(script->timer > 0)
			script->timer++;

		if(tempFlags & NMEFLAGS_DONT_CHANGE_ROT)
			tempFlags &= -1 - NMEFLAGS_DONT_CHANGE_ROT;

		if(tempFlags & NMEFLAGS_DONT_FACE_DIR)
			tempFlags &= -1 - NMEFLAGS_DONT_FACE_DIR;
	}


	if((tempFlags & NMEFLAGS_DONT_FACE_DIR) == 0)
	{
		if(tempFlags & NMEFLAGS_USE_Y_AXIS)
		{
			SubVector2D(&tempVect,&script->tempDest,&enemy->actor.pos);
			MakeUnit2D(&tempVect);
			enemy->actor.rotaim.v[Y] = Aabs(atan2(tempVect.v[X], tempVect.v[Z]));
		}
	}

	tempVect.v[X] = tempVect.v[Y] = tempVect.v[Z] = 0;
	if(instruction->flags & NMEFLAGS_USE_X_AXIS)
	{	tempVect.v[Y] = 1;axis = X;}
	if(instruction->flags & NMEFLAGS_USE_Y_AXIS)
	{	tempVect.v[X] = 1;axis = Y;}
	if(instruction->flags & NMEFLAGS_USE_Z_AXIS)
	{	tempVect.v[Y] = 1;axis = Z;}

	RotateVector2DXYZ(&tempVect, &tempVect, script->tempFloat, axis);
	ScaleVector(&tempVect, instruction->val2.f);

	AddVector(&script->tempDest, &tempVect, &instruction->val1.v);
//	scenicItems[0].frame->SetPosition(g_lpWorld, script->tempDest.v[X], script->tempDest.v[Y], script->tempDest.v[Z]);

	if(enemy->actor.flying == TRUE)
		NMEActionMoveTo(enemy, &script->tempDest, -1, tempFlags);
	else
		NMEActionMoveTo2D(enemy, &script->tempDest, -1, tempFlags);

	return FALSE;
}

/*	--------------------------------------------------------------------------------
	Function 	: NMEActionFire1
	Purpose 	: performs the 'fire' action for the specified enemy
	Parameters 	: ENEMY_TYPE *
	Returns 	: none
	Info 		:
*/
void NMEActionFire1(ENEMY_TYPE *enemy)
{

	switch(enemy->actor.type)
	{
		case CYLON:
			AnimateActor(&enemy->actor, NMEANIM_ATTACK1, NO, NO, 0);
			break;
		case FIGHTER:
			FireBulletFromActor(&enemy->actor, FIGHTER_BULLET);
			break;	
		case DROID:
			FireBulletFromActor(&enemy->actor, DROID_BULLET);
			break;
	}


}
