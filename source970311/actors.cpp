/*
 *		Project:		Oddballs
 *		File:			actors.cpp
 *		Description:	handles actor movement and list
 */
					  

#include "oddballs.h"
#include "actors.h"
#include "maths.h"
#include "collision.h"
#include "anim.h"
#include "nmebehaviour.h"
#include "hand.h"
#include "cam.h"
#include "platforms.h"
#include "collectables.h"
#include "personal.h"
#include "sound.h"
#include "bullets.h"

char inertia = YES;

/*	--------------------------------------------------------------------------------
	Function 	: InitActorList()
	Purpose 	: initialises the linked list of actors
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void InitActorList()
{
	actorList.numEntries = 0;
	actorList.head.next = actorList.head.prev = &actorList.head;
}

/*	--------------------------------------------------------------------------------
	Function 	: AddActor()
	Purpose 	: adds an actor to the linked list
	Parameters 	: pointer to actor
	Returns 	: none
	Info 		:
*/
void AddActor(ACTOR *actor)
{
	int	j;

	if(actor->listPointer == 0)
	{
		actorList.numEntries++;
		actor->listPointer = (ACTORLISTENTRY *)MyMalloc(sizeof(ACTORLISTENTRY));
		actor->listPointer->prev = &actorList.head;
		actor->listPointer->next = actorList.head.next;
		actorList.head.next->prev = actor->listPointer;
		actorList.head.next = actor->listPointer;
		actor->listPointer->actor = actor;
		for(j = 0;j < NUMACTORPLANES;j++)
		{
			actor->plane[j] = NULL;
			actor->contactTime[j] = 0;
		}
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: CalcMaxActorRadius()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void CalcMaxActorRadius(ACTOR *actor)
{
	int	j;
	double	max = actor->radius,temp;

	for(j = 0;j < actor->numExtraCollisionSpheres;j++)
	{
		temp = Magnitude(&actor->collisionSphere[j].offset);
		temp += actor->collisionSphere[j].radius;
		if(temp > max)
			max = temp;
	}
	actor->maxRadius = max;
}

/*	--------------------------------------------------------------------------------
	Function 	: SubActor()
	Purpose 	: removes an actor from the linked list
	Parameters 	: pointer to actor
	Returns 	: none
	Info 		:
*/
void SubActor(ACTOR *actor)
{
	if(actor->listPointer != 0)
	{
		if(actor->frame != NULL)
			RemoveVisuals(actor->frame);
		actor->listPointer->prev->next = actor->listPointer->next;
		actor->listPointer->next->prev = actor->listPointer->prev;
		MyFree((void **)&actor->listPointer);
		actorList.numEntries--;
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: TransformActor()
	Purpose 	: applies translation and rotations to an actor
	Parameters 	: actor to update
	Returns 	: none
	Info 		:
*/
void TransformActor(ACTOR *actor)
{
	actor->frame->AddRotation(D3DRMCOMBINE_REPLACE, D3DVAL(0), D3DVAL(1), D3DVAL(0), D3DVAL(actor->rot.v[Y]));
	actor->frame->AddRotation(D3DRMCOMBINE_BEFORE,  D3DVAL(1), D3DVAL(0), D3DVAL(0), D3DVAL(actor->rot.v[X]));
	actor->frame->AddRotation(D3DRMCOMBINE_BEFORE,  D3DVAL(0), D3DVAL(0), D3DVAL(1), D3DVAL(actor->rot.v[Z]));

	if(actor == &ball)
		actor->frame->AddScale(D3DRMCOMBINE_BEFORE,D3DVAL(actor->scale.v[X]*actor->radius/BALL_NORMAL_RADIUS),D3DVAL(actor->scale.v[Y]*actor->radius/BALL_NORMAL_RADIUS),D3DVAL(actor->scale.v[Z]*actor->radius/BALL_NORMAL_RADIUS));

	actor->frame->AddScale(D3DRMCOMBINE_BEFORE,D3DVAL(actor->scale.v[X]),D3DVAL(actor->scale.v[Y]),D3DVAL(actor->scale.v[Z]));

	actor->frame->SetPosition(g_lpScene, D3DVAL(actor->pos.v[X]), D3DVAL(actor->pos.v[Y]), D3DVAL(actor->pos.v[Z]));
}


/*	--------------------------------------------------------------------------------
	Function 	: TransformEnemySpheres()
	Purpose 	: applies translation and rotations to an actor
	Parameters 	: actor to update
	Returns 	: none
	Info 		:
*/
void TransformEnemySpheres(ENEMY_TYPE *enemy)
{
	LPDIRECT3DRMFRAME	frame;
	VECTOR				*tempPos;
	ACTOR				*actor = &enemy->actor;
	D3DVECTOR			tempVect;
	int					x = 0;

	while(enemy->sphere[x])
	{
		frame = enemy->sphere[x]->frame;
		tempPos = &enemy->sphere[x]->offset;
		TRY_MYD3DRM(frame->SetPosition(actor->frame, D3DVAL(tempPos->v[X]), D3DVAL(tempPos->v[Y]), D3DVAL(tempPos->v[Z])));
		x++;
	}
}


/*	--------------------------------------------------------------------------------
	Function 	: AddConveyorSpeed
	Purpose 	: adds (rotated) conveyor belt speed to a vector
	Parameters 	: vector, platform
	Returns 	: none
	Info 		:
*/
void AddConveyorSpeed(VECTOR *vector,PLATFORM *platform)
{
	double speed;
	VECTOR tempVect;

	SetVector(&tempVect,&platform->conveyorSpeed);
	if(platform->rot.v[Y])
		RotateVector2D(&tempVect,&tempVect,platform->rot.v[Y]);
	else if(platform->rot.v[X])
		RotateVector2DXYZ(&tempVect,&tempVect,-platform->rot.v[X],X);
	else if(platform->rot.v[Z])
		RotateVector2DXYZ(&tempVect,&tempVect,-platform->rot.v[Z],Z);

	AddVector(vector,vector,&tempVect);
}

/*	--------------------------------------------------------------------------------
	Function 	: RollActor()
	Purpose 	: rolls an actor
	Parameters 	: actor to roll
	Returns 	: none
	Info 		:
*/
void RollActor(ACTOR *actor)
{
	double	speed,x,z,a,b,angle;

	if((actor->platform) && (actor->platform->flags & CONVEYOR))
	{
		a = actor->platform->conveyorSpeed.v[X];
		b = actor->platform->conveyorSpeed.v[Z];

		angle = -actor->platform->rot.v[Y];
		x = (cos(angle) * a) - (sin(angle) * b);
		z = (cos(angle) * b) + (sin(angle) * a);

		x -= actor->pos.v[X] - actor->oldpos.v[X];
		z -= actor->pos.v[Z] - actor->oldpos.v[Z];
	}
	else
	{
		x = actor->oldpos.v[X] - actor->pos.v[X];
		z = actor->oldpos.v[Z] - actor->pos.v[Z];
	}

	speed = sqrt(x*x + z*z);
	actor->rot.v[X] = Aabs(actor->rot.v[X] + speed/actor->radius);
	if((speed > 0.1) || ((actor == &ball) && (hand.action == HAND_BALLWALK) && (speed)))
		actor->rotaim.v[Y] = Aabs(PI + PI/2 + atan2(-z,x));
}

/*	--------------------------------------------------------------------------------
	Function 	: UpdateActor()
	Purpose 	: updates an actor - movement and collision
	Parameters 	: actor to update
	Returns 	: none
	Info 		:
*/
void UpdateActor(ACTOR *actor)
{
	int					i,j,k;
	DWORD				numVertices;
	double				dist,speed,angle,xdist,zdist,num = 0,ang,d[3];
	VECTOR				nearestPoint,vel[NUMACTORPLANES];
	LPDIRECT3DRMFACE	face;
	D3DVECTOR			vert[4];
	D3DVECTOR			vNormal[4];
	double				actorGravity = gravity;
	PLATFORM			*platform,*plat;
	VECTOR				tempVect;
	TELEPORT			*teleport;
	
//	if(actor->status == INACTIVE)
//		return;

	if(actor->type == BULLET)
	{
		if(actor->lifetime > 0)
		{
			actor->lifetime--;
			if(actor->lifetime == 0)
			{
				if(actor->destroyCallBack)
				{
					actor->destroyCallBack(actor);
				}
				g_lpScene->DeleteChild(actor->frame);
				SubActor(actor);
				MyFree((void **)&actor);
				return;
			}
		}
	}

	if(actor->maintainCallBack)
	{
		if(actor->maintainCallBack(actor))
		{
			if(actor->destroyCallBack)
			{
				actor->destroyCallBack(actor);
			}
			g_lpScene->DeleteChild(actor->frame);
			SubActor(actor);
			MyFree((void **)&actor);
			return;

		}
	}


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/*
	if((actor->enemyNum != -1) && (actor->pos.v[Y] < -2))
	{
		actor->status = INACTIVE;
		g_lpScene->DeleteChild(actor->frame);
		SubActor(actor);
		return;
	}
*/
	if(actor->status == DEAD)
	{
		if(actor->reachedEndOfAnimation == TRUE)
		{
			actor->status = INACTIVE;
			g_lpScene->DeleteChild(actor->frame);
			CreateStarCircle(&actor->pos, 6);
			SubActor(actor);
			return;
		}
	}

	SetVector(&actor->oldVel, &actor->vel);
	if(actor->flags & LO_GRAVITY)
		actorGravity /= 2;

//handle changes in size
	if(actor->radiusAim != actor->radius)
		actor->radius += (actor->radiusAim - actor->radius) / 2;	

	speed = Magnitude2D(&actor->vel);
	if(speed > actor->maxSpeed)
	{
		actor->vel.v[X] *= actor->maxSpeed/speed;
		actor->vel.v[Z] *= actor->maxSpeed/speed;
	}

	if((actor->platform != NULL) && (actor->platform->flags & CONVEYOR))
		AddConveyorSpeed(&actor->pos,actor->platform);

	if((actor->oldPlatform != NULL) && (actor->platform != actor->oldPlatform) && (actor->oldPlatform->flags & CONVEYOR))
		AddConveyorSpeed(&actor->vel,actor->oldPlatform);

	if(actor->flags & ROLLING)
	{
		if(actor == &ball)
		{
			if(actor->reachedEndOfAnimation == TRUE)
			{
				if(handControl.catPlat == NULL)
					RollActor(actor);
			}
			else
			{
				SetVector(&ball.rot, &zero);
				SetVector(&ball.rotaim, &zero);
			}
		}
		else
			RollActor(actor);
	}

	if(actor->platform != NULL)
		platform = actor->platform;
	else if(actor->oldPlatform != NULL)
		platform = actor->oldPlatform;
	else 
		platform = NULL;

	if(platform != NULL)
	{
		if(platform->pos.v[Y] - platform->oldpos.v[Y] > -1)
		{
			AddVector(&actor->pos,&actor->pos,&platform->pos);
			SubVector(&actor->pos,&actor->pos,&platform->oldpos);

			plat = platform->parentPlatform;
			while(plat != NULL)
			{
				AddVector(&actor->pos,&actor->pos,&plat->pos);
				SubVector(&actor->pos,&actor->pos,&plat->oldpos);
				plat = plat->parentPlatform;
			}

			if((platform->catapult) || (platform->flags & GO_FORWARDS))
			{
				if(platform->rot.v[Y] != platform->oldrot.v[Y])
				{
					RotateVector2D(&actor->vel,&actor->vel,platform->rot.v[Y] - platform->oldrot.v[Y]);
					if((actor->rAim.v[Y] != 0) && (actor->rAim.v[X] == 0) && (actor->rAim.v[Z] == 0) && ((actor->flags & ROLLING) == 0))				
					{
						actor->rot.v[Y] += platform->rot.v[Y] - platform->oldrot.v[Y];
						actor->rotaim.v[Y] += platform->rot.v[Y] - platform->oldrot.v[Y];
					}
					d[0] = actor->pos.v[X] - platform->pos.v[X];
					d[1] = actor->pos.v[Z] - platform->pos.v[Z];

					dist = sqrt(d[0]*d[0] + d[1]*d[1]);
					ang = atan2(d[1],d[0]);

  					ang -= platform->rot.v[Y] - platform->oldrot.v[Y];

					ang = Aabs(ang);
					d[0] = dist*cos(ang);
					d[1] = dist*sin(ang);
					actor->pos.v[X] = d[0] + platform->pos.v[X];
					actor->pos.v[Z] = d[1] + platform->pos.v[Z];
				}
				if(platform->rot.v[X] != platform->oldrot.v[X])
				{
					SubVector(&tempVect,&actor->pos,&platform->pos);

					RotateVector2DXYZ(&tempVect,&tempVect,-platform->rot.v[Y],Y);

					d[0] = tempVect.v[Y];
					d[1] = tempVect.v[Z];

					dist = sqrt(d[0]*d[0] + d[1]*d[1]);
					ang = atan2(d[1],d[0]);

					ang += platform->rot.v[X] - platform->oldrot.v[X];

					ang = Aabs(ang);
					d[0] = dist*cos(ang);
					d[1] = dist*sin(ang);

					tempVect.v[Y] = d[0];
					tempVect.v[Z] = d[1];

					RotateVector2DXYZ(&tempVect,&tempVect,platform->rot.v[Y],Y);

					AddVector(&actor->pos,&platform->pos,&tempVect);
				}
				if(platform->rot.v[Z] != platform->oldrot.v[Z])
				{
					SubVector(&tempVect,&actor->pos,&platform->pos);

					RotateVector2DXYZ(&tempVect,&tempVect,-platform->rot.v[Y],Y);

					d[0] = tempVect.v[X];
					d[1] = tempVect.v[Y];

					dist = sqrt(d[0]*d[0] + d[1]*d[1]);
					ang = atan2(d[1],d[0]);

					ang += platform->rot.v[Z] - platform->oldrot.v[Z];

					ang = Aabs(ang);
					d[0] = dist*cos(ang);
					d[1] = dist*sin(ang);

					tempVect.v[X] = d[0];
					tempVect.v[Y] = d[1];

					RotateVector2DXYZ(&tempVect,&tempVect,platform->rot.v[Y],Y);

					AddVector(&actor->pos,&platform->pos,&tempVect);
				}
			}
			else if(platform->flags & TILTING)
			{
				if(platform->rot.v[Y] != platform->oldrot.v[Y])
				{
					RotateVector2D(&actor->vel,&actor->vel,platform->rot.v[Y] - platform->oldrot.v[Y]);
					if((actor->rAim.v[Y] != 0) && (actor->rAim.v[Z] == 0) && ((actor->flags & ROLLING) == 0))				
					{
						actor->rot.v[Y] += platform->rot.v[Y] - platform->oldrot.v[Y];
						actor->rotaim.v[Y] += platform->rot.v[Y] - platform->oldrot.v[Y];
					}
				}

				j = 0;
				k = 10;
				for(i = X;i <= Z;i++)
				{
					if(platform->rot.v[i] == platform->oldrot.v[i])
						d[j++] = actor->pos.v[i] - platform->pos.v[i];
					else
						k = i;
				}
				if(k == Y)
				{
					dist = sqrt(d[0]*d[0] + d[1]*d[1]);
					ang = atan2(d[1],d[0]);

  					ang -= platform->rot.v[Y] - platform->oldrot.v[Y];

					ang = Aabs(ang);
					d[0] = dist*cos(ang);
					d[1] = dist*sin(ang);
					j = 0;
					for(i = X;i <= Z;i++)
					{
						if(platform->rot.v[i] == platform->oldrot.v[i])
							actor->pos.v[i] = d[j++] + platform->pos.v[i];
					}
				}
			}
		}
	}

	if(actor->teleport)
	{
		for(j = 0;j < 1;j++)
		{
			SetVector(&tempVect, &actor->pos);
			dist = (rand() % 30) - 15;
			dist /= 10;
			tempVect.v[X] += dist;
			dist = (rand() % 30) - 15;
			dist /= 10;
			tempVect.v[Y] += dist;
			dist = (rand() % 30) - 15;
			dist /= 10;
			tempVect.v[Z] += dist;
			AddDebris(SPARK, &tempVect, &zero);
		}

		if(actor->teleportCounter == 0)
		{
			teleport = actor->teleport;
			actor->teleportCounter = -teleport->appearDelay;
			if(actor->teleportCounter == 0)
			{
				actor->scale.v[X] = actor->scale.v[Y] = actor->scale.v[Z] = 1;
				actor->teleport = NULL;
			}
			if(teleport->platform)
			{
				if(teleport->platform->rot.v[Y])
					RotateVector2D(&tempVect,&teleport->destination,teleport->platform->rot.v[Y]);
				else if(teleport->platform->rot.v[X])
					RotateVector2DXYZ(&tempVect,&teleport->destination,-teleport->platform->rot.v[X],X);
				else if(teleport->platform->rot.v[Z])
					RotateVector2DXYZ(&tempVect,&teleport->destination,-teleport->platform->rot.v[Z],Z);
				AddVector(&actor->pos,&tempVect,&teleport->platform->pos);
				if(teleport->flags & TELEPORT_GOTOPLATFORM)
				{
					actor->oldPlatform = actor->platform = teleport->platform;
					actor->pos.v[Y] = teleport->platform->pos.v[Y] + (teleport->platform->size.v[Y] * teleport->platform->scale.v[Y])/teleport->platform->initScale.v[Y];
				}
			}
			else
				SetVector(&actor->pos,&teleport->destination);
			if(teleport->flags & TELEPORT_ZEROVEL)
				SetVector(&actor->vel,&zero);
			actor->pos.v[Y] += actor->radius;
			SetVector(&actor->oldpos,&actor->pos);
		}
		else if(actor->teleportCounter > 0)
		{
			actor->teleportCounter--;
			actor->scale.v[X] = actor->scale.v[Y] = actor->scale.v[Z] = ((double)actor->teleportCounter)/actor->teleport->disappearDelay;
			return;
		}
		else
		{
			actor->teleportCounter++;
			actor->scale.v[X] = actor->scale.v[Y] = actor->scale.v[Z] = ((double)(actor->teleport->appearDelay + actor->teleportCounter))/actor->teleport->appearDelay;
			if(actor->teleportCounter == 0)
			{
				actor->scale.v[X] = actor->scale.v[Y] = actor->scale.v[Z] = 1;
				actor->teleport = NULL;
			}
//			else
//				return;
		}
	}

	if((handControl.catPlat) && (handControl.catPlat->catapult->catCounter > 0))
	{
		if(hand.action == HAND_JOINED)
			SetVector(&ball.vel,&zero);
		else
			SetVector(&hand.vel,&zero);
	}

	if((actor == &hand) && (hand.onGround) && (hand.action != HAND_HURT) && (handControl.currentActionTime > 2) && (hand.touchingElectric == NO))
	{
//		scenicItems[0].frame->SetPosition(g_lpWorld,hand.pos.v[X],hand.pos.v[Y],hand.pos.v[Z]);
		handControl.safePlat = hand.platform;
		if(hand.platform)
			SubVector(&handControl.safePos,&hand.pos,&hand.platform->pos);
		else
			SetVector(&handControl.safePos,&hand.pos);
	}

	for(i = X;i <= Z;i++)
	{
		if(actor->type != BULLET)
		{
			actor->oldpos.v[i] = actor->pos.v[i];
			actor->pos.v[i] += actor->vel.v[i];
		}

		if((actor->plane[0] == NULL) && (actor->type != BULLET) && (actor->type != CAMERA))
			actor->vel.v[i] *= 1 - ((1 - actor->normInertia)/4);
		else
		{
			if(i == Y)
				actor->vel.v[i] *= actor->normInertia;
			else
			{
				if(actor->plane[0])
					actor->vel.v[i] *= 1 - ((1 - actor->velInertia)/(terrain[actor->plane[0]->terrain].slidy));
				else
					actor->vel.v[i] *= actor->velInertia;
			}
		}

		if(actor->type == BULLET)
		{
			actor->oldpos.v[i] = actor->pos.v[i];
			actor->pos.v[i] += actor->vel.v[i];
		}


	//handle rotations
		actor->rot.v[i] += actor->rotvel.v[i];
		actor->rot.v[i] = Aabs(actor->rot.v[i]);

		if(actor->rAim.v[i] == ON)
		{
			actor->rotaim.v[i] = Aabs(actor->rotaim.v[i]);

			if(FindShortestAngle(actor->rot.v[i],actor->rotaim.v[i]) <= 0.01)
				actor->rot.v[i] = actor->rotaim.v[i];
			else if(actor->rot.v[i] < actor->rotaim.v[i])
			{
				if(actor->rotaim.v[i] - actor->rot.v[i] > PI)
					actor->rot.v[i] -= Min(actor->turnSpeed,FindShortestAngle(actor->rot.v[i], actor->rotaim.v[i])/2 );
				else		
					actor->rot.v[i] += Min(actor->turnSpeed,FindShortestAngle(actor->rot.v[i], actor->rotaim.v[i])/2 );
			}
			else if(actor->rot.v[i] > actor->rotaim.v[i])
			{
				if(actor->rot.v[i] - actor->rotaim.v[i] > PI)
					actor->rot.v[i] += Min(actor->turnSpeed,FindShortestAngle(actor->rot.v[i], actor->rotaim.v[i])/2 );
				else		
					actor->rot.v[i] -= Min(actor->turnSpeed,FindShortestAngle(actor->rot.v[i], actor->rotaim.v[i])/2 );
			}
		}

		actor->rotvel.v[i] *= actor->rotInertia;
		if(inertia == NO)
			actor->rotvel.v[i] = 0;
	}	

	if(actor->flags & COLLISION)
	{
		actor->speed = Magnitude(&actor->vel);

		actor->oldPlatform = actor->platform;
		actor->platform = NULL;
		actor->oldTouchPlatform = actor->touchPlatform;
		actor->touchPlatform = NULL;
		actor->touchingElectric = NO;
  
		CheckCollisions(actor);
		CheckActorCollisions(actor);

		if(actor == &hand)
		{
			if((handControl.catPlat) && (handControl.catPlat->catapult->catCounter > 0))
			{
				if(hand.action == HAND_JOINED)
					ball.platform = hand.platform = handControl.catPlat;
				else
					hand.platform = handControl.catPlat;
			}

			if(handControl.sitPlat)
				hand.platform = handControl.sitPlat;
		}

//		actor->speed = Magnitude(&actor->vel);

		for(j = 0;j < NUMACTORPLANES;j++)
			if((actor->plane[j] != NULL) && (actor->contactTime[j] != 0) && (actor->rolling[j] == YES))
				break;
	}
	if((actor->flags & GRAVITY) && (inertia == YES))
	{
		if((j == NUMACTORPLANES) || ((actor->flags & COLLISION) == 0))
	 		actor->vel.v[Y] = Max(actor->vel.v[Y] - actorGravity,-TERMINAL_VELOCITY);
		else
		{
			for(j = 0;j < NUMACTORPLANES;j++)
			{
//				if((actor->plane[j] != NULL) && (((actor->platform) && (actor->platform->flags & TILTING)) || (actor->plane[0]->normal.v[Y] <= actor->slideLimit) || (actor->vel.v[Y] > 0)) && (actor->contactTime[j] != 0) && (actor->rolling[j] == YES))
//				if((actor->plane[j] != NULL) && ((actor->plane[j]->normal.v[Y] <= actor->slideLimit) || (actor->vel.v[Y] > 0) || (terrain[actor->plane[j]->terrain].slidy > 1)) && (actor->contactTime[j] != 0) && (actor->rolling[j] == YES))
				if((actor->plane[j] != NULL)
				 && ((actor->plane[j]->normal.v[Y] <= 1 - ((1 - actor->slideLimit)/(terrain[actor->plane[j]->terrain].slidy*terrain[actor->plane[j]->terrain].slidy))) || (actor->vel.v[Y] > 0)) && (actor->contactTime[j] != 0) && (actor->rolling[j] == YES))
				{
					dist = -(actor->pos.v[X] * actor->plane[j]->normal.v[X] + (actor->pos.v[Y] - actorGravity) * actor->plane[j]->normal.v[Y] + actor->pos.v[Z] * actor->plane[j]->normal.v[Z] + actor->plane[j]->J);

					nearestPoint.v[X] = actor->pos.v[X] + dist*actor->plane[j]->normal.v[X];
					nearestPoint.v[Y] = (actor->pos.v[Y] - actorGravity) + dist*actor->plane[j]->normal.v[Y];
					nearestPoint.v[Z] = actor->pos.v[Z] + dist*actor->plane[j]->normal.v[Z];

					SubVector(&vel[j],&nearestPoint,&(actor->nearestPoint[j]));
					num++;
				}
				else
					SetVector(&vel[j],&zero);
			}
			for(j = 1;j < num;j++)
				AddVector(&vel[0],&vel[0],&vel[j]);

			if(num != 0)
				AddOneScaledVector(&actor->vel,&actor->vel,&vel[0],1/num);
		}
	}
	speed = Magnitude2D(&actor->vel);
	if(speed > actor->maxSpeed)
	{
		actor->vel.v[X] *= actor->maxSpeed/speed;
		actor->vel.v[Z] *= actor->maxSpeed/speed;
	}
//	else if((actor->flags & COLLISION) && (speed < actor->stopSpeed) && ((speed == 0) || (actor->velInertia < actor->normInertia) || (((actor->plane[0]) && (terrain[actor->plane[0]->terrain].slidy != 1) && (actor->flags & DECELERATE) == 0))))
	else if((actor->flags & COLLISION) && ((actor->velInertia < actor->normInertia) || ((actor->flags & DECELERATE) == 0)))
	{
		if((actor->plane[0] != NULL) && (speed < actor->stopSpeed/max(1,terrain[actor->plane[0]->terrain].slidy)) && (actor->plane[0]->normal.v[Y] > 1 - ((1 - actor->slideLimit)/(terrain[actor->plane[0]->terrain].slidy*terrain[actor->plane[0]->terrain].slidy))) && (actor->rolling[0] == YES))
		{
			SubVector(&actor->vel,&zero,(&actor->plane[0]->normal));
			ScaleVector(&actor->vel,gravity);
		}
		else if((speed < actor->stopSpeed * 1.5) && ((joystick.magnitude == 0) || (actor != &hand) || ((hand.action == HAND_JOINED) && (actor != &ball))))
		{
			ScaleVector(&actor->vel,0.95);
		}
	}

	if(actor->vel.v[Y] > TERMINAL_VELOCITY)
		actor->vel.v[Y] = TERMINAL_VELOCITY;
	actor->speed = Magnitude(&actor->vel);


/////////////////////////////////////////////////////////////////////////////////////////


	actor->onGround = FALSE;
	i = 0;
	while((actor->plane[i] != NULL) && (i < NUMACTORPLANES))
	{
		if(actor->plane[i]->normal.v[Y] < 0.5)
			i++;
		else
		{
			actor->onGround = TRUE;
			break;
		}
	}


/*
	if(actor->plane[0])
		actor->onGround = TRUE;
	else
		actor->onGround = FALSE;
*/
	if((actor->flags & DECELERATE) && ((actor->plane[0] == NULL) || (terrain[actor->plane[0]->terrain].slidy <= 1)))
		actor->velInertia *= 0.99;

	if(inertia == NO)
	{
		SetVector(&actor->vel,&zero);
		actor->speed = 0;
	}

	if(actor->flags & COLLISION)
	{
		if((actor->pos.v[Y] <= 2.78) && (actor->oldpos.v[Y] > 2.78) && (actor->oldVel.v[Y] < -0.5))
		{
			SetVector(&tempVect, &actor->pos);
			tempVect.v[Y] = 1.78;
			CreateSplashCircle(&tempVect, 4, 0);
			PlaySample(SFX_SPLASH1, 0, &actor->pos, 100);

		}
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: AccelerateActor2D
	Purpose 	: increase the actor's speed
	Parameters 	: speed to add
	Returns 	: none
	Info 		:
*/
void AccelerateActor2D(ACTOR *actor,double speed)
{
	actor->vel.v[X] += speed * sin(actor->rot.v[Y]);
	actor->vel.v[Z] += speed * cos(actor->rot.v[Y]);
	actor->speed = Magnitude(&actor->vel);
	speed = Magnitude2D(&actor->vel);
	if(speed > actor->maxSpeed)
	{
		actor->vel.v[X] *= actor->maxSpeed/speed;
		actor->vel.v[Z] *= actor->maxSpeed/speed;
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: AccelerateActor
	Purpose 	: increase the actor's speed
	Parameters 	: speed to add
	Returns 	: none
	Info 		:
*/
void AccelerateActor(ACTOR *actor,double speed)
{
	VECTOR tempVect;

	tempVect.v[Z] = -1;
	tempVect.v[Y] = tempVect.v[X] = 0;

	RotateVector2DXYZ(&tempVect, &tempVect, -actor->rot.v[X], X);
	RotateVector2D(&tempVect, &tempVect, actor->rot.v[Y]);

	ScaleVector(&tempVect, speed);
	actor->vel.v[X] += tempVect.v[X];
	actor->vel.v[Y] += tempVect.v[Y];
	actor->vel.v[Z] += tempVect.v[Z];

	speed = Magnitude(&actor->vel);
	if(speed > actor->maxSpeed)
	{
		actor->vel.v[X] *= actor->maxSpeed/speed;
		actor->vel.v[Y] *= actor->maxSpeed/speed;
		actor->vel.v[Z] *= actor->maxSpeed/speed;
	}
	actor->speed = Magnitude(&actor->vel);
}


/*	--------------------------------------------------------------------------------
	Function 	: AccelerateActorFromActor
	Purpose 	: increase the actor's speed
	Parameters 	: speed to add
	Returns 	: none
	Info 		:
*/
void AccelerateActorFromActor(ACTOR *sourceActor, ACTOR *targetActor, double speed)
{
	targetActor->vel.v[X] += speed * sin(sourceActor->rot.v[Y]);
	targetActor->vel.v[Z] += speed * cos(sourceActor->rot.v[Y]);
	targetActor->speed = Magnitude(&targetActor->vel);
}

/*	--------------------------------------------------------------------------------
	Function 	: ActorAddVelocity
	Purpose 	: adds to an actor's velocity 
	Parameters 	: vector to add
	Returns 	: none
	Info 		:
*/
void ActorAddVelocity(ACTOR *actor,VECTOR *vel)
{
	actor->vel.v[X] += vel->v[X];
	actor->vel.v[Y] += vel->v[Y];
	actor->vel.v[Z] += vel->v[Z];
}

/*	--------------------------------------------------------------------------------
	Function 	: IsActorFacingActor()
	Purpose 	: check to see if an actor is facing another actor
	Parameters 	: pointers to actor and target, angular range
	Returns 	: BOOL
	Info 		:
*/
BOOL IsActorFacingActor(ACTOR *actor, ACTOR *target, double range)
{
	VECTOR tempVect;
	double val;

	SubVector(&tempVect, &actor->pos, &target->pos);
	val = Aabs(atan2(tempVect.v[X], tempVect.v[Z]));
	if(FindShortestAngle(actor->rot.v[Y],val) < range)
		return TRUE;

	return FALSE;
}

/*	--------------------------------------------------------------------------------
	Function 	: IsActorFacingPoint()
	Purpose 	: check to see if an actor is facing a point
	Parameters 	: pointers to actor and target point, angular range
	Returns 	: BOOL
	Info 		:
*/
BOOL IsActorFacingPoint(ACTOR *actor, VECTOR *target, double range)
{
	VECTOR tempVect;
	double val;
	double temp;

	SubVector(&tempVect, &actor->pos, target);
	val = Aabs(atan2(tempVect.v[X], tempVect.v[Z]));
//	temp = Aabs(actor->rot.v[Y] + PI);
	if(fabs(actor->rot.v[Y] - val) < range)
		return TRUE;

	return FALSE;
}


/*	--------------------------------------------------------------------------------
	Function 	: AreActorsTouching()
	Purpose 	: check to see if an actor is touching another actor
	Parameters 	: pointers to actor and target
	Returns 	: BOOL
	Info 		:
*/
BOOL AreActorsTouching(ACTOR *actor1, ACTOR *actor2)
{
	VECTOR tempVect;
				  
	SubVector(&tempVect, &actor1->pos, &actor2->pos);
	if(MagnitudeSquared(&tempVect) <= (actor1->radius + actor2->radius + 0.3)*(actor1->radius + actor2->radius + 0.3))
		return TRUE;

	return FALSE;
}

/*	--------------------------------------------------------------------------------
	Function 	: RotateActor()
	Purpose 	: adds to an actor's rotational velocities
	Parameters 	: rotation vector
	Returns 	: none
	Info 		:
*/
void RotateActor(ACTOR *actor,VECTOR *rot)
{
	actor->rotvel.v[X] += rot->v[X];
	actor->rotvel.v[Y] += rot->v[Y];
	actor->rotvel.v[Z] += rot->v[Z];
}

/*	--------------------------------------------------------------------------------
	Function 	: UpdateActorPositionDependencies()
	Purpose 	: to UpdateActorPositionDependencies()
	Parameters 	: nun
	Returns 	: nun
	Info 		:
*/
void UpdateActorPositionDependencies()
{
	double	val,dist;
	VECTOR	temp,temp2,temp3;
	double	factor,accel = 0.12,vel;
	static	double max = 0;
	static	VECTOR lastVel;
	float	camSpeed = 2;
	float	velMultiplier;

	static	float lookOffset = 0;
	static	float lookOffsetVel = 0;
	static	float lastHandYRot;

//little garib to check if hand is on an edge
	SetVector(&handControl.verticalEdgeChecker.vel, &zero);
	SetVector(&temp, &zero);
	temp.v[Z] = -0.5;
	RotateVector2D(&temp, &temp, hand.rot.v[Y]);
	AddVector(&handControl.verticalEdgeChecker.pos, &hand.pos, &temp);

//little garibs to check if hand is to cling onto an edge
	SetVector(&handControl.frontProbe[0].vel, &zero);
	SetVector(&temp, &zero);
	temp.v[Z] = -1.8;
	temp.v[Y] = 1.6;
	RotateVector2D(&temp, &temp, hand.rot.v[Y] + 0.5);
	AddVector(&handControl.frontProbe[0].pos, &hand.pos, &temp);

	SetVector(&handControl.frontProbe[1].vel, &zero);
	SetVector(&temp, &zero);
	temp.v[Z] = -1.8;
	temp.v[Y] = 1.6;
	RotateVector2D(&temp, &temp, hand.rot.v[Y] - 0.5);
	AddVector(&handControl.frontProbe[1].pos, &hand.pos, &temp);
	
// hand standing on top of ball
	if(hand.action == HAND_BALLWALK)
	{
		SetVector2D(&hand.pos, &ball.pos);
		float temp = ball.radius + hand.radius - 0.2;
		if((hand.pos.v[Y] - ball.pos.v[Y]) < temp)
		{
			hand.pos.v[Y] = ball.pos.v[Y] + temp;
			if(ball.vel.v[Y] > 0)
				hand.vel.v[Y] = ball.vel.v[Y] * 1.2;
			else
				hand.vel.v[Y] = 0;
		}
	}


//ball being held by an enemy
	if(ballControl.status == BALL_HELD)
	{
		RotateVector2D(&temp2, &ballControl.holdingOffset, ballControl.holdingActor->rot.v[Y]);

		AddVector(&temp, &ballControl.holdingActor->pos, &temp2);

		SubVector(&temp2, &ball.pos, &temp);
		ScaleVector(&temp2, 0.5);
		SubVector(&ball.pos, &ball.pos, &temp2);

//		SetVector(&ball.pos, &temp);
		SetVector(&ball.vel, &ballControl.holdingActor->vel);
	}
//hand being held by an enemy
	if(handControl.status == HAND_HELD)
	{
		DisableControl(2);				//mustnt be able to move
		handControl.newActionDelay = 2;	//or get new actions while being held

		hand.rotaim.v[Y] = handControl.holdingActor->rot.v[Y];
		RotateVector2D(&temp2, &handControl.holdingOffset, hand.rotaim.v[Y]);

		AddVector(&temp, &handControl.holdingActor->pos, &temp2);

		SubVector(&temp2, &hand.pos, &temp);
		ScaleVector(&temp2, 0.5);
		SubVector(&hand.pos, &hand.pos, &temp2);

		SetVector(&hand.vel, &handControl.holdingActor->vel);
	}
//hand joined to ball
	if((hand.action == HAND_JOINED) || (hand.action == HAND_THROW2CUP) || (hand.action == HAND_BOUNCE2CUP))
	{
//		SetVector(&hand.vel, &ball.vel);
		MoveHandToBall();
//		MoveBallToHand();
	}
//hand aiming the ball
	if(hand.action == HAND_THROWAIM)
	{
		MoveBallToHand();
		SetVector(&ball.rot, &hand.rot);
		SetVector(&ball.vel, &hand.vel);
	}
//hand bouncing
	if(hand.action == HAND_BOUNCE)
	{
		SetVector2D(&hand.pos, &ball.pos);
		SetVector2D(&hand.vel, &ball.vel);

		{
			val = ball.pos.v[Y] - 1.3 + ball.radius;
			if(val > hand.pos.v[Y])
			{
				hand.pos.v[Y] = val;
				if(hand.vel.v[Y] < 0)
					hand.vel.v[Y] = 0;
			}


		}
	}




//handle camera garrib

	if(cameraInfo.overrideCounter == 0)
	{
		if((cameraInfo.positionToTrack == &hand.pos) && (TrackBall()))
		{
			cameraInfo.positionToTrack = &ball.pos;
			cameraInfo.velToTrack = &ball.vel;
		}
		else if((cameraInfo.positionToTrack == &ball.pos) && ((hand.action == HAND_THROWAIM) || (hand.action == HAND_AIRBOURNEWHACK)))
		{
			cameraInfo.positionToTrack = &hand.pos;
			cameraInfo.velToTrack = &hand.vel;
		}

		if(((cameraInfo.positionToTrack == &hand.pos) || (cameraInfo.positionToTrack == &ball.pos)) && (hand.action == HAND_BOUNCE))
		{
			cameraInfo.garibHeightAim = ball.pos.v[Y] + ((hand.pos.v[Y] - ball.pos.v[Y]) / 2);
		}
		else 
		{
			cameraInfo.garibHeightAim = cameraInfo.positionToTrack->v[Y] + cameraInfo.garibHeight;
		}
		
		cameraInfo.garibHeight = cameraInfo.distanceFromHand / 11;

		if(cameraInfo.garib.pos.v[Y] < cameraInfo.garibHeightAim)
		{
			cameraInfo.garib.pos.v[Y] += Min(2, (cameraInfo.garibHeightAim - cameraInfo.garib.pos.v[Y]) / 5);
		}
		if(cameraInfo.garib.pos.v[Y] > cameraInfo.garibHeightAim)
		{
			cameraInfo.garib.pos.v[Y] -= Min(2, (cameraInfo.garib.pos.v[Y] - cameraInfo.garibHeightAim) / 5);
		}

		camera.vel.v[Y] *= 0.8;

		val = FindShortestAngleSigned(camera.rot.v[Y], hand.rot.v[Y]);
		if(((fabs(PI - fabs(val))) < 0.4) || ((fabs(PI - fabs(val))) > PI - 0.4))
//		 || (Magnitude2D(&hand.vel) < 0.04))
		{
			if(fabs(lookOffset) > 0.5)
			{
				if(lookOffset > 0)
					lookOffsetVel -= 0.01;
				if(lookOffset < 0)
					lookOffsetVel += 0.01;
			}

		}
		else
		{
			if((val > 0.4) && (lookOffset < 2))
				lookOffsetVel += 0.01;
			if((val < 0) && (lookOffset > -2))
				lookOffsetVel -= 0.01;
		}

		if(lookOffsetVel > 0.7)
			lookOffsetVel = 0.7;
		if(lookOffsetVel < -0.7)
			lookOffsetVel = -0.7;


		lookOffset += lookOffsetVel;
		lookOffsetVel *= 0.84;

		temp.v[X] = lookOffset;
		temp.v[Y] = 0;
		temp.v[Z] = 0;
		RotateVector2D(&temp, &temp, camera.rot.v[Y]);
	}
	else
		SetVector(&temp,&zero);

	SubVector2D(&temp2, cameraInfo.positionToTrack, &cameraInfo.garib.pos);
	AddVector(&temp2, &temp2, &temp);	

	lastHandYRot = hand.rot.v[Y];

	factor = 1;

	if(temp2.v[X] >= 0)
		cameraInfo.garib.pos.v[X] += Min(camSpeed, temp2.v[X] / factor);
	else
		cameraInfo.garib.pos.v[X] += Max(-camSpeed, temp2.v[X] / factor);
	if(temp2.v[Z] >= 0)
		cameraInfo.garib.pos.v[Z] += Min(camSpeed, temp2.v[Z] / factor);
	else
		cameraInfo.garib.pos.v[Z] += Max(-camSpeed, temp2.v[Z] / factor);

/*
	SubVector2D(&temp2, cameraInfo.positionToTrack, &cameraInfo.garib.pos);
	temp.v[X] = 5;
	temp.v[Y] = 0;
	temp.v[Z] = 0;
	RotateVector2D(&temp, &temp, camera.rot.v[Y]);
	AddVector(&temp2, &temp, &temp2);	




	if(cameraInfo.slowGaribTimer > 0)
		if(--cameraInfo.slowGaribTimer > 0)
			camSpeed = cameraInfo.slowGaribSpeed;

	if(camMode == 0)
	{
		velMultiplier = min(Magnitude2D(cameraInfo.velToTrack)*Magnitude2D(cameraInfo.velToTrack)*100,5);
	
		if(cameraInfo.velToTrack)
			AddOneScaledVector2D(&temp2,&temp2,cameraInfo.velToTrack,velMultiplier);

		factor = Magnitude2D(&temp2);
		if(factor)
		{
			temp2.v[X] /= factor;
			temp2.v[Z] /= factor;
			if(cameraInfo.velToTrack)
				ScaleVector2D(&temp2,min(max(Magnitude2D(cameraInfo.velToTrack),camSpeed),factor/3));
			else
				ScaleVector2D(&temp2,min(camSpeed,factor/2));
			AddVector(&cameraInfo.garib.pos,&cameraInfo.garib.pos,&temp2);
		}
	}
	else
	{
		factor = 1;

		if(temp2.v[X] >= 0)
			cameraInfo.garib.pos.v[X] += Min(camSpeed, temp2.v[X] / factor);
		else
			cameraInfo.garib.pos.v[X] += Max(-camSpeed, temp2.v[X] / factor);
		if(temp2.v[Z] >= 0)
			cameraInfo.garib.pos.v[Z] += Min(camSpeed, temp2.v[Z] / factor);
		else
			cameraInfo.garib.pos.v[Z] += Max(-camSpeed, temp2.v[Z] / factor);
	}
//	SetVector(&cameraInfo.garib.pos, &temp2);	
*/
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void MoveHandToBall()
{
	float scale;
	float val;
	VECTOR tempVect;

	if((FindShortestAngle(Aabs(atan2(ball.vel.v[X], ball.vel.v[Z]) + PI), hand.rot.v[Y]) < PI /2)
	  && (ball.pos.v[Y] < ball.oldpos.v[Y]))
	{
		SetVector(&tempVect, &ball.oldpos);
	}
	else   
	{
		SetVector(&tempVect, &ball.pos);
	}

	SubVector(&tempVect, &tempVect, &hand.pos);
	ScaleVector(&tempVect, 0.6);
	AddVector(&hand.pos, &hand.pos, &tempVect);


	scale = ball.radius - BALL_NORMAL_RADIUS;
	if(scale > 0)
		scale += min(0.5,(ball.radius - BALL_NORMAL_RADIUS)/3);
	
	hand.pos.v[X] += sin(hand.rot.v[Y]) * scale;
	hand.pos.v[Z] += cos(hand.rot.v[Y]) * scale;
}
/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void MoveBallToHand()
{
	VECTOR	tempVect;

	tempVect.v[X] = tempVect.v[Y] = 0;
	tempVect.v[Z] = ball.radius - BALL_NORMAL_RADIUS;
	if(tempVect.v[Z] > 0)
		tempVect.v[Z] += min(0.4,(ball.radius - BALL_NORMAL_RADIUS)/4);
	else
		tempVect.v[Z] *= 0.5;


	RotateVector2DXYZ(&tempVect,&tempVect,5.35 - hand.rot.v[X],X);
	RotateVector2DXYZ(&tempVect,&tempVect,hand.rot.v[Y],Y);

	SubVector(&ball.pos,&hand.pos,&tempVect);
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
BOOL TrackBall()
{
	return((hand.action == HAND_JOINED) ||
	(hand.action == HAND_PREWHACK) ||
	(hand.action == HAND_BOUNCE) ||
	(hand.action == HAND_BOUNCE2CUP));
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
BOOL CheckActorForTeleport(ACTOR *actor,PLATFORM *platform)
{
	PLATFORM	*dest;
	VECTOR		tempVect;
	TELEPORT	*teleport;
//	int			i;

	if((platform->teleport) && (platform->teleport->status == ON))
	{
		teleport = platform->teleport;
		if(((actor->type == HAND) && (teleport->flags & TELEPORT_HAND) && (IsHandHoldingBall() == FALSE) && (hand.action != HAND_BALLWALK) && (hand.action != HAND_WHACK))
		|| ((actor->type == BALL) && (((teleport->flags & TELEPORT_BALL) && (hand.action != HAND_PREWHACK) && (hand.action != HAND_JOINED)) || ((hand.action == HAND_JOINED) && (teleport->flags & TELEPORT_JOINED))))
		|| ((teleport->flags & TELEPORT_NME) && (actor->type != DEBRIS) && (actor->type != BULLET)))
		{
			if(actor == &ball)
			{
				switch(hand.action)
				{
					case HAND_BOUNCE:
						ApplyAction(HAND_RELEASE);
						hand.flags &= -1 - LO_GRAVITY;
						break;

					case HAND_BALLWALK:
						ApplyAction(HAND_WALK);
						ball.owner = NULL;
						break;
				}
			}

			if((hand.action == HAND_JOINED) && ((actor == &hand) || (actor == &ball)))
			{
				if((hand.teleport == NULL) && (ball.teleport == NULL))
				{
					hand.teleport = teleport;
					hand.teleportCounter = teleport->disappearDelay;
					ball.teleport = teleport;
					ball.teleportCounter = teleport->disappearDelay;
				}
				else
					return NO;
			}
			else
			{
				actor->teleport = teleport;
				actor->teleportCounter = teleport->disappearDelay;
			}
			return YES;
		}
	}
	return NO;
}

