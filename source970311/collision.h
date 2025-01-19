/*
 *	Project:		Oddballs
 *	File:			collision.h
 *	Description:	detects and responds to collisions
 */

#ifndef __COLLISION_H
#define __COLLISION_H

#define TERMINAL_VELOCITY	2.0

extern double	gravity;
extern TERRAIN_TYPE	terrain[NUM_TERRAIN_TYPES];

void InitTerrain();
void CheckCollisions(ACTOR *actor);
void CalcBounce(VECTOR *vel,VECTOR *normal,double scale);
double NearEdges(VECTOR *point,PLANE *plane,double r,VECTOR *nearestPoint);
void InitPlaneInfo(char *file);
int PartitionIndex(VECTOR *p);
int PointToPartition(VECTOR *pos);
void LoadPlaneData(char *filename);
void CheckActorCollisions(ACTOR *actor);
BOOL CollideActorWithPlane(ACTOR *actor,PLANE *plane,VECTOR *nearestEdgePoint,char *useEdge,double *edge,PLANE **corner,VECTOR *origPos,int *facenum,int *fnum,PLANE **actorPlane,int *numBounces,VECTOR *totalVel,VECTOR *rot,int *sphere,PLATFORM *platform,VECTOR *bounceVel);
int ActorCornerToPartition(ACTOR *actor,int corner);
double Altitude(ACTOR *actor,PLANE **floor,PLATFORM **platform);
double AltitudeCheckPlane(ACTOR *actor,PLANE *plane,VECTOR *point,double minHeight);

#endif
