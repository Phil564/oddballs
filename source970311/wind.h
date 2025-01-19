#ifndef __WIND_H__
#define __WIND_H__
/*
typedef struct
{
	FAN		head;
	int		numEntries;
}FANLIST;
*/


typedef struct TAGWIND
{
	BOX				box;
	VECTOR			windDir;
	BOOL			status;
	double			windStrength;
	double			dropOffRange;
	int				tag;
	struct TAGWIND	*next;
	struct TAGWIND	*prev;
}WIND;

typedef struct 
{
	WIND	head;
	int		numEntries;
}WINDLIST;

extern WINDLIST		windList;

void UpdateFan(PLATFORM *platform);

void InitWindList();
void AddWind(WIND *wind);
void SubWind(WIND *wind);
void UpdateWinds();
WIND *FindTaggedWind(WIND *startWind,int tag);


#endif