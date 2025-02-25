/*
 *	Project:		Oddballs
 *	File:			collision.cpp
 *	Description:	detects and responds to collisions
 */

#include "oddballs.h"
#include "collision.h"
#include "personal.h"
#include "actors.h"
#include "platforms.h"
#include "nmebehaviour.h"
#include "enemies.h"
#include "hand.h"
#include "anim.h"
#include "sound.h"

double	gravity = 0.1;
//double	gravity = 0.17 * SCALINGFACTOR;

int numPartitions;

VECTOR	mapMin,mapMax,mapSize;

SPACE_PARTITION		*spacePartition;


TERRAIN_TYPE	terrain[NUM_TERRAIN_TYPES];

/*	--------------------------------------------------------------------------------
	Function 	: CheckActorCollisions()
	Purpose 	: checks for collisions between actors
	Parameters 	: actor pointer
	Returns 	: none
	Info 		:
*/
void CheckActorCollisions(ACTOR *actor)
{
	ACTORLISTENTRY	*ptr;
	VECTOR			dist,vel,v0,v1,pos,pos2,rot,rot2,sep,sep2;
	double			val,m,mass,spinAdd;
	int				nme,j,k,doIt;

	if(actor->mass == 0)
		return;

	for(ptr = actorList.head.prev;ptr != &actorList.head;ptr = ptr->prev)
//	for(ptr = actorList.head.next;ptr != &actorList.head;ptr = ptr->next)
	{
		if((ptr->actor->teleport == NULL) && (ptr->actor != actor) 
		   && (ptr->actor->mass != 0) && (ptr->actor->flags & COLLISION)
		   && (ptr->actor->type != 0) && (ptr->actor->owner != actor) 
		   && ((hand.action != HAND_JOINED) || ((actor != &hand) && (ptr->actor != &hand))) && (ptr->actor != actor->owner) && ((actor->type != BULLET) || (ptr->actor->type != BULLET)))
		{
			SubVector(&dist,&actor->pos,&ptr->actor->pos);
			if(MagnitudeSquared(&dist) > (actor->maxRadius+ptr->actor->maxRadius)*(actor->maxRadius+ptr->actor->maxRadius))
				continue;
			for(j = 0;j <= actor->numExtraCollisionSpheres;j++)
			{
				if(j == 0)
				{
					SetVector(&pos,&actor->pos);
					val = actor->radius;
				}
				else
				{
					val = actor->collisionSphere[j-1].radius;
					RotateVector2DXYZ(&rot,&actor->collisionSphere[j-1].offset,-actor->rot.v[Z],Z);
					RotateVector2DXYZ(&rot,&rot,-actor->rot.v[X],X);
					RotateVector2D(&rot,&rot,actor->rot.v[Y]);
					AddVector(&pos,&actor->pos,&rot);

//					if(actor->type == CYLON)
//						scenicItems[j-1].frame->SetPosition(g_lpWorld,pos.v[X],pos.v[Y],pos.v[Z]);
				}
				for(k = 0;k <= actor->numExtraCollisionSpheres;k++)
				{
					if(k == 0)
					{
						SetVector(&pos2,&ptr->actor->pos);
						val += ptr->actor->radius;
					}
					else
					{
						val += ptr->actor->collisionSphere[k-1].radius;
						RotateVector2DXYZ(&rot2,&ptr->actor->collisionSphere[k-1].offset,-ptr->actor->rot.v[Z],Z);
						RotateVector2DXYZ(&rot2,&rot2,-ptr->actor->rot.v[X],X);
						RotateVector2D(&rot2,&rot2,ptr->actor->rot.v[Y]);
						AddVector(&pos2,&ptr->actor->pos,&rot2);
					}
					SubVector(&dist,&pos,&pos2);

					if((m = Magnitude(&dist)) <= val)
					{
						if(actor->speed >= val)
						{
							AddOneScaledVector(&actor->pos,&actor->oldpos,&actor->vel,0.5);
							if(j == 0)
								SetVector(&pos,&actor->pos);
							else
								AddVector(&pos,&actor->pos,&rot);
							SubVector(&dist,&pos,&pos2);
							m = Magnitude(&dist);
						}

						doIt = YES;
						ptr->actor->health -= actor->damage;
						if(actor->flags & DIESONCONTACT)
							actor->lifetime = 1;
						if(ptr->actor->flags & DIESONCONTACT)
							ptr->actor->lifetime = 1;
						if(((ptr->actor->enemyNum != -1) && (actor == &ball) && (enemies[ptr->actor->enemyNum].flags & DEADLY_TO_BALL))
						 || ((actor->enemyNum != -1) && (ptr->actor == &ball) && (enemies[actor->enemyNum].flags & DEADLY_TO_BALL)))
							BurstBall();
						if(((ptr->actor->enemyNum != -1) && (actor == &hand) && (enemies[ptr->actor->enemyNum].flags & DEADLY_TO_HAND))
						 || ((actor->enemyNum != -1) && (ptr->actor == &hand) && (enemies[actor->enemyNum].flags & DEADLY_TO_HAND)))
						{
							HurtHand(HURT_GO_BACKWARDS);
							doIt = NO;
						}
						else if(hand.action == HAND_SLAM)
						{
							nme = -1;
							if((ptr->actor->enemyNum != -1) && (actor == &hand) && (enemies[ptr->actor->enemyNum].flags & HURT_BY_SLAM))
								nme = ptr->actor->enemyNum;
							else if((actor->enemyNum != -1) && (ptr->actor == &hand) && (enemies[actor->enemyNum].flags & HURT_BY_SLAM))
								nme = actor->enemyNum;
							if((nme != -1) && (enemies[nme].actor.status != DEAD))
							{
								AnimateActor(&enemies[nme].actor, NMEANIM_DEATH, NO, NO, 0);
								player.score += 100;
								enemies[nme].actor.status = DEAD;
								PlaySample(SFX_SQUASH, 0, &enemies[nme].actor.pos,100);
								enemies[nme].actor.mass = 0;
								return;
							}
						}
						if(doIt)
						{
							mass = actor->mass + ptr->actor->mass;
							ScaleVector(&dist,1/m);					
							SubVector(&vel,&actor->vel,&ptr->actor->vel);
							SetVector(&v1,&dist);

							ScaleVector(&v1,(2*actor->mass*DotProduct(&v1,&vel))/mass);
							AddOneScaledVector(&v0,&vel,&v1,-(ptr->actor->mass)/actor->mass);

							if(ptr->actor->type == BULLET)
								ScaleVector(&v0,2);
							if(actor->type == BULLET)
								ScaleVector(&v1,2);

//							spinny bit						
							if((j) && (actor->collisionSphere[j-1].spinVal))
							{
								SubVector(&sep,&pos,&actor->centreOfGravity);
								SubVector(&sep,&sep,&actor->pos);
								sep2.v[X] = -sep.v[Z];
								sep2.v[Z] = sep.v[X];
								spinAdd = max(-0.5,min(0.5,(DotProduct2D(&sep2,&vel)*ptr->actor->mass)/(actor->mass * actor->collisionSphere[j-1].spinVal)));
								actor->rotvel.v[Y] += spinAdd;
								AddOneScaledVector(&actor->vel,&v0,&ptr->actor->vel,1 - fabs(spinAdd)*2);
							}
							else
								AddVector(&actor->vel,&v0,&ptr->actor->vel);

							if((k) && (ptr->actor->collisionSphere[k-1].spinVal))
							{
								SubVector(&sep,&pos2,&ptr->actor->centreOfGravity);
								SubVector(&sep,&sep,&ptr->actor->pos);
								sep2.v[X] = -sep.v[Z];
								sep2.v[Z] = sep.v[X];
								spinAdd = max(-0.5,min(0.5,(DotProduct2D(&sep2,&vel)*actor->mass)/(ptr->actor->mass * ptr->actor->collisionSphere[k-1].spinVal)));
								ptr->actor->rotvel.v[Y] += spinAdd;
								AddOneScaledVector(&ptr->actor->vel,&ptr->actor->vel,&v1,1 - fabs(spinAdd)*2);
							}
							else
								AddVector(&ptr->actor->vel,&v1,&ptr->actor->vel);

	//						ScaleVector(&actor->vel,actor->bounce);
	//						ScaleVector(&ptr->actor->vel,ptr->actor->bounce);

							if((actor->onGround) && (actor->vel.v[Y] < 0))
								actor->vel.v[Y] *= 0.25;
							if((ptr->actor->onGround) && (ptr->actor->vel.v[Y] < 0))
								ptr->actor->vel.v[Y] *= 0.25;

							AddOneScaledVector(&actor->pos,&actor->pos,&dist,((val-m)*ptr->actor->mass)/mass);
							AddOneScaledVector(&ptr->actor->pos,&ptr->actor->pos,&dist,((m-val)*actor->mass)/mass);
	/*
							if((actor->type == BIGBALL) && (ptr->actor == &ball) && (actor->actorScript.currentCommand == 0))
							{
								PlaySample(SFX_BIGBALL, 0, &ball.pos, min(100,Magnitude2D(&vel)*150));

	//							NmeGetNextInstruction(actor,1);
							}
							if((ptr->actor->type == BIGBALL) && (actor == &ball) && (ptr->actor->actorScript.currentCommand == 0))
							{
								PlaySample(SFX_BIGBALL, 0, &ball.pos, min(100,Magnitude2D(&vel)*150));
	//							NmeGetNextInstruction(ptr->actor,1);
							}
							if((actor->type == BIGBALL) && (ptr->actor->type == BIGBALL))
								PlaySample(SFX_BIGBALL, 0, &ball.pos, min(100,Magnitude2D(&vel)*150));
	*/						
								
						}
						if(ptr->actor->health < 0)
							ptr->actor->flags |= DESTROYED;
					}
				}
			}
		}
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: CheckCollisions()
	Purpose 	: checks for collisions between actor and landscape
	Parameters 	: pointer to actor
	Returns 	: none
	Info 		:
*/
void CheckCollisions(ACTOR *actor)
{
	int polyDone[8][3000],numParts = 0,part,p,j,k,l,facenum = 0,numBounces = 0,fnum = 0,skip,pnum[8] = {0,0,0,0,0,0,0,0},partDone[8],sphere;
	char useEdge = 0,collVar = 0;
	VECTOR nearestEdgePoint,tempPoint,vel,pos,totalVel,origPos,tempPos,bounceVel,rot;
	double	initVelY,m,lastSpeed,edge = 0,thisEdge = 0,tempEdge,value;
	PLANE	*plane,*actorPlane[NUMACTORPLANES],*corner = NULL;
	PLANELISTENTRY	*ptr;
	PLATFORMLISTENTRY *platformPtr;
	PLATFORM	*platform;

	initVelY = actor->vel.v[Y];

	SetVector(&bounceVel,&zero);
	SetVector(&totalVel,&zero);
	SetVector(&origPos,&actor->pos);

	for(j = 0;j < NUMACTORPLANES;j++)
	{
		actorPlane[j] = actor->plane[j];
		actor->plane[j] = NULL;
	}

	for(part = 0;part < 8;part++)
	{
		p = ActorCornerToPartition(actor,part);
		if(p != -1)
		{
			for(j = 0;j < numParts;j++)
				if(partDone[j] == p)
					break;
			if(j < numParts)
				continue;
			partDone[numParts++] = p;
			for(ptr = (spacePartition+p)->planeList.head.next;ptr != &(spacePartition+p)->planeList.head;ptr = ptr->next)
			{
				j = ptr->plane->index;
				for(l = 0;l < part;l++)
				{
					for(k = 0;k < pnum[l];k++)
						if(j == polyDone[l][k])
							goto cont;
				}

				polyDone[part][pnum[l]++] = j;

 				plane = landscape.planes+j;

				if(CollideActorWithPlane(actor,plane,&nearestEdgePoint,&useEdge,&edge,&corner,&origPos,&facenum,&fnum,actorPlane,&numBounces,&totalVel,&rot,&sphere,NULL,NULL))
					return;
				cont:;
			}
		}
	}

	for(platformPtr = platformList.head.next;platformPtr != &platformList.head;platformPtr = platformPtr->next)
	{
		platform = platformPtr->platform;

		if((platform->flags & DUMMY) || (platform->flags & BACKGROUND))
			continue;

		SubVector(&tempPos,&actor->pos,&platform->pos);
		if(MagnitudeSquared(&tempPos) > (actor->maxRadius + platform->radius) * (actor->maxRadius + platform->radius))
			continue;

		if(frameCount > platform->lastCalcFrame)
			RecalculatePlatformInfo(platform);
		for(ptr = platform->planeList.head.next;ptr != &platform->planeList.head;ptr = ptr->next)
		{
			if(CollideActorWithPlane(actor,ptr->plane,&nearestEdgePoint,&useEdge,&edge,&corner,&origPos,&facenum,&fnum,actorPlane,&numBounces,&totalVel,&rot,&sphere,platform,&bounceVel))
				return;
		}
	}

	if(numBounces > 0)
	{
		if(actor == &ball)
			PlaySample(SFX_HANDBOUNCE,NO,&ball.pos,min(100,Magnitude(&ball.vel)*100));
		if(actor->vel.v[Y] < -0.3)
			actor->velInertia = actor->normInertia;
//		if(actor->bounce > 0)
		{
			ScaleVector(&totalVel,(actor->speed*actor->bounce)/numBounces);
			SetVector(&actor->vel,&totalVel);
		}
		AddVector(&actor->vel,&actor->vel,&bounceVel);
	}
	else 
//	if((fnum > 0) && (Magnitude(&actor->vel) > 0.1) && (actor->pos.v[Y] - actor->oldpos.v[Y] > actor->vel.v[Y]))
	{
		SetVector(&vel,&actor->vel);
		SubVector(&actor->vel,&actor->pos,&actor->oldpos);
		if((inertia) && (actor->flags & GRAVITY) && (fabs(actor->vel.v[Y]) < 0.01))
			actor->vel.v[Y] -= gravity/7;

/*		if(fabs(actor->vel.v[Y]) > fabs(initVelY) + 0.001)
		{
			SetVector(&actor->vel,&vel);
			MakeUnit(&actor->vel);
			ScaleVector(&actor->vel,actor->speed);
		}
		else
*/		{
			m = Magnitude(&actor->vel);	
			if((actor->speed/m < 2) && ((actor->speed/m < 1) || (vel.v[Y] < -gravity)))
			{
				actor->vel.v[X] *= actor->speed/m;
				actor->vel.v[Z] *= actor->speed/m;
			}
		}
		if(actor->flying)
		{
			MakeUnit(&actor->vel);
			ScaleVector(&actor->vel,actor->speed);
		}
	}
				
	SubVector(&vel,&actor->pos,&actor->oldpos);
	lastSpeed = Magnitude2D(&vel);

	if(lastSpeed < 0.01)
		actor->vel.v[X] = actor->vel.v[Z] = 0;

	if((edge != 0) && (actor->contactTime[0] > 0) && ((facenum == 0) || (corner != NULL) || (useEdge > 0)))
	{
		skip = NO;
		if(corner != NULL)
		{
			skip = YES;
			if(((facenum == 0) || (actor->onGround == 0)) && (actor->vel.v[Y] < -TERMINAL_VELOCITY/4))
			{
				if((fnum == 0) || ((fnum == 1) && (actor->plane[0] == corner)))
				{
					for(k = 0;k < 10;k++)
					{
						AddScaledVector(&tempPos,&actor->pos,((float)k)/10,&actor->oldpos,((float)((10-k)))/10);
						AddVector(&tempPos,&tempPos,&rot);

						if(sphere == 0)
							tempEdge = NearEdges(&tempPos,corner,actor->radius,&tempPoint);
						else
							tempEdge = NearEdges(&tempPos,corner,actor->collisionSphere[sphere-1].radius,&tempPoint);

						if(tempEdge != 0)
						{
							SetVector(&nearestEdgePoint,&tempPoint);
							SetVector(&actor->pos,&tempPos);
							thisEdge = tempEdge;
							skip = NO;
							break;
						}
					}
				}
			}
		}
//		if((facenum == 0) || (corner == NULL))
		if(skip == NO)
		{
			SetVector(&tempPoint,&actor->pos);
			SubVector(&pos,&actor->pos,&nearestEdgePoint);
			AddVector(&pos,&pos,&rot);
			MakeUnit(&pos);
			if(((facenum == 0) || (actor->onGround == 0)) && (actor->vel.v[Y] < -TERMINAL_VELOCITY/4))
	 			SetVector(&actor->oldpos,&actor->pos);
			if(sphere == 0)
				AddOneScaledVector(&actor->pos,&nearestEdgePoint,&pos,actor->radius);
			else
			{
				AddOneScaledVector(&actor->pos,&nearestEdgePoint,&pos,actor->collisionSphere[sphere-1].radius);
				SubVector(&actor->pos,&actor->pos,&rot);
			}

//			if((corner != NULL) || (numBounces == 0) || (actor->speed < 0.4) || (actor->onGround == YES))
			{
				if(corner != NULL)
				{
					SubVector(&actor->vel,&tempPos,&nearestEdgePoint);
					MakeUnit(&actor->vel);
					ScaleVector(&actor->vel,actor->speed*actor->bounce);
				}
				else
				{
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//					if((actor->speed < 0.4) || (actor->plane[0] != NULL) || (actor->contactTime[0] < 3))
					{
						SubVector(&actor->vel,&actor->pos,&actor->oldpos);
						if(((facenum == 0) || (actor->onGround == 0)) && (initVelY < -TERMINAL_VELOCITY/4))
						{
							MakeUnit(&actor->vel);
							ScaleVector(&actor->vel,actor->speed*actor->bounce);
						}
					}
//					else
//					{
//						SetVector(&actor->vel,&pos);
//						ScaleVector(&actor->vel,actor->speed*actor->bounce);
//					}
/*
					if((actor->speed < 0.4) || (actor->plane[0] != NULL) || (ball.onGround))
					{
						SubVector(&actor->vel,&actor->pos,&actor->oldpos);
						if(((facenum == 0) || (actor->onGround == 0)) && (initVelY < -TERMINAL_VELOCITY/4))
						{
							MakeUnit(&actor->vel);
							ScaleVector(&actor->vel,actor->speed*actor->bounce);
						}
					}
					else
					{
						SetVector(&actor->vel,&pos);
						ScaleVector(&actor->vel,actor->speed*actor->bounce);
					}
*/
//					MakeUnit(&actor->vel);
//					ScaleVector(&actor->vel,actor->speed*actor->bounce);
				}
			}
/*			else
			{
				SubVector(&vel,&actor->pos,&nearestEdgePoint);
				ScaleVector(&vel,actor->speed/Magnitude(&vel));
				AddScaledVector(&actor->vel,&vel,0.5,&actor->vel,0.5);
				MakeUnit(&actor->vel);
				ScaleVector(&actor->vel,actor->speed*actor->bounce);
			}
*/		}
	}
/*
	if((edge != 0) && (actor->contactTime[0] > 0) && ((facenum == 0) || (corner != NULL) || (useEdge > 0)))
	{
		skip = NO;
		if(corner != NULL)
		{
			skip = YES;
//			if(((facenum == 0) || (actor->onGround == 0)) && (actor->vel.v[Y] < -TERMINAL_VELOCITY/4))
			if(((facenum == 0) || (actor->onGround == 0)) && (actor->speed >= 0.4))
			{
//				if((fnum == 0) || ((fnum == 1) && (actor->plane[0] == corner)))
				{
					for(k = 0;k < 10;k++)
					{
						AddScaledVector(&tempPos,&actor->pos,((float)k)/10,&actor->oldpos,((float)((10-k)))/10);
						AddVector(&tempPos,&tempPos,&rot);

						if(sphere == 0)
							tempEdge = NearEdges(&tempPos,corner,actor->radius,&tempPoint);
						else
							tempEdge = NearEdges(&tempPos,corner,actor->collisionSphere[sphere-1].radius,&tempPoint);
						if(tempEdge != 0)
						{
							SetVector(&nearestEdgePoint,&tempPoint);
							SetVector(&actor->pos,&tempPos);
							thisEdge = tempEdge;
							skip = NO;
							break;
						}
					}
				}
			}
		}
//		if((facenum == 0) || (corner == NULL))
		if(skip == NO)
		{
			SetVector(&tempPoint,&actor->pos);
			SubVector(&pos,&actor->pos,&nearestEdgePoint);
			AddVector(&pos,&pos,&rot);
			MakeUnit(&pos);
			if(((facenum == 0) || (actor->onGround == 0)) && (actor->vel.v[Y] < -TERMINAL_VELOCITY/4))
	 			SetVector(&actor->oldpos,&actor->pos);
			if(sphere == 0)
				AddOneScaledVector(&actor->pos,&nearestEdgePoint,&pos,actor->radius);
			else
			{
				AddOneScaledVector(&actor->pos,&nearestEdgePoint,&pos,actor->collisionSphere[sphere-1].radius);
				SubVector(&actor->pos,&actor->pos,&rot);
			}
//			if((corner != NULL) || (numBounces == 0) || (actor->speed < 0.4) || (actor->onGround == YES))
			{
				if(corner != NULL)
				{
					SubVector(&actor->vel,&tempPos,&nearestEdgePoint);
					MakeUnit(&actor->vel);
					ScaleVector(&actor->vel,actor->speed*actor->bounce);
				}
				else
				{
					if((value = DotProduct(&actor->vel,&pos)) >= -0.1)
					{
						SubVector(&actor->vel,&actor->pos,&actor->oldpos);
						if(((facenum == 0) || (actor->onGround == 0)) && (initVelY < -TERMINAL_VELOCITY/4))
						{
							MakeUnit(&actor->vel);
							ScaleVector(&actor->vel,actor->speed*actor->bounce);
						}
//						MakeUnit(&actor->vel);
//						ScaleVector(&actor->vel,actor->speed*actor->bounce);
					}
					else
					{
//						SubVector(&actor->vel,&tempPoint,&nearestEdgePoint);
//						AddVector(&actor->vel,&actor->vel,&rot);
//						MakeUnit(&actor->vel);
						SetVector(&actor->vel,&pos);
						ScaleVector(&actor->vel,actor->speed*actor->bounce);
					}
				}
			}
//			else
//			{
//				SubVector(&vel,&actor->pos,&nearestEdgePoint);
//				ScaleVector(&vel,actor->speed/Magnitude(&vel));
//				AddScaledVector(&actor->vel,&vel,0.5,&actor->vel,0.5);
//				MakeUnit(&actor->vel);
//				ScaleVector(&actor->vel,actor->speed*actor->bounce);
//			}
		}
	}
*/
	for(k = 0;k < NUMACTORPLANES;k++)
	{
		for(j = 0;j < NUMACTORPLANES;j++)
		{													
			if(actor->plane[k] == actorPlane[j])
			{
				actor->contactTime[k]++;
				break;
			}
		}
		if(j == NUMACTORPLANES)
			actor->contactTime[k] = 0;
	}
}


/*	--------------------------------------------------------------------------------
	Function 	: CalcBounce()
	Purpose 	: bounces a velocity vector off a plane defined by normal
	Parameters 	: velocity and normal vector pointers and scale
	Returns 	: none
	Info 		:
*/
void CalcBounce(VECTOR *vel,VECTOR *normal,double scale)
{
	double		NdotL;
	VECTOR		ref;

	MakeUnit(vel);
	NdotL = -DotProduct(vel,normal);
	SetVector(&ref,normal);
	ScaleVector(&ref,NdotL*2);
	AddVector(&ref,&ref,vel);
	MakeUnit(&ref);
	vel->v[X] = ref.v[X]*scale;
	vel->v[Y] = ref.v[Y]*scale;
	vel->v[Z] = ref.v[Z]*scale;
}
void CalcBounceOld(VECTOR *vel,VECTOR *normal,double scale)
{
	double		NdotL;
	VECTOR		ref;

	MakeUnit(vel);
	FlipVector(vel);
	NdotL = DotProduct(vel,normal);
	SetVector(&ref,normal);
	ScaleVector(&ref,NdotL*2);
	SubVector(&ref,&ref,vel);
	MakeUnit(&ref);
	vel->v[X] = ref.v[X]*scale;
	vel->v[Y] = ref.v[Y]*scale;
	vel->v[Z] = ref.v[Z]*scale;
}


/*	--------------------------------------------------------------------------------
	Function 	: NearEdges()
	Purpose 	: checks whether a point is within a given distance of the edges of a triangle
	Parameters 	: point, plane, distance
	Returns 	: YES or NO
	Info 		:
*/
double NearEdges(VECTOR *point,PLANE *plane,double r,VECTOR *nearestPoint)
{
	VECTOR	normal,diff;
	short	j,k;
	double	t,J,dist,len;

//	for(j = 0;j < 3;j++)
//		verts[j] = &plane->vertex[j];

	for(j = 0;j < 3;j++)
	{
		k = (j+1) MOD 3;
		SubVector(&normal,&plane->vertex[k],&plane->vertex[j]);

		len = MagnitudeSquared(&normal);
		MakeUnit(&normal);
		
		J = DotProduct(&normal,point);
		t = J - DotProduct(&plane->vertex[j],&normal);
		if((t < 0) || (t*t > len))
			continue;
		AddOneScaledVector(nearestPoint,&plane->vertex[j],&normal,t);
		SubVector(&diff,point,nearestPoint);

		if((dist = MagnitudeSquared(&diff)) < r*r)
			return sqrt(dist);
	}
	return 0;
}

double NearEdgesOld(VECTOR *point,PLANE *plane,double r,VECTOR *nearestPoint)
{
	VECTOR	normal,diff,*verts[3];
	short	j,k;
	double	t,J,dist,len;

	for(j = 0;j < 3;j++)
		verts[j] = &plane->vertex[j];

	for(j = 0;j < 3;j++)
	{
		k = (j+1) MOD 3;
		SubVector(&normal,verts[k],verts[j]);
		len = Magnitude(&normal);
		MakeUnit(&normal);
		
//		J = -DotProduct(&normal,point);
//		t = -(J + DotProduct(verts[j],&normal));
		J = DotProduct(&normal,point);
		t = J - DotProduct(verts[j],&normal);
		if((t < 0) || (t > len))
			continue;
		AddOneScaledVector(nearestPoint,verts[j],&normal,t);

		SubVector(&diff,point,nearestPoint);
		if((dist = Magnitude(&diff)) < r)
			return dist;
	}
	return 0;
}

/*	--------------------------------------------------------------------------------
	Function 	: PointToPartition()
	Purpose 	: calculates which space partition a point is in
	Parameters 	: point
	Returns 	: partition index, or -1 if outside map area
	Info 		:
*/
int PointToPartition(VECTOR *point)
{
	int i;
	VECTOR	p;

	for(i = X;i <= Z;i++)
		if((point->v[i] <= mapMin.v[i]) || (point->v[i] >= mapMax.v[i]))
			return -1;
		else
			p.v[i] = (int)(((point->v[i] - mapMin.v[i])*mapRes.v[i])/(mapSize.v[i]));

	return PartitionIndex(&p);
}

/*	--------------------------------------------------------------------------------
	Function 	: PartitionIndex()
	Purpose 	: calculates the index of space partition defined by 3D position
	Parameters 	: vector
	Returns 	: index into partition array
	Info 		:
*/
int PartitionIndex(VECTOR *p)
{
	return p->v[X] + p->v[Z]*mapRes.v[X] + p->v[Y]*mapRes.v[X]*mapRes.v[Z];
}

/*	--------------------------------------------------------------------------------
	Function 	: InitPlaneInfo()
	Purpose 	: initialises info for landscape polygons for collision routines
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void InitPlaneInfo(char *file)
{
	int					j,k,big,n = 0;
	char				filename[512];
	float				l,m;

	sprintf(filename,"%sgraphics\\%s.sp",rootPath,file);
	LoadPlaneData(filename);

	j = 0;
	for(k = 0;k < numPartitions;k++)
	{
		if((k == 0) || ((spacePartition+k)->planeList.numEntries > big))
			big = (spacePartition+k)->planeList.numEntries;
		j += (spacePartition+k)->planeList.numEntries;
		if((spacePartition+k)->planeList.numEntries)
			n++;
	}
	l = (float)j/(float)numPartitions;
	m = (float)j/(float)n;
	
	dprintf("\n\nTotal partitions: %d\nEmpty partitions: %d\n",numPartitions,numPartitions - n);
	dprintf("%d references to %d polygons\nBiggest partition has %d references\nAverage partition has %.1f references\nAverage non-empty partition has %.1f references\n\n",j,landscape.numPlanes,big,l,m);
}
						    
/*	--------------------------------------------------------------------------------
	Function 	: LoadPlaneData()
	Purpose 	: loads data from a space-partitioning file
	Parameters 	: filename
	Returns 	: none
	Info 		:
*/
void LoadPlaneData(char *filename)
{
	FILE *fp;
	int j,k,num,index;

	InitTerrain();

	if((fp = fopen(filename,"rb")) == NULL)
	{
		dprintf("Error opening file %s",filename);
		return;
	}
	else
		dprintf("Opening file %s",filename);

	landscape.numPlanes = 0;
	for(k = 0;k < landscape.numObjects;k++)
		if((landscape.object[k].flags & BACKGROUND) == 0)
			landscape.numPlanes += landscape.object[k].faceCount;

	landscape.planes = (PLANE *)MyMalloc(landscape.numPlanes * sizeof(PLANE));	

	fread(landscape.planes,sizeof(PLANE),landscape.numPlanes,fp);
	fread(&mapMin,sizeof(VECTOR),1,fp);
	fread(&mapMax,sizeof(VECTOR),1,fp);
	fread(&mapRes,sizeof(VECTOR),1,fp);
	fread(&mapSize,sizeof(VECTOR),1,fp);

	numPartitions = mapRes.v[X] * mapRes.v[Y] * mapRes.v[Z];
	spacePartition = (SPACE_PARTITION *)MyMalloc(numPartitions*sizeof(SPACE_PARTITION));

	for(j = 0;j < numPartitions;j++)
	{
		fread(&num,sizeof(int),1,fp);
		InitPlaneList(&(spacePartition+j)->planeList);
		for(k = 0;k < num;k++)
		{
			fread(&index,sizeof(int),1,fp);

			AddPlaneToList(landscape.planes+index,&(spacePartition+j)->planeList);

			(landscape.planes+index)->terrain = STONE;
		}
	}
 
	fclose(fp);
}

/*	--------------------------------------------------------------------------------
	Function 	: CollideActorWithPlane()
	Purpose 	: checks for collision between an actor and a polygon
	Parameters 	: lots!
	Returns 	: none
	Info 		:
*/
BOOL CollideActorWithPlane(ACTOR *actor,PLANE *plane,VECTOR *nearestEdgePoint,char *useEdge,double *edge,PLANE **corner,VECTOR *origPos,int *facenum,int *fnum,PLANE **actorPlane,int *numBounces,VECTOR *totalVel,VECTOR *rot,int *sphere,PLATFORM *platform,VECTOR *bounceVel)
{
	double dist,alpha,beta,u[3],v[3],thisEdge,m,val,range;
	VECTOR *norm,pos,nearestPoint,tempPoint,vel,tempVel,rotvel,relvel,rot2;
	int k,i[2],j,bounce,l,d[3],retval = NO,loop;

	norm = &plane->normal;

	for(loop = 0;loop <= actor->numExtraCollisionSpheres;loop++)
	{
		if(loop == 0)
		{
			SetVector(&pos,&actor->pos);
			range = actor->radius;
			SetVector(&rot2,&zero);
		}
		else
		{
			range = actor->collisionSphere[loop-1].radius;
			RotateVector2DXYZ(&rot2,&actor->collisionSphere[loop-1].offset,-actor->rot.v[Z],Z);
			RotateVector2DXYZ(&rot2,&rot2,-actor->rot.v[X],X);
			RotateVector2D(&rot2,&rot2,actor->rot.v[Y]);
			AddVector(&pos,&actor->pos,&rot2);
		}
		dist = -(DotProduct(&pos,norm) + plane->J);

		if(dist > 0)
		{
			if(-(DotProduct(&actor->oldpos,norm) + plane->J) <= 0)
			{
				k = 0;
				while(dist > 0)
				{
					AddScaledVector(&pos,&pos,0.9,&actor->oldpos,0.1);
					dist = -(DotProduct(&pos,norm) + plane->J);
					k++;
					if(k == 20)
						SetVector(&pos,&actor->oldpos);
				}
			}
			else
				continue;
		}

		if(-dist >= range + 0.05)
			continue;

		AddOneScaledVector(&nearestPoint,&pos,norm,dist);

		alpha = -1;

		i[0] = plane->i[0];
		i[1] = plane->i[1];

		u[0] = nearestPoint.v[i[0]] - plane->vertex[0].v[i[0]];
		u[1] = plane->u1;
		u[2] = plane->u2;
		
		v[0] = nearestPoint.v[i[1]] - plane->vertex[0].v[i[1]];
		v[1] = plane->v1;
		v[2] = plane->v2;

		if(u[1] == 0)
		{
			beta = u[0]/u[2];
			if((beta >= 0.0) && (beta <= 1.0))
				alpha = (v[0] - beta*v[2])/v[1];
		}
		else
		{
			beta = (v[0]*u[1] - u[0]*v[1])/(v[2]*u[1] - u[2]*v[1]);
			if((beta >= 0.0) && (beta <= 1.0))
				alpha = (u[0] - beta*u[2])/u[1];
		}

		thisEdge = 0;
		if((alpha < 0.0) || (alpha+beta > 1.0))
		{
			if(((thisEdge = NearEdges(&pos,plane,range,&tempPoint)) == 0))
				continue;
			else
			{
				SetVector(nearestEdgePoint,&tempPoint);
				*edge = thisEdge;
				*sphere = loop;
				SetVector(rot,&rot2);

	//			if((actor->plane[0] != NULL) && (fabs(norm->v[Y] - actor->plane[0]->normal.v[Y]) > 0.1))
				if((actor->plane[0] == NULL) || (fabs(norm->v[Y] - actor->plane[0]->normal.v[Y]) > 0.1))
					*useEdge = 1;
	//			SetVector(&nearestPoint,&tempPoint);
	//			goto plat;
				if((platform != NULL) && (actor == &ball) && (platform->counter == -3) && ((platform->timeBonus != 0) || (plane->normal.v[Y] > 0.95)))
				{
					platform->counter = 1;
	//				platform->nextPoint = (platform->nextPoint + platform->numPathPoints + platform->direction) MOD platform->numPathPoints;
				}
				if(actor->flags & DIESONCONTACT)
				{
					actor->lifetime = 1;
					retval = YES;
	//				return YES;
				}

				if(platform)
					if(CheckActorForTeleport(actor,platform))
						retval = YES;
	//					return YES;
	//			return NO;
				goto plat;
			}
		}
		else
		{
	//					if((actor->onGround == 0) && (facenum == 0))
	//		{
				thisEdge = NearEdges(origPos,plane,Max(range,actor->speed),&tempPoint);
				if(thisEdge != 0)
				{
					SetVector(nearestEdgePoint,&tempPoint);
					*edge = thisEdge;
					*sphere = loop;
					*corner = plane;
					SetVector(rot,&rot2);
				}
	//		}

				actor->plane[*facenum] = plane;

				SetVector(&(actor->nearestPoint[*facenum]),&nearestPoint);
				if(norm->v[Y] > 0)
					actor->rolling[*facenum] = YES;
				(*facenum)++;
		}
																	   
	//	if(fabs(dist) >= actor->radius)
	//		return NO;

		(*fnum)++;
	//		if(/*(actor->speed < 0.02) || */(fabs(DotProduct(&actor->vel,&norm)) > 0.001))

		if(loop == 0)
			AddOneScaledVector(&actor->pos,&nearestPoint,norm,range);
		else
		{
			AddOneScaledVector(&actor->pos,&nearestPoint,norm,range);
			SubVector(&actor->pos,&actor->pos,&rot2);
		}
			

		SetVector(&vel,&actor->vel);
		if(platform)
			SubVector(&vel,&vel,&platform->vel);
		CalcBounce(&vel,norm,1);

		if((edge == 0) && (fabs(actor->vel.v[Y]) < 0.4) && (actor->vel.v[Y] < 0))
			vel.v[Y] = 0;

		bounce = YES;
	//		if(speed < 2.0)
		if((fabs(actor->vel.v[Y]) < gravity) && (actor->speed < gravity*2))
			bounce = NO;
		else if(norm->v[Y] >= -0.000001)
		{
	//			if(actor->onGround)
			{
				for(k = 0;k < NUMACTORPLANES;k++)
				{
					if(actorPlane[k] == plane)
						if(actor->contactTime[k] > 0)
							bounce = NO;
				}
			}
			SubVector(&tempVel,&pos,&actor->oldpos);
			if(loop)
				SubVector(&tempVel,&tempVel,&rot2);
			MakeUnit(&tempVel);
			m = DotProduct(&tempVel,norm);
			if((m > -0.3) || ((m > -0.8) && (actor->onGround)))
				bounce = NO;
			else
			{
				SetVector(&tempVel,&actor->vel);
				MakeUnit(&tempVel);
				m = DotProduct(&tempVel,norm);
	//				if((m > -0.3) || ((m > Max(-0.8,actor->bounce*2 - 2.2)) && (actor->onGround)))
				if((m > -0.3) || ((m > -0.8) && (actor->onGround)))
					bounce = NO;
			}
		}

		if(bounce == YES)
		{
			(*numBounces)++;
			if(platform != NULL)
			{
				if(platform->flags & DESTRUCTIBLE)
				{
					if((actor->flags & DESTROYER) && (actor->speed > 1))
						platform->flags |= DESTROYED;
				}

				if(platform->flags & CONVEYOR)
					AddConveyorSpeed(totalVel,platform);
				for(j = X;j <= Z;j++)
					if(platform->bounceDir[j] != 0)
						vel.v[j] *= platform->bounce;
			}
			AddVector(totalVel,totalVel,&vel);
			actor->rolling[*facenum-1] = NO;
  		}
		else if(plane->normal.v[Y] > 0.3)
		{
			actor->platform = platform;
		}

	plat:
		if(actor->flags & DIESONCONTACT)
			actor->lifetime = 1;

		if(platform != NULL)
		{
			actor->touchPlatform = platform;
			if(platform->flags & ELECTRICAL)
				actor->touchingElectric = YES;
			if((platform->electric) && (actor == &hand))
			{
				SetVector(&hand.vel,&zero);
				HurtHand(HURT_GO_TO_SAFE);
				return YES;
			}

			if(CheckActorForTeleport(actor,platform))
				return YES;

			if((platform->crumbleLimit > 0) && (platform->crumbleCounter == 0))
				platform->crumbleCounter = 1;

			if((actor == &ball) && (platform->counter == -3) && ((platform->timeBonus != 0) || (plane->normal.v[Y] > 0.95)))
			{
				platform->counter = 1;
	//			platform->nextPoint = (platform->nextPoint + platform->numPathPoints + platform->direction) MOD platform->numPathPoints;
			}

			if(platform->flags & TILTING)
			{
				SetVector(&rotvel,&platform->rotvel);
				j = k = 0;
				for(l = X;l <= Z;l++)
					if(platform->spinAxis.v[l] == 0)
						d[j++] = l;
					else
						k = l;
			}

			if((actor->mass == 0) || (platform->mass == 0))
				goto tilt;

			
			SubVector(&relvel,&actor->vel,&platform->vel);
			val = -DotProduct(norm,&relvel);
			if(val > 0)
			{
				SetVector(&vel,norm);
				ScaleVector(&vel,-val);
				tempVel.v[X] = norm->v[Z];
				tempVel.v[Z] = -norm->v[X];
				tempVel.v[Y] = 0;
				val = DotProduct(&tempVel,&relvel);
				ScaleVector(&tempVel,val/(terrain[plane->terrain].slidy * 4 + 4));
				for(l = X;l <= Z;l++)
					if(platform->pushDir.v[l] == 0)
						vel.v[l] = tempVel.v[l] = 0;

				ScaleVector(&vel,actor->mass/platform->mass);
				AddVector(&platform->vel,&platform->vel,&vel);
				ScaleVector(&tempVel,actor->mass/platform->mass);
				AddVector(&platform->vel,&platform->vel,&tempVel);
				platform->speed = Magnitude(&platform->vel);
				if(platform->speed > platform->maxSpeed)
				{
					ScaleVector(&platform->vel,platform->maxSpeed/platform->speed);
					platform->speed = Magnitude(&platform->vel);
				}
			}

			if(platform->flags & SEESAW)
			{
				SetVector(&vel,&actor->vel);

				if(actor->flags & GRAVITY)
					vel.v[Y] -= gravity;

				m = sqrt(vel.v[d[0]]*vel.v[d[0]] + vel.v[d[1]]*vel.v[d[1]]);
				if(m != 0)
				{
					vel.v[d[0]] /= m;
					vel.v[d[1]] /= m;
				}
				if((val = vel.v[d[0]]*plane->normal.v[d[0]] + vel.v[d[1]]*plane->normal.v[d[1]]) >= 0)
					goto tilt;

				SubVector(&tempVel,&nearestPoint,&platform->pos);

				if(platform->rot.v[Y])
					RotateVector2D(&tempVel,&tempVel,-platform->rot.v[Y]);

				dist = sqrt(tempVel.v[d[0]]*tempVel.v[d[0]] + tempVel.v[d[1]]*tempVel.v[d[1]]);
				dist *= -val*m;

				val = vel.v[d[0]]*tempVel.v[d[1]] - vel.v[d[1]]*tempVel.v[d[0]];
				if(fabs(val) < 0.25)
					goto tilt;

				platform->noActorInContactCount = 0;

				dist *= val;

				dist *= actor->mass;
				dist /= platform->mass;

				if(k == Y)
					platform->rotvel.v[k] += dist;
				else
					platform->rotvel.v[k] -= dist;

			}

	tilt:;
			if(platform->flags & TILTING)
			{
				if((d[0] > Z) || (d[1] > Z))
					if(retval)
						return retval;
					else
						continue;

				if(actor->oldTouchPlatform != platform)
				{
					SubVector(&tempPoint,&nearestPoint,&platform->pos);
					tempPoint.v[k] = 0;
					dist = Magnitude(&tempPoint);

					m = atan2(tempPoint.v[d[1]],tempPoint.v[d[0]]);

					if(k == Y)
						m += rotvel.v[k];
					else
						m -= rotvel.v[k];

					tempVel.v[d[0]] = tempPoint.v[d[0]] - dist*cos(m);
					tempVel.v[d[1]] = tempPoint.v[d[1]] - dist*sin(m);
					tempVel.v[k] = 0;

					MakeUnit(&tempVel);

					SubVector(&tempPoint,&actor->pos,&nearestPoint);
					if(loop)
						AddVector(&tempPoint,&tempPoint,&rot2);
					if(DotProduct(&tempPoint,&tempVel) <= 0)
						if(retval)
							return retval;
						else
							continue;

					if((rotvel.v[k] != 0) && (platform->flags & SEESAW))
						for(j = 0;actor->plane[j] != NULL;j++)
							if((plane != actor->plane[j]) && (DotProduct(&tempVel,&actor->plane[j]->normal) < -0.5))
								platform->rotvel.v[k] = -platform->rotvel.v[k]*actor->bounce;

					ScaleVector(&tempVel,dist*fabs(rotvel.v[k]));
					AddVector(&actor->vel,&actor->vel,&tempVel);
					SubVector(&actor->oldpos,&actor->oldpos,&tempVel);
					AddVector(totalVel,totalVel,&tempVel);
					actor->speed = Magnitude(&actor->vel);					
				}
			}
		}
	}
	return retval;
}

/*	--------------------------------------------------------------------------------
	Function 	: ActorCornerToPartition()
	Purpose 	: works out which partition a corner of an actor's bounding cube is in
	Parameters 	: actor, corner index
	Returns 	: partition index
	Info 		:
*/
int ActorCornerToPartition(ACTOR *actor,int corner)
{
	VECTOR pos;

	switch(corner)
	{
		case 0:
			pos.v[X] = min(mapMax.v[X]-0.000001,actor->pos.v[X] - actor->maxRadius);
			pos.v[Y] = min(mapMax.v[Y]-0.000001,actor->pos.v[Y] - actor->maxRadius);
			pos.v[Z] = min(mapMax.v[Z]-0.000001,actor->pos.v[Z] - actor->maxRadius);
			break;

		case 1:
			pos.v[X] = min(mapMax.v[X]-0.000001,actor->pos.v[X] - actor->maxRadius);
			pos.v[Y] = min(mapMax.v[Y]-0.000001,actor->pos.v[Y] - actor->maxRadius);
			pos.v[Z] = max(mapMin.v[Z]+0.000001,actor->pos.v[Z] + actor->maxRadius);
			break;

		case 2:
			pos.v[X] = min(mapMax.v[X]-0.000001,actor->pos.v[X] - actor->maxRadius);
			pos.v[Y] = max(mapMin.v[Y]+0.000001,actor->pos.v[Y] + actor->maxRadius);
			pos.v[Z] = max(mapMin.v[Z]+0.000001,actor->pos.v[Z] + actor->maxRadius);
			break;

		case 3:
			pos.v[X] = min(mapMax.v[X]-0.000001,actor->pos.v[X] - actor->maxRadius);
			pos.v[Y] = max(mapMin.v[Y]+0.000001,actor->pos.v[Y] + actor->maxRadius);
			pos.v[Z] = min(mapMax.v[Z]-0.000001,actor->pos.v[Z] - actor->maxRadius);
			break;

		case 4:
			pos.v[X] = max(mapMin.v[X]+0.000001,actor->pos.v[X] + actor->maxRadius);
			pos.v[Y] = min(mapMax.v[Y]-0.000001,actor->pos.v[Y] - actor->maxRadius);
			pos.v[Z] = min(mapMax.v[Z]-0.000001,actor->pos.v[Z] - actor->maxRadius);
			break;

		case 5:
			pos.v[X] = max(mapMin.v[X]+0.000001,actor->pos.v[X] + actor->maxRadius);
			pos.v[Y] = min(mapMax.v[Y]-0.000001,actor->pos.v[Y] - actor->maxRadius);
			pos.v[Z] = max(mapMin.v[Z]+0.000001,actor->pos.v[Z] + actor->maxRadius);
			break;

		case 6:
			pos.v[X] = max(mapMin.v[X]+0.000001,actor->pos.v[X] + actor->maxRadius);
			pos.v[Y] = max(mapMin.v[Y]+0.000001,actor->pos.v[Y] + actor->maxRadius);
			pos.v[Z] = max(mapMin.v[Z]+0.000001,actor->pos.v[Z] + actor->maxRadius);
			break;

		case 7:
			pos.v[X] = max(mapMin.v[X]+0.000001,actor->pos.v[X] + actor->maxRadius);
			pos.v[Y] = max(mapMin.v[Y]+0.000001,actor->pos.v[Y] + actor->maxRadius);
			pos.v[Z] = min(mapMax.v[Z]-0.000001,actor->pos.v[Z] - actor->maxRadius);
			break;

	}	

	return(PointToPartition(&pos));
}

/*	--------------------------------------------------------------------------------
	Function 	: Altitude()
	Purpose 	: calculates height above ground
	Parameters 	: actor
	Returns 	: altitude
	Info 		:
*/
double Altitude(ACTOR *actor,PLANE **floor,PLATFORM **plat)
{
	PLANELISTENTRY	*ptr;
	int p,num = 0;
	double height;
	double minHeight = -1;
	PLANE *plane;
	VECTOR point,checkPoint;
	PLATFORMLISTENTRY	*platPtr;
	PLATFORM	*platform;
	
	point.v[X] = checkPoint.v[X] = actor->pos.v[X];
	point.v[Z] = checkPoint.v[Z] = actor->pos.v[Z];
	
	*plat = NULL;

	do
	{
		checkPoint.v[Y] = actor->pos.v[Y] - (((num++) * mapSize.v[Y])/mapRes.v[Y]);
		p = PointToPartition(&checkPoint);
		if(p == -1)
			continue;

		for(ptr = (spacePartition+p)->planeList.head.next;ptr != &(spacePartition+p)->planeList.head;ptr = ptr->next)
		{
			plane = ptr->plane;
			if((height = AltitudeCheckPlane(actor,plane,&point,minHeight)) == -1)
				continue;

			minHeight = height;
			*floor = plane;
		}
		if(minHeight != -1)
			checkPoint.v[Y] = mapMin.v[Y] - 1;

 	} while(checkPoint.v[Y] > mapMin.v[Y]);
	
	for(platPtr = platformList.head.next;platPtr != &platformList.head;platPtr = platPtr->next)
	{
		platform = platPtr->platform;

		if((platform->flags & DUMMY) || (platform->flags & BACKGROUND))
			continue;

		SubVector2D(&checkPoint,&actor->pos,&platform->pos);
		if(Magnitude2D(&checkPoint) < actor->radius + platform->radius)
		{
			if(frameCount > platform->lastCalcFrame)
				RecalculatePlatformInfo(platform);
			for(ptr = platform->planeList.head.next;ptr != &platform->planeList.head;ptr = ptr->next)
			{
				plane = ptr->plane;
				if((height = AltitudeCheckPlane(actor,plane,&point,minHeight)) == -1)
					continue;

				minHeight = height;
				*floor = plane;
				*plat = platform;
			}
		}
	}

	return minHeight;
}


/*	--------------------------------------------------------------------------------
	Function 	: AltitudeCheckPlane()
	Purpose 	: calculates height of actor above a plane
	Parameters 	: actor,plane,point,minimum height so far
	Returns 	: height above plane if lower than min, else -1
	Info 		:
*/
double AltitudeCheckPlane(ACTOR *actor,PLANE *plane,VECTOR *point,double minHeight)
{
	double	d,t,u0,v0,alpha = -1,beta;

	d = plane->normal.v[Y];
	if(d <= 0)
		return -1;
	t = DotProduct(&actor->pos,&plane->normal) + plane->J;
	t = t/d;

	if((t > 0) && ((t < minHeight) || (minHeight == -1)))
	{
		alpha = -10.0;

		point->v[Y] = actor->pos.v[Y] - t;
		u0 = point->v[plane->i[0]] - plane->vertex[0].v[plane->i[0]];
		
		v0 = point->v[plane->i[1]] - plane->vertex[0].v[plane->i[1]];

		if(plane->u1 == 0)
		{
			beta = u0/plane->u2;
			if((beta >= 0.0) && (beta <= 1.0))
				alpha = (v0 - beta*plane->v2)/plane->v1;
		}
		else
		{
			beta = (v0*plane->u1 - u0*plane->v1)/(plane->v2*plane->u1 - plane->u2*plane->v1);
			if((beta >= 0.0) && (beta <= 1.0))
				alpha = (u0 - beta*plane->u2)/plane->u1;
		}

		if((alpha < 0.0) || (alpha+beta > 1.0))
			return -1;

		return t;
	}
	else
		return -1;
}

/*	--------------------------------------------------------------------------------
	Function 	: InitTerrain
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void InitTerrain()
{
	terrain[STONE].slidy = 1;
	terrain[STONE].bouncy = 1;

	terrain[GRASS].slidy = 0.5;
	terrain[GRASS].bouncy = 1;

	terrain[ICE].slidy = 6;
	terrain[ICE].bouncy = 1;

	terrain[MUD].slidy = 0.5;
	terrain[MUD].bouncy = 0.5;

	terrain[SAND].slidy = 1;
	terrain[SAND].bouncy = 0.5;

	terrain[SHALLOW_WATER].slidy = 0.7;
	terrain[SHALLOW_WATER].bouncy = 0.7;

	terrain[DEEP_WATER].slidy = 0.2;
	terrain[DEEP_WATER].bouncy = 0.7;
}
