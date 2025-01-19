/*
 *  Project:         oddballs
 *  File:            catapults.cpp
 *  Description:     handles all catapults and sling-shots
 *
 *  (c)1996 ISL.
 */


#include "oddballs.h"
#include "catapults.h"
#include "actors.h"
#include "hand.h"

/*	--------------------------------------------------------------------------------
	Function 	: UpdateCatapult
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void UpdateCatapult(PLATFORM *platform)
{
	CATAPULT	*catapult = platform->catapult;
	VECTOR		tempVect;

	return;
	if(catapult->active == TRUE)
	{
		SetVector(&hand.vel, &zero);
		hand.rotaim.v[Y] = platform->rot.v[Y];
		if(--catapult->delayCounter == 0)
		{
			AccelerateActor2D(&hand, -8);
			hand.vel.v[Y] += 1;
			catapult->active = FALSE;
		}
		return;
	}


	SetVector(&tempVect, &platform->pos);
	AddVector(&tempVect, &tempVect, &catapult->activateOffset);
	scenicItems[0].frame->SetPosition(g_lpWorld, tempVect.v[X], tempVect.v[Y], tempVect.v[Z]);
	if(DistanceBetweenPoints(&hand.pos, &tempVect) < 1)
	{
		catapult->active = TRUE;
		dprintf("catapult activated\n");
		DisableControl(50);
		catapult->delayCounter = 20;
	}


}