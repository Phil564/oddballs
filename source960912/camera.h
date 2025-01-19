#ifndef __CAMERA_H
#define __CAMERA_H

void ResetCamera();
void ResetHand();

void UpdateActor(ACTOR *actor);
void RotateActor(ACTOR *actor,VECTOR *rot);
void ActorAddVelocity(ACTOR *actor,VECTOR *vel);
void AccelerateActor(ACTOR *actor,double speed);


void ProcessKeys(WPARAM key);

#endif