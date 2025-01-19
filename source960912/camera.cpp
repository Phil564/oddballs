/*
 *		Project:	Oddballs
 *		File:		camera.cpp
 *		Description:handles camera movement
 *
 *
 *
 */


#include "oddballs.h"


#define CAMERA_INERTIA	0.8
#define HAND_INERTIA	0.6

/*	--------------------------------------------------------------------------------
	Function 	: ResetCamera()
	Purpose 	: resets the camera positoins
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void ResetCamera()
{
	camera.pos.v[X] = 0;
	camera.pos.v[Y] = 10;
	camera.pos.v[Z] = -40;
	camera.rot.v[X] = 0;
	camera.rot.v[Y] = 0;
	camera.rot.v[Z] = 0;
	camera.rotvel.v[X] = 0.02;
	camera.rotvel.v[Y] = 0;
	camera.rotvel.v[Z] = 0;
	camera.inertia = CAMERA_INERTIA;
}

/*	--------------------------------------------------------------------------------
	Function 	: ResetCamera()
	Purpose 	: resets the camera positoins
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void UpdateActor(ACTOR *actor)
{
	int	i;

//handle velocities
	actor->pos.v[X] += sin(actor->rot.v[Y]) * (actor->speed);
	actor->pos.v[Z] += cos(actor->rot.v[Y]) * (actor->speed);

	for(i = X;i <= Z;i++)
	{
		actor->pos.v[i] += actor->vel.v[i];
		actor->vel.v[i] *= actor->inertia;

	//handle rotations
		actor->rot.v[i] += actor->rotvel.v[i];
		actor->rot.v[i] = actor->rot.v[i] > (2 * PI) ? actor->rot.v[i] - 2 * PI : actor->rot.v[i];
		actor->rot.v[i] = actor->rot.v[i] < 0 ? actor->rot.v[i] += 2 * PI : actor->rot.v[i];

		actor->rotvel.v[i] *= actor->inertia;
	}	

	actor->speed *= actor->inertia;

	actor->frame->AddRotation(D3DRMCOMBINE_REPLACE, D3DVAL(0), D3DVAL(1), D3DVAL(0), D3DVAL(actor->rot.v[Y]));
	actor->frame->AddRotation(D3DRMCOMBINE_BEFORE,  D3DVAL(1), D3DVAL(0), D3DVAL(0), D3DVAL(actor->rot.v[X]));
	actor->frame->SetPosition(myglobs.scene, D3DVAL(actor->pos.v[X]), D3DVAL(actor->pos.v[Y]), D3DVAL(actor->pos.v[Z]));
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
	actor->speed += speed;
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
	int i;

	for(i = X;i <= Z;i++)
		actor->vel.v[i] += vel->v[i];
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
	int i;

	for(i = X;i <= Z;i++)
		actor->rotvel.v[i] += rot->v[i];
}


////////////////////////////////////////////////////////////////////////////////////



/*	--------------------------------------------------------------------------------
	Function 	: ResetHand()
	Purpose 	: resets the camera positoins
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void ResetHand()
{
	hand.pos.v[X] = 0;
	hand.pos.v[Y] = 15;
	hand.pos.v[Z] = 13;
	hand.rot.v[X] = 0;
	hand.rot.v[Y] = 0;
	hand.rot.v[Z] = 0;
	hand.rotvel.v[X] = 0;
	hand.rotvel.v[Y] = 0;
	hand.rotvel.v[Z] = 0;
	hand.inertia = HAND_INERTIA;
}

