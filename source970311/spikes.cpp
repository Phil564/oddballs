/*
 *  Project:         oddballs
 *  File:            spikes.cpp
 *  Description:     handles all scenic items that hurt hand\ball
 *
 *  (c)1996 ISL.
 */

#include "oddballs.h"
#include "spikes.h"
#include "personal.h"
#include "hand.h"
#include "collectables.h"
#include "sound.h"

SPIKELIST spikeList;
LPDIRECT3DRMTEXTURE spikeTexture;



/*	--------------------------------------------------------------------------------
	Function 	: InitSpikes()
	Purpose 	: loads the spike bitmap into spiketexture, and initialises the position of
				  all spikes, and adds them to the list
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void InitSpikes()
{
	char	temp[256];
	
	sprintf(temp, "%s\\graphics\\scenery\\spike.ppm", rootPath);
	g_lpD3DRM->LoadTexture(temp, &spikeTexture);
	spikeTexture->SetDecalTransparency(TRUE);
	spikeTexture->SetDecalTransparentColor(RGB_MAKE(0,0,0));
	spikeTexture->SetDecalSize(2, 3);
	spikeTexture->SetDecalOrigin(15, 28);

}

/*	--------------------------------------------------------------------------------
	Function 	: TranslateSpikes()
	Purpose 	: goes through the spike list, and sets their positions
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void TranslateSpikes()
{
	SPIKELISTENTRY	*spikePtr;
	SPIKE			*spike;

	for(spikePtr = spikeList.head.next;spikePtr != &spikeList.head;spikePtr = spikePtr->next)
	{
		spike = spikePtr->spike;
		spike->frame->SetPosition(g_lpScene, spike->actualPos.v[X], spike->actualPos.v[Y], spike->actualPos.v[Z]);
	}	

}


/*	--------------------------------------------------------------------------------
	Function 	: UpdateSpikes()
	Purpose 	: moves spikes from present position to next position after delay
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void UpdateSpikes()
{
	SPIKELISTENTRY	*spikePtr;
	SPIKE			*spike;
	VECTOR			tempVect;
	VECTOR			tempVect2;
	double			temp;


	for(spikePtr = spikeList.head.next;spikePtr != &spikeList.head;spikePtr = spikePtr->next)
	{
		spike = spikePtr->spike;
		if(spike->numPositions > 1)
		{

			if(spike->counter > 0)
			{
				spike->counter--;
				if(spike->counter == 0)
				{
					spike->currentPosition = (spike->currentPosition + 1) MOD spike->numPositions;
					spike->counter = spike->posDelay[spike->currentPosition];
					if(spike->currentPosition == 1)
					{
						SetVector(&tempVect, &spike->actualPos);
						tempVect.v[Y] += 2;			
						SetVector(&tempVect2, &zero);
						tempVect2.v[Y] = 0.7;
						tempVect2.v[X] = 0.3;
						RotateVector2D(&tempVect2, &tempVect2, rand()%5);
						AddDebris(PLATBIT, &tempVect, &tempVect2);
			
						PlaySample(SFX_SPIKE, 0, &spike->actualPos,100);
					}

				}
				
			}
			SubVector(&tempVect, &spike->actualPos, &spike->pos[spike->currentPosition]);
			if(MagnitudeSquared(&tempVect) > spike->speed[spike->currentPosition] * spike->speed[spike->currentPosition])
			{
				MakeUnit(&tempVect);
				ScaleVector(&tempVect, spike->speed[spike->currentPosition]);
			}
			SubVector(&spike->actualPos, &spike->actualPos, &tempVect);
		}

		SetVector(&tempVect, &spike->actualPos);
		tempVect.v[Y] += 0.5;
		if(DistanceBetweenPoints(&hand.pos, &tempVect) < hand.radius + 0.4)
		{
			if((hand.speed > 0.2) || (hand.onGround == FALSE))
			{
				HurtHand(HURT_GO_TO_SAFE);
			}
		}
		if(DistanceBetweenPoints(&ball.pos, &spike->actualPos) < ball.radius + 0.4)
		{
			if(ballControl.status != BALL_BURST)
				BurstBall();
		}



	}	

}
/*	--------------------------------------------------------------------------------
	Function 	: InitSpikeList()
	Purpose 	: initialises the linked list of spikes
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void InitSpikeList()
{
	spikeList.numEntries = 0;
	spikeList.head.next = spikeList.head.prev = &spikeList.head;
}

/*	--------------------------------------------------------------------------------
	Function 	: AddPlatform()
	Purpose 	: adds a platform to the linked list
	Parameters 	: pointer to platform
	Returns 	: none
	Info 		:
*/
void AddSpike(SPIKE *spike)
{
	if(spike->listPointer == 0)
	{
		spikeList.numEntries++;
		spike->listPointer = (SPIKELISTENTRY *)MyMalloc(sizeof(SPIKELISTENTRY));
		spike->listPointer->prev = &spikeList.head;
		spike->listPointer->next = spikeList.head.next;
		spikeList.head.next->prev = spike->listPointer;
		spikeList.head.next = spike->listPointer;
		spike->listPointer->spike = spike;
	}
}


/*	--------------------------------------------------------------------------------
	Function 	: SubSpike()
	Purpose 	: removes an spike from the linked list
	Parameters 	: pointer to spike
	Returns 	: none
	Info 		:
*/
void SubSpike(SPIKE *spike)
{
	if(spike->listPointer != 0)
	{
		spike->listPointer->prev->next = spike->listPointer->next;
		spike->listPointer->next->prev = spike->listPointer->prev;
		MyFree((void **)&spike->listPointer);
		spikeList.numEntries--;
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: FindTaggedSpike()
	Purpose 	: finds a spike with specified tag
	Parameters 	: spike to start search from, tag number
	Returns 	: spike pointer of spike with correct tag
	Info 		:
*/
SPIKELISTENTRY *FindTaggedSpike(SPIKELISTENTRY *startSpike, int tag)
{
	SPIKELISTENTRY *spikePtr;
			 
 	for(spikePtr = startSpike->next; spikePtr != &spikeList.head; spikePtr = spikePtr->next)
	{
		if(spikePtr->spike->tag == tag)
		{
			return spikePtr;
//			spike = spikePtr->spike;
//			return spike;
		}	
	}
	
	return NULL;
}


void InitSpikePuzzle()
{
	SPIKELISTENTRY *spikePtr;
	int runStart = 90;
	int runStep = 3;

//first bunch of spikes
	spikePtr = &spikeList.head;
	do
	{
		spikePtr = FindTaggedSpike(spikePtr, 1);
		if(spikePtr)
			spikePtr->spike->counter = 20;
	}while(spikePtr);


	spikePtr = &spikeList.head;
	do
	{
		spikePtr = FindTaggedSpike(spikePtr, 2);
		if(spikePtr)
			spikePtr->spike->counter = 22;
	}while(spikePtr);

	spikePtr = &spikeList.head;
	do
	{
		spikePtr = FindTaggedSpike(spikePtr, 3);
		if(spikePtr)
			spikePtr->spike->counter = 24;
	}while(spikePtr);

	spikePtr = &spikeList.head;
	do
	{
		spikePtr = FindTaggedSpike(spikePtr, 4);
		if(spikePtr)
			spikePtr->spike->counter = 26;
	}while(spikePtr);

	spikePtr = &spikeList.head;
	do
	{
		spikePtr = FindTaggedSpike(spikePtr, 5);
		if(spikePtr)
			spikePtr->spike->counter = 28;
	}while(spikePtr);


//second bunch of spikes

	spikePtr = &spikeList.head;
	do
	{
		spikePtr = FindTaggedSpike(spikePtr, 6);
		if(spikePtr)
			spikePtr->spike->counter = 50;
	}while(spikePtr);


	spikePtr = &spikeList.head;
	do
	{
		spikePtr = FindTaggedSpike(spikePtr, 7);
		if(spikePtr)
			spikePtr->spike->counter = 52;
	}while(spikePtr);

	spikePtr = &spikeList.head;
	do
	{
		spikePtr = FindTaggedSpike(spikePtr, 8);
		if(spikePtr)
			spikePtr->spike->counter = 54;
	}while(spikePtr);

	spikePtr = &spikeList.head;
	do
	{
		spikePtr = FindTaggedSpike(spikePtr, 9);
		if(spikePtr)
			spikePtr->spike->counter = 56;
	}while(spikePtr);

	spikePtr = &spikeList.head;
	do
	{
		spikePtr = FindTaggedSpike(spikePtr, 10);
		if(spikePtr)
			spikePtr->spike->counter = 58;
	}while(spikePtr);

//chicken run!!!

	spikePtr = &spikeList.head;
	do
	{
		spikePtr = FindTaggedSpike(spikePtr, 11);
		if(spikePtr)
			spikePtr->spike->counter = runStart + (runStep * 1);
	}while(spikePtr);
	spikePtr = &spikeList.head;
	do
	{
		spikePtr = FindTaggedSpike(spikePtr, 12);
		if(spikePtr)
			spikePtr->spike->counter = runStart + (runStep * 2);
	}while(spikePtr);

	spikePtr = &spikeList.head;
	do
	{
		spikePtr = FindTaggedSpike(spikePtr, 13);
		if(spikePtr)
			spikePtr->spike->counter = runStart + (runStep * 3);
	}while(spikePtr);

	spikePtr = &spikeList.head;
	do
	{
		spikePtr = FindTaggedSpike(spikePtr, 14);
		if(spikePtr)
			spikePtr->spike->counter = runStart + (runStep * 4);
	}while(spikePtr);

	spikePtr = &spikeList.head;
	do
	{
		spikePtr = FindTaggedSpike(spikePtr, 15);
		if(spikePtr)
			spikePtr->spike->counter = runStart + (runStep * 5);
	}while(spikePtr);

	spikePtr = &spikeList.head;
	do
	{
		spikePtr = FindTaggedSpike(spikePtr, 16);
		if(spikePtr)
			spikePtr->spike->counter = runStart + (runStep * 6);
	}while(spikePtr);

	spikePtr = &spikeList.head;
	do
	{
		spikePtr = FindTaggedSpike(spikePtr, 17);
		if(spikePtr)
			spikePtr->spike->counter = runStart + (runStep * 7);
	}while(spikePtr);

	spikePtr = &spikeList.head;
	do
	{
		spikePtr = FindTaggedSpike(spikePtr, 18);
		if(spikePtr)
			spikePtr->spike->counter = runStart + (runStep * 8);
	}while(spikePtr);

	spikePtr = &spikeList.head;
	do
	{
		spikePtr = FindTaggedSpike(spikePtr, 19);
		if(spikePtr)
			spikePtr->spike->counter = runStart + (runStep * 9);
	}while(spikePtr);








}