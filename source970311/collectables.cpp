/*
 *  Project:         oddballs
 *  File:            collectables.cpp
 *  Description:     guess what?
 *
 *  (c)1996 ISL.
 */

#include "oddballs.h"
#include "collectables.h"
#include "personal.h"
#include "anim.h"
#include "nmebehaviour.h"
#include "actors.h"
#include "hand.h"
#include "sound.h"

COLLECTABLE_TYPE collectables[MAX_COLLECTABLES];
int numberOfCollectables = 0;

int speedBonus = 1;

int pickupToIcon[NUMPICKUPS] = 
{
	SUCTION_ICON,
	GAUNTLET_ICON,
	SPEEDUP_ICON,
	HEAVYBALL_ICON,
	LIGHTBALL_ICON
};

DEBRIS_TYPE debris[MAX_DEBRIS];

/*
LPDIRECT3DRMTEXTURE	starTexture[NUM_STAR_FRAMES];
LPDIRECT3DRMTEXTURE	dustTexture[NUM_DUST_FRAMES];
LPDIRECT3DRMTEXTURE	platTexture[NUM_PLAT_FRAMES];
LPDIRECT3DRMTEXTURE	splashTexture[NUM_SPLASH_FRAMES];
LPDIRECT3DRMTEXTURE	fireballTexture[NUM_FIREBALL_FRAMES];
LPDIRECT3DRMTEXTURE	magicTexture[NUM_MAGIC_FRAMES];
LPDIRECT3DRMTEXTURE	teleportTexture[NUM_MAGIC_FRAMES];
*/

LPDIRECT3DRMTEXTURE	starTexture[20];
LPDIRECT3DRMTEXTURE	dustTexture[20];
LPDIRECT3DRMTEXTURE	platTexture[20];
LPDIRECT3DRMTEXTURE	splashTexture[20];
LPDIRECT3DRMTEXTURE	fireballTexture[20];
LPDIRECT3DRMTEXTURE	magicTexture[20];
LPDIRECT3DRMTEXTURE	teleportTexture[20];
int					numDebrisFrames[NUM_DEBRIS_TYPES];


LPDIRECT3DRMMESHBUILDER pickupMesh[NUMPICKUPS];

char *pickupName[] = 
{
	"SUCTION",
	"GAUNTLET",
	"SPEEDUP",
	"HEAVYBALL",
	"LIGHTBALL"
};


/*	--------------------------------------------------------------------------------
	Function 	: InitCollectables()
	Purpose 	: loads and positions all the collectables
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void InitCollectables()
{

	int x, y;
	float cx, cy;
	char temp[256];
    DDSURFACEDESC       ddsd;
    LPDIRECTDRAWSURFACE buffer; 
    DDCOLORKEY          ddck;
	D3DRMIMAGE			*image;
	DWORD				col;
	
//init icons for collectables

	sprintf(userSurface[LIVES_ICON].filename, "%sgraphics\\icons\\live", rootPath);
	userSurface[LIVES_ICON].active = TRUE;
	userSurface[LIVES_ICON].display = TRUE;
	userSurface[LIVES_ICON].position.cx = 20;
	userSurface[LIVES_ICON].position.cy = 420;

	sprintf(userSurface[SUCTION_ICON].filename, "%sgraphics\\icons\\suct", rootPath);
	userSurface[SUCTION_ICON].active = TRUE;
	userSurface[SUCTION_ICON].display = FALSE;
	userSurface[SUCTION_ICON].position.cx = 20;
	userSurface[SUCTION_ICON].position.cy = 100;

	sprintf(userSurface[GAUNTLET_ICON].filename, "%sgraphics\\icons\\gaun", rootPath);
	userSurface[GAUNTLET_ICON].active = TRUE;
	userSurface[GAUNTLET_ICON].display = FALSE;
	userSurface[GAUNTLET_ICON].position.cx = 20;
	userSurface[GAUNTLET_ICON].position.cy = 60;

	sprintf(userSurface[SPEEDUP_ICON].filename, "%sgraphics\\icons\\spee", rootPath);
	userSurface[SPEEDUP_ICON].active = TRUE;
	userSurface[SPEEDUP_ICON].display = FALSE;
	userSurface[SPEEDUP_ICON].position.cx = 20;
	userSurface[SPEEDUP_ICON].position.cy = 20;
/*
	sprintf(userSurface[ISL_ICON].filename, "%sgraphics\\icons\\isl", rootPath);
	userSurface[ISL_ICON].active = TRUE;
	userSurface[ISL_ICON].display = TRUE;
	userSurface[ISL_ICON].position.cx = 600;
	userSurface[ISL_ICON].position.cy = 440;
*/
	sprintf(userSurface[HEAVYBALL_ICON].filename, "%sgraphics\\icons\\heav", rootPath);
	userSurface[HEAVYBALL_ICON].active = TRUE;
	userSurface[HEAVYBALL_ICON].display = FALSE;
	userSurface[HEAVYBALL_ICON].position.cx = 20;
	userSurface[HEAVYBALL_ICON].position.cy = 140;

	sprintf(userSurface[LIGHTBALL_ICON].filename, "%sgraphics\\icons\\lite", rootPath);
	userSurface[LIGHTBALL_ICON].active = TRUE;
	userSurface[LIGHTBALL_ICON].display = FALSE;
	userSurface[LIGHTBALL_ICON].position.cx = 20;
	userSurface[LIGHTBALL_ICON].position.cy = 140;

	sprintf(userSurface[TIMEOUT_ICON].filename, "%sgraphics\\icons\\time", rootPath);
	userSurface[TIMEOUT_ICON].active = TRUE;
	userSurface[TIMEOUT_ICON].display = FALSE;
	userSurface[TIMEOUT_ICON].position.cx = (640 - 148) /2 + 400 - 64;
	userSurface[TIMEOUT_ICON].position.cy = (480 - 92) / 2;

	sprintf(userSurface[GAMEWON_ICON].filename, "%sgraphics\\icons\\comp", rootPath);
	userSurface[GAMEWON_ICON].active = TRUE;
	userSurface[GAMEWON_ICON].display = FALSE;
	userSurface[GAMEWON_ICON].position.cx = (640 - 156) /2 + 400 - 64;
	userSurface[GAMEWON_ICON].position.cy = (480 - 99) / 2;

	sprintf(userSurface[LIVESGONE_ICON].filename, "%sgraphics\\icons\\over", rootPath);
	userSurface[LIVESGONE_ICON].active = TRUE;
	userSurface[LIVESGONE_ICON].display = FALSE;
	userSurface[LIVESGONE_ICON].position.cx = (640 - 128) /2 - 400 + 64;
	userSurface[LIVESGONE_ICON].position.cy = (480 - 74) / 2;

	InitUserSurfaces();

//load stars in
	for(x = 0; ; x++)
	{
		sprintf(temp, "%s\\graphics\\debris\\star%04d.ppm", rootPath, x+1);
		if(FileExists(temp))
		{		
			g_lpD3DRM->LoadTexture(temp, &starTexture[x]);
			starTexture[x]->SetDecalTransparency(TRUE);
			starTexture[x]->SetDecalTransparentColor(RGB_MAKE(0,0,0));
			starTexture[x]->SetDecalSize(2, 2);
			starTexture[x]->SetDecalOrigin(16, 16);
		}
		else
			break;
		numDebrisFrames[STARS] = x + 1;
	}

//load dust in
	for(x = 0; ; x++)
//	for(x = 0; x < NUM_DUST_FRAMES; x++)
	{
  		sprintf(temp, "%sgraphics\\debris\\dust%04d.ppm", rootPath, x+1);
		if(FileExists(temp))
		{		
			g_lpD3DRM->LoadTexture(temp, &dustTexture[x]);
			dustTexture[x]->SetDecalSize(2, 2);
			dustTexture[x]->SetDecalTransparency(TRUE);
			dustTexture[x]->SetDecalTransparentColor(RGB_MAKE(0,0,0));
			dustTexture[x]->SetDecalOrigin(16, 16);
		}
		else
			break;
		numDebrisFrames[DUST] = x + 1;
	}

//load platform debris in
	for(x = 0; ; x++)
//	for(x = 0; x < NUM_PLAT_FRAMES; x++)
	{
  		sprintf(temp, "%sgraphics\\debris\\plat%04d.ppm", rootPath, x+1);
		if(FileExists(temp))
		{		
			g_lpD3DRM->LoadTexture(temp, &platTexture[x]);
			platTexture[x]->SetDecalSize(1, 1);
			platTexture[x]->SetDecalTransparency(TRUE);
			platTexture[x]->SetDecalTransparentColor(RGB_MAKE(0,0,0));
			platTexture[x]->SetDecalOrigin(16, 16);
		}
		else
			break;
		numDebrisFrames[PLATBIT] = x + 1;
	}

//load splash debris in
	for(x = 0; ; x++)
//	for(x = 0; x < NUM_SPLASH_FRAMES; x++)
	{
		sprintf(temp, "%sgraphics\\debris\\watr%04d.ppm", rootPath, x+1);
		if(FileExists(temp))
		{		
			TRY_MYD3DRM(g_lpD3DRM->LoadTexture(temp, &splashTexture[x]));
			splashTexture[x]->SetDecalSize(2, 2);
			splashTexture[x]->SetDecalTransparency(TRUE);
			splashTexture[x]->SetDecalTransparentColor(RGB_MAKE(0,0,0));
			splashTexture[x]->SetDecalOrigin(16, 16);
		}
		else
			break;
		numDebrisFrames[SPLASH] = x + 1;
	}

//load fireballs
	for(x = 0; ; x++)
//	for(x = 0; x < NUM_FIREBALL_FRAMES; x++)
	{
		sprintf(temp, "%sgraphics\\debris\\fbal%04d.ppm", rootPath, x+1);
		if(FileExists(temp))
		{		
			TRY_MYD3DRM(g_lpD3DRM->LoadTexture(temp, &fireballTexture[x]));
			fireballTexture[x]->SetDecalSize(3, 3);
			fireballTexture[x]->SetDecalTransparency(TRUE);
			fireballTexture[x]->SetDecalTransparentColor(RGB_MAKE(0,0,0));
			fireballTexture[x]->SetDecalOrigin(16, 16);
		}
		else
			break;
		numDebrisFrames[FIREBALL] = x + 1;
	}

//load magic
	for(x = 0; ; x++)
//	for(x = 0; x < NUM_MAGIC_FRAMES; x++)
	{
		sprintf(temp, "%sgraphics\\debris\\magi%04d.ppm", rootPath, x+1);
		if(FileExists(temp))
		{		
			TRY_MYD3DRM(g_lpD3DRM->LoadTexture(temp, &magicTexture[x]));
			magicTexture[x]->SetDecalSize(2, 2);
			magicTexture[x]->SetDecalTransparency(TRUE);
			magicTexture[x]->SetDecalTransparentColor(RGB_MAKE(0,0,0));
			magicTexture[x]->SetDecalOrigin(16, 16);
		}
		else
			break;
		numDebrisFrames[MAGIC] = x + 1;
	}

//load oringina
	for(x = 0; ; x++)
//	for(x = 0; x < NUM_SPARK_FRAMES; x++)
	{
		sprintf(temp, "%sgraphics\\debris\\swir%04d.ppm", rootPath, x+1);
		if(FileExists(temp))
		{		
			TRY_MYD3DRM(g_lpD3DRM->LoadTexture(temp, &teleportTexture[x]));
			teleportTexture[x]->SetDecalSize(2, 2);
			teleportTexture[x]->SetDecalTransparency(TRUE);
			teleportTexture[x]->SetDecalTransparentColor(RGB_MAKE(0,0,0));
			teleportTexture[x]->SetDecalOrigin(16, 16);
		}
		else
			break;
		numDebrisFrames[SPARK] = x + 1;
	}

	for(x = 0; x < MAX_DEBRIS; x++)
	{

		debris[x].actor = (ACTOR *)MyCalloc(1, sizeof(ACTOR));
		debris[x].actor->type = DEBRIS;
		debris[x].actor->velInertia = 0.7;
		debris[x].actor->rotInertia = 0.7;
		debris[x].actor->flags = COLLISION + GRAVITY + DECELERATE;
		debris[x].actor->radius = debris[x].actor->radiusAim = 0.5;
		debris[x].actor->mass = 0;
		debris[x].actor->maxSpeed = 1;
		debris[x].actor->bounce = 0.9;
		debris[x].actor->stopSpeed = 0.12;
		debris[x].actor->slideLimit = 0.1;
		debris[x].actor->normInertia = debris[x].actor->velInertia;

		InitActor(debris[x].actor);
	    TRY_MYD3DRM(g_lpD3DRM->CreateFrame(g_lpScene, &debris[x].actor->frame))
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: ProcessCollectables()
	Purpose 	: checks for collision with hand, and generates stars
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void ProcessCollectables()
{
	int x = 0, y = 0;
	VECTOR tempVect;
	double angle = 0;
	int numStars = 8;
	double angleStep = (2*PI) / numStars;
	double burstRadius = 0.4;
	double scale;
	int temp;

//shrink the collectable and generate stars
	for(x = 0; x < numberOfCollectables; x++)
	{
		if(collectables[x].delay > 1)
		{
			collectables[x].delay--;
			scale = 0.5;
//			scale = (double)collectables[x].delay / 5;
			collectables[x].frame->AddScale(D3DRMCOMBINE_BEFORE, D3DVALUE(scale),D3DVALUE(scale),D3DVALUE(scale));
			if(collectables[x].delay == 2)
			{
				PlaySample(SFX_COLLECTABLE, 0, &collectables[x].pos,100);
				RemoveVisuals(collectables[x].frame);
				CreateStarCircle(&collectables[x].pos, 6);
				player.score += 300;
			}
		}
	}


//check for collision with hand
	for(x = 0; x < numberOfCollectables; x++)
	{
		if(collectables[x].active == TRUE)
		{
			SubVector(&tempVect, &hand.pos, &collectables[x].pos);
			if(fabs(Magnitude(&tempVect)) < collectables[x].size)
			{
				collectables[x].active = FALSE;
				collectables[x].delay = 5;

				ActivateCollectable(collectables[x].type);
			}
		}
	}


	for(x = 0;x < NUMPICKUPS;x++)
	{
		if(player.pickupTimer[x] > 0)
		{
			player.pickupTimer[x]--;
			if(player.pickupTimer[x] > 0)
			{
				if((player.pickupTimer[x] > 40) || ((player.pickupTimer[x] MOD 8) > 3))
					userSurface[pickupToIcon[x]].display = TRUE;
				else
					userSurface[pickupToIcon[x]].display = FALSE;
			}
			else
				DeactivateCollectable(x);

			if((x == SPEEDUP) && (player.pickupTimer[x] < 5))
			{
				hand.maxAccelSpeed = DEFAULT_HAND_MAXSPEED + (double)(player.pickupTimer[x]*speedBonus)/5;
				hand.accel= HAND_ACCELERATION;
			}
		}
		else
			userSurface[pickupToIcon[x]].display = FALSE;
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void ActivateCollectable(int type)
{
	player.pickupTimer[type] += 2000;
	switch(type)
	{
		case SPEEDUP:
			hand.accel = HAND_ACCELERATION*2;
			hand.maxAccelSpeed = SPEEDUP_HAND_MAXSPEED;
//			hand.maxSpeed = 1 + speedBonus;
			break;

		case HEAVYBALL:
			player.pickupTimer[LIGHTBALL] = 0;
			userSurface[LIGHTBALL_ICON].display = FALSE;
			ball.mass = 23;
			ball.bounce = 0.6;
			ball.radiusAim = 1.4;
			ballControl.ballType = HEAVYBALL;
			break;

		case LIGHTBALL:
			player.pickupTimer[HEAVYBALL] = 0;
			userSurface[HEAVYBALL_ICON].display = FALSE;
			ball.mass = 8;
			ball.bounce = 0.93;
			ball.radiusAim = 0.6;
			ballControl.ballType = LIGHTBALL;
			break;
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void DeactivateCollectable(int type)
{
	switch(type)
	{
		case HEAVYBALL:
		case LIGHTBALL:
			ball.mass = BALL_MASS;
			ball.bounce = BALL_BOUNCE;
			ball.radiusAim = BALL_NORMAL_RADIUS;
			ballControl.ballType = BALL_NORMAL;
			break;
	}
}
/*	--------------------------------------------------------------------------------
	Function 	: AddDebris()
	Purpose 	: creates debris ie dust, stars etc
	Parameters 	: type, position, velocity
	Returns 	: none
	Info 		:
*/

void AddDebris(int debrisType, VECTOR *position, VECTOR *velocity)
{
	
	int x = 0;


//	if((DistanceBetweenPoints(&camera.pos, position) > 150) || (IsActorFacingPoint(&camera, position, PI/2) == NO))
	if((DistanceBetweenPoints(&camera.pos, position) > 150) || (IsActorFacingPoint(&camera,position,PI/2) == YES))
		return;


	for(x = 0; debris[x].active == TRUE; x++);
	{
		if(x >= MAX_DEBRIS)
			return;

		AddActor(debris[x].actor);
		debris[x].actor->radius = debris[x].actor->radiusAim = debris[x].actor->maxRadius = 0.5;
		debris[x].actor->flags = COLLISION + GRAVITY + DECELERATE;

		SetVector(&debris[x].actor->pos, position);
		SetVector(&debris[x].actor->oldpos, position);
		SetVector(&debris[x].actor->vel, velocity);
		debris[x].active = TRUE;
		debris[x].actor->status = ACTIVE;
		debris[x].currentFrame = 0;
		debris[x].type = debrisType;

		switch(debrisType)
		{
			case DUST:
					debris[x].animationType = DEBRIS_ANIM_ONESHOT;
					debris[x].numFrames = NUM_DUST_FRAMES;
					debris[x].texture = dustTexture;
					debris[x].actor->flags &= -1 - COLLISION;
					debris[x].actor->flags &= -1 - GRAVITY;
					break;
			case STARS:
					debris[x].animationType = DEBRIS_ANIM_CYCLE;
					debris[x].numFrames = NUM_STAR_FRAMES;
					debris[x].currentFrame = rand() % NUM_STAR_FRAMES;
					debris[x].actor->lifetime = 60 + (rand()%10);
					debris[x].texture = starTexture;
					break;
			case PLATBIT:
					debris[x].animationType = DEBRIS_ANIM_CYCLE;
					debris[x].numFrames = NUM_PLAT_FRAMES;
					debris[x].texture = platTexture;
					debris[x].actor->lifetime = 20;
					break;
			case SPLASH:
					debris[x].animationType = DEBRIS_ANIM_ONESHOT;
					debris[x].numFrames = NUM_SPLASH_FRAMES;
					debris[x].texture = splashTexture;
					debris[x].actor->flags &= -1 - COLLISION;
					debris[x].actor->flags &= -1 - GRAVITY;
					break;
			case MAGIC:
					debris[x].animationType = DEBRIS_ANIM_CYCLE;
					debris[x].numFrames = NUM_MAGIC_FRAMES;
					debris[x].texture = magicTexture;
					debris[x].actor->flags &= -1 - COLLISION;
					debris[x].actor->flags &= -1 - GRAVITY;
					debris[x].actor->lifetime = 10 + (rand()%5);
					debris[x].currentFrame = rand() % NUM_MAGIC_FRAMES;
					break;
			case FIREBALL:
					debris[x].animationType = DEBRIS_ANIM_CYCLE;
					debris[x].numFrames = NUM_FIREBALL_FRAMES;
					debris[x].texture = fireballTexture;
					debris[x].actor->lifetime = 60 + (rand()%10);
					debris[x].actor->radius = debris[x].actor->radiusAim = 2;
					break;
			case SPARK:
					debris[x].animationType = DEBRIS_ANIM_ONESHOT;
					debris[x].numFrames = NUM_SPARK_FRAMES;
					debris[x].texture = teleportTexture;
					debris[x].actor->radius = debris[x].actor->radiusAim = 2;
					debris[x].actor->flags &= -1 - COLLISION;
					debris[x].actor->flags &= -1 - GRAVITY;
					break;
					
		}
		TransformActor(debris[x].actor);
		debris[x].actor->frame->AddVisual(debris[x].texture[debris[x].currentFrame]);

	}



}



/*	--------------------------------------------------------------------------------
	Function 	: UpdateDebris()
	Purpose 	: animates debris, and kills it when it runs out of time, or animation frames
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void UpdateDebris()
{
	int		x;
	double	temp;

//kill stars + makethem flash
	for(x = 0; x < MAX_DEBRIS; x++)
	{
		if(debris[x].active == TRUE)
		{
			if(debris[x].animationType != DEBRIS_ANIM_NONE)
			{
				RemoveVisuals(debris[x].actor->frame);
				debris[x].currentFrame++;

				if(debris[x].currentFrame >= debris[x].numFrames)
				{
					if(debris[x].animationType == DEBRIS_ANIM_CYCLE)
					{
						debris[x].currentFrame = 0;		
						debris[x].actor->frame->AddVisual(debris[x].texture[debris[x].currentFrame]);
					}
					if(debris[x].animationType == DEBRIS_ANIM_ONESHOT)
					{
						if(debris[x].currentFrame > debris[x].numFrames)
						{
//							RemoveVisuals(debris[x].actor->frame);
							debris[x].active = FALSE;
							SubActor(debris[x].actor);
						}
					}
				}
				else
					debris[x].actor->frame->AddVisual(debris[x].texture[debris[x].currentFrame]);

			}

			if(debris[x].actor->lifetime > 0)
			{
				debris[x].actor->lifetime--;
				if(debris[x].actor->lifetime == 0) //kill them
				{
					RemoveVisuals(debris[x].actor->frame);
					SubActor(debris[x].actor);
					debris[x].active = FALSE;
				}
				if(debris[x].type == FIREBALL)
				{
					temp = 100 - debris[x].actor->lifetime;
					temp /= 100;
//					debris[x].actor->frame->AddScale(D3DRMCOMBINE_AFTER, temp,temp,temp);
					if((!(debris[x].actor->lifetime % 2)) && (debris[x].actor->lifetime > 30))
					{
						AddDebris(DUST, &debris[x].actor->pos, &zero);
					}
				}

  			}

		}
	}


}



/*	--------------------------------------------------------------------------------
	Function 	: CreateDustCircle()
	Purpose 	: creates dust debris in a circle about a given point
	Parameters 	: vector *pos
	Returns 	: none
	Info 		:
*/
void CreateDustCircle(VECTOR *pos, int numStars, double verticalVelocity)
{

	int y;
	double angle;
	double angleStep = (2*PI) / numStars;
	double burstRadius = 0.4;
	VECTOR tempVect;
	VECTOR tempVect2;

	angle = rand() % 5;
	for(y = 0; y < numStars; y++)
	{
		SetVector(&tempVect, &zero);
		tempVect.v[X] = burstRadius;
		RotateVector2D(&tempVect, &tempVect, angle);
		tempVect.v[Y] = verticalVelocity;
		angle = Aabs(angle + angleStep);
		AddDebris(DUST, pos, &tempVect);
	}
}
/*	--------------------------------------------------------------------------------
	Function 	: CreateSplashCircle()
	Purpose 	: creates water debris in a circle about a given point
	Parameters 	: vector *pos
	Returns 	: none
	Info 		:
*/
void CreateSplashCircle(VECTOR *pos, int numStars, double verticalVelocity)
{

	int y;
	double angle;
	double angleStep = (2*PI) / numStars;
	double burstRadius = 0.4;
	VECTOR tempVect;
	VECTOR tempVect2;

	angle = rand() % 5;
	for(y = 0; y < numStars; y++)
	{
		SetVector(&tempVect, &zero);
		tempVect.v[X] = burstRadius;
		RotateVector2D(&tempVect, &tempVect, angle);
		tempVect.v[Y] = verticalVelocity;
		angle = Aabs(angle + angleStep);
		AddDebris(SPLASH, pos, &tempVect);
	}
}


/*	--------------------------------------------------------------------------------
	Function 	: CreateStarCircle()
	Purpose 	: creates star debris in a circle about a given point
	Parameters 	: vector *pos
	Returns 	: none
	Info 		:
*/
void CreateStarCircle(VECTOR *pos, int numStars)
{

	int y;
	double angle, scale;
	double angleStep = (2*PI) / numStars;
	double burstRadius = 0.4;
	VECTOR tempVect;
	VECTOR tempVect2;

	angle = 0;
	for(y = 0; y < numStars; y++)
	{
		SetVector(&tempVect, &zero);
		tempVect.v[X] = burstRadius;
		RotateVector2D(&tempVect, &tempVect, angle);
		scale = rand()%10;
		scale /= 60;
		tempVect.v[Y] = 1.3+scale;
		angle += angleStep;

		AddDebris(STARS, pos, &tempVect);
		
	}




}

/*	--------------------------------------------------------------------------------
	Function 	: CreateStarCircleXYZ()
	Purpose 	: lazy bastard routine
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void CreateStarCircleXYZ(double x, double y, double z, int numStars)
{
	VECTOR tempVect;

	tempVect.v[X] = x;
	tempVect.v[Y] = y;
	tempVect.v[Z] = z;

	CreateStarCircle(&tempVect, numStars);

}




void ResetCollectables()
{
	int x;

	for(x = 0; x < numberOfCollectables; x++)
	{
		RemoveVisuals(collectables[x].frame);
//		TRY_MYD3DRM(collectables[x].frame->AddVisual((LPDIRECT3DRMVISUAL)pickupMesh[2]));
		TRY_MYD3DRM(collectables[x].frame->AddVisual((LPDIRECT3DRMVISUAL)pickupMesh[collectables[x].type]));
		collectables[x].active = TRUE;
		if(collectables[x].delay)
		{
	 		collectables[x].frame->AddScale(D3DRMCOMBINE_BEFORE, D3DVALUE(16),D3DVALUE(16),D3DVALUE(16));
			collectables[x].delay = 0;
		}
 	}

}