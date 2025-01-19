/*
 *  Project:         Oddballs
 *  File:            timegates.cpp
 *  Description:     
 *
 *  (c)1996 ISL.
 */


#include "oddballs.h"
#include "timegates.h"
#include "scene.h"
#include "personal.h"
#include "collectables.h"
#include "sound.h"
#include "hand.h"
#include "cam.h"

TIMEGATE timeGate[NUMTIMEGATES];
LPDIRECT3DRMMESHBUILDER timeGateMesh;
int numTimeGates;

/*	--------------------------------------------------------------------------------
	Function 	: InitTimeGates()
	Purpose 	: der
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void InitTimeGates()
{
	int		rval,j;
	char	fName[512];

	numTimeGates = 0;

    if (FAILED(g_lpD3DRM->CreateMeshBuilder(&timeGateMesh)))
        return;

	sprintf(fName,"%sgraphics\\dock\\mark.x",rootPath);

    rval = timeGateMesh->Load(fName, NULL, D3DRMLOAD_FROMFILE, LoadTextures, NULL);
    if (rval != D3DRM_OK) 
	{
		dprintf("Failed to load file %s\n",fName);
        return;
    }
	timeGateMesh->SetPerspective(TRUE);

	for(j = 0;j < NUMTIMEGATES;j++)
	{
		memset(&timeGate[j],0,sizeof(TIMEGATE));
	    if (FAILED(g_lpD3DRM->CreateFrame(g_lpScene, &(timeGate[j].frame))))
		    return;
	}
 }


/*	--------------------------------------------------------------------------------
	Function 	: TransformTimeGates()
	Purpose 	: blah blah
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void TransformTimeGates()
{
	int	j;
	TIMEGATE	*gate;

	for(j = 0;j < numTimeGates;j++)
	{
		gate = &timeGate[j];

		timeGate[j].frame->AddRotation(D3DRMCOMBINE_REPLACE, D3DVAL(0), D3DVAL(1), D3DVAL(0), D3DVAL(timeGate[j].rot.v[Y]));
		timeGate[j].frame->AddRotation(D3DRMCOMBINE_BEFORE,  D3DVAL(1), D3DVAL(0), D3DVAL(0), D3DVAL(timeGate[j].rot.v[X]));
		timeGate[j].frame->AddRotation(D3DRMCOMBINE_BEFORE,  D3DVAL(0), D3DVAL(0), D3DVAL(1), D3DVAL(timeGate[j].rot.v[Z]));

		timeGate[j].frame->SetPosition(g_lpScene, D3DVAL(timeGate[j].pos.v[X]), D3DVAL(timeGate[j].pos.v[Y]), D3DVAL(timeGate[j].pos.v[Z]));
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: UpdateTimeGates()
	Purpose 	: checks for hand going through
	Parameters 	: none
	Returns 	: none
	Info 		:
*/
void UpdateTimeGates()
{
	int	j,k;

	for(j = 0;j < numTimeGates;j++)
	{
		if(timeGate[j].active)
		{
			if(DistanceBetweenPoints(&timeGate[j].pos,&hand.pos) <= hand.radius + timeGate[j].radius)
			{
				if(userSurface[TIMEOUT_ICON].display)
					player.messageTimer = 1;

				RemoveVisuals(timeGate[j].frame);
				CreateStarCircle(&timeGate[j].pos,8);
				PlaySample(SFX_TIMEGATE, 0, &timeGate[j].pos, 100);
				player.score += player.timer;
				player.timer += timeGate[j].timeBonus;

				timeGate[j].active = 0;

				handControl.lastTimeGate = j;
				SetVector(&cameraInfo.lastTimeGatePos,&camera.pos);
				SetVector(&cameraInfo.garibLastTimeGatePos,&cameraInfo.garib.pos);

				for(k = 0;k < numTimeGates;k++)
				{
					if(timeGate[k].trigger == j)
					{
						timeGate[k].frame->AddVisual((LPDIRECT3DRMVISUAL)timeGateMesh);
						timeGate[k].active = 1;
					}
				}
			}
		}
	}
}




/*	--------------------------------------------------------------------------------
	Function 	: ResetTimeGates()
	Purpose 	: guess
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void ResetTimeGates()
{
	int j = 0;

	for(j = 0;j < numTimeGates;j++)
	{
		RemoveVisuals(timeGate[j].frame);
		if(timeGate[j].trigger == -1)
		{
			timeGate[0].active = TRUE;
			timeGate[0].frame->AddVisual((LPDIRECT3DRMVISUAL)timeGateMesh);
		}
		else
			timeGate[j].active = FALSE;
	}
}
