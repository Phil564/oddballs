#ifndef __TIMEGATES_H__
#define __TIMEGATES_H__

#define NUMTIMEGATES 50

typedef struct
{
	VECTOR				pos;
	VECTOR				rot;
	int					timeBonus;
	LPDIRECT3DRMFRAME	frame;
	double				radius;
	int					trigger;
	int					active;
}TIMEGATE;

extern TIMEGATE timeGate[];
extern int numTimeGates;
extern LPDIRECT3DRMMESHBUILDER timeGateMesh;

void InitTimeGates();
void TransformTimeGates();
void UpdateTimeGates();
void ResetTimeGates();


#endif