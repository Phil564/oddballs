/*
 *  Project:		Oddballs
 *  File:			wind.cpp
 *  Description:     
 *
 *  (c)1996 ISL.
 */


#include "oddballs.h"
#include "collision.h"
#include "wind.h"
#include "platforms.h"
#include "nmebehaviour.h"
#include "hand.h"

WINDLIST		windList;

/*	--------------------------------------------------------------------------------
	Function 	: InitWindList()
	Purpose 	: duh
	Parameters 	: nun
	Returns 	: nun
	Info 		:
*/
void InitWindList()
{
	windList.numEntries = 0;
	windList.head.next = windList.head.prev = &windList.head;
}

/*	--------------------------------------------------------------------------------
	Function 	: AddWind()
	Purpose 	: adds a wind to the linked list
	Parameters 	: pointer to wind
	Returns 	: none
	Info 		:
*/
void AddWind(WIND *wind)
{
	if(wind->next == NULL)
	{
		windList.numEntries++;
		wind->prev = &windList.head;
		wind->next = windList.head.next;
		windList.head.next->prev = wind;
		windList.head.next = wind;

		MakeUnit(&wind->windDir);
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: SubWind()
	Purpose 	: removes a wind from the linked list
	Parameters 	: pointer to wind
	Returns 	: none
	Info 		:
*/
void SubWind(WIND *wind)
{
	if(wind->next != NULL)
	{
		wind->prev->next = wind->next;
		wind->next->prev = wind->prev;
		windList.numEntries--;
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: UpdateFan()
	Purpose 	: 
	Parameters 	: fan pointer
	Returns 	: 
	Info 		:
*/
void UpdateFan(PLATFORM *platform)
{
	FAN				*fan;
	ACTORLISTENTRY	*ptr;
	ACTOR			*actor;
	VECTOR			wind,sep,angsep;
	double			dist,ang,mass,ang2,val;

	fan = platform->fan;

	if(platform->flags & TILTING)
	{
		if(platform->rot.v[Y] - platform->oldrot.v[Y])
			RotateVector2D(&fan->windDir,&fan->windDir,platform->rot.v[Y] - platform->oldrot.v[Y]);
		else if(platform->rot.v[X] - platform->oldrot.v[X])
			RotateVector2DXYZ(&fan->windDir,&fan->windDir,platform->oldrot.v[X] - platform->rot.v[X],X);
		else if(platform->rot.v[Z] - platform->oldrot.v[Z])
			RotateVector2DXYZ(&fan->windDir,&fan->windDir,platform->oldrot.v[Z] - platform->rot.v[Z],Z);
	}

	if(fan->status == ON)
	{
		for(ptr = actorList.head.next;ptr != &actorList.head;ptr = ptr->next)
		{
			if(((fan->windStrength > 0) && (ptr->actor->flags & BLOWN_BY_WIND)) || ((fan->windStrength < 0) && (ptr->actor->flags & MAGNETIC)))
			{
				actor = ptr->actor;
				SubVector(&sep,&actor->pos,&platform->pos);
				if((ang2 = DotProduct(&sep,&fan->windDir)) <= 0)
					continue;

				AddOneScaledVector(&angsep,&sep,&fan->windDir,fan->offset);
				MakeUnit(&angsep);

				if((ang = DotProduct(&angsep,&fan->windDir)) > fan->angle)
				{
					dist = MagnitudeSquared(&sep);
					val = sqrt(dist);
					ang2 /= val;
					if(ang2 <= 0.1)
						continue;
					if(dist < actor->radius*actor->radius)
						continue;

					dist = ((fan->windStrength)*(ang - fan->angle))/(dist * ang2 * ang2 * (1 - fan->angle));
					mass = max(actor->mass,0.1);
					dist *= 10/mass;
					if(fabs(dist*(1 + fan->spread)) > 0.01)
					{
						AddScaledVector(&wind,&fan->windDir,1 - fan->spread,&sep,fan->spread/val);

						AddOneScaledVector(&actor->vel,&actor->vel,&wind,dist);

//						AddOneScaledVector(&actor->vel,&actor->vel,&fan->windDir,dist);
//						AddOneScaledVector(&actor->vel,&actor->vel,&sep,(dist*fan->spread)/Magnitude(&sep));
						actor->vel.v[Y] = min(actor->vel.v[Y],TERMINAL_VELOCITY);
						actor->flags |= BEING_BLOWN;
					}
				}
			}
		}
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: UpdateWinds()
	Purpose 	: 		   
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void UpdateWinds()
{
	WIND			*wind;
	ACTORLISTENTRY	*ptr;
	ACTOR			*actor;
	int				i,num;
	BYTEVECTOR		outBox;
	VECTOR			dist;
	double			vel,mass;

	for(wind = windList.head.next;wind != &windList.head;wind = wind->next)
	{
		if(wind->status == ON)
		{
			for(ptr = actorList.head.next;ptr != &actorList.head;ptr = ptr->next)
			{
				if(ptr->actor->flags & BLOWN_BY_WIND)
				{
					actor = ptr->actor;
					num = 3;
					mass = max(actor->mass,0.1);
					for(i = X;i <= Z;i++)
					{
						if(actor->pos.v[i] < wind->box.pos.v[i])
						{
							outBox.v[i] = -1;
							num--;
						}
						else if(actor->pos.v[i] > wind->box.pos.v[i] + wind->box.size.v[i])
						{
							outBox.v[i] = 1;
							num--;
						}
						else
							outBox.v[i] = 0;
					}
					if(num == 3)
					{
						AddOneScaledVector(&actor->vel,&actor->vel,&wind->windDir,(wind->windStrength*10)/mass);
					}
					else if(num)
					{
						for(i = X;i <= Z;i++)
						{
							if(outBox.v[i] == 1)
								dist.v[i] = actor->pos.v[i] - wind->box.pos.v[i] - wind->box.size.v[i];
							else if(outBox.v[i] == -1)
								dist.v[i] = wind->box.pos.v[i] - actor->pos.v[i];
							else
								dist.v[i] = 0;
						}
						vel = Magnitude(&dist);
						if(vel < wind->dropOffRange)
						{
							vel = (wind->windStrength*(wind->dropOffRange - vel)*10)/(wind->dropOffRange * mass);
							if(vel > 0.01)
							{
								AddOneScaledVector(&actor->vel,&actor->vel,&wind->windDir,vel);
								actor->flags |= BEING_BLOWN;
							}
						}
					}
				}
			}
		}
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: FindTaggedWind()
	Purpose 	: finds a wind with specified tag
	Parameters 	: wind to start search from, tag number
	Returns 	: pointer to wind with correct tag
	Info 		:
*/
WIND *FindTaggedWind(WIND *startWind,int tag)
{
	WIND *wind;
		 
 	for(wind = startWind->next;wind != &windList.head;wind = wind->next)
		if(wind->tag == tag)
			return wind;
	
	return NULL;
}







