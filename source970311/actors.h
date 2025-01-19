#ifndef __ACTORS_H
#define __ACTORS_H

#define CAMERA_INERTIA	0.8
#define HAND_INERTIA	0.8

extern char inertia;

void InitActorList();
void AddActor(ACTOR *actor);
void SubActor(ACTOR *actor);
void TransformActor(ACTOR *actor);
void TransformEnemySpheres(ENEMY_TYPE *enemy);
void UpdateActor(ACTOR *actor);
void RotateActor(ACTOR *actor,VECTOR *rot);
void ActorAddVelocity(ACTOR *actor,VECTOR *vel);
void AccelerateActor2D(ACTOR *actor,double speed);
void AccelerateActor(ACTOR *actor,double speed);
void AccelerateActorFromActor(ACTOR *sourceActor,ACTOR *targetActor, double speed);
void UpdateActorPositionDependencies();
BOOL AreActorsTouching(ACTOR *actor1, ACTOR *actor2);
BOOL IsActorFacingActor(ACTOR *actor, ACTOR *target, double range);
BOOL IsActorFacingPoint(ACTOR *actor, VECTOR *target, double range);
void MoveHandToBall();
void MoveBallToHand();
BOOL TrackBall();
void AddConveyorSpeed(VECTOR *actor,PLATFORM *platform);
BOOL CheckActorForTeleport(ACTOR *actor,PLATFORM *platform);
void CalcMaxActorRadius(ACTOR *actor);

void ProcessKeys(WPARAM key);

#endif