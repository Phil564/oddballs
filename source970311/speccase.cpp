/*
 *  Project:         
 *  File:            
 *  Description:     
 *
 *  (c)1996 ISL.
 */

#include "oddballs.h"
#include "speccase.h"
#include "hand.h"
#include "platforms.h"
#include "collectables.h"
#include "cam.h"
#include "actors.h"
#include "spikes.h"
#include "personal.h"
#include "sound.h"
#include "timegates.h"

VECTOR shipFocalPoint;
ACTOR		*seaActor;


struct
{
	int			timesCalled;
	BOOL		switchPuzzle1Completed;
	int			switchPuzzleDelay;
	int			teleportBodge;
	int			gateBodge;
	int			gateBodgeTimer;
	BOOL		volcanoSwitchDone;
	int			teleportBodge2;
	int			stepCounter;
	int			chickenRunBodge;
	int			rotSwitch;
	VECTOR		seaOffset;
	int			handDeathCounter;

}myBodge;




/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void CheckSpecialCases()
{
	PLATFORM			*platform;
	PLATFORMLISTENTRY	*platformPtr;
	VECTOR				tempVect;	
	VECTOR				tempVect2;	
	int					x = 0;
	int					switchPuzzle1Count = 0;
	double				temp;	
	ACTOR				*actor;

	if((hand.pos.v[Y] < -200) && (!myBodge.handDeathCounter))
	{
		PlaySample(SFX_HANDDROWN, 0, &hand.pos,100);

		myBodge.handDeathCounter = 20;
		DisableControl(20);
	}
	if((ball.pos.v[Y] < -200) && (!IsHandHoldingBall()))
	{
		RestartBall();
		LoseLife();
	}
	if(myBodge.handDeathCounter)
	{
		SetVector(&tempVect, &hand.pos);
		tempVect.v[Y] = 1.78;
		CreateSplashCircle(&tempVect, 2, 0);
		if(--myBodge.handDeathCounter == 0)
		{
			myBodge.gateBodge = FALSE;
			RestartHand();
			RestartCamera();
			LoseLife();
			if(handControl.lastTimeGate != -1)
			{
				SetVector(&hand.pos,&timeGate[handControl.lastTimeGate].pos);
				hand.pos.v[Y] += hand.radius;
				SetVector(&camera.pos,&cameraInfo.lastTimeGatePos);
				SetVector(&cameraInfo.garib.pos,&cameraInfo.garibLastTimeGatePos);
			}
			if(ball.owner == &hand)
			{
				RestartBall();
				if(handControl.lastTimeGate != -1)
					SetVector(&ball.pos,&hand.pos);
			}
		}
	}	

//	return;
/*
	if(cameraInfo.overrideCounter == 0)
	{
		if(IsPointWithinBox(39, 39+22, -15, -15+20, 173, 173+11, &hand.pos))
		{
			cameraInfo.overrideRotation = TRUE;
			cameraInfo.overriddenRotation = -0.52;
		}
		else
		if(IsPointWithinBox(45, 45+38, -102, -102+26, 200, 200+16, &hand.pos))
		{
			cameraInfo.overrideRotation = TRUE;
			cameraInfo.overriddenRotation = -0.2;
		}
		else
		if(IsPointWithinBox(-8, -8+16, 11, 11+13, 159, 159+10, &hand.pos))
		{
			cameraInfo.overrideRotation = TRUE;
			cameraInfo.overriddenRotation = 2;
		}
		else
		if(IsPointWithinBox(-8, -8+16, 11, 11+13, 148, 148+10, &hand.pos))
		{
			cameraInfo.overrideRotation = TRUE;
			cameraInfo.overriddenRotation = 1;
		}
		else
			cameraInfo.overrideRotation = FALSE;

		if(IsPointWithinBox(-2, -2+45, -67, -67+16, 206, 206+32, &hand.pos))
		{
			cameraInfo.loCamera = FALSE;
			cameraInfo.hiCamera = TRUE;
		}
		else
		{
			cameraInfo.hiCamera = FALSE;
			cameraInfo.loCamera = FALSE;
		}



	}

*/
	return;


//move the sea
	if(seaActor != NULL)
	{
		myBodge.seaOffset.v[X] += 0.01;
		myBodge.seaOffset.v[Z] += 0.02;
		myBodge.seaOffset.v[Y] += 0.005;

		seaActor->pos.v[X] += (sin(myBodge.seaOffset.v[X])) / 10;
		seaActor->pos.v[Z] += (sin(myBodge.seaOffset.v[Z])) / 10;

		seaActor->frame->SetPosition(g_lpScene, seaActor->pos.v[X],seaActor->pos.v[Y],seaActor->pos.v[Z]); 
	}



//spit fire out of my hole
	if(!(frameCount % 20))
	{
		if(rand()%2)
		{
			//position vector
			tempVect.v[X] = -88.7; tempVect.v[Y] = 37.1; tempVect.v[Z] = 109.2;

			//velocity vector
			tempVect2.v[X] = (rand() % 10) + 4;
			tempVect2.v[X] /= 2;
			tempVect2.v[Y] = (rand() % 10) + 4;
			tempVect2.v[Y] /= 2;
			tempVect2.v[Z] = 0;
			RotateVector2D(&tempVect2, &tempVect2, rand() % 5);
			AddDebris(FIREBALL, &tempVect, &tempVect2);
		}
	}


//generate splashes on water
	if(hand.pos.v[Y] <= 2.78)
	{
		if(((IsHandHoldingBall()) && (ball.speed > 0.2)) || ((!IsHandHoldingBall()) && (hand.speed > 0.2)))
		{
			if(!(frameCount % 5))
			{
				SetVector(&tempVect, &hand.pos);
				tempVect.v[Y] = 1.78;
				CreateSplashCircle(&tempVect, 2, 0);
//				PlaySample(SFX_SPLASH2, 0, &hand.pos);
			}
		}
	}
//generate splashes on oasis
	if((hand.pos.v[X] >= 36) && (hand.pos.v[X] <= 64) && (hand.pos.v[Z] >= -99) && (hand.pos.v[Z] <= -69.6))
	{
		if(hand.pos.v[Y] <= 7.93)
		{
			if(hand.speed > 0.2)
			{
				if(!(frameCount % 5))
				{
					SetVector(&tempVect, &hand.pos);
					tempVect.v[Y] = 7.5;
					CreateSplashCircle(&tempVect, 2, 0);
//					PlaySample(SFX_SPLASH2, 0, &hand.pos);
				}
			}
		}
	}
//generate splashes on water
	if(hand.action != HAND_JOINED)
	{

		if(ball.pos.v[Y] <= 2.78)
		{
			if(ball.speed > 0.2)
			{
				if(!(frameCount % 5))
				{
					SetVector(&tempVect, &ball.pos);
					tempVect.v[Y] = 1.78;
					CreateSplashCircle(&tempVect, 1, 0);
//					PlaySample(SFX_SPLASH2, 0, &hand.pos);
				}
			}
		}
	//generate splashes on oasis
		if((ball.pos.v[X] >= 36) && (ball.pos.v[X] <= 64) && (ball.pos.v[Z] >= -99) && (ball.pos.v[Z] <= -69.6))
		{
			if(ball.pos.v[Y] <= 7.93)
			{
				if(ball.speed > 0.2)
				{
					if(!(frameCount % 5))
					{
						SetVector(&tempVect, &ball.pos);
						tempVect.v[Y] = 7.5;
						CreateSplashCircle(&tempVect, 1, 0);
//						PlaySample(SFX_SPLASH2, 0, &hand.pos);
					}
				}
			}
		}
	}




//	move platform if hand slams it
	if((hand.platform != NULL) && (hand.platform->counter == -2) && ((hand.action == HAND_SLAM) || (hand.action == HAND_SLAM2ST)))
	{
		hand.platform->counter = 1;
	}

		
	
			
//	handle crows nest
	platformPtr = FindTaggedPlatform(&platformList.head, 6);
	if((platformPtr != NULL) && (platformPtr->platform))
	{
		platform = platformPtr->platform;
		temp = platform->rotvel.v[Z];
		platformPtr = FindTaggedPlatform(&platformList.head, 5);
		platform = platformPtr->platform;
		platform->pos.v[Y] += temp * 3;
		if(platform->pos.v[Y] < 19.4)
			platform->pos.v[Y] = 19.4;
		if(platform->pos.v[Y] > 48)
			platform->pos.v[Y] = 48;
	}


//====================================================================
//huge switch bodge
//====================================================================

//	set all switch platforms to NOTSPIN	
	for(x = 2; x < 5; x++)
	{
		platformPtr = FindTaggedPlatform(&platformList.head, x);
		if(platformPtr != NULL)
		{
			platform = platformPtr->platform;
			if((platform) && (platform->rotvel.v[Y] == 0))
				platform->spinSpeed = 0;
		}
	}


	if(myBodge.switchPuzzle1Completed == FALSE)
	{
		if(/*(hand.action == HAND_SLAM) || */(hand.action == HAND_SLAM2ST))
		{
			if(hand.platform != NULL)
			{
				if( (hand.platform->tag == 2) || (hand.platform->tag == 3) || (hand.platform->tag == 4) )
				{
					if(hand.platform->spinSpeed == 0)
					{
						PlaySample(SFX_SNAPPY, 0, &hand.platform->pos,100);
						hand.platform->spinSpeed = 0.2;
						hand.platform->spinCounter = 1;
					}
				}
			}

		}

//		check to see switches are aligned
		platformPtr = FindTaggedPlatform(&platformList.head, 2);
		if((platformPtr != NULL) && (platformPtr->platform) && ((platformPtr->platform->rot.v[Y] == PI/2) || (platformPtr->platform->rot.v[Y] == PI+PI/2)))
		{
			switchPuzzle1Count++;
		}
		platformPtr = FindTaggedPlatform(&platformList.head, 3);
		if((platformPtr != NULL) && (platformPtr->platform) && ((platformPtr->platform->rot.v[Y] == 0) || (platformPtr->platform->rot.v[Y] == PI)))
		{
			switchPuzzle1Count++;
		}
		platformPtr = FindTaggedPlatform(&platformList.head, 4);
		if((platformPtr != NULL) && (platformPtr->platform) && ((platformPtr->platform->rot.v[Y] == PI/2) || (platformPtr->platform->rot.v[Y] == PI+PI/2)))
//		if((platformPtr->platform) && (platformPtr->platform->rot.v[Y] == PI))
		{
			switchPuzzle1Count++;
		}
		if(switchPuzzle1Count == 3)
		{
			dprintf("Puzzle Completed!\n");
			myBodge.switchPuzzle1Completed = TRUE;
			platformPtr = FindTaggedPlatform(&platformList.head, 7);
			if(platformPtr != NULL)
			{
	 			platform = platformPtr->platform;
				platform->counter = 5;
//				platform->nextPoint = (platform->nextPoint + 1) MOD platform->numPathPoints;
				myBodge.switchPuzzleDelay = 4;

			//look at platform
				SetCameraOverride(camera.pos.v[X], camera.pos.v[Y], camera.pos.v[Z], &platform->pos,&platform->vel, 20);
				PlaySample(SFX_MOTOR, 0, &platform->pos,100);
			}
		}
	}

	if(myBodge.switchPuzzleDelay > 0)
	{
		myBodge.switchPuzzleDelay--;
		if(myBodge.switchPuzzleDelay == 0)
		{
			CreateStarCircleXYZ( 50.21, 9, 111.28, 8);
			CreateStarCircleXYZ( 50.21, 11,111.28, 8);

		}
	}
		

	if((hand.platform) && (hand.platform->tag == 7) && (myBodge.stepCounter == 0))
	{
		if(hand.platform->currentPoint == 1)
		{
			if(hand.action == HAND_SLAM2ST)
			{
				platformPtr = &platformList.head;
				do
				{
					platformPtr = FindTaggedPlatform(platformPtr, 10);
					if(platformPtr != NULL)
						platformPtr->platform->counter = 20;
					
				}while(platformPtr);
				myBodge.stepCounter = 20;
				PlaySample(SFX_SWITCH, 0, &hand.platform->pos,100);
			}
		}
	}

	if(myBodge.stepCounter)
	{
		myBodge.stepCounter--;
		if(myBodge.stepCounter == 0)
		{
			platformPtr = FindTaggedPlatform(&platformList.head, 10);
			if(platformPtr != NULL)
			{
				SetCameraOverride(37,24,45,&platformPtr->platform->pos,&platformPtr->platform->vel,30);
				PlaySample(SFX_MOTOR,0,&platform->pos,100);
			}
		}
	}

//====================================================================
//huge chicken run
//====================================================================
	platformPtr = FindTaggedPlatform(&platformList.head, 12);
	if((platformPtr != NULL) && (platformPtr->platform->currentPoint == 1))
	{
		if(myBodge.chickenRunBodge == 0)
		{
			myBodge.chickenRunBodge = 300;
			InitSpikePuzzle();
			PlaySample(SFX_SPIKESWITCH, 0, &platformPtr->platform->pos,100);
		}

	}
	if(myBodge.chickenRunBodge)
		myBodge.chickenRunBodge--;
//====================================================================
//huge gate bodge
//====================================================================
//	repel hand from front of hole

	//only do this if the gate has not been opened
	platformPtr = FindTaggedPlatform(&platformList.head, 1);
	if(platformPtr != NULL)
	{
	 	platform = platformPtr->platform;
		if(platform->currentPoint == 2)
		{
			DxlStopSampleDirect(&sounds[SFX_GATEOPEN]);
		}
		if((platform->currentPoint == 1) && (platform->counter == -10))
		{
		
			tempVect.v[X] = 19.65; tempVect.v[Y] = 9; tempVect.v[Z] = 75.55;
			if(DistanceBetweenPoints(&hand.pos, &tempVect) < 1)
			{
				if(hand.action == HAND_JOINED)
				{
					ball.vel.v[Z] -= 2;
				}
				else
					hand.vel.v[Z] -= 2;

			}
		//	repel hand from back of hole
			tempVect.v[X] = 19.65; tempVect.v[Y] = 9; tempVect.v[Z] = 76.8;
			if(DistanceBetweenPoints(&hand.pos, &tempVect) < 1)
			{
				if(hand.action == HAND_JOINED)
				{
					ball.vel.v[Z] += 1;
				}
				else
					hand.vel.v[Z] += 1;

			}
		}
	}

//	if ball has passed through hole, open gate
	tempVect.v[X] = 19.73; tempVect.v[Y] = 9; tempVect.v[Z] = 76.59;
	if(DistanceBetweenPoints(&ball.pos, &tempVect) < 1)
	{
		platformPtr = FindTaggedPlatform(&platformList.head, 1);
		if(platformPtr != NULL)
		{
			platform = platformPtr->platform;
			if(platform->currentPoint == 1)
			{
				if(platform->counter == -10)
				{
					platform->counter = 1;
					PlaySample(SFX_GATEOPEN, 1, &platform->pos,100);
				}
			}
		}
	}

//====================================================================
//airbourne switch
//====================================================================

	platformPtr = FindTaggedPlatform(&platformList.head, 14);
	if(platformPtr != NULL)
	{
		platform = platformPtr->platform;
		if(platform->rotaim.v[X] != platform->initRot)
		{		
			PlaySample(SFX_SWITCH, 0, &platform->pos,100);

			platform->initRot = platform->rotaim.v[X];		
			platformPtr = FindTaggedPlatform(&platformList.head, 1);
			platform = platformPtr->platform;
	
			tempVect.v[X] = 91;
			tempVect.v[Y] = 10;
			tempVect.v[Z] = 34.7;
			SetVector(&ball.pos, &tempVect);
			SetVector(&cameraInfo.garib.pos, &ball.pos);
			tempVect.v[X] -= 20;
			SetVector(&camera.pos, &tempVect);			
			ApplyAction(HAND_JOINED);

			handControl.status = HAND_TELEPORTING;
			hand.lifetime = 100;

			PlaySample(SFX_GATEOPEN, 0, &platform->pos,100);


			SetCameraOverride(platform->pos.v[X],platform->pos.v[Y] + 10,platform->pos.v[Z] - 40,&platform->pos,&platform->vel,50);
			if((platform->currentPoint == 0) && (platform->counter == -10))
				platform->counter = 1;
		}
	}

//====================================================================
//huge rotating platforms bodge
//====================================================================
	
	if(myBodge.rotSwitch == FALSE)
	{
		if((ball.platform) && (ball.platform->tag == 11))
		{
			if(ball.platform->currentPoint == 1)
			{
				myBodge.rotSwitch = TRUE;
				PlaySample(SFX_SPIKESWITCH, 0, &hand.pos,100);
				platformPtr = &platformList.head;
				do
				{
					platformPtr = FindTaggedPlatform(platformPtr,13);
					if(platformPtr != NULL)
						platformPtr->platform->spinSpeed = 0.1;
				}while(platformPtr);
			}
		}
	}

//====================================================================
//huge teleport bodge
//====================================================================
	if(IsHandHoldingBall() == TRUE)
		actor = &ball;
	else
		actor = &hand;


	tempVect.v[X] = -49; tempVect.v[Z] = 58;
	if((DistanceBetweenPoints2D(&hand.pos, &tempVect) < 4) && (hand.pos.v[Y] < 25))
//	if( ((int)hand.pos.v[X] == -49) && ((int)hand.pos.v[Y] < 30) && ((int)hand.pos.v[Z] == 58) )
	{
		if(myBodge.teleportBodge == 0)
		{
			myBodge.teleportBodge = 130 + 50;
			cameraInfo.positionLocked = TRUE;
			cameraInfo.lockedPosition.v[X] = -100;
			cameraInfo.lockedPosition.v[Y] = 91;
			cameraInfo.lockedPosition.v[Z] = 43;
		}
	}
	if(myBodge.teleportBodge > 0)
	{
		myBodge.teleportBodge--;
		if(myBodge.teleportBodge == 80 + 70)
		{
			actor->pos.v[Y] = 29;
			actor->vel.v[Y] = 2;
		}
		if((myBodge.teleportBodge <= 80 + 70) && (myBodge.teleportBodge >= 50))
		{
			if(actor == &ball)
				actor->maxSpeed = 1.2;
			else				
				actor->maxSpeed = 1.5;

			tempVect.v[X] = -114;
			tempVect.v[Z] = 165;
			SubVector(&tempVect2, &tempVect, &actor->pos);
			MakeUnit2D(&tempVect2);
			ScaleVector(&tempVect, 2);
			tempVect2.v[Y] = 0.14;
			AddVector(&actor->vel, &actor->vel, &tempVect2);
			
//			if(frameCount & 1)
			{
				AddDebris(MAGIC, &actor->pos, &zero);
			}
			DisableControl(10);
			if(myBodge.teleportBodge == 0 + 50)
			{
				handControl.inputDisabledTimer = 0;
				cameraInfo.positionLocked = FALSE;
				//reset max speed
				if(actor == &hand)
					actor->maxSpeed = 1;
				else
					actor->maxSpeed = 2.5;
			}
		}
		if(myBodge.teleportBodge == 0)
			ball.bounce = 0.8;
		else
			ball.bounce = 0.5;

	}		


//====================================================================
//huge hand teleport bodge(S)
//====================================================================
	if((hand.platform) && (hand.platform->tag == 11) && (myBodge.teleportBodge2 == 0) && (hand.action == HAND_JOINED) && (MagnitudeSquared(&hand.vel) < 0.01))
	{

		myBodge.teleportBodge2 = 21;
		DisableControl(21);
		handControl.status = HAND_TELEPORTING;
		hand.lifetime = 50;
		PlaySample(SFX_TELEPORT, 0, &hand.pos,100);

	}
	if(myBodge.teleportBodge2 > 0)
	{
		SetVector(&hand.vel, &zero);
		myBodge.teleportBodge2--;
		if(!(myBodge.teleportBodge2 % 5))
		{
			CreateStarCircle(&hand.pos, 4); 
		}

		if(myBodge.teleportBodge2 == 0)
		{
			handControl.inputDisabledTimer = 0;
			ball.pos.v[X] = 76;
			ball.pos.v[Y] = 40;
			ball.pos.v[Z] = -88;
			MoveHandToBall();
//			ApplyAction(HAND_JUMP);
			SetVector(&cameraInfo.garib.pos, &ball.pos);
			userSurface[GAMEWON_ICON].display = TRUE;		
			userSurface[LIVESGONE_ICON].display = FALSE;		
			userSurface[TIMEOUT_ICON].display = FALSE;		
			player.messageTimer = 100;
		}
	
	}
//====================================================================
//huge volcano switch bodge(S)
//====================================================================
	if((ball.platform) && (ball.platform->targetTag))
	{
		platformPtr = FindTaggedPlatform(&platformList.head,ball.platform->targetTag);
		if(platformPtr != NULL)
		{
			platform = platformPtr->platform;
			if(ball.platform->currentPoint == 1)
			{
				if(myBodge.volcanoSwitchDone == FALSE)
				{
					myBodge.volcanoSwitchDone = TRUE;

					tempVect.v[X] = -58;
					tempVect.v[X] = 49;
					tempVect.v[X] = 100;
					SetCameraOverride(-58,49,100,&platform->pos,&platform->vel,50);

					SetVector(&ball.pos, &ball.platform->pos);
					SetVector(&hand.pos, &ball.platform->pos);
					ball.pos.v[Y] += 1;
					hand.pos.v[Y] += 1;
					SetVector(&ball.vel, &zero);
					SetVector(&hand.vel, &zero);


				}
			}


			if(platform->counter == -10)
			{
				PlaySample(SFX_SWITCH, 0, &ball.pos,100);
				platform->counter = 20;
//				platform->nextPoint = (platform->nextPoint + 1) MOD platform->numPathPoints;
			}
		}
	}




//====================================================================
//huge camera bodge(S)
//====================================================================
	if((myBodge.teleportBodge == 0) && (cameraInfo.overrideCounter == 0))
		cameraInfo.positionLocked = FALSE;

	if(cameraInfo.overrideCounter == 0)
	{
		if( (hand.pos.v[X] >= 211) && (hand.pos.v[X] <= 250) && (hand.pos.v[Z] < 26) && (hand.pos.v[Z] > -32) )
		{
			//hand is by crows nest
			cameraInfo.positionLocked = TRUE;
			cameraInfo.lockedPosition.v[X] = 184;
			cameraInfo.lockedPosition.v[Y] = 41;
			cameraInfo.lockedPosition.v[Z] = 1.6;
	//		cameraInfo.positionToTrack = &hand.pos;
		}
		if( (hand.pos.v[X] >= 211) && (hand.pos.v[X] <= 250) && (hand.pos.v[Z] < -32) && (hand.pos.v[Z] > -51) )
		{
			//hand is by ships wheel
			cameraInfo.lockedPosition.v[X] = 189;
			cameraInfo.lockedPosition.v[Y] = 57;
			cameraInfo.lockedPosition.v[Z] = -84;
			cameraInfo.positionLocked = TRUE;
		}
	}



	//hand is by tower
	tempVect2.v[X] = -86; tempVect2.v[Z] = -116; tempVect.v[Y] = 0;
	if(DistanceBetweenPoints2D(&hand.pos, &tempVect2) < 25)
	{
		if(hand.pos.v[Y] < 37)
		{
			SubVector2D(&tempVect, &hand.pos, &tempVect2);
			MakeUnit2D(&tempVect);
			ScaleVector(&tempVect, 40);
			AddVector2D(&cameraInfo.lockedPosition, &tempVect2, &tempVect);
			cameraInfo.lockedPosition.v[Y] = hand.pos.v[Y] + 4;
			cameraInfo.positionLocked = TRUE;
		}
	}

	//hand is by finish tower
	tempVect2.v[X] = 76; tempVect2.v[Z] = -87; tempVect.v[Y] = 0;
	if(DistanceBetweenPoints2D(&hand.pos, &tempVect2) < 15)
	{
		if(hand.pos.v[Y] < 30)
		{
			SubVector2D(&tempVect, &hand.pos, &tempVect2);
//			MakeUnit2D(&tempVect);
			ScaleVector(&tempVect, 3);
			AddVector2D(&cameraInfo.lockedPosition, &tempVect2, &tempVect);
			cameraInfo.lockedPosition.v[Y] = hand.pos.v[Y] + 4;

			cameraInfo.positionLocked = TRUE;
			

		}
	}


	if( (hand.pos.v[X] >= -64) && (hand.pos.v[X] <= -38) && (hand.pos.v[Z] >= 37) && (hand.pos.v[Z] <= 73) )
	{
		//penal tower
		if(myBodge.teleportBodge == 0)
		{
			if((hand.pos.v[Y] >= 18) && (hand.pos.v[Y] <30))
			{
				tempVect2.v[X] = -49;
				tempVect2.v[Y] = 0;
				tempVect.v[Y] = 0;
				tempVect2.v[Z] = 58;
				SubVector2D(&tempVect, &hand.pos, &tempVect2);
				MakeUnit2D(&tempVect);
				ScaleVector(&tempVect, 35);
				AddVector2D(&cameraInfo.lockedPosition, &tempVect2, &tempVect);
				cameraInfo.lockedPosition.v[Y] = hand.pos.v[Y] + 8;

				cameraInfo.positionLocked = TRUE;
			}
		}
	}

//massive bodge for camera behind the gate	
	if( (hand.pos.v[X] >= 12) && (hand.pos.v[X] <= 26) && (hand.pos.v[Z] >= 72) && (hand.pos.v[Z] <= 81))
	{
		if(hand.pos.v[Z] > 76)
			myBodge.gateBodge = TRUE;
		else
		{
			if(myBodge.gateBodge == TRUE)
			{
				myBodge.gateBodgeTimer = 10;

			}
			myBodge.gateBodge = FALSE;		
		}
	}				   
	if((myBodge.gateBodge == TRUE) && (cameraInfo.overrideCounter == 0))
	{
		cameraInfo.positionLocked = TRUE;
		cameraInfo.lockedPosition.v[X] = -5;
		cameraInfo.lockedPosition.v[Y] = 29;
		cameraInfo.lockedPosition.v[Z] = 85;

	}
	if(myBodge.gateBodgeTimer > 0)
	{
		myBodge.gateBodgeTimer--;
		SetVector(&tempVect, &zero);
		tempVect.v[Z] = 1;
		RotateVector2D(&tempVect, &tempVect, Aabs(camera.rot.v[Y] + PI/2));
		ScaleVector(&tempVect, 2);
		ActorAddVelocity(&camera, &tempVect);
	}


//-----------------------
//      massive rotating platform bodge!
//-----------------------

	if((hand.pos.v[X] <= 41) && (hand.pos.v[X] >= -5.6) && (hand.pos.v[Z] >= 17.5) && (hand.pos.v[Z] <= 33.6))
	{
		if(hand.pos.v[Y] >= 23)
		{
			if(hand.pos.v[Z] >= 25.67)
			{
				cameraInfo.positionLocked = TRUE;
				cameraInfo.lockedPosition.v[X] = 2;
				cameraInfo.lockedPosition.v[Y] = 39;
				cameraInfo.lockedPosition.v[Z] = 57;
			}
  /*			else
			{
				cameraInfo.positionLocked = TRUE;
				cameraInfo.forcedPosition.v[X] = 2;
				cameraInfo.forcedPosition.v[Y] = 39;
				cameraInfo.forcedPosition.v[Z] = -1;

			}
*/
		}

	}







	myBodge.timesCalled++;
}




void RestartPuzzels()
{
	PLATFORM *platform;
	PLATFORMLISTENTRY *platformPtr;

	ResetCollectables();


	memset(&myBodge, 0, sizeof(myBodge));

//reset three rotating switches
	platformPtr = FindTaggedPlatform(&platformList.head, 2);
	if(platformPtr)
	{
		platformPtr->platform->rot.v[Y] = 0;
		platformPtr->platform->rotaim.v[Y] = 0;
		platformPtr->platform->spinNextPoint = 1;
	}
	platformPtr = FindTaggedPlatform(&platformList.head, 3);
	if(platformPtr)
	{
		platformPtr->platform->rot.v[Y] = 0;
		platformPtr->platform->rotaim.v[Y] = 0;
		platformPtr->platform->spinNextPoint = 1;
	}
	platformPtr = FindTaggedPlatform(&platformList.head, 4);
	if(platformPtr)
	{
		platformPtr->platform->rot.v[Y] = 0;
		platformPtr->platform->rotaim.v[Y] = 0;
		platformPtr->platform->spinNextPoint = 1;
	}
//airbourne switch
	platformPtr = FindTaggedPlatform(&platformList.head, 14);
	if(platformPtr)
	{
		platformPtr->platform->rot.v[X] = 0;
		platformPtr->platform->rotaim.v[X] = 0;
		platformPtr->platform->spinNextPoint = 1;
	}
//gate
	platformPtr = FindTaggedPlatform(&platformList.head, 1);
	if(platformPtr)
	{
		SetVector(&platformPtr->platform->pos, platformPtr->platform->pathPoint[0]);
		platformPtr->platform->currentPoint = 0;
		platformPtr->platform->nextPoint = 0;
	}
//pop up switch
	platformPtr = FindTaggedPlatform(&platformList.head, 7);
	if(platformPtr)
	{
		SetVector(&platformPtr->platform->pos, platformPtr->platform->pathPoint[0]);
		platformPtr->platform->currentPoint = 0;
		platformPtr->platform->nextPoint = 0;
	}
//steps
	platformPtr = FindTaggedPlatform(&platformList.head, 10);
	if(platformPtr)
	{
		SetVector(&platformPtr->platform->pos, platformPtr->platform->pathPoint[0]);
		platformPtr->platform->currentPoint = 0;
		platformPtr->platform->nextPoint = 0;
	}
	platformPtr = FindTaggedPlatform(platformPtr, 10);
	if(platformPtr)
	{
		SetVector(&platformPtr->platform->pos, platformPtr->platform->pathPoint[0]);
		platformPtr->platform->currentPoint = 0;
		platformPtr->platform->nextPoint = 0;
	}
	platformPtr = FindTaggedPlatform(platformPtr, 10);
	if(platformPtr)
	{
		SetVector(&platformPtr->platform->pos, platformPtr->platform->pathPoint[0]);
		platformPtr->platform->currentPoint = 0;
		platformPtr->platform->nextPoint = 0;
	}
//crows nest
	platformPtr = FindTaggedPlatform(&platformList.head, 5);
	if(platformPtr)
	{
		SetVector(&platformPtr->platform->pos, platformPtr->platform->pathPoint[0]);
	}
//rotating platforms
 
	platformPtr = &platformList.head;
	do
	{
		platformPtr = FindTaggedPlatform(platformPtr,13);
		if(platformPtr != NULL)
		{
			platformPtr->platform->rot.v[Y] = platformPtr->platform->rotaim.v[Y] = platformPtr->platform->initRot;
			platformPtr->platform->spinSpeed = 0;
			SetVector(&platformPtr->platform->rotvel, &zero);
		}
	}while(platformPtr);
 
	

}