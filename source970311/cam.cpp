/*
 *		Project:	Oddballs
 *		File:		camera.cpp
 *		Description:handles camera movement
 *
 *
 *
 */


#include "oddballs.h"
#include "cam.h"
#include "actors.h"
#include "hand.h"
#include "collision.h"

CAMERA_INFO_TYPE cameraInfo;
BOOL cameraFollow = 1;

CAMERACASE_LIST cameraCaseList;

char *cameraActionText[] = 
{
	"CAM_HEIGHT",
	"CAM_DISTANCE",
	"CAM_PARALLEL",
	"CAM_FIXEDPOS",
	"CAM_FIXEDROT",
};


/*	--------------------------------------------------------------------------------
	Function 	: InitCameraCaseList()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void InitCameraCaseList()
{
	int	j;

	cameraCaseList.numEntries = 0;
	cameraCaseList.head.next = cameraCaseList.head.prev = &cameraCaseList.head;
}


/*	--------------------------------------------------------------------------------
	Function 	: NewCameraCase()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
CAMERACASE *NewCameraCase(FILE *in)
{
	CAMERACASE	*cameraCase;
	char		string[256];
	int			j;
	float		tempFloat;
	float		inFloat[7];

	cameraCase = (CAMERACASE *)MyCalloc(1,sizeof(CAMERACASE));
	AddCameraCase(cameraCase);

//sort out action list
	cameraCase->actionList.numEntries = 0;
	cameraCase->actionList.head.next = cameraCase->actionList.head.prev = &cameraCase->actionList.head;


//determine camera condition
	fscanf(in,"%s",string);

	if(strcmp(string, "CAM_HAND_CROSSES_LINE") == 0)
	{
		fscanf(in,"%f %f %f %f %f %f %f",&inFloat[0], &inFloat[1], &inFloat[2], &inFloat[3],
										&inFloat[4], &inFloat[5], &inFloat[6]);
		cameraCase->condition.type = CAM_HAND_CROSSES_LINE;
		cameraCase->condition.value1.b.pos.v[X] = inFloat[0];
		cameraCase->condition.value1.b.pos.v[Y] = inFloat[1];
		cameraCase->condition.value1.b.pos.v[Z] = inFloat[2];
		cameraCase->condition.value1.b.size.v[X] = inFloat[3];
		cameraCase->condition.value1.b.size.v[Y] = inFloat[4];
		cameraCase->condition.value1.b.size.v[Z] = inFloat[5];
		cameraCase->condition.value2.f = inFloat[6];			//how much extra to swing round by
	}else
	if(strcmp(string, "CAM_HAND_IN_BOX") == 0)
	{
		fscanf(in,"%f %f %f %f %f %f",&inFloat[0], &inFloat[1], &inFloat[2], &inFloat[3], 
											&inFloat[4], &inFloat[5]);
		cameraCase->condition.type = CAM_HAND_IN_BOX;
		cameraCase->condition.value1.b.pos.v[X] = inFloat[0];
		cameraCase->condition.value1.b.pos.v[Y] = inFloat[1];
		cameraCase->condition.value1.b.pos.v[Z] = inFloat[2];
		cameraCase->condition.value1.b.size.v[X] = inFloat[3];
		cameraCase->condition.value1.b.size.v[Y] = inFloat[4];
		cameraCase->condition.value1.b.size.v[Z] = inFloat[5];
	}else
	if(strcmp(string, "CAM_HAND_IN_RANGE") == 0)
	{
		fscanf(in,"%f %f %f %f",&inFloat[0], &inFloat[1], &inFloat[2], &inFloat[3]);
		cameraCase->condition.type = CAM_HAND_IN_RANGE;
		cameraCase->condition.value1.v.v[X] = inFloat[0];
		cameraCase->condition.value1.v.v[Y] = inFloat[1];
		cameraCase->condition.value1.v.v[Z] = inFloat[2];
		cameraCase->condition.value2.f = inFloat[3];
	}else
	if(strcmp(string, "CAM_BALL_IN_BOX") == 0)
	{
		fscanf(in,"%f %f %f %f %f %f",&inFloat[0], &inFloat[1], &inFloat[2], &inFloat[3], 
											&inFloat[4], &inFloat[5]);
		cameraCase->condition.type = CAM_BALL_IN_BOX;
		cameraCase->condition.value1.b.pos.v[X] = inFloat[0];
		cameraCase->condition.value1.b.pos.v[Y] = inFloat[1];
		cameraCase->condition.value1.b.pos.v[Z] = inFloat[2];
		cameraCase->condition.value1.b.size.v[X] = inFloat[3];
		cameraCase->condition.value1.b.size.v[Y] = inFloat[4];
		cameraCase->condition.value1.b.size.v[Z] = inFloat[5];
	}else
	if(strcmp(string, "CAM_BALL_IN_RANGE") == 0)
	{
		fscanf(in,"%f %f %f %f",&inFloat[0], &inFloat[1], &inFloat[2], &inFloat[3]);
		cameraCase->condition.type = CAM_BALL_IN_RANGE;
		cameraCase->condition.value1.v.v[X] = inFloat[0];
		cameraCase->condition.value1.v.v[Y] = inFloat[1];
		cameraCase->condition.value1.v.v[Z] = inFloat[2];
		cameraCase->condition.value2.f = inFloat[3];
	}else
	if(strcmp(string, "CAM_CAM_IN_BOX") == 0)
	{
		fscanf(in,"%f %f %f %f %f %f",&inFloat[0], &inFloat[1], &inFloat[2], &inFloat[3], 
											&inFloat[4], &inFloat[5]);
		cameraCase->condition.type = CAM_CAM_IN_BOX;
		cameraCase->condition.value1.b.pos.v[X] = inFloat[0];
		cameraCase->condition.value1.b.pos.v[Y] = inFloat[1];
		cameraCase->condition.value1.b.pos.v[Z] = inFloat[2];
		cameraCase->condition.value1.b.size.v[X] = inFloat[3];
		cameraCase->condition.value1.b.size.v[Y] = inFloat[4];
		cameraCase->condition.value1.b.size.v[Z] = inFloat[5];

	}else
	if(strcmp(string, "CAM_CAM_IN_RANGE") == 0)
	{
		fscanf(in,"%f %f %f %f",&inFloat[0], &inFloat[1], &inFloat[2], &inFloat[3]);
		cameraCase->condition.type = CAM_CAM_IN_RANGE;
		cameraCase->condition.value1.v.v[X] = inFloat[0];
		cameraCase->condition.value1.v.v[Y] = inFloat[1];
		cameraCase->condition.value1.v.v[Z] = inFloat[2];
		cameraCase->condition.value2.f = inFloat[3];
	}else
	if(strcmp(string, "CAM_ALWAYS") == 0)
	{
		cameraCase->condition.type = CAM_ALWAYS;
	}else
	if(strcmp(string, "CAM_NEVER") == 0)
	{
		cameraCase->condition.type = CAM_NEVER;
	}else
		dprintf("Camera actions must have an ascociated condition!!!!!!\n");


	return cameraCase;
}


/*	--------------------------------------------------------------------------------
	Function 	: AddCameraAction()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void AddCameraAction(FILE *in, CAMERACASE *cameraCase)
{
	CAMERA_ACTION	*cameraAction = NULL;		
	char			string[256];
	float			inFloat[6];

	cameraAction = (CAMERA_ACTION *)MyCalloc(1,sizeof(CAMERA_ACTION));

	cameraCase->actionList.numEntries++;
	cameraAction->prev = &cameraCase->actionList.head;
	cameraAction->next = cameraCase->actionList.head.next;
	cameraCase->actionList.head.next->prev = cameraAction;
	cameraCase->actionList.head.next = cameraAction;


//determine camera action
	fscanf(in,"%s",string);
	if(strcmp(string, "CAM_HEIGHT") == 0)
	{
		fscanf(in,"%f",&inFloat[0]);
		cameraAction->type = CAM_HEIGHT;
		cameraAction->value1.f = inFloat[0];
	}else
	if(strcmp(string, "CAM_DISTANCE") == 0)
	{
		fscanf(in,"%f",&inFloat[0]);
		cameraAction->type = CAM_DISTANCE;
		cameraAction->value1.f = inFloat[0];
	}else
	if(strcmp(string, "CAM_PARALLEL") == 0)
	{
		fscanf(in,"%f",&inFloat[0]);
		cameraAction->type = CAM_PARALLEL;
		cameraAction->value1.f = inFloat[0];
	}else
	if(strcmp(string, "CAM_FIXEDROT") == 0)
	{
		fscanf(in,"%f %f",&inFloat[0], &inFloat[1]);
		cameraAction->type = CAM_FIXEDROT;
		cameraAction->value1.f = inFloat[0];
		cameraAction->value2.f = inFloat[1];
	}else
	if(strcmp(string, "CAM_FIXEDPOS") == 0)
	{
		fscanf(in,"%f %f %f",&inFloat[0], &inFloat[1], &inFloat[2]);
		cameraAction->type = CAM_FIXEDPOS;
		cameraAction->value1.v.v[X] = inFloat[0];
		cameraAction->value1.v.v[Y] = inFloat[1];
		cameraAction->value1.v.v[Z] = inFloat[2];
	}else
	if(strcmp(string, "CAM_CORNER") == 0)
	{
/*
		fscanf(in,"%f %f %f %f",&inFloat[0], &inFloat[1], &inFloat[2], &inFloat[3]);
		cameraAction->type = CAM_CORNER;
		cameraAction->value.b.pos.v[X] = inFloat[0];
		cameraAction->value.b.pos.v[Z] = inFloat[1];
		cameraAction->value.b.size.v[X] = inFloat[2];
		cameraAction->value.b.size.v[Z] = inFloat[3];
*/
		fscanf(in,"%f",&inFloat[0]);
		cameraAction->type = CAM_CORNER;
	}else
		dprintf("Warning:Unknown Action!!!!!!!!!!!\n");



}

/*	--------------------------------------------------------------------------------
	Function 	: AddCameraCase()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void AddCameraCase(CAMERACASE *cameraCase)
{
	if(cameraCase->next == NULL)
	{
		cameraCaseList.numEntries++;
		cameraCase->prev = &cameraCaseList.head;
		cameraCase->next = cameraCaseList.head.next;
		cameraCaseList.head.next->prev = cameraCase;
		cameraCaseList.head.next = cameraCase;
	}
}



/*	--------------------------------------------------------------------------------
	Function 	: CAMConditionMet()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
BOOL CAMConditionMet(CAMERA_CONDITION *condition)
{
	POINT2D	point[4];
	float	ix, iy;
	VECTOR	tempVect;

	switch(condition->type)
	{
		case CAM_HAND_CROSSES_LINE:
			if((hand.pos.v[Y] < condition->value1.b.pos.v[Y]) || (hand.pos.v[Y] > condition->value1.b.size.v[Y]))
				break;

			point[0].x = condition->value1.b.pos.v[X];
			point[0].z = condition->value1.b.pos.v[Z];
			point[1].x = condition->value1.b.size.v[X];
			point[1].z = condition->value1.b.size.v[Z];
			point[2].x = hand.pos.v[X];
			point[2].z = hand.pos.v[Z];
			point[3].x = hand.oldpos.v[X];
			point[3].z = hand.oldpos.v[Z];

			if(IntersectEdges(&ix, &iy, &point[0], &point[1], &point[2], &point[3]))
			{
				float	lineMag, distanceFromPoint;

				ix = Aabs(atan2(point[1].z - point[0].z, point[0].x - point[1].x) + PI);
				iy = Aabs(atan2(hand.vel.v[X], hand.vel.v[Z]) + PI);
				if(FindShortestAngle(ix, camera.rot.v[Y]) < 2)
				{
					if(FindShortestAngleSigned(ix, camera.rot.v[Y]) > 0)
						return FALSE;
					
					if(FindShortestAngle(Aabs(ix + PI), iy) < 2)
					{
						dprintf("whone way\n");
						cameraInfo.cornerCounter = 50;
						tempVect.v[X] = point[1].x - point[0].x;
						tempVect.v[Z] = point[1].z - point[0].z;
						lineMag = Magnitude2D(&tempVect);
						tempVect.v[X] = point[2].x - point[0].x;
						tempVect.v[Z] = point[2].z - point[0].z;
						distanceFromPoint = Magnitude2D(&tempVect);
						cameraInfo.cornerAngle = Aabs(ix - (1 - (distanceFromPoint / lineMag)));
						cameraInfo.preferedRotation = cameraInfo.cornerAngle;
						return TRUE;
					}
				}
				if(FindShortestAngle(Aabs(ix + PI), camera.rot.v[Y]) < 2)
				{
					if(FindShortestAngleSigned(Aabs(ix + PI), camera.rot.v[Y]) < 0)
						return FALSE;

					if(FindShortestAngle(ix, iy) < 2)
					{
						dprintf("t'other way\n");
						cameraInfo.cornerCounter = 50;
//						cameraInfo.cornerAngle = Aabs(ix + PI + 1);
						tempVect.v[X] = point[1].x - point[0].x;
						tempVect.v[Z] = point[1].z - point[0].z;
						lineMag = Magnitude2D(&tempVect);
						tempVect.v[X] = point[2].x - point[0].x;
						tempVect.v[Z] = point[2].z - point[0].z;
						distanceFromPoint = Magnitude2D(&tempVect);
						cameraInfo.cornerAngle = Aabs(ix + PI + (1 - (distanceFromPoint / lineMag)));
						cameraInfo.preferedRotation = cameraInfo.cornerAngle;
						return TRUE;
					}
				}
			}	
			break;
		case CAM_HAND_IN_BOX:
			if(IsPointWithinBox(&condition->value1.b, &hand.pos))
				return TRUE;
			break;
		case CAM_HAND_IN_RANGE:
			if(DistanceBetweenPoints(&hand.pos, &condition->value1.v) < condition->value2.f)
				return TRUE;
			break;
		case CAM_BALL_IN_BOX:
			if(IsPointWithinBox(&condition->value1.b, &ball.pos))
				return TRUE;
			break;
		case CAM_BALL_IN_RANGE:
			if(DistanceBetweenPoints(&ball.pos, &condition->value1.v) < condition->value2.f)
				return TRUE;
			break;
		case CAM_CAM_IN_BOX:
			if(IsPointWithinBox(&condition->value1.b, &camera.pos))
				return TRUE;
			break;
		case CAM_CAM_IN_RANGE:
			if(DistanceBetweenPoints(&camera.pos, &condition->value1.v) < condition->value2.f)
				return TRUE;
			break;
	}

	return FALSE;
}

/*	--------------------------------------------------------------------------------
	Function 	: CAMSetCameraState
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void CAMSetCameraState(CAMERA_ACTION *action)
{

	switch(action->type)
	{
		case CAM_HEIGHT:
			cameraInfo.additionalHeight = action->value1.f;
			break;
		case CAM_DISTANCE:
			cameraInfo.additionalDistance = action->value1.f;
			break;
		case CAM_PARALLEL:
			cameraInfo.type = 1;
			break;
		case CAM_FIXEDROT:
			cameraInfo.overrideRotation = TRUE;
			cameraInfo.overriddenRotation = action->value1.f;
			cameraInfo.overrideRotationSpeed = action->value2.f;
			break;
		case CAM_FIXEDPOS:
			cameraInfo.positionLocked = TRUE;
			SetVector(&cameraInfo.lockedPosition, &action->value1.v);
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
void ProcessCameraCases()
{
	CAMERACASE		*cameraCase;
	CAMERA_ACTION	*action;

	cameraInfo.type = 0;
	cameraInfo.additionalHeight = 0;
	cameraInfo.additionalDistance = 0;
	cameraInfo.overrideRotation = FALSE;
	cameraInfo.positionLocked = FALSE;

	for(cameraCase = cameraCaseList.head.next; cameraCase != &cameraCaseList.head; cameraCase = cameraCase->next)
	{
		if(CAMConditionMet(&cameraCase->condition))
		{
			for(action = cameraCase->actionList.head.next; action != &cameraCase->actionList.head; action = action->next)
			{
				CAMSetCameraState(action);
			}
		}
	}

}


/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void SetCameraOverride(double camX, double camY, double camZ, VECTOR *camTargetPos, VECTOR *camTargetVel, int timer)
{
	cameraInfo.overrideCounter = timer;
	cameraInfo.lastPositionToTrack = cameraInfo.positionToTrack;
	cameraInfo.lastVelToTrack = cameraInfo.velToTrack;
	cameraInfo.lastPositionLocked = cameraInfo.positionLocked;
	SetVector(&cameraInfo.lastPosition, &camera.pos);

	camera.pos.v[X] = camX;
	camera.pos.v[Y] = camY;
	camera.pos.v[Z] = camZ;
	SetVector(&camera.vel, &zero);
//	SetVector(&cameraInfo.lockedPosition, &camera.pos);
	cameraInfo.positionToTrack = camTargetPos;
	cameraInfo.velToTrack = camTargetVel;
	SetVector(&cameraInfo.garib.pos,camTargetPos);
//	cameraInfo.positionLocked = TRUE;


	DisableControl(timer + 10);
}



/*	--------------------------------------------------------------------------------
	Function 	: MoveCamera
	Purpose 	: handles camera movement
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void MoveCam()
{
	VECTOR	temp;
	double	val;
	int		x;
	PLANE	*plane = NULL;
	float	camSpeed = 2;
	float	angle;
	VECTOR	tempVect;
	ACTOR	*actor = &hand;
	float	bestSlope = 0.5;
	if(cameraFollow == OFF)
		return;


	if(cameraInfo.overrideCounter)
	{
		cameraInfo.overrideCounter--;
		if(cameraInfo.overrideCounter == 0)
		{
//restore camera values
			cameraInfo.positionToTrack = cameraInfo.lastPositionToTrack;
			cameraInfo.velToTrack = cameraInfo.lastVelToTrack;
			cameraInfo.positionLocked = cameraInfo.lastPositionLocked;
			SetVector(&camera.pos, &cameraInfo.lastPosition);
			SetVector(&cameraInfo.garib.pos, cameraInfo.positionToTrack);
			if(cameraInfo.positionLocked == TRUE)
			{
				SetVector(&cameraInfo.lockedPosition, &cameraInfo.lastLockedPosition);
			}
		}	
	}	




	if(cameraInfo.cornerCounter)
	{
		cameraInfo.cornerCounter--;

		SetVector(&tempVect, &zero);
		tempVect.v[Z] = FindShortestAngleSigned(cameraInfo.cornerAngle, camera.rot.v[Y]);
		if(fabs(tempVect.v[Z]) < 0.05)
			cameraInfo.cornerCounter = 0;
		tempVect.v[Z] *= 0.5;
		RotateVector2D(&tempVect, &tempVect, Aabs(camera.rot.v[Y] - PI/2));
		ActorAddVelocity(&camera, &tempVect);


	}
	else
		ProcessCameraCases();			
			
	
	if(hand.action == HAND_JOINED)
		actor = &ball;
	if((cameraInfo.positionLocked == FALSE) && (cameraInfo.overrideCounter == 0))
	{
//sort out automatic height thingy
		x = 0;
		val = 0;

		while(actor->plane[x] != NULL)
		{
			val = 1 - actor->plane[x]->normal.v[Y];
			if(val < bestSlope)
			{
				bestSlope = val;
				plane = actor->plane[x];
			}
			x++;
		}



		if(plane)
		{
			float perfectDist, actualDist;
			VECTOR tempVect;
			VECTOR normalVect;
			//test distance to point exactly above hand
			normalVect.v[X] = normalVect.v[Z] = 0;
			normalVect.v[Y] = 1;
			AddVector(&tempVect, &normalVect, &actor->pos);
			perfectDist = DistanceBetweenPoints(&tempVect, &camera.pos);

			//test distance to point projected from face normal
			AddVector(&tempVect, &plane->normal, &actor->pos);
			actualDist = DistanceBetweenPoints(&tempVect, &camera.pos);
			
			val = (actualDist - perfectDist) * 15;

			if(val >= 0)
				val = Min(val, 9);
			else
				val = Max(val, -6);

//handle hi/lo camera overrides
		cameraInfo.positionBias.v[Y] = val - 1;
		cameraInfo.positionBias.v[Y] += cameraInfo.additionalHeight;

//handle near/far camera overrides
		if((cameraInfo.distanceFromHand - 16) > cameraInfo.additionalDistance)
			cameraInfo.distanceFromHand -= 0.5;
		if((cameraInfo.distanceFromHand - 16) < cameraInfo.additionalDistance)
			cameraInfo.distanceFromHand += 0.5;


		}
	}


//make camera face the garib
	SubVector(&temp, &camera.pos, &cameraInfo.garib.pos);
	val = atan2(temp.v[X], temp.v[Z]);
	camera.rot.v[Y] = Aabs(val + PI);	

//make camera frame lookat the garib
	TRY_MYD3DRM(camera.frame->LookAt(cameraInfo.garib.frame, g_lpScene, D3DRMCONSTRAIN_Z));
	val = Magnitude2D(&temp);



	if(cameraInfo.positionLocked == TRUE)
	{
		cameraInfo.positionBias.v[Y] = 0;
		SubVector(&temp, &camera.pos, &cameraInfo.lockedPosition);
		ScaleVector(&temp, 0.25);
		temp.v[Y] *= 2;

		if(MagnitudeSquared(&temp) > 4)
		{
			MakeUnit(&temp);
			ScaleVector(&temp, 2);
		}

//			SubVector(&camera.vel, &camera.vel, &temp);
		SubVector(&camera.pos, &camera.pos, &temp);
	}
	else if(cameraInfo.overrideCounter == 0)
	{

//----------super forced rotation code
		if(cameraInfo.overrideRotation == TRUE)
		{
			cameraInfo.preferedRotation = cameraInfo.overriddenRotation;

			SubVector(&tempVect, &hand.pos, &camera.pos);
			angle = atan2(tempVect.v[X], tempVect.v[Z]);
			angle -= cameraInfo.overriddenRotation;

			if(angle > 0)
			{
				SetVector(&tempVect, &zero);
				tempVect.v[Z] = Min(angle, cameraInfo.overrideRotationSpeed);// / 4;
				RotateVector2D(&tempVect, &tempVect, Aabs(camera.rot.v[Y] + PI/2));
				ActorAddVelocity(&camera, &tempVect);
			}
			else
			{
				SetVector(&tempVect, &zero);
				tempVect.v[Z] = Max(-angle, -cameraInfo.overrideRotationSpeed);// / 4;
				RotateVector2D(&tempVect, &tempVect, Aabs(camera.rot.v[Y] - PI/2));
				ActorAddVelocity(&camera, &tempVect);
			}
		}


		if(val > cameraInfo.distanceFromHand + 1)
			AccelerateActor2D(&camera, Min(1, (val - (cameraInfo.distanceFromHand+1))/2));
		if(val < cameraInfo.distanceFromHand - 1)
			AccelerateActor2D(&camera, -Min(1, ((cameraInfo.distanceFromHand - 1) - val)/2));


//normal camera type
		if(cameraInfo.type == 0)
		{
			SubVector(&tempVect, &hand.pos, &camera.pos);
			cameraInfo.preferedRotation = atan2(tempVect.v[X], tempVect.v[Z]);
			
			//make camera swing round if hand is turning
			if(!cameraInfo.overrideRotation)
			{
				if(Magnitude2D(&hand.vel) > 0.2)
				{
					angle = FindShortestAngleSigned(camera.rot.v[Y], Aabs(hand.rot.v[Y] + PI));
					if((fabs(angle) > 0.7) && (fabs(angle) < PI - 0.7))
					{
						if(angle > 0)
						{
							SetVector(&tempVect, &zero);
							tempVect.v[Z] = 0.12;
							RotateVector2D(&tempVect, &tempVect, Aabs(camera.rot.v[Y] + PI/2));
							ActorAddVelocity(&camera, &tempVect);
						}
						else
						{
							SetVector(&tempVect, &zero);
							tempVect.v[Z] = 0.12;
							RotateVector2D(&tempVect, &tempVect, Aabs(camera.rot.v[Y] - PI/2));
							ActorAddVelocity(&camera, &tempVect);

						}
					}

				}
			}

			//make camera swing round if hand stationary
			
			if((!cameraInfo.overrideRotation) && (cameraInfo.swingBehind))
			{
				if(joystick.magnitude == 0)
				{
					angle = FindShortestAngleSigned(camera.rot.v[Y], Aabs(hand.rot.v[Y] + PI));
//					if((fabs(angle) > 0.1) && (fabs(angle) < PI - 0.1))
					{
						if(angle > 0)
						{
							SetVector(&tempVect, &zero);
							tempVect.v[Z] = Min(angle, 0.2);
							RotateVector2D(&tempVect, &tempVect, Aabs(camera.rot.v[Y] + PI/2));
							ActorAddVelocity(&camera, &tempVect);
						}
						else
						{
							SetVector(&tempVect, &zero);
							tempVect.v[Z] = Min(-angle, 0.2);
							RotateVector2D(&tempVect, &tempVect, Aabs(camera.rot.v[Y] - PI/2));
							ActorAddVelocity(&camera, &tempVect);

						}
					}

				}
			}


		}

//parallel cam
		if(cameraInfo.type == 1)
		{
			if(cameraInfo.overrideRotation == FALSE)
			{
				SubVector(&tempVect, &hand.pos, &camera.pos);
				angle = atan2(tempVect.v[X], tempVect.v[Z]);
				angle = FindShortestAngleSigned(angle, cameraInfo.preferedRotation);
	//			angle -= cameraInfo.preferedRotation;

				if(angle > 0)
				{
					SetVector(&tempVect, &zero);
					tempVect.v[Z] = angle;// / 4;
					RotateVector2D(&tempVect, &tempVect, Aabs(camera.rot.v[Y] + PI/2));
					ActorAddVelocity(&camera, &tempVect);
				}
				else
				{
					SetVector(&tempVect, &zero);
					tempVect.v[Z] = -angle;// / 4;
					RotateVector2D(&tempVect, &tempVect, Aabs(camera.rot.v[Y] - PI/2));
					ActorAddVelocity(&camera, &tempVect);
				}
			}
		}
	}

	if(cameraInfo.overrideCounter == 0)
	{
		temp.v[Y] -= (cameraInfo.distanceFromHand / 3) + cameraInfo.positionBias.v[Y];
//		if((Altitude(&camera, &plane) > 5) || (temp.v[Y] <= 0))
		camera.pos.v[Y] -= temp.v[Y] /8;
	}
}

