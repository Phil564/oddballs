/*
 *		Project:	Oddballs
 *		File:		camera.cpp
 *		Description:handles camera movement
 *
 *
 *
 */


#include "oddballs.h"
#include "camera.h"
#include "actors.h"

CAMERA_INFO_TYPE cameraInfo;




/*	--------------------------------------------------------------------------------
	Function 	: MoveCamera
	Purpose 	: handles camera movement
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void MoveCamera()
{
	VECTOR temp, temp2;
	D3DVECTOR vect;	
	double val;


	SubVector(&temp, &camera.pos, &hand.pos);
	val = atan2(temp.v[X], temp.v[Z]);	
	camera.rot.v[Y] = Aabs(val + PI);	

	camera.frame->LookAt(hand.frame, myglobs.scene, D3DRMCONSTRAIN_Z);

	val = Magnitude2D(&temp);

	temp.v[Y] -= 10;
	camera.pos.v[Y] -= temp.v[Y] /2;

	if((joystick.xPos == 0) && (joystick.yPos == 0))
	{
 
//swing camera around to face behind hand 
 		if(cameraInfo.rotation < hand.rot.v[Y])
		{
			if(hand.rot.v[Y] - cameraInfo.rotation > PI)
				cameraInfo.rotation -= Min(0.02, FindShortestAngle(cameraInfo.rotation, hand.rot.v[Y]) / 4);
			else		
				cameraInfo.rotation += Min(0.02, FindShortestAngle(cameraInfo.rotation, hand.rot.v[Y]) / 4);
		}
		if(cameraInfo.rotation > hand.rot.v[Y])
		{
			if(cameraInfo.rotation - hand.rot.v[Y] > PI)
				cameraInfo.rotation += Min(0.02, FindShortestAngle(cameraInfo.rotation, hand.rot.v[Y]) / 4);
			else		
				cameraInfo.rotation -= Min(0.02, FindShortestAngle(cameraInfo.rotation, hand.rot.v[Y]) / 4);
		}
 
		camera.pos.v[X] = hand.pos.v[X];
		camera.pos.v[Z] = hand.pos.v[Z];
		camera.pos.v[X] += sin(cameraInfo.rotation) * val;
		camera.pos.v[Z] += cos(cameraInfo.rotation) * val;
	}
	else
	{
		cameraInfo.rotation = Aabs(camera.rot.v[Y] + PI);
/*
		if(val > 20)
			AccelerateActor(&camera, Min(2, (val - 20)/4));
		if(val <20)
			AccelerateActor(&camera, -Min(2, (20 - val)/4));
*/
	}
}

