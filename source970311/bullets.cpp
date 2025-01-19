/*
 *  Project:         Oddballs
 *  File:            bullets.cpp
 *  Description:     
 *
 *  (c)1996 ISL.
 */


#include "oddballs.h"
#include "bullets.h"
#include "actors.h"
#include "nmebehaviour.h"
#include "anim.h"
#include "personal.h"
#include "collectables.h"
#include "actors.h"
#include "hand.h"
#include "enemies.h"

LPDIRECT3DRMVISUAL	bulletVisuals[10];
BULLET_TYPE standardBullets[NUM_BULLETS];


/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void InitBulletStructures()
{
	char					temp[256];
	LPDIRECT3DRMMESHBUILDER mb[5];
	LPDIRECT3DRMTEXTURE		tx[5];
	int						j;
		
//setup bullet visuals
    TRY_MYD3DRM(g_lpD3DRM->CreateMeshBuilder(&mb[0]));
	sprintf(temp, "%sgraphics\\bullets\\marker.x", rootPath);
    TRY_MYD3DRM(mb[0]->Load(temp, NULL, D3DRMLOAD_FROMFILE, LoadTextures, NULL));
	bulletVisuals[0] = (LPDIRECT3DRMVISUAL)mb;

	sprintf(temp, "%sgraphics\\bullets\\spark.ppm", rootPath);
	TRY_MYD3DRM(g_lpD3DRM->LoadTexture(temp, &tx[0]));
	tx[0]->SetDecalTransparency(TRUE);
	tx[0]->SetDecalTransparentColor(RGB_MAKE(0,0,0));
	tx[0]->SetDecalSize(1, 1);
	tx[0]->SetDecalOrigin(16, 16);
	bulletVisuals[1] = (LPDIRECT3DRMVISUAL)tx[0];
	
	sprintf(temp, "%sgraphics\\bullets\\spine.ppm", rootPath);
	TRY_MYD3DRM(g_lpD3DRM->LoadTexture(temp, &tx[1]));
	tx[1]->SetDecalTransparency(TRUE);
	tx[1]->SetDecalTransparentColor(RGB_MAKE(0,0,0));
	tx[1]->SetDecalSize(1, 1);
	tx[1]->SetDecalOrigin(8, 8);
	bulletVisuals[2] = (LPDIRECT3DRMVISUAL)tx[1];



//fighter bullet
	standardBullets[FIGHTER_BULLET].dirFromActor = 2;
	standardBullets[FIGHTER_BULLET].numGuns = 2;
	for(j = 0; j < standardBullets[FIGHTER_BULLET].numGuns; j++)
	{
		standardBullets[FIGHTER_BULLET].originalVel[j].v[X] = 2;
		standardBullets[FIGHTER_BULLET].originalVel[j].v[Y] = 0;
		standardBullets[FIGHTER_BULLET].originalVel[j].v[Z] = 0;
	}
	standardBullets[FIGHTER_BULLET].mass = 3;
	standardBullets[FIGHTER_BULLET].velInertia = 1;
	standardBullets[FIGHTER_BULLET].normInertia = 1;
	standardBullets[FIGHTER_BULLET].maxSpeed = 2;
	standardBullets[FIGHTER_BULLET].radius = 0.5;
	standardBullets[FIGHTER_BULLET].type = FIGHTER_BULLET;
	standardBullets[FIGHTER_BULLET].lifetime = 50;
	standardBullets[FIGHTER_BULLET].flags = DIESONCONTACT | COLLISION;
	standardBullets[FIGHTER_BULLET].offsetFromActor[0].v[X] = -1.63;
	standardBullets[FIGHTER_BULLET].offsetFromActor[0].v[Y] = -0.74;
	standardBullets[FIGHTER_BULLET].offsetFromActor[0].v[Z] = -1.97;
	standardBullets[FIGHTER_BULLET].offsetFromActor[1].v[X] = 1.54;
	standardBullets[FIGHTER_BULLET].offsetFromActor[1].v[Y] = -0.74;
	standardBullets[FIGHTER_BULLET].offsetFromActor[1].v[Z] = -1.97;
	standardBullets[FIGHTER_BULLET].destroyCallBack = DestroyBulletFighter;
	standardBullets[FIGHTER_BULLET].maintainCallBack = MaintainGenericBullet;
//	standardBullets[FIGHTER_BULLET].visual = bulletVisuals[1];

//droid bullet
	standardBullets[DROID_BULLET].numGuns = 2;
	standardBullets[DROID_BULLET].dirFromActor = 1;
	for(j = 0; j < standardBullets[DROID_BULLET].numGuns; j++)
	{
		standardBullets[DROID_BULLET].originalVel[j].v[X] = 0;
		standardBullets[DROID_BULLET].originalVel[j].v[Y] = 0;
		standardBullets[DROID_BULLET].originalVel[j].v[Z] = -1;
	}
	standardBullets[DROID_BULLET].mass = 1;
	standardBullets[DROID_BULLET].velInertia = 1;
	standardBullets[DROID_BULLET].normInertia = 1;
	standardBullets[DROID_BULLET].maxSpeed = 3;
	standardBullets[DROID_BULLET].radius = 0.2;
	standardBullets[DROID_BULLET].type = DROID_BULLET;
	standardBullets[DROID_BULLET].lifetime = 4;
	standardBullets[DROID_BULLET].flags = DIESONCONTACT | COLLISION;
	standardBullets[DROID_BULLET].offsetFromActor[0].v[X] = -1.04;
	standardBullets[DROID_BULLET].offsetFromActor[0].v[Y] = -0.2;
	standardBullets[DROID_BULLET].offsetFromActor[0].v[Z] = 0.96;
	standardBullets[DROID_BULLET].offsetFromActor[1].v[X] = 1.07;
	standardBullets[DROID_BULLET].offsetFromActor[1].v[Y] = -0.2;
	standardBullets[DROID_BULLET].offsetFromActor[1].v[Z] = 0.96;
//	standardBullets[DROID_BULLET].destroyCallBack = DestroyBulletDroid;
	standardBullets[DROID_BULLET].maintainCallBack = MaintainBulletDroid;
	standardBullets[DROID_BULLET].visual = bulletVisuals[1];


//sneezy bullet
	standardBullets[SNEEZY_BULLET].numGuns = 6;
	standardBullets[SNEEZY_BULLET].dirFromActor = 1;
	for(j = 0; j < standardBullets[SNEEZY_BULLET].numGuns; j++)
	{
		standardBullets[SNEEZY_BULLET].originalVel[j].v[X] = 0;
		standardBullets[SNEEZY_BULLET].originalVel[j].v[Y] = 0.6;
		standardBullets[SNEEZY_BULLET].originalVel[j].v[Z] = -0.7;
		RotateVector2D(&standardBullets[SNEEZY_BULLET].originalVel[j], &standardBullets[SNEEZY_BULLET].originalVel[j], ((2*PI) / standardBullets[SNEEZY_BULLET].numGuns) * j);
	}
	standardBullets[SNEEZY_BULLET].mass = 1;
	standardBullets[SNEEZY_BULLET].velInertia = 0.9;
	standardBullets[SNEEZY_BULLET].normInertia = 0.9;
	standardBullets[SNEEZY_BULLET].maxSpeed = 1;
	standardBullets[SNEEZY_BULLET].radius = 0.2;
	standardBullets[SNEEZY_BULLET].type = SNEEZY_BULLET;
	standardBullets[SNEEZY_BULLET].lifetime = 30;
	standardBullets[SNEEZY_BULLET].flags =  DIESONCONTACT | COLLISION | GRAVITY;
	for(j = 0; j < standardBullets[SNEEZY_BULLET].numGuns; j++)
	{
		standardBullets[SNEEZY_BULLET].offsetFromActor[j].v[X] = 0;
		standardBullets[SNEEZY_BULLET].offsetFromActor[j].v[Y] = 1;
		standardBullets[SNEEZY_BULLET].offsetFromActor[j].v[Z] = 0;
	}
	standardBullets[SNEEZY_BULLET].maintainCallBack = MaintainGenericBullet;
	standardBullets[SNEEZY_BULLET].visual = bulletVisuals[2];

	
}



/*	--------------------------------------------------------------------------------
	Function 	: FireBullet()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void FireBulletFromActor(ACTOR *actor, short type)
{
	ACTOR				*bullet;
	LPDIRECT3DRMFRAME	temp = NULL;
	BULLET_TYPE			*stdBul = &standardBullets[type];
	VECTOR				tempVect;
	BOOL				gun = enemies[actor->enemyNum].whichGun;
	int					from, to, j;


//firingType
//0 fire from gun 0
//1 fire from alternating guns
//2 fire from all guns

	from = gun;
	to = gun+1;

//firing type 2 means fire from both guns at once
	if(enemies[actor->enemyNum].firingType == 2)
	{
		from = 0;
		to = standardBullets[type].numGuns;
	}

//loop through active guns
	for(j = from; j < to; j++)
	{
		bullet = (ACTOR *)MyCalloc(1,sizeof(ACTOR));
		memset(bullet, 0, sizeof(ACTOR));
		AddActor(bullet);
		InitActor(bullet);

		if(stdBul->visual)
		{
			TRY_MYD3DRM(g_lpD3DRM->CreateFrame(g_lpWorld, &(bullet->frame)));
			bullet->frame->AddVisual((LPDIRECT3DRMVISUAL)stdBul->visual);
		}
		
	//copy data from pre-defined structures...
	//this will end up being a pointer to predefined bullet types.
		bullet->lifetime = stdBul->lifetime;
		bullet->mass = stdBul->mass;
		bullet->type = BULLET;
		bullet->flags = stdBul->flags;
		bullet->maxSpeed = stdBul->maxSpeed;
		bullet->radius = bullet->radiusAim = bullet->maxRadius = stdBul->radius;
		bullet->velInertia = stdBul->velInertia;
		bullet->normInertia = stdBul->normInertia;
		bullet->destroyCallBack = stdBul->destroyCallBack;
		bullet->maintainCallBack = stdBul->maintainCallBack;



		bullet->owner = actor;
		bullet->enemyNum = -1;

		switch(stdBul->dirFromActor)
		{
			case 0:
				ActorAddVelocity(bullet, &stdBul->originalVel[j]);
				break;
			case 1:
				RotateVector2D(&tempVect, &stdBul->originalVel[j], actor->rot.v[Y]);
				if(type == SNEEZY_BULLET)
					tempVect.v[Y] += ((rand()%10)-5) / 10;
				ActorAddVelocity(bullet, &tempVect);
				break;
			case 2:
				SetVector(&bullet->rot, &actor->rot);
				AccelerateActor(bullet, stdBul->originalVel[j].v[X]);
				break;
		}
		

		RotateVector2DXYZ(&tempVect, &stdBul->offsetFromActor[j], -actor->rot.v[X], X);
		RotateVector2DXYZ(&tempVect, &tempVect, -actor->rot.v[Y], Y);

//		RotateVector
		AddVector(&bullet->pos, &actor->pos, &tempVect);
		AddVector(&bullet->pos, &bullet->pos, &bullet->vel);

		if(bullet->frame)
			TransformActor(bullet);

	}

	if(enemies[actor->enemyNum].firingType == 1)
	{
		enemies[actor->enemyNum].whichGun++;
		if(enemies[actor->enemyNum].whichGun >= standardBullets[type].numGuns)
			enemies[actor->enemyNum].whichGun = 0;
			
	}

}


/*	--------------------------------------------------------------------------------
	Function 	: FireSlapBullet
	Purpose 	: fires a bullet from hand to slap things
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void FireSlapBullet()
{
	ACTOR *bullet;
	ACTOR *actor = &hand;
	LPDIRECT3DRMFRAME temp = NULL;

	bullet = (ACTOR *)MyCalloc(1,sizeof(ACTOR));
	memset(bullet, 0, sizeof(ACTOR));
	AddActor(bullet);
	InitActor(bullet);

	bullet->lifetime = 8;
	bullet->flags = DIESONCONTACT | COLLISION;
	bullet->owner = actor;
	AccelerateActorFromActor(actor,bullet,-0.035);
	SetVector(&bullet->pos,&actor->pos);

	bullet->pos.v[Y] -= 0.1;
	bullet->mass = 30;
	bullet->type = BULLET;
	bullet->maxSpeed = 0.26;
	bullet->velInertia = bullet->normInertia = 1.8;
	bullet->radius = bullet->radiusAim = bullet->maxRadius = 1.2;
	bullet->enemyNum = -1;
}


/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void DestroyBulletFighter(ACTOR *actor)
{
	VECTOR	tempVect;
	float	ang = rand()%10;


	tempVect.v[X] = 0.4;
	tempVect.v[Y] = 1;
	tempVect.v[Z] = 0;
	RotateVector2D(&tempVect, &tempVect, ang);
	AddDebris(PLATBIT, &actor->pos, &tempVect);

	AddDebris(DUST, &actor->pos, &zero);

}
/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
BOOL MaintainBulletDroid(ACTOR *actor)
{
	if(DistanceBetweenPoints(&hand.pos, &actor->pos) - hand.radius - actor->radius < 1)
	{
		HurtHand(HURT_GO_BACKWARDS);
		return TRUE;
	}
	return FALSE;
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
BOOL MaintainGenericBullet(ACTOR *actor)
{
	if((DistanceBetweenPoints(&hand.pos, &actor->pos) - hand.radius - actor->radius) < 2)
	{
		HurtHand(HURT_GO_BACKWARDS);
		return TRUE;
	}
	if((DistanceBetweenPoints(&ball.pos, &actor->pos) - ball.radius - actor->radius) < 2)
	{
		BurstBall();
		return TRUE;
	}
	return FALSE;
}

