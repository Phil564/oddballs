/*
 *  Project:         MapChopper
 *  File:            platforms.cpp
 *  Description:     
 *
 *  (c)1996 ISL.
 */


#include "oddballs.h"
#include "platforms.h"
#include "collectables.h"
#include "sound.h"
#include "collision.h"
#include "wind.h"
#include "hand.h"
#include "puzzles.h"

PLATFORMLIST	platformList;
ROPEBRIDGE		ropeBridge[NUMROPEBRIDGES];

PLATFORMIDLIST	platformIdList;


/*	--------------------------------------------------------------------------------
	Function 	: InitPlatformIdList()
	Purpose 	: initialises the linked list of platforms for id purposes
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void InitPlatformIdList()
{
	platformIdList.numEntries = 0;
	platformIdList.head.next = platformIdList.head.prev = &platformIdList.head;
}

/*	--------------------------------------------------------------------------------
	Function 	: AddPlatformId()
	Purpose 	: adds a platform to the linked list
	Parameters 	: pointer to platform
	Returns 	: none
	Info 		:
*/
void AddPlatformId(PLATFORMID *platformId)
{
	int	j;

	if(platformId->next == NULL)
	{
		platformIdList.numEntries++;
		platformId->prev = &platformIdList.head;
		platformId->next = platformIdList.head.next;
		platformIdList.head.next->prev = platformId;
		platformIdList.head.next = platformId;
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: AddPlaneToList()
	Purpose 	: adds a plane pointer to a plane list
	Parameters 	: plane pointer, plane list pointer
	Returns 	: none
	Info 		:
*/
void AddPlaneToList(PLANE *plane,PLANELIST *pl)
{
	PLANELISTENTRY	*newPtr;

	newPtr = (PLANELISTENTRY *)malloc(sizeof(PLANELISTENTRY));

	newPtr->next = pl->head.next;
	newPtr->prev = &pl->head;
	pl->head.next->prev = newPtr;
	pl->head.next = newPtr;

	newPtr->plane = plane;
	pl->numEntries++;
}

/*	--------------------------------------------------------------------------------
	Function 	: InitPlaneList()
	Purpose 	: initialises a plane list
	Parameters 	: pointer to list
	Returns 	: none
	Info 		:
*/
void InitPlaneList(PLANELIST *pl)
{
	pl->numEntries = 0;
	pl->head.next = &pl->head;
	pl->head.prev = &pl->head;
	pl->head.plane = NULL;
}

/*	--------------------------------------------------------------------------------
	Function 	: InitPlatformList()
	Purpose 	: initialises the linked list of platforms
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void InitPlatformList()
{
	platformList.numEntries = 0;
	platformList.head.next = platformList.head.prev = &platformList.head;
}

/*	--------------------------------------------------------------------------------
	Function 	: ClearPlatformList()
	Purpose 	: frees up memory used in platform list
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void ClearPlatformList()
{
	PLATFORMLISTENTRY	*ptr1,*ptr2;

	for(ptr1 = platformList.head.next;ptr1 != &platformList.head;ptr1 = ptr2)
	{
		ptr1->platform->listPointer = 0;
		ptr2 = ptr1->next;
		MyFree((void **)&ptr1);
	}
	InitPlatformList();
}

/*	--------------------------------------------------------------------------------
	Function 	: AddPlatform()
	Purpose 	: adds a platform to the linked list
	Parameters 	: pointer to platform
	Returns 	: none
	Info 		:
*/
void AddPlatform(PLATFORM *platform)
{
	if(platform->listPointer == 0)
	{
		platformList.numEntries++;
		platform->listPointer = (PLATFORMLISTENTRY *)MyMalloc(sizeof(PLATFORMLISTENTRY));
		platform->listPointer->next = &platformList.head;
		platform->listPointer->prev = platformList.head.prev;
		platformList.head.prev->next = platform->listPointer;
		platformList.head.prev = platform->listPointer;
		platform->listPointer->platform = platform;
	}
}


/*	--------------------------------------------------------------------------------
	Function 	: SubPlatform()
	Purpose 	: removes a platform from the linked list
	Parameters 	: pointer to platform
	Returns 	: none
	Info 		:
*/
void SubPlatform(PLATFORM *platform)
{
	if(platform->listPointer != 0)
	{
		platform->listPointer->prev->next = platform->listPointer->next;
		platform->listPointer->next->prev = platform->listPointer->prev;
		MyFree((void **)&platform->listPointer);
		platformList.numEntries--;
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: KillPlatform()
	Purpose 	: destroys a platform and all it's children for good
	Parameters 	: pointer to platform
	Returns 	: none
	Info 		:
*/
void KillPlatform(PLATFORM *platform)
{
	int	j,numChecked = 0;
	PLATFORMLISTENTRY *ptr1,*ptr2;
	ACTORLISTENTRY *aptr;
	PLATFORM *plat;

	for(aptr = actorList.head.next;aptr != &actorList.head;aptr = aptr->next)
	{
		if(aptr->actor->platform == platform)
			aptr->actor->platform = NULL;
		if(aptr->actor->oldPlatform == platform)
			aptr->actor->oldPlatform = NULL;
	}

	for(ptr1 = platformList.head.next;numChecked < platformList.numEntries;ptr1 = ptr2)
	{
		numChecked++;
		if(ptr1->platform->parentPlatform == platform)
		{
			KillPlatform(ptr1->platform);
			ptr2 = platformList.head.next;
			numChecked = 0;
		}
		else
			ptr2 = ptr1->next;
	}
	SubPlatform(platform);
	for(j = 0;j < MAXPLATFORMPATHPOINTS;j++)
		MyFree((void **)&platform->pathPoint[j]);
	MyFree((void **)&platform->confineBox);
	if((platform->flags & DUMMY) == 0)
		RemoveVisuals(platform->frame);
	MyFree((void **)&platform);
}

/*	--------------------------------------------------------------------------------
	Function 	: TransformPlatform()
	Purpose 	: applies translation and rotations to an platform
	Parameters 	: platform to update
	Returns 	: none
	Info 		:
*/
void TransformPlatform(PLATFORM *platform)
{
	volatile float	x,y,z;

	if(platform->flags & DUMMY)
		return;

	platform->frame->AddRotation(D3DRMCOMBINE_REPLACE, D3DVAL(0), D3DVAL(1), D3DVAL(0), D3DVAL(platform->rot.v[Y]));
	platform->frame->AddRotation(D3DRMCOMBINE_BEFORE,  D3DVAL(1), D3DVAL(0), D3DVAL(0), D3DVAL(platform->rot.v[X]));
	platform->frame->AddRotation(D3DRMCOMBINE_BEFORE,  D3DVAL(0), D3DVAL(0), D3DVAL(1), D3DVAL(platform->rot.v[Z]));
	platform->frame->AddScale(D3DRMCOMBINE_BEFORE,D3DVAL(platform->scale.v[X]),D3DVAL(platform->scale.v[Y]),D3DVAL(platform->scale.v[Z]));
	if((platform->crumbleCounter) && ((platform->crumbleGravityMultiplier == 0) || (platform->crumbleCounter <= platform->crumbleGravityDelay)))
	{
		x = platform->pos.v[X] + ((double)(rand() MOD 120)/300) - 0.4;
		y = platform->pos.v[Y];// + ((double)(rand() MOD 100)/400) - 0.25;
		z = platform->pos.v[Z] + ((double)(rand() MOD 120)/300) - 0.4;
		platform->frame->SetPosition(platform->parent,D3DVAL(x),D3DVAL(y),D3DVAL(z));
	}
	else
		platform->frame->SetPosition(platform->parent, D3DVAL(platform->pos.v[X]), D3DVAL(platform->pos.v[Y]), D3DVAL(platform->pos.v[Z]));
}

/*	--------------------------------------------------------------------------------
	Function 	: ScalePlatform()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void ScalePlatform(PLATFORM *platform)
{
	int i,j;

	if(platform->stretchTime)
	{
		j = 0;
		if(platform->stretchCounter > 1)
			platform->stretchCounter--;
		else if(platform->stretchCounter < -1)
			platform->stretchCounter++;
		else if(platform->stretchCounter != 0)
		{
			for(i = X;i <= Z;i++)
			{
				if(platform->stretchCounter == 1)
					platform->scale.v[i] += (platform->stretchScale.v[i] - platform->initScale.v[i])/platform->stretchTime;
				else //if(platform->stretchCounter == -1)
					platform->scale.v[i] -= (platform->stretchScale.v[i] - platform->initScale.v[i])/platform->stretchTime;

				if(fabs(platform->scale.v[i] - platform->stretchScale.v[i]) <= 0.0001)
				{
					j++;
					platform->scale.v[i] = platform->stretchScale.v[i];
				}
				else if(fabs(platform->scale.v[i] - platform->initScale.v[i]) <= 0.0001)
				{
					j++;
					platform->scale.v[i] = platform->initScale.v[i];
				}
			}
		}
		if(j == 3)
			platform->stretchCounter = -platform->stretchCounter*(platform->stretchWait + 1);
	
		platform->radius = platform->initRadius * max(max(platform->scale.v[X]/platform->initScale.v[X],platform->scale.v[Y]/platform->initScale.v[Y]),platform->scale.v[Z]/platform->initScale.v[Z]);
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: CrumblePlatform()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void CrumblePlatform(PLATFORM *platform)
{
	if(platform->crumbleCounter > 0)
	{
		if(platform->crumbleCounter++ > platform->crumbleLimit)
			platform->flags |= DESTROYED;
		if(platform->crumbleCounter > platform->crumbleGravityDelay)
			platform->vel.v[Y] -= gravity *	platform->crumbleGravityMultiplier;
	}
}


/*	--------------------------------------------------------------------------------
	Function 	: MovePlatform()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void MovePlatform(PLATFORM *platform,VECTOR *nextPoint)
{
	VECTOR	pos,distance;
	double	dist,val;
	int		p,accel = YES;

	ScaleVector(&platform->vel,platform->velInertia);

	if(platform->numPathPoints > 1)  	
	{
		if((platform->counter <= -4) && (platform->counter >= -9))
		{
			if(ball.platform == platform)
				platform->counter = -4;
			else
			{
				if(--platform->counter == -10)
					platform->counter = 1;
			}
		}
		if((platform->counter <= -10) && (platform->counter >= -15))
		{
			if(hand.platform == platform)
				platform->counter = -10;
			else
			{
				if(--platform->counter == -16)
					platform->counter = 1;
			}
		}
		if(platform->counter > 0)
		{
			if(--platform->counter == 0)
				platform->nextPoint = (platform->nextPoint + platform->numPathPoints + platform->direction) MOD platform->numPathPoints;
		}
		else if((platform->counter == 0) && (((platform->flags & LOOK_AT_POINT) == 0) || (platform->rot.v[Y] == platform->rotaim.v[Y]) || (platform->flags & GO_FORWARDS)))
		{
			if(platform->parentPlatform)
				SubVector(&pos,&platform->pos,&platform->parentPlatform->pos);
			else
				SetVector(&pos,&platform->pos);

			SubVector(&distance,nextPoint,&pos);
			dist = MagnitudeSquared(&distance);
			if(dist <= platform->speed * platform->speed)
			{
				if((platform->flags & GO_FORWARDS) == 0)
				{
					accel = NO;
					if(platform->parentPlatform)
						AddVector(&platform->pos,nextPoint,&platform->parentPlatform->pos);
					else
						SetVector(&platform->pos,nextPoint);

					SetVector(&platform->vel,&zero);
					platform->speed = 0;
				}

				platform->counter = platform->waitTime[platform->nextPoint];
				platform->currentPoint = platform->nextPoint;//(platform->currentPoint + 1) MOD platform->numPathPoints;

				if(platform->flags & LOOK_AT_POINT)
				{
					p = (platform->nextPoint + platform->numPathPoints + platform->direction) MOD platform->numPathPoints;
					platform->rotaim.v[Y] = Aabs(atan2(platform->pos.v[X] - platform->pathPoint[p]->v[X],platform->pos.v[Z] - platform->pathPoint[p]->v[Z]));
				}

				if((platform->flags & REVERSING) && ((platform->currentPoint == 0) || (platform->currentPoint == platform->numPathPoints - 1)))
					platform->direction = -platform->direction;
				if(platform->currentPoint == 1)
				{
					player.timer += platform->timeBonus;
					if(platform->timeBonus != 0)
					{
						CreateStarCircle(&platform->pos, 3);
						PlaySample(SFX_SWITCH, 0, &platform->pos,100);				
					}
				}
				if((platform->actionFlags & ACTION_GO_ONE_STEP) || ((platform->currentPoint == 0) && (platform->actionFlags & ACTION_GO_ONE_CYCLE)))
					platform->maxSpeed = 0;
			}
			if(accel)
			{
				if(platform->flags & GO_FORWARDS)
				{
					platform->vel.v[X] -= platform->accel * sin(platform->rot.v[Y]);
					platform->vel.v[Z] -= platform->accel * cos(platform->rot.v[Y]);
				}
				else 
				{
					val = (platform->accel + platform->speed)*platform->speed;
					val /= 2*platform->accel;
					MakeUnit(&distance);
					if(dist < val * val)
					{
						if(platform->speed > platform->accel)
							AddOneScaledVector(&platform->vel,&platform->vel,&distance,-platform->accel);
					}
					else
						AddOneScaledVector(&platform->vel,&platform->vel,&distance,platform->accel);
				}

				platform->speed = Magnitude(&platform->vel);
				if(platform->speed > platform->maxSpeed)
				{
					ScaleVector(&platform->vel,platform->maxSpeed/platform->speed);
					platform->speed = Magnitude(&platform->vel);
				}
			}
		}
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: PlatformInherit()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void PlatformInherit(PLATFORM *platform,VECTOR *nextPoint)
{
	PLATFORM	*plat;
	int			i,j,k;
	double		d[2],dd[2],dist,ang;

	plat = platform->parentPlatform;
	while(plat != NULL)
	{
		for(i = X;i <= Z;i++)
			platform->pos.v[i] += plat->pos.v[i] - plat->oldpos.v[i];
		plat = plat->parentPlatform;
	}

	plat = platform->parentPlatform;

	SetVector(&platform->oldpos,&platform->pos);
	SetVector(&platform->oldrot,&platform->rot);
	if(platform->numPathPoints > 1)
	{
		SetVector(nextPoint,platform->pathPoint[platform->nextPoint]);
	}

	//parent's orientation
	if((plat != NULL) && (plat->flags & TILTING))
	{
		if(plat->rot.v[Y] - plat->oldrot.v[Y])
			RotateVector2D(&platform->vel,&platform->vel,plat->rot.v[Y] - plat->oldrot.v[Y]);
		else if(plat->rot.v[X] - plat->oldrot.v[X])
			RotateVector2DXYZ(&platform->vel,&platform->vel,plat->oldrot.v[X] - plat->rot.v[X],X);
		else if(plat->rot.v[Z] - plat->oldrot.v[Z])
			RotateVector2DXYZ(&platform->vel,&platform->vel,plat->oldrot.v[Z] - plat->rot.v[Z],Z);

		j = 0;
		k = 10;
		for(i = X;i <= Z;i++)
		{
			if(platform->flags & TAKESPIN)
			{
				if(plat->rot.v[i] != plat->oldrot.v[i])
				{
					platform->rot.v[i] = Aabs(platform->rot.v[i] + plat->rot.v[i] - plat->oldrot.v[i]);
					platform->rotaim.v[i] = Aabs(platform->rot.v[i] + plat->rot.v[i] - plat->oldrot.v[i]);
				}
			}

			if(plat->spinAxis.v[i] == 0)
			{
				d[j] = platform->pos.v[i] - plat->pos.v[i];
				dd[j] = platform->pathPoint[platform->nextPoint]->v[i];
				j++;
			}
			else
				k = i;
		}

		if(k != 10)
		{
			dist = sqrt(d[0]*d[0] + d[1]*d[1]);
			ang = atan2(d[1],d[0]);

			if(k == Y)
				ang -= plat->rot.v[Y] - plat->oldrot.v[Y];
			else
				ang += plat->rot.v[k] - plat->oldrot.v[k];

			ang = Aabs(ang);

			d[0] = dist*cos(ang);
			d[1] = dist*sin(ang);
			j = 0;
			for(i = X;i <= Z;i++)
			{
				if(plat->spinAxis.v[i] == 0)
					platform->pos.v[i] = d[j++] + plat->pos.v[i];
			}

			if(platform->numPathPoints > 1)
			{
				dist = sqrt(dd[0]*dd[0] + dd[1]*dd[1]);
				ang = atan2(dd[1],dd[0]);

				if(k == Y)
					ang -= plat->rot.v[Y];
				else
					ang += plat->rot.v[k];

				ang = Aabs(ang);

				dd[0] = dist*cos(ang);
				dd[1] = dist*sin(ang);
				j = 0;
				for(i = X;i <= Z;i++)
				{
					if(plat->spinAxis.v[i] == 0)
						nextPoint->v[i] = dd[j++];
				}
			}
		}
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: SpinPlatform()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void SpinPlatform(PLATFORM *platform)
{
	int		i;
	double	ang2,ang;

	if(platform->spinSpeed != 0)
	{
		for(i = X;i <= Z;i++)
		{
			if(platform->spinAxis.v[i] != 0)
			{
				if(platform->spinCounter > 0)
				{
					platform->spinCounter--;
					if((platform->spinNumPauses == 1) && (platform->spinCounter == 0))
						platform->spinCounter = -2;
				}
				else if(platform->spinCounter == 0)
				{
					if(platform->spinAxis.v[Y])
					{
						if(platform->spinSpeed > 0)
							ang2 = Aabs(platform->rot.v[i] - platform->initRot);
						else
							ang2 = Aabs(platform->initRot - platform->rot.v[i]);
					}
					else
					{
						if(platform->spinSpeed < 0)
							ang2 = Aabs(platform->rot.v[i] - platform->initRot);
						else
							ang2 = Aabs(platform->initRot - platform->rot.v[i]);
					}

					if(platform->spinNumPauses > 0)
					{
						if((ang = FindShortestAngle(ang2,(PI*2*platform->spinNextPoint)/platform->spinNumPauses)) < fabs(platform->spinSpeed))
//						if(FindShortestAngle(Aabs(platform->rot.v[i]),Aabs((PI*2*platform->spinNextPoint)/platform->spinNumPauses)) < fabs(platform->spinSpeed))
						{
							platform->spinCounter = platform->spinPauseTime;
							platform->rotvel.v[i] = 0;
							if(ang < 0.0001)
								platform->rot.v[i] = Aabs((PI*2*platform->spinNextPoint)/platform->spinNumPauses);
							platform->spinNextPoint = (platform->spinNextPoint + 1) MOD platform->spinNumPauses;
							if((platform->actionFlags & ACTION_GO_ONE_STEP) || ((platform->spinNextPoint == 1) && (platform->actionFlags & ACTION_GO_ONE_CYCLE)))
								platform->spinSpeed = 0;
						}
					}
					else if(platform->spinFlipFlop)
					{				
   						if((FindShortestAngle(ang2,platform->spinFlipFlop) < fabs(platform->spinSpeed)) || (FindShortestAngle(2*PI-platform->spinFlipFlop,ang2) < fabs(platform->spinSpeed)))
						{
							platform->spinCounter = platform->spinPauseTime;
							platform->spinSpeed = -platform->spinSpeed;
							platform->rotvel.v[i] = 0;
							if((platform->actionFlags & ACTION_GO_ONE_STEP) || ((platform->spinNextPoint == 1) && (platform->actionFlags & ACTION_GO_ONE_CYCLE)))
								platform->spinSpeed = 0;
						}
					}
				}
				if(platform->spinCounter <= 0)
				{
					if(platform->spinCounter < 0)
					{
						if(platform->spinCounter > -10)
						{
							platform->spinCounter++;
							platform->rotvel.v[i] = platform->spinSpeed;
						}
						else if(platform->currentPoint == -platform->spinCounter/10)
						{
							if(platform->nextPoint == platform->currentPoint)
								platform->spinCounter = 1;
						}
					}
					else
						platform->rotvel.v[i] = platform->spinSpeed;
				}
			}
		}
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: OrbitPlatform()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void OrbitPlatform(PLATFORM *platform)
{
	int			i,j,k;
	double		d[2],dist,ang,ang2;
	PLATFORM	*plat = platform->parentPlatform;

	if(platform->orbitSpeed != 0)
	{
		j = 0;
		k = 10;
		for(i = X;i <= Z;i++)
		{
			if(platform->orbitAxis.v[i] == 0)
			{
				d[j] = platform->pos.v[i] - platform->pathPoint[1]->v[i];
				if(plat != NULL)
					d[j] -= plat->pos.v[i];
				j++;
			}
			else
				k = i;
		}
		dist = sqrt(d[0]*d[0] + d[1]*d[1]);
		ang = atan2(d[1],d[0]);

		if(frameCount == 0)
			platform->initOrbitRot = ang;

		if(platform->orbitCounter > 0)
		{
			platform->orbitCounter--;
			if((platform->orbitNumPauses == 1) && (platform->orbitCounter == 0))
				platform->orbitCounter = -2;
		}
		else if(platform->orbitCounter == 0)
		{
			if(platform->orbitAxis.v[Y])
			{
				if(platform->orbitSpeed < 0)
					ang2 = Aabs(ang - platform->initOrbitRot);
				else
					ang2 = Aabs(platform->initOrbitRot - ang);
			}
			else
			{
				if(platform->orbitSpeed > 0)
					ang2 = Aabs(ang - platform->initOrbitRot);
				else
					ang2 = Aabs(platform->initOrbitRot - ang);
			}

			if(platform->orbitNumPauses > 0)
			{
				if(FindShortestAngle(ang2,(PI*2*platform->orbitNextPoint)/platform->orbitNumPauses) < fabs(platform->orbitSpeed))
				{
					platform->orbitCounter = platform->orbitPauseTime;
					SetVector(&platform->vel,&zero);
					platform->orbitNextPoint = (platform->orbitNextPoint + 1) MOD platform->orbitNumPauses;
					if((platform->actionFlags & ACTION_GO_ONE_STEP) || ((platform->orbitNextPoint == 1) && (platform->actionFlags & ACTION_GO_ONE_CYCLE)))
						platform->orbitSpeed = 0;
				}
			}
			else if(platform->orbitFlipFlop)
			{				
   				if((FindShortestAngle(ang2,platform->orbitFlipFlop) < fabs(platform->orbitSpeed)) || (FindShortestAngle(2*PI-platform->orbitFlipFlop,ang2) < fabs(platform->orbitSpeed)))
				{
					platform->orbitCounter = platform->orbitPauseTime;
					platform->orbitSpeed = -platform->orbitSpeed;
					SetVector(&platform->vel,&zero);
					if((platform->actionFlags & ACTION_GO_ONE_STEP) || ((platform->orbitNextPoint == 1) && (platform->actionFlags & ACTION_GO_ONE_CYCLE)))
						platform->orbitSpeed = 0;
				}
			}

		}
		if(platform->orbitCounter <= 0)
		{
			if(platform->orbitCounter < 0)
				platform->orbitCounter++;			
			if(k == Y)
				ang -= platform->orbitSpeed;
			else
				ang += platform->orbitSpeed;
			ang = Aabs(ang);
			d[0] = dist*cos(ang);
			d[1] = dist*sin(ang);
			j = 0;
			if(plat)
			{
				for(i = X;i <= Z;i++)
					if(platform->orbitAxis.v[i] == 0)
						platform->vel.v[i] = d[j++] + platform->pathPoint[1]->v[i] - platform->oldpos.v[i] + plat->pos.v[i];
			}
			else
			{
				for(i = X;i <= Z;i++)
					if(platform->orbitAxis.v[i] == 0)
						platform->vel.v[i] = d[j++] + platform->pathPoint[1]->v[i] - platform->oldpos.v[i];
			}
		}
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: ConfinePlatform()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void ConfinePlatform(PLATFORM *platform)
{
	int			i;
	PLATFORM	*plat = platform->parentPlatform;
	VECTOR		pos;

	for(i = X;i <= Z;i++)
		platform->pos.v[i] += platform->vel.v[i];

	if(platform->confineBox)
	{
		for(i = X;i <= Z;i++)
		{
			if(platform->confineBox->size.v[i] >= 0)
			{
				pos.v[i] = platform->pos.v[i];
				if(plat != NULL)
					pos.v[i] -= plat->pos.v[i];

				if(pos.v[i] < platform->confineBox->pos.v[i])
				{
					pos.v[i] = platform->confineBox->pos.v[i];
					platform->vel.v[i] = 0;
				}
				else if(pos.v[i] > platform->confineBox->pos.v[i] + platform->confineBox->size.v[i])
				{
					pos.v[i] = platform->confineBox->pos.v[i] + platform->confineBox->size.v[i];
					platform->vel.v[i] = 0;
				}
				platform->pos.v[i] = pos.v[i];
				if(plat != NULL)
					platform->pos.v[i] += plat->pos.v[i];
			}
		}
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: RotatePlatform()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void RotatePlatform(PLATFORM *platform)
{
	int		p,i,quad;
	double	ang,turnSpeed,maxRot;
	ACTOR	*actor=  NULL;
	PLATFORMLISTENTRY	*platformPtr;

	if((platform->flags & TILTING) || (platform->catapult))
	{
		if(platform->flags & LOOK_AT_BALL)
			actor = &ball;
		else if (platform->flags & LOOK_AT_HAND)
			actor = &hand;

		if(actor)
		{
			platform->rotaim.v[Y] = Aabs(atan2(actor->pos.v[X] - platform->pos.v[X],actor->pos.v[Z] - platform->pos.v[Z]));
		}

		for(i = X;i <= Z;i++)
		{
			if(platform->flags & SEESAW)
			{
				if(platform->rotvel.v[i] > platform->maxRotVel)
					platform->rotvel.v[i] = platform->maxRotVel;
				if(platform->rotvel.v[i] < -platform->maxRotVel)
					platform->rotvel.v[i] = -platform->maxRotVel;
			}

			if(platform->rAim.v[i] == ON)
			{
				if(platform->spinSymmetry > 1)
				{
					quad = (platform->spinSymmetry*(Aabs(platform->rot.v[i] - platform->initRot + PI/platform->spinSymmetry)))/(2*PI);
					platform->rotaim.v[i] = Aabs((quad * PI * 2)/platform->spinSymmetry + platform->initRot);
				}

//				if((platform->noActorInContactCount > 3) && (platform->turnSpeed != 0))			
				ang = FindShortestAngle(platform->rot.v[i],platform->rotaim.v[i]);
				if((i == Z) && (platform->flags & GO_FORWARDS))
					turnSpeed = platform->bank * 0.002;
				else
					turnSpeed = platform->turnSpeed;
				if(((platform->noActorInContactCount > 1) || ((platform->flags & SEESAW) == 0)) && (turnSpeed != 0))
//				if(turnSpeed != 0)
				{
					if(fabs(ang) <= 0.01)
					{
						platform->rot.v[i] = platform->rotaim.v[i];
						platform->rotvel.v[i] *= 0.8;
					}
					else if(platform->rot.v[i] < platform->rotaim.v[i] - turnSpeed)
					{
						if(platform->rotaim.v[i] - platform->rot.v[i] > PI - turnSpeed)
							platform->rotvel.v[i] -= Min(turnSpeed,ang);
						else		
							platform->rotvel.v[i] += Min(turnSpeed,ang);
					}
					else if(platform->rot.v[i] > platform->rotaim.v[i] + turnSpeed)
					{
						if(platform->rot.v[i] - platform->rotaim.v[i] > PI + turnSpeed)
							platform->rotvel.v[i] += Min(turnSpeed,ang);
						else		
							platform->rotvel.v[i] -= Min(turnSpeed,ang);
					}
					else if(((platform->flags & SEESAW) == 0) && ((platform->rotInertia == 1) || (fabs(platform->rotvel.v[i]) <= turnSpeed)))
					{
						platform->rot.v[i] = platform->rotaim.v[i];
						platform->rotvel.v[i] = 0;
					}
				}
			}

			if(platform->pendulumSwing)
				platform->rotvel.v[i] -= FindShortestAngleSigned(platform->rot.v[i],0)/platform->pendulumSwing;
	
			if((platform->toppleRot) && (ang > platform->toppleRot))
			{
				platform->rotvel.v[i] *= 1.3;
				platform->turnSpeed = 0;
			}

			platform->rot.v[i] += platform->rotvel.v[i];

			platform->rot.v[i] = Aabs(platform->rot.v[i]);

			if((platform->maxRot > 0) && (platform->spinAxis.v[i] == 1))
			{
				ang = platform->rot.v[i] > PI ? platform->rot.v[i] - 2 * PI : platform->rot.v[i];
				ang = ang < -PI ? ang + 2 * PI : ang;

				if(platform->toppleFlags & TOPPLE_POS)
					maxRot = platform->maxRot;
				else
					maxRot = (platform->toppleRot*4)/5;

				if(ang > platform->initRot + maxRot)
				{
					platform->rot.v[i] = platform->initRot + maxRot;
					platform->rotvel.v[i] = 0;
					if((platform->toppleRot) && (maxRot == platform->maxRot))
					{
						platform->flags &= -1 - SEESAW;
						platform->rAim.v[i] = 0;
					}
				}

				if(platform->toppleFlags & TOPPLE_NEG)
					maxRot = platform->maxRot;
				else
					maxRot = (platform->toppleRot*3)/5;

				if(ang < platform->initRot - maxRot)
				{
					platform->rot.v[i] = platform->initRot - maxRot;
					platform->rotvel.v[i] = 0;
					if((platform->toppleRot) && (maxRot == platform->maxRot))
					{
						platform->flags &= -1 - SEESAW;
						platform->rAim.v[i] = 0;
					}
				}
			}

			if((i == Z) && (platform->flags & GO_FORWARDS))
				platform->rotvel.v[i] = 0;
			else
				platform->rotvel.v[i] *= platform->rotInertia;
			if(fabs(platform->rotvel.v[i]) <= 0.0005)
				platform->rotvel.v[i] = 0;
			
			if(platform->spinToMove)
			{
				platformPtr = FindTaggedPlatform(&platformList.head,platform->targetTag);
				while((platformPtr != NULL) && (platformPtr->platform))
				{
					SetVector(&platformPtr->platform->vel,&platform->targetSpeed);
					AddVector(&platformPtr->platform->vel,&platformPtr->platform->vel,&platformPtr->platform->moveBySpeed);
					ScaleVector(&platformPtr->platform->vel,platform->rotvel.v[i] * platform->spinToMove);
					platformPtr = FindTaggedPlatform(platformPtr,platform->targetTag);
				}
			}
			if(platform->spinToSpin)
			{
				platformPtr = FindTaggedPlatform(&platformList.head,platform->targetTag);
				while((platformPtr != NULL) && (platformPtr->platform))
				{
					platformPtr->platform->spinSpeed = platform->rotvel.v[i] * platform->spinToSpin;
					platformPtr = FindTaggedPlatform(platformPtr,platform->targetTag);
				}
			}					
		}	

		if(platform->flags & GO_FORWARDS)
		{
			p = platform->nextPoint;
			if((p != platform->currentPoint) && (platform->maxSpeed))
			{
				platform->rotaim.v[Y] = Aabs(atan2(platform->pos.v[X] - platform->pathPoint[p]->v[X],platform->pos.v[Z] - platform->pathPoint[p]->v[Z]));
				ang = FindShortestAngleSigned(platform->rot.v[Y],platform->oldrot.v[Y]);
				if(fabs(ang) >= 0.018)
					platform->rotaim.v[Z] = Aabs(ang * platform->bank);
				else
					platform->rotaim.v[Z] = 0;
			}
			else
			{
				platform->rotaim.v[Y] = platform->rot.v[Y];
				platform->rotaim.v[Z] = 0;
			}
		}
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: UpdatePlatform()
	Purpose 	: updates an platform - movement and collision
	Parameters 	: platform to update
	Returns 	: none
	Info 		:
*/
void UpdatePlatform(PLATFORM *platform)
{
	VECTOR	nextPoint;

	CrumblePlatform(platform);
	ScalePlatform(platform);
	PlatformInherit(platform,&nextPoint);
	MovePlatform(platform,&nextPoint);
	SpinPlatform(platform);
	OrbitPlatform(platform);
	ConfinePlatform(platform);
	RotatePlatform(platform);

	platform->noActorInContactCount++;
}

/*	--------------------------------------------------------------------------------
	Function 	: InitPlatformInfo()
	Purpose 	: initialises info for platform's polygons for collision routines
	Parameters 	: platform
	Returns 	: none
	Info 		:
*/
void InitPlatformInfo(PLATFORM *platform)
{
	D3DVECTOR			vert[4];
	D3DVECTOR			vNormal[4];
	D3DVECTOR			normal;
	LPDIRECT3DRMFACE	face;
	int					j,k;
	DWORD				numVertices = 3;
	double				m;
	VECTOR				p;
	PLANE				*planes;
	double				max = 0;
	PLATFORM			*plat;

	plat = platform->parentPlatform;
	while(plat != NULL)
	{
		if(plat->flags & TAKESPIN)
			platform->flags & TILTING;
		plat = plat->parentPlatform;
	}

	if((platform->flags & DUMMY) || (platform->flags & BACKGROUND))
		return;

	SetVector(&platform->initScale,&platform->scale);

	planes = (PLANE *)MyMalloc(platform->id->faceCount * sizeof(PLANE));	

	InitPlaneList(&platform->planeList);

	for(j = 0;j < platform->id->faceCount;j++)
	{
		(planes+j)->index = j;
		(planes+j)->terrain = STONE;

		platform->id->faceArray->GetElement(j,&face);
//		face->GetVertices(&numVertices,NULL,NULL);		
		face->GetVertices(&numVertices,vert,vNormal);
		face->GetNormal(&normal);

		for(k = 0;k < numVertices;k++)
		{
			vert[k].x *= platform->scale.v[X];
			vert[k].y *= platform->scale.v[Y];
			vert[k].z *= platform->scale.v[Z];

			vert[k].x += platform->pos.v[X];
			vert[k].y += platform->pos.v[Y];
			vert[k].z += platform->pos.v[Z];
		}

		(planes+j)->J = -(vert[0].x*normal.x + vert[0].y*normal.y + vert[0].z*normal.z);
		m = max(fabs(normal.x),max(fabs(normal.y),fabs(normal.z)));
		if(m == fabs(normal.x))
		{
			(planes+j)->i[0] = 1;
			(planes+j)->i[1] = 2;
		}
		else if (m == fabs(normal.y))
		{
			(planes+j)->i[0] = 2;
			(planes+j)->i[1] = 0;
		}
		else
		{
			(planes+j)->i[0] = 0;
			(planes+j)->i[1] = 1;
		}
		(planes+j)->normal.v[X] = normal.x;
		(planes+j)->normal.v[Y] = normal.y;
		(planes+j)->normal.v[Z] = normal.z;

		(planes+j)->numVertices	= numVertices;

		for(k = 0;k < (planes+j)->numVertices;k++)
		{
			(planes+j)->vertex[k].v[X] = vert[k].x;
			(planes+j)->vertex[k].v[Y] = vert[k].y;
			(planes+j)->vertex[k].v[Z] = vert[k].z;

			SubVector(&p,&(planes+j)->vertex[k],&platform->pos);
			m = MagnitudeSquared(&p);
			if(m > max)
				max = m;			
			if(vert[k].x - platform->pos.v[X] > platform->size.v[X])
				platform->size.v[X] = vert[k].x - platform->pos.v[X];
			if(vert[k].y - platform->pos.v[Y] > platform->size.v[Y])
				platform->size.v[Y] = vert[k].y - platform->pos.v[Y];
			if(vert[k].z - platform->pos.v[Z] > platform->size.v[Z])
				platform->size.v[Z] = vert[k].z - platform->pos.v[Z];
		}

		(planes+j)->u1 = (planes+j)->vertex[1].v[(planes+j)->i[0]] - (planes+j)->vertex[0].v[(planes+j)->i[0]];
		(planes+j)->u2 = (planes+j)->vertex[2].v[(planes+j)->i[0]] - (planes+j)->vertex[0].v[(planes+j)->i[0]];

		(planes+j)->v1 = (planes+j)->vertex[1].v[(planes+j)->i[1]] - (planes+j)->vertex[0].v[(planes+j)->i[1]];
		(planes+j)->v2 = (planes+j)->vertex[2].v[(planes+j)->i[1]] - (planes+j)->vertex[0].v[(planes+j)->i[1]];

		AddPlaneToList(planes+j,&platform->planeList);
	}
	platform->radius = platform->initRadius = sqrt(max);
}
						    

/*	--------------------------------------------------------------------------------
	Function 	: RecalculatePlatformInfo()
	Purpose 	: recalculates planes info for a platform 
	Parameters 	: platform
	Returns 	: none
	Info 		:
*/
void RecalculatePlatformInfo(PLATFORM *platform)
{
	PLANE				*plane;
	PLANELISTENTRY		*ptr = platform->planeList.head.prev;
	D3DVECTOR			vert[4];
	D3DVECTOR			vNormal[4];
	D3DVECTOR			normal;
	LPDIRECT3DRMFACE	face;
	int					j,k;
	DWORD				numVertices = 3;
	double				m,dist,ang;

	if((platform->flags & DUMMY) || (platform->flags & BACKGROUND))
		return;

	platform->lastCalcFrame = frameCount;

	if((platform->flags & TILTING) || (platform->initRot))
	{
		for(j = 0;j < platform->id->faceCount;j++)
		{
			plane = ptr->plane;

			ptr = ptr->prev;

			platform->id->faceArray->GetElement(j,&face);
	//		face->GetVertices(&numVertices,NULL,NULL);		
			face->GetVertices(&numVertices,vert,vNormal);
			face->GetNormal(&normal);

			for(k = 0;k < numVertices;k++)
			{
				vert[k].x *= platform->scale.v[X];
				vert[k].y *= platform->scale.v[Y];
				vert[k].z *= platform->scale.v[Z];
			}

			if(platform->rot.v[X] != 0)
			{
				for(k = 0;k < numVertices;k++)
				{
					dist = sqrt(vert[k].y*vert[k].y + vert[k].z*vert[k].z);

					ang = atan2(vert[k].z,vert[k].y);
					ang += platform->rot.v[X];
					ang = Aabs(ang);

					vert[k].z = dist*sin(ang);
					vert[k].y = dist*cos(ang);
				}
				ang = atan2(normal.z,normal.y);
				ang += platform->rot.v[X];
				ang = Aabs(ang);

				dist = sqrt(normal.y*normal.y + normal.z*normal.z);

				normal.z = dist*sin(ang);
				normal.y = dist*cos(ang);
			}
			if(platform->rot.v[Z] != 0)
			{
				for(k = 0;k < numVertices;k++)
				{
					dist = sqrt(vert[k].y*vert[k].y + vert[k].x*vert[k].x);

					ang = atan2(vert[k].y,vert[k].x);
					ang += platform->rot.v[Z];
					ang = Aabs(ang);

					vert[k].y = dist*sin(ang);
					vert[k].x = dist*cos(ang);
				}
				ang = atan2(normal.y,normal.x);
				ang += platform->rot.v[Z];
				ang = Aabs(ang);

				dist = sqrt(normal.y*normal.y + normal.x*normal.x);

				normal.y = dist*sin(ang);
				normal.x = dist*cos(ang);
			}
			if(platform->rot.v[Y] != 0)
			{
				for(k = 0;k < numVertices;k++)
				{
					dist = sqrt(vert[k].x*vert[k].x + vert[k].z*vert[k].z);

					ang = atan2(vert[k].x,vert[k].z);
					ang += platform->rot.v[Y];
					ang = Aabs(ang);

					vert[k].x = dist*sin(ang);
					vert[k].z = dist*cos(ang);
				}
				ang = atan2(normal.x,normal.z);
				ang += platform->rot.v[Y];
				ang = Aabs(ang);

				dist = sqrt(normal.x*normal.x + normal.z*normal.z);

				normal.x = dist*sin(ang);
				normal.z = dist*cos(ang);
			}

			m = max(fabs(normal.x),max(fabs(normal.y),fabs(normal.z)));
			if(m == fabs(normal.x))
			{
				plane->i[0] = 1;
				plane->i[1] = 2;
			}
			else if(m == fabs(normal.y))
			{
				plane->i[0] = 2;
				plane->i[1] = 0;
			}
			else
			{
				plane->i[0] = 0;
				plane->i[1] = 1;
			}
			plane->normal.v[X] = normal.x;
			plane->normal.v[Y] = normal.y;
			plane->normal.v[Z] = normal.z;

			for(k = 0;k < plane->numVertices;k++)
			{
				plane->vertex[k].v[X] = vert[k].x + platform->pos.v[X];
				plane->vertex[k].v[Y] = vert[k].y + platform->pos.v[Y];
				plane->vertex[k].v[Z] = vert[k].z + platform->pos.v[Z];
			}
			plane->J = -(plane->vertex[0].v[X]*normal.x + plane->vertex[0].v[Y]*normal.y + plane->vertex[0].v[Z]*normal.z);

			plane->u1 = plane->vertex[1].v[plane->i[0]] - plane->vertex[0].v[plane->i[0]];
			plane->u2 = plane->vertex[2].v[plane->i[0]] - plane->vertex[0].v[plane->i[0]];

			plane->v1 = plane->vertex[1].v[plane->i[1]] - plane->vertex[0].v[plane->i[1]];
			plane->v2 = plane->vertex[2].v[plane->i[1]] - plane->vertex[0].v[plane->i[1]];
		}
	}
	else
	{
		for(j = 0;j < platform->id->faceCount;j++)
		{
			plane = ptr->plane;

			ptr = ptr->prev;

			platform->id->faceArray->GetElement(j,&face);
	//		face->GetVertices(&numVertices,NULL,NULL);		
			face->GetVertices(&numVertices,vert,vNormal);
			face->GetNormal(&normal);

			for(k = 0;k < numVertices;k++)
			{
				vert[k].x *= platform->scale.v[X];
				vert[k].y *= platform->scale.v[Y];
				vert[k].z *= platform->scale.v[Z];

				plane->vertex[k].v[X] = vert[k].x + platform->pos.v[X];
				plane->vertex[k].v[Y] = vert[k].y + platform->pos.v[Y];
				plane->vertex[k].v[Z] = vert[k].z + platform->pos.v[Z];
			}
			plane->J = -(plane->vertex[0].v[X]*normal.x + plane->vertex[0].v[Y]*normal.y + plane->vertex[0].v[Z]*normal.z);

			plane->u1 = plane->vertex[1].v[plane->i[0]] - plane->vertex[0].v[plane->i[0]];
			plane->u2 = plane->vertex[2].v[plane->i[0]] - plane->vertex[0].v[plane->i[0]];

			plane->v1 = plane->vertex[1].v[plane->i[1]] - plane->vertex[0].v[plane->i[1]];
			plane->v2 = plane->vertex[2].v[plane->i[1]] - plane->vertex[0].v[plane->i[1]];
		}
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: TagToFirstPlatform()
	Purpose 	: finds first platform in list with given tag
	Parameters 	: tag
	Returns 	: pointer to platform
	Info 		:
*/
PLATFORM *TagToFirstPlatform(int tag)
{
	PLATFORMLISTENTRY	*ptr;

	for(ptr = platformList.head.next;ptr != &platformList.head;ptr = ptr->next)
		if(ptr->platform->tag == tag)
			return ptr->platform;
	return NULL;
}

/*	--------------------------------------------------------------------------------
	Function 	: FindTaggedPlatform()
	Purpose 	: finds a platform with specified tag
	Parameters 	: platform to start search from, tag number
	Returns 	: platform pointer of platform with correct tag
	Info 		:
*/
PLATFORMLISTENTRY *FindTaggedPlatform(PLATFORMLISTENTRY *startPlatform, int tag)
{
	PLATFORMLISTENTRY *platformPtr;
		 
 	for(platformPtr = startPlatform->next; platformPtr != &platformList.head; platformPtr = platformPtr->next)
		if(platformPtr->platform->tag == tag)
			return platformPtr;
	
	return NULL;
}




/*	--------------------------------------------------------------------------------
	Function 	: UpdateRopeBridges()
	Purpose 	: updates...
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void UpdateRopeBridges()
{
	int		j,k,x;
	VECTOR	rotvel[100],tempVect;
	PLATFORM	*platform1,*platform2;

	for(k = 0;k < NUMROPEBRIDGES;k++)
	{
		if(ropeBridge[k].rotVelTransmit == 0)
			return;

 		for(x = ropeBridge[k].minTag;x <= ropeBridge[k].maxTag;x++)
			SetVector(&rotvel[x-ropeBridge[k].minTag],&zero);

 		for(x = ropeBridge[k].minTag;x <= ropeBridge[k].maxTag;x++)
		{
			platform1 = TagToFirstPlatform(x);

			if(x > ropeBridge[k].minTag)
			{
				platform2 = TagToFirstPlatform(x-1);
				for(j = X;j <= Z;j++)
 					tempVect.v[j] = FindShortestAngleSigned(platform1->rot.v[j],platform2->rot.v[j]);
				AddOneScaledVector(&rotvel[x-1-ropeBridge[k].minTag],&rotvel[x-1-ropeBridge[k].minTag],&tempVect,ropeBridge[k].rotVelTransmit);
			}
			if(x < ropeBridge[k].maxTag)
			{
				platform2 = TagToFirstPlatform(x+1);
				for(j = X;j <= Z;j++)
					tempVect.v[j] = FindShortestAngleSigned(platform1->rot.v[j],platform2->rot.v[j]);
				AddOneScaledVector(&rotvel[x+1-ropeBridge[k].minTag],&rotvel[x+1-ropeBridge[k].minTag],&tempVect,ropeBridge[k].rotVelTransmit);
			}
		}

		if(ropeBridge[k].type == 1)
			ScaleVector(&rotvel[0],0.075);
		platform1 = TagToFirstPlatform(ropeBridge[k].minTag);
		AddVector(&platform1->rotvel,&platform1->rotvel,&rotvel[0]);

		if(ropeBridge[k].type == 1)
			ScaleVector(&rotvel[ropeBridge[k].maxTag - ropeBridge[k].minTag],0.075);
		platform1 = TagToFirstPlatform(ropeBridge[k].maxTag);
		AddVector(&platform1->rotvel,&platform1->rotvel,&rotvel[ropeBridge[k].maxTag - ropeBridge[k].minTag]);

		for(x = ropeBridge[k].minTag + 1;x <= ropeBridge[k].maxTag - 1;x++)
		{
			platform1 = TagToFirstPlatform(x);
			if(platform1->noActorInContactCount > 1)
				AddVector(&platform1->rotvel,&platform1->rotvel,&rotvel[x-ropeBridge[k].minTag]);
		}	
 	}
}

/*	--------------------------------------------------------------------------------
	Function 	: LoadMeshIntoPlatform
	Purpose 	: loads a .x file and adds it to the specified frame.
	Parameters 	: filename
	Returns 	: pointer to frame
	Info 		:
*/
BOOL LoadMeshIntoPlatform(PLATFORM *platform,LPDIRECT3DRMFRAME parent,char *fName)
{
    LPDIRECT3DRMMESHBUILDER mesh_builder = NULL;
	int rval = 0;
	PLATFORMID			*platformId;
	PLATFORM			*tempPlat;
//check to see if platform is already loaded

	for(platformId = platformIdList.head.next;platformId != &platformIdList.head;platformId = platformId->next)
	{
		if(strcmp(platformId->name, fName) == 0)
		{
			dprintf("LoadMeshIntoPlatform: Platform %s already exists\n", fName);
//			tempPlat = platformId->platform;

			platform->parent = parent;
		    if (FAILED(g_lpD3DRM->CreateFrame(parent, &(platform->frame))))
		        goto generic_error;

		    if (FAILED(platform->frame->AddVisual(platformId->visual)))
		        goto generic_error;

			platform->id = platformId;
			platform->visible = TRUE;
			return TRUE;
		}
	}
	


    if (FAILED(g_lpD3DRM->CreateMeshBuilder(&mesh_builder)))
        goto generic_error;

    rval = mesh_builder->Load(fName, NULL, D3DRMLOAD_FROMFILE, LoadTextures, NULL);
    if (rval != D3DRM_OK) 
	{
        Msg("Failed to load file [%s]\n",fName);
        goto ret_with_error;
    }

	mesh_builder->SetPerspective(TRUE);


	// create an child frame from the parent
	platform->parent = parent;
    if (FAILED(g_lpD3DRM->CreateFrame(parent, &(platform->frame))))
        goto generic_error;

     // add the loaded mesh into the frame
    if (FAILED(platform->frame->AddVisual((LPDIRECT3DRMVISUAL)mesh_builder)))
        goto generic_error;


	platformId = (PLATFORMID *)MyCalloc(1, sizeof(PLATFORMID));
	AddPlatformId(platformId);
	sprintf(platformId->name, "%s", fName);

	platformId->visual = (LPDIRECT3DRMVISUAL)mesh_builder;

	platform->visible = TRUE;
	platform->id = platformId;

	platformId->faceCount = mesh_builder->GetFaceCount();
	mesh_builder->GetFaces(&(platformId->faceArray));

    RELEASE(mesh_builder);

	return TRUE;
generic_error:
    Msg("A failure occured while building the scene.\n");
ret_with_error:
    RELEASE(mesh_builder);
    return FALSE;

}
