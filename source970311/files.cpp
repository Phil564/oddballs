/*
 *  Project:         oddballs
 *  File:            files.cpp
 *  Description:     handles all file control
 *
 *  (c)1996 ISL.
 */



#include "oddballs.h"
#include "personal.h"
#include "platforms.h"
#include "spikes.h"
#include "timegates.h"
#include "enemies.h"
#include "nmebehaviour.h"
#include "collectables.h"
#include "speccase.h"
#include "wind.h"
#include "puzzles.h"
#include "cam.h"

char *sceneryName[] =
{
	"PTREE1",
	"PTREE2",
	"GRASS1"
};

char *conditionName[] = 
{
	"AT_POINT",
	"AT_SPIN",
	"AT_ORBIT",
	"GOT_TO_POINT",
	"GOT_TO_SPIN",
	"GOT_TO_ORBIT",
	"HAND_ON",
	"HAND_GOT_ON",
	"BALL_ON",
	"BALL_GOT_ON",
	"NME_ON",
	"NME_GOT_ON",
	"HAND_TOUCHING",
	"HAND_JUST_TOUCHED",
	"BALL_TOUCHING",
	"BALL_JUST_TOUCHED",
	"NME_TOUCHING",
	"NME_JUST_TOUCHED",
	"VAR_SET",
	"VAR_NOT_SET",
	"CONFINED",
	"JUST_CONFINED",
	"NME_DEAD"
};

char *actionName[] = 
{
	"START_MOVE",
	"START_SPIN",
	"START_ORBIT",
	"SET_NEXT_POINT",
	"SET_NEXT_SPIN",
	"SET_SPIN_AXIS",
	"SET_STATE",
	"SET_EXISTANCE",
	"SET_VAR",
	"CLEAR_VAR",
	"LOOK_AT_PLAT",
	"LOOK_AT_POINT",
	"SET_CAMERA_POS",
	"NME_GOTO_ACTION",
	"SET_WIND"
};

struct
{
	double	width,height;
	int		originX,originY;
}
sceneryInfo[NUM_SCENERY_TYPES] = 
{
	{16,16,81,122},
	{16,16,38,121},
	{1,1,16,32}
};

LPDIRECT3DRMTEXTURE	sceneryTexture[NUM_SCENERY_TYPES];
/*	--------------------------------------------------------------------------------
	Function 	: FileExists
	Purpose 	: check to see if a file exists
	Parameters 	: filename
	Returns 	: true/false
	Info 		:
*/
BOOL FileExists(char *filename)
{
	FILE *file;

	file = fopen(filename, "r");
	if(file == NULL)
		return FALSE;

	fclose(file);
	return TRUE;		
}

/*	--------------------------------------------------------------------------------
	Function 	: ParseLandscapeFile()
	Purpose 	: loads in all the meshes that make up the landscape
	Parameters 	: filename
	Returns 	: success/failure
	Info 		:
*/
BOOL ParseLandscapeFile(char *file)
{
	FILE		*fp;
	int			i,j,k,inInt,inInt2,nme;
	char		string[512];
	char		fileName[512];
	PLATFORM	*platform,*plat;
	PLATFORMID	*platformId;
	float		inFloat[10],w,x,y,z,s,x2,y2,z2,x3,y3,z3;
	char		inChar;
	int			commentLevel = 0;
	SPIKE		*spike;
	int			rope = 0;
	int			tag = 100000;
	double		ang,dist,d[2];
	LPDIRECT3DRMMESHBUILDER meshBuilder;
	FAN			*fan;
	WIND		*wind;
	TELEPORT	*teleport;
	PLATFORMLISTENTRY	*platformPtr;
	PUZZLE		*puzzle;
	CONDITION	*condition;
	ACTION		*action;

	CAMERACASE	*cameraCase;

	memset(&landscape,0,sizeof(LANDSCAPE));
	for(j = 0;j < NUMROPEBRIDGES;j++)
		memset(&ropeBridge[j],0,sizeof(ROPEBRIDGE));

	InitPlatformList();
	InitPuzzleList();
	InitSpikeList();
	InitSpikes();
	InitCameraCaseList();

	for(j = 0;j < NUMPICKUPS;j++)
		pickupMesh[j] = NULL;

	sprintf(fileName,"%sgraphics\\%s.lnd",rootPath,file);

	if((fp = fopen(fileName,"r")) == NULL)
	{
		dprintf("Error opening file %s\n",fileName);
		return FALSE;
	}

	while(fscanf(fp,"%s",string) != EOF)
	{
		if((strlen(string) > 1) && (string[0] == '/') && (string[1] == '*'))
		{
			commentLevel++;
			if((strlen(string) > 1) && (string[strlen(string)-2] == '*') && (string[strlen(string)-1] == '/'))
				commentLevel--;
			continue;
		}
		if((strlen(string) > 1) && (string[strlen(string)-2] == '*') && (string[strlen(string)-1] == '/'))
		{
			if(commentLevel > 0)
				commentLevel--;
			continue;
		}
		if((strcmp(string,"{") == 0) || (strcmp(string,"/*") == 0))
		{
			commentLevel++;
			continue;
		}
		if((strcmp(string,"}") == 0) || (strcmp(string,"*/") == 0))
		{
			if(commentLevel > 0)
				commentLevel--;
			continue;
		}
		if(commentLevel)
			continue;

		if(strcmp(string,"HAND_START") == 0)
		{
			fscanf(fp,"%f %f %f",&x,&y,&z);

			handStart.v[X] = x;
			handStart.v[Y] = y;
			handStart.v[Z] = z;

			SetVector(&hand.pos,&handStart);

			continue;
		}
		if(strcmp(string,"BALL_START") == 0)
		{
			fscanf(fp,"%f %f %f",&x,&y,&z);

			ballStart.v[X] = x;
			ballStart.v[Y] = y;
			ballStart.v[Z] = z;

			SetVector(&ball.pos,&ballStart);
			continue;
		}
		if(strcmp(string,"CAMERA_START") == 0)
		{
			fscanf(fp,"%f %f %f %f %f",&x,&y,&z,&x2,&y2);

			camera.pos.v[X] = x;
			camera.pos.v[Y] = y;
			camera.pos.v[Z] = z;

			camera.rot.v[X] = x2;
			camera.rot.v[Y] = y2;

			SetVector(&cameraInfo.garib.pos, &hand.pos);
			continue;
		}
		if(strcmp(string,"PUZZLE") == 0)
		{
			puzzle = NewPuzzle();
			continue;
		}
		if(strcmp(string,"AND") == 0)
		{
			if(puzzle)
				puzzle->conditionList.andor = CONDITION_AND;
			else
				dprintf("WARNING!! Cannot have an AND without a PUZZLE\n");
			continue;
		}
		if(strcmp(string,"OR") == 0)
		{
			if(puzzle)
				puzzle->conditionList.andor = CONDITION_OR;
			else
				dprintf("WARNING!! Cannot have an OR without a PUZZLE\n");
			continue;
		}
		if(strcmp(string,"NUMTIMES") == 0)
		{
			fscanf(fp,"%d",&inInt);
			
			if(puzzle)
				puzzle->conditionList.numTimes = inInt;
			else
				dprintf("WARNING!! Cannot have a NUMTIMES without a PUZZLE\n");
			continue;
		}
		if(strcmp(string,"ANY") == 0)
		{
			fscanf(fp,"%d",&inInt);
	
			if(puzzle)
			{
				puzzle->conditionList.andor = inInt;
			}
			else
				dprintf("WARNING!! Cannot have an ANY without a PUZZLE\n");
			continue;
		}
		if(strcmp(string,"CONDITION") == 0)
		{
			fscanf(fp,"%s %d %d",string,&inInt,&inInt2);

			if(puzzle)
			{
				for(j = 0;;j++)
				{
					if(j >= NUM_CONDITION_TYPES)
						break;
					if(strcmp(string,conditionName[j]) == 0)
						break;
				}

				if(j < NUM_CONDITION_TYPES)
				{
					condition = (CONDITION *)MyCalloc(1,sizeof(CONDITION));
					AddConditionToList(condition,&puzzle->conditionList);
					condition->type = j;
					condition->targetTag = inInt;
					condition->value = inInt2;
				}
				else
					dprintf("WARNING!! Unknown condition %s\n",string);
			}
			else
				dprintf("WARNING!! Cannot have a CONDITION without a PUZZLE\n");
			continue;
		}
		if(strcmp(string,"ACTION") == 0)
		{
			fscanf(fp,"%s",string);

			if(puzzle)
			{
				for(j = 0;;j++)
				{
					if(j >= NUM_ACTION_TYPES)
						break;
					if(strcmp(string,actionName[j]) == 0)
						break;
				}

				if(j < NUM_ACTION_TYPES)
				{
					action = (ACTION *)MyCalloc(1,sizeof(ACTION));
					if((j == ACTION_LOOK_AT_PLAT) || (j == ACTION_LOOK_AT_POINT) || (j == ACTION_SET_CAMERA_POS))
					{
						fscanf(fp,"%d %f %f %f %f %d %d",&inInt,&inFloat[0],&inFloat[1],&inFloat[2],&inFloat[3],&inInt2,&k);
						action->vect.v[X] = inFloat[1];
						action->vect.v[Y] = inFloat[2];
						action->vect.v[Z] = inFloat[3];
					}
					else
						fscanf(fp,"%d %f %d %d",&inInt,&inFloat[0],&inInt2,&k);

					action->value = inFloat[0];
					AddActionToList(action,&puzzle->actionList);
					action->type = j;
					action->targetTag = inInt;
					action->flags = inInt2;
					action->delay = k;
					action->timer = -1;
				}
				else
					dprintf("WARNING!! Unknown action %s\n",string);
			}
			else
				dprintf("WARNING!! Cannot have an ACTION without a PUZZLE\n");
			continue;
		}
		if(strcmp(string,"SPINTOMOVE") == 0)
		{
			fscanf(fp,"%f",&inFloat[0]);

			platform->spinToMove = inFloat[0];
			continue;
		}
		if(strcmp(string,"SPINTOSPIN") == 0)
		{
			fscanf(fp,"%f",&inFloat[0]);

			platform->spinToSpin = inFloat[0];
			continue;
		}
		if(strcmp(string,"PUSH") == 0)
		{
			fscanf(fp,"%s %f %f %f",string,&inFloat[0],&inFloat[1],&inFloat[2]);

			for(j = 0;j < strlen(string);j++)
				switch(string[j])
				{
					case 'X':
						platform->pushDir.v[X] = 1;
						break;
					case 'Y':
						platform->pushDir.v[Y] = 1;
						break;
					case 'Z':
						platform->pushDir.v[Z] = 1;
						break;
				}
			platform->mass = inFloat[0];
			platform->velInertia = inFloat[1];
			platform->maxSpeed = inFloat[2];
			continue;
		}
		if(strcmp(string,"BACKGROUND") == 0)
		{
			fscanf(fp,"%s %f %f %f",string,&x,&y,&z);
			
			landscape.object[landscape.numObjects].pos.v[X] = x;
			landscape.object[landscape.numObjects].pos.v[Y] = y;
			landscape.object[landscape.numObjects].pos.v[Z] = z;
			landscape.object[landscape.numObjects].flags = BACKGROUND;
			sprintf(fileName,"%sgraphics\\%s",rootPath,string);
			LoadMeshIntoActor(&landscape.object[landscape.numObjects],g_lpBackground,fileName);
			landscape.object[landscape.numObjects].frame->SetPosition(g_lpBackground, D3DVAL(landscape.object[landscape.numObjects].pos.v[X]), D3DVAL(landscape.object[landscape.numObjects].pos.v[Y]), D3DVAL(landscape.object[landscape.numObjects].pos.v[Z]));
			if(strcmp(string, "dock\\sea.x") == 0)
			{
				seaActor = &landscape.object[landscape.numObjects];
				dprintf("Located Sea actor\n");
			}
			landscape.numObjects++;
			continue;
		}
		if(strcmp(string,"LANDSCAPE") == 0)
		{
			fscanf(fp,"%s %f %f %f",string,&x,&y,&z);
			
			landscape.object[landscape.numObjects].pos.v[X] = x;
			landscape.object[landscape.numObjects].pos.v[Y] = y;
			landscape.object[landscape.numObjects].pos.v[Z] = z;
			sprintf(fileName,"%sgraphics\\%s",rootPath,string);
			LoadMeshIntoActor(&landscape.object[landscape.numObjects],g_lpScene,fileName);
			landscape.object[landscape.numObjects].frame->SetPosition(g_lpScene, D3DVAL(landscape.object[landscape.numObjects].pos.v[X]), D3DVAL(landscape.object[landscape.numObjects].pos.v[Y]), D3DVAL(landscape.object[landscape.numObjects].pos.v[Z]));
			landscape.numObjects++;
			continue;
		}
		if(strcmp(string,"HEADSTART") == 0)
		{
			fscanf(fp,"%d",&inInt);

			if(platform)
			{
				for(j = 0;j < inInt;j++)
					UpdatePlatform(platform);
			}
			else
				dprintf("WARNING!! Cannot have HEADSTART without PLATFORM\n");
			continue;
		}
		if(strcmp(string,"DUMMY") == 0)
		{
			platform = (PLATFORM *)MyCalloc(1,sizeof(PLATFORM));
			AddPlatform(platform);
			platform->direction = 1;
			platform->flags |= DUMMY;
			continue;
		}
		if(strcmp(string,"LOOKATPOINT") == 0)
		{
			fscanf(fp,"%f %f",&inFloat[0],&inFloat[1]);

			if(platform)
			{
				platform->rotInertia = inFloat[0];
				platform->turnSpeed = inFloat[1];
				platform->flags |= TILTING | LOOK_AT_POINT;
				platform->rAim.v[Y] = ON;
				platform->spinAxis.v[Y] = 1;
				platform->rotaim.v[Y] = platform->rot.v[Y] = Aabs(atan2(platform->pathPoint[0]->v[X] - platform->pathPoint[1]->v[X],platform->pathPoint[0]->v[Z] - platform->pathPoint[1]->v[Z]));
			}
			else
				dprintf("WARNING!! Cannot have LOOKATPOINT without PLATFORM\n");
			continue;
		}
		if(strcmp(string,"GOFORWARDS") == 0)
		{
			fscanf(fp,"%f %f %f %d",&inFloat[0],&inFloat[1],&inFloat[2],&inInt);

			if(platform)
			{
				platform->rotInertia = inFloat[0];
				platform->turnSpeed = inFloat[1];
				platform->velInertia = inFloat[2];
				platform->bank = inInt;
				if(inInt)
					platform->rAim.v[Z] = YES;
				platform->flags |= TILTING | LOOK_AT_POINT | GO_FORWARDS;
				platform->rAim.v[Y] = ON;
				platform->spinAxis.v[Y] = 1;
				platform->rotaim.v[Y] = platform->rot.v[Y] = Aabs(atan2(platform->pathPoint[0]->v[X] - platform->pathPoint[1]->v[X],platform->pathPoint[0]->v[Z] - platform->pathPoint[1]->v[Z]));
			}
			else
				dprintf("WARNING!! Cannot have GOFORWARDS without PLATFORM\n");
			continue;
		}
		if(strcmp(string,"LOOKATHAND") == 0)
		{
			fscanf(fp,"%f %f",&inFloat[0],&inFloat[1]);
			
			if(platform)
			{
				platform->spinAxis.v[Y] = 1;
				platform->rotInertia = inFloat[0];
				platform->flags |= TILTING | LOOK_AT_HAND;
				platform->turnSpeed = inFloat[1];
				platform->rAim.v[Y] = ON;
			}
			else
				dprintf("WARNING!! Cannot have LOOKATHAND without PLATFORM\n");
			continue;
		}
		if(strcmp(string,"LOOKATBALL") == 0)
		{
			fscanf(fp,"%f %f",&inFloat[0],&inFloat[1]);

			if(platform)
			{
				platform->spinAxis.v[Y] = 1;
				platform->rotInertia = inFloat[0];
				platform->flags |= TILTING | LOOK_AT_BALL;
				platform->turnSpeed = inFloat[1];
				platform->rAim.v[Y] = ON;
			}
			else
				dprintf("WARNING!! Cannot have LOOKATBALL without PLATFORM\n");
			continue;
		}
		if(strcmp(string,"NOVISUAL") == 0)
		{
			if(platform)
				RemoveVisuals(platform->frame);
			else
				dprintf("WARNING!! Cannot have NOVISUAL without PLATFORM\n");
			continue;
		}
		if(strcmp(string,"PLATFORM") == 0)
		{
			fscanf(fp,"%s",string);

			platform = (PLATFORM *)MyCalloc(1,sizeof(PLATFORM));
			AddPlatform(platform);
			platform->direction = 1;
			sprintf(fileName,"%sgraphics\\%s",rootPath,string);
			LoadMeshIntoPlatform(platform,g_lpScene,fileName);
			platform->scale.v[X] = platform->scale.v[Y] = platform->scale.v[Z] = 1;
			platform->velInertia = 1;
			continue;
		}
		if(strcmp(string,"NOCOLLISION") == 0)
		{
			platform->flags |= BACKGROUND;
			continue;
		}
		if(strcmp(string,"DESTRUCTIBLE") == 0)
		{
			platform->flags |= DESTRUCTIBLE;
			continue;
		}
		if(strcmp(string,"REVERSING") == 0)
		{
			platform->flags |= REVERSING;
			continue;
		}
		if(strcmp(string,"TIMEBONUS") == 0)
		{
			fscanf(fp,"%f",&x);
			platform->timeBonus = (int)x;
			continue;
		}
		if(strcmp(string,"CRUMBLE") == 0)
		{
			fscanf(fp,"%d %f %d",&inInt,&inFloat[0],&inInt2);

			platform->crumbleLimit = inInt;
			platform->crumbleGravityMultiplier = inFloat[0];
			platform->crumbleGravityDelay = inInt2;
			continue;
		}
		if(strcmp(string,"CONVEYOR") == 0)
		{
			fscanf(fp,"%f %f %f",&x,&y,&z);

			platform->flags |= CONVEYOR;

			platform->conveyorSpeed.v[X] = x;
			platform->conveyorSpeed.v[Y] = y;
			platform->conveyorSpeed.v[Z] = z;
			continue;
		}
		if(strcmp(string,"CATAPULT") == 0)
		{
			platform->catapult = (CATAPULT *)MyCalloc(1, sizeof(CATAPULT));

			platform->rotInertia = 1;
//			platform->flags |= TILTING;
			fscanf(fp,"%f %f %f %f %f %f %d",&x,&y,&z, &inFloat[0], &inFloat[1],&inFloat[2],&inInt);
			platform->catapult->activateOffset.v[X] = x;
			platform->catapult->activateOffset.v[Y] = y;
			platform->catapult->activateOffset.v[Z] = z;
			platform->catapult->strengthXZ = inFloat[0];
			platform->catapult->strengthY = inFloat[1];
			platform->catapult->rotSpeed = inFloat[2];
			platform->catapult->rotTime = inInt;
			platform->catapult->status = ON;
			continue;
		}
		if(strcmp(string,"TERRAIN") == 0)
		{
			fscanf(fp,"%s",string);

			if(strcmp(string,"STONE") == 0)
				inInt = STONE;
			if(strcmp(string,"GRASS") == 0)
				inInt = GRASS;
			if(strcmp(string,"ICE") == 0)
				inInt = ICE;
			if(strcmp(string,"MUD") == 0)
				inInt = MUD;
			if(strcmp(string,"SAND") == 0)
				inInt = SAND;
			if(strcmp(string,"SHALLOW_WATER") == 0)
				inInt = SHALLOW_WATER;
			if(strcmp(string,"DEEP_WATER") == 0)
				inInt = DEEP_WATER;

			SetPlatformTerrain(platform,inInt);
			continue;
		}
		if(strcmp(string,"POINT") == 0)
		{
			fscanf(fp,"%f %f %f %d",&x,&y,&z,&inInt);

			PlatformAddPoint(platform,x,y,z,inInt);
			continue;
		}
		if(strcmp(string,"SPEED") == 0)
		{
			fscanf(fp,"%f",&inFloat[0]);

			platform->maxSpeed = inFloat[0];
			continue;
		}
		if(strcmp(string,"ACCEL") == 0)
		{
			fscanf(fp,"%f",&inFloat[0]);

			platform->accel = inFloat[0];
			continue;
		}
		if(strcmp(string,"BOUNCE") == 0)
		{
			fscanf(fp,"%s %f",string,&inFloat[0]);

			for(j = 0;j < strlen(string);j++)
				switch(string[j])
				{
					case 'X':
						platform->bounceDir[X] = 1;
						break;
					case 'Y':
						platform->bounceDir[Y] = 1;
						break;
					case 'Z':
						platform->bounceDir[Z] = 1;
						break;
				}
			platform->bounce = inFloat[0];
			continue;
		}
		if(strcmp(string,"TAG") == 0)
		{
			fscanf(fp,"%d",&inInt);

			platform->tag = inInt;
			continue;
		}
		if(strcmp(string,"TARGETTAG") == 0)
		{
			fscanf(fp,"%d",&inInt);

			platform->targetTag = inInt;
			if(inInt != 0)
			{
				if(TagToFirstPlatform(inInt) == NULL)
					dprintf("WARNING!! No such target tag %d for platform %d\n",inInt,platformList.numEntries);
			}
			continue;
		}
		if(strcmp(string,"PENDULUM") == 0)
		{
			fscanf(fp," %c %f %f %d",&inChar,&inFloat[0],&x,&inInt);

			switch(inChar)
			{
				case 'X':
					platform->spinAxis.v[X] = 1;
					platform->rot.v[X] = platform->initRot = x;
					break;
				case 'Y':
					platform->spinAxis.v[Y] = 1;
					platform->rot.v[Y] = platform->initRot = x;
					break;
				case 'Z':
					platform->spinAxis.v[Z] = 1;
					platform->rot.v[Z] = platform->initRot = x;
					break;
			}
			platform->pendulumSwing = inFloat[0];
			platform->flags |= TILTING;
			platform->rotInertia = 1;
			for(j = 0;j < inInt;j++)
			{
				for(k = X;k <= Z;k++)
				{
					platform->rotvel.v[k] -= FindShortestAngleSigned(platform->rot.v[k],0)/platform->pendulumSwing;
					platform->rot.v[k] += platform->rotvel.v[k];
					platform->rot.v[k] = Aabs(platform->rot.v[k]);
					if(fabs(platform->rotvel.v[k]) <= 0.0005)
						platform->rotvel.v[k] = 0;
				}
			}
			continue;
		}
		if(strcmp(string,"PARENTTAG") == 0)
		{
			fscanf(fp,"%d",&inInt);

			if(inInt != 0)
			{
				if(platform->numPathPoints == 0)
				{
					platform->parentPlatform = TagToFirstPlatform(inInt);
					if(platform->parentPlatform == NULL)
						dprintf("WARNING!! No such parent tag %d for platform %d\n",inInt,platformList.numEntries);
				}
				else
					dprintf("WARNING!! Platform %d has parenttag after first point\n",platformList.numEntries);
			}
			continue;
		}
		if(strcmp(string,"CONFINE") == 0)
		{
			fscanf(fp,"%f %f %f %f %f %f",&x,&y,&z,&x2,&y2,&z2);

			platform->confineBox = (BOX *)MyCalloc(1,sizeof(BOX));

			platform->confineBox->pos.v[X] = x;
			platform->confineBox->pos.v[Y] = y;
			platform->confineBox->pos.v[Z] = z;

			platform->confineBox->size.v[X] = x2;
			platform->confineBox->size.v[Y] = y2;
			platform->confineBox->size.v[Z] = z2;
			continue;
		}
		if(strcmp(string,"MOVETARGET") == 0)
		{
			fscanf(fp,"%f %f %f",&x,&y,&z);

			platform->targetSpeed.v[X] = x;
			platform->targetSpeed.v[Y] = y;
			platform->targetSpeed.v[Z] = z;
			continue;
		}
		if(strcmp(string,"MOVEBYSPEED") == 0)
		{
			fscanf(fp,"%f %f %f",&x,&y,&z);

			platform->moveBySpeed.v[X] = x;
			platform->moveBySpeed.v[Y] = y;
			platform->moveBySpeed.v[Z] = z;
			continue;
		}
		if(strcmp(string,"ORBIT") == 0)
		{
			fscanf(fp," %c %f %f %f %f",&inChar,&x,&y,&z,&inFloat[0]);

			switch(inChar)
			{
				case 'X':
					platform->orbitAxis.v[X] = 1;
					break;
				case 'Y':
					platform->orbitAxis.v[Y] = 1;
					break;
				case 'Z':
					platform->orbitAxis.v[Z] = 1;
					break;
			}
//			platform->orbitPoint.v[X] = x;
//			platform->orbitPoint.v[Y] = y;
//			platform->orbitPoint.v[Z] = z;

			if(platform->numPathPoints <= 1)
			{
				platform->pathPoint[1] = (VECTOR *)MyMalloc(sizeof(VECTOR));
				platform->pathPoint[1]->v[X] = x;
				platform->pathPoint[1]->v[Y] = y;
				platform->pathPoint[1]->v[Z] = z;

				platform->orbitSpeed = inFloat[0];
			}
			else
				dprintf("WARNING!! Platform %d cannot orbit - more than one point specified\n",platformList.numEntries);
			continue;
		}
		if(strcmp(string,"ORBITPAUSE") == 0)
		{
			fscanf(fp,"%f %f",&x,&y);

			if(platform->orbitFlipFlop == 0)
			{
				platform->orbitNumPauses = x;
				platform->orbitPauseTime = y;
				platform->orbitNextPoint = min(platform->orbitNumPauses - 1,1);
			}
			else
				dprintf("WARNING!! Platform %d cannot ORBITPAUSE - ORBITFLIP already specified\n",platformList.numEntries);
			continue;
		}
		if(strcmp(string,"ORBITFLIP") == 0)
		{
			fscanf(fp,"%f %f",&inFloat[0],&inFloat[1]);
			
			if(platform->orbitNumPauses == 0)
			{
				platform->orbitFlipFlop = inFloat[0];
				platform->orbitPauseTime = inFloat[1];
			}
			else
				dprintf("WARNING!! Platform %d cannot ORBITFLIP - ORBITPAUSE already specified\n",platformList.numEntries);
			continue;
		}
		if(strcmp(string,"SYMMETRY") == 0)
		{
			fscanf(fp,"%f %f",&x);

			platform->spinSymmetry = x;
			continue;
		}
		if(strcmp(string,"SCALE") == 0)
		{
			fscanf(fp,"%f %f %f",&x,&y,&z);

			if(platform->numPathPoints == 0)
			{
				platform->scale.v[X] = x;
				platform->scale.v[Y] = y;
				platform->scale.v[Z] = z;
			}
			else
				dprintf("WARNING!! Platform %d has scale after first point\n",platformList.numEntries);
			continue;
		}
		if(strcmp(string,"STRETCH") == 0)
		{
			fscanf(fp,"%f %f %f %d %d",&x,&y,&z,&inInt,&inInt2);

			platform->stretchScale.v[X] = x;
			platform->stretchScale.v[Y] = y;
			platform->stretchScale.v[Z] = z;

			platform->stretchTime = inInt;
			platform->stretchWait = inInt2;
			platform->stretchCounter = 1;
			continue;
		}
		if(strcmp(string,"TAKESPIN") == 0)
		{
			platform->flags |= TAKESPIN | TILTING;

			plat = platform->parentPlatform;

			if((plat != NULL) && ((plat->flags & TILTING) || (plat->initRot)))
			{
				memcpy(&platform->spinAxis,&plat->spinAxis,sizeof(BYTEVECTOR));

				j = 0;
				k = 10;
				for(i = X;i <= Z;i++)
				{
					platform->rot.v[i] += plat->rot.v[i];
					platform->rotaim.v[i] += plat->rot.v[i];
					if(plat->spinAxis.v[i] == 0)
					{
//						platform->rot.v[i] += plat->rot.v[i];
//						platform->rotaim.v[i] += plat->rot.v[i];
						d[j] = platform->pos.v[i] - plat->pos.v[i];
						j++;
					}
					else
						k = i;
				}

				if(k != 10)
				{
					dist = sqrt(d[0]*d[0] + d[1]*d[1]);
					ang = atan2(d[1],d[0]);

					if(k == Y)
						ang -= plat->rot.v[Y];
					else
						ang += plat->rot.v[k];

					ang = Aabs(ang);

					d[0] = dist*cos(ang);
					d[1] = dist*sin(ang);
					j = 0;
					for(i = X;i <= Z;i++)
					{
						if(plat->spinAxis.v[i] == 0)
							platform->pos.v[i] = d[j++] + plat->pos.v[i];
					}
				}
			}
			continue;
		}
		if(strcmp(string,"SPINPAUSE") == 0)
		{
			fscanf(fp,"%f %f",&x,&y);

			if(platform->spinFlipFlop == 0)
			{
				platform->spinNumPauses = x;
				platform->spinPauseTime = y;
				platform->spinNextPoint = min(platform->spinNumPauses - 1,1);
			}
			else
				dprintf("WARNING!! Platform %d cannot SPINPAUSE - SPINFLIP already specified\n",platformList.numEntries);
			continue;
		}
		if(strcmp(string,"SPINFLIP") == 0)
		{
			fscanf(fp,"%f %f",&inFloat[0],&inFloat[1]);
			
			if(platform->spinNumPauses == 0)
			{
				platform->spinFlipFlop = inFloat[0];
				platform->spinPauseTime = inFloat[1];
			}
			else
				dprintf("WARNING!! Platform %d cannot SPINFLIP - SPINPAUSE already specified\n",platformList.numEntries);
			continue;
		}
		if(strcmp(string,"MAXROT") == 0)
		{
			fscanf(fp,"%f",&inFloat[0]);

			platform->maxRot = inFloat[0];
			continue;
		}
		if(strcmp(string,"SPIN") == 0)
		{
			fscanf(fp," %c %f %f",&inChar,&inFloat[0],&x);

			switch(inChar)
			{
				case 'X':
					platform->spinAxis.v[X] = 1;
					platform->rot.v[X] = platform->initRot = x;
					break;
				case 'Y':
					platform->spinAxis.v[Y] = 1;
					platform->rot.v[Y] = platform->initRot = x;
					break;
				case 'Z':
					platform->spinAxis.v[Z] = 1;
					platform->rot.v[Z] = platform->initRot = x;
					break;
			}
			platform->spinSpeed = inFloat[0];
			platform->flags |= TILTING;
			platform->rotInertia = 1;
			continue;
		}
//timegates
		if(strcmp(string,"TIMEGATE") == 0)
		{
			fscanf(fp,"%f %f %f",&x,&y,&z);

			timeGate[numTimeGates].pos.v[X] = x;
			timeGate[numTimeGates].pos.v[Y] = y;
			timeGate[numTimeGates].pos.v[Z] = z;

			fscanf(fp,"%f %f %f %d %f %d",&x,&y,&z,&inInt,&inFloat[0],&inInt2);

			timeGate[numTimeGates].rot.v[X] = x;
			timeGate[numTimeGates].rot.v[Y] = y;
			timeGate[numTimeGates].rot.v[Z] = z;

			timeGate[numTimeGates].timeBonus = inInt;
			timeGate[numTimeGates].radius = inFloat[0];
			timeGate[numTimeGates].trigger = inInt2;
			if(inInt2 == -1)
			{
			    timeGate[numTimeGates].frame->AddVisual((LPDIRECT3DRMVISUAL)timeGateMesh);
				timeGate[numTimeGates].active = 1;
			}

			if(numTimeGates < NUMTIMEGATES)
				numTimeGates++;
			continue;
		}
		if(strcmp(string,"SEESAW") == 0)
		{
			fscanf(fp," %c %f %f %f %f %f %d",&inChar,&inFloat[0],&x,&y,&z,&w,&inInt);

			switch(inChar)
			{
				case 'X':
					platform->spinAxis.v[X] = 1;
					platform->rot.v[X] = platform->initRot = platform->rotaim.v[X] = z;
					if(x > 0)
						platform->rAim.v[X] = ON;
					break;
				case 'Y':
					platform->spinAxis.v[Y] = 1;
					platform->rot.v[Y] = platform->initRot = platform->rotaim.v[Y] = z;
					if(x > 0)
						platform->rAim.v[Y] = ON;
					break;
				case 'Z':
					platform->spinAxis.v[Z] = 1;
					platform->rot.v[Z] = platform->initRot = platform->rotaim.v[Z] = z;
					if(x > 0)
						platform->rAim.v[Z] = ON;
					break;
			}
			platform->flags |= TILTING | SEESAW;
			platform->rotInertia = inFloat[0];		  
			platform->turnSpeed = x;	
			platform->maxRot = y;
			platform->maxRotVel = w;
			platform->mass = inInt;
			platform->toppleFlags |= TOPPLE_NEG | TOPPLE_POS;
			continue;
		}
		if(strcmp(string,"TOPPLE") == 0)
		{
			fscanf(fp," %c %f %f %f %f %f %d %d",&inChar,&inFloat[0],&x,&y,&z,&w,&inInt,&inInt2);

			switch(inChar)
			{
				case 'X':
					platform->spinAxis.v[X] = 1;
					if(x > 0)
						platform->rAim.v[X] = ON;
					break;
				case 'Y':
					platform->spinAxis.v[Y] = 1;
					if(x > 0)
						platform->rAim.v[Y] = ON;
					break;
				case 'Z':
					platform->spinAxis.v[Z] = 1;
					if(x > 0)
						platform->rAim.v[Z] = ON;
					break;
			}
			platform->flags |= TILTING | SEESAW;
			platform->rotInertia = inFloat[0];		  
			platform->turnSpeed = x;	
			platform->maxRot = y;
			platform->toppleRot = z;
			platform->maxRotVel = w;
			platform->mass = inInt;
			platform->toppleFlags = inInt2;
			continue;
		}
//this is the start of spike reading section
		if(strcmp(string,"SPIKE") == 0)
		{
			fscanf(fp," %f %f %f %f %f",&x, &y, &z, &w, &s);

			spike = (SPIKE *)MyCalloc(1, sizeof(SPIKE));
			AddSpike(spike);
			sprintf(fileName,"graphics\\%s", string);
		    TRY_D3DRM(g_lpD3DRM->CreateFrame(g_lpScene, &(spike->frame)));
			spike->frame->AddVisual(spikeTexture);	
		   
		   	spike->pos[0].v[X] = x;
			spike->pos[0].v[Y] = y;
			spike->pos[0].v[Z] = z;
			spike->posDelay[0] = w;
			spike->counter = w;
			SetVector(&spike->actualPos, &spike->pos[0]);
			spike->numPositions = 1;
			spike->currentPosition = 0;
		   	spike->speed[0] = s;
			continue;
		}
		if(strcmp(string,"SPIKE_NEXTPOS") == 0)
		{
			fscanf(fp," %f %f %f %f %f",&x, &y, &z, &w, &s);
			spike->pos[spike->numPositions].v[X] = x;
			spike->pos[spike->numPositions].v[Y] = y;
			spike->pos[spike->numPositions].v[Z] = z;
			spike->posDelay[spike->numPositions] = w;
			spike->speed[spike->numPositions] = s;
			spike->numPositions++;
			continue;
		}
		if(strcmp(string,"SPIKE_TAG") == 0)
		{
			fscanf(fp," %f",&x);
			spike->tag = x;
			continue;
		}
//enemies
		if(strcmp(string,"ENEMY") == 0)
		{
			fscanf(fp,"%s %f %f %f %f %d %d",string,&x,&y,&z,&inFloat[0],&inInt,&inInt2);

			for(j = 0;;j++)
			{
				if(strcmp(string,actorName[j]) == 0)
				{
					nme = j;
					break;
				}
			}


			LoadEnemy(nme,x,y,z,inFloat[0],inInt,inInt2);
			continue;
		}
		if(strcmp(string,"END_ENEMY") == 0)
		{
			NMEGetNewInstruction(&enemies[numberOfEnemies-1], 0, NO);
			continue;
		}
//enemy script instructions
		if(strcmp(string,"NORMAL_INSTRUCTION") == 0)
		{
			NMEAddNormalInstruction(fp);
			continue;
		}
		if(strcmp(string,"CONDITIONAL_INSTRUCTION") == 0)
		{
			NMEAddConditionalInstruction(fp);
			continue;
		}
		if(strcmp(string,"ATTACK_INSTRUCTION") == 0)
		{
			NMEAddAttackInstruction(fp);
			continue;
		}
//camera special cases
		if(strcmp(string,"CAMERA") == 0)
		{
			cameraCase = NewCameraCase(fp);	//adds camera case to list and parses condition
			continue;
		}
		if(strcmp(string,"CAMERA_ACTION") == 0)
		{
			AddCameraAction(fp, cameraCase);	//adds camera action to list
			continue;
		}


//collectables
		if(strcmp(string,"PICKUP") == 0)
		{
			fscanf(fp,"%s %f %f %f",string,&x,&y,&z);

			for(j = 0;;j++)
			{
				if(strcmp(string,pickupName[j]) == 0)
					break;
			}

			if(j < NUMPICKUPS)
			{
				if(pickupMesh[j] == NULL)
				{
					sprintf(string,"%sgraphics\\icons\\%s.x",rootPath,pickupName[j]);
					pickupMesh[j] = LoadMesh(string);
				}
			    g_lpD3DRM->CreateFrame(g_lpScene,&(collectables[numberOfCollectables].frame));
				collectables[numberOfCollectables].frame->AddVisual((LPDIRECT3DRMVISUAL)pickupMesh[j]);

				collectables[numberOfCollectables].pos.v[X] = x;
				collectables[numberOfCollectables].pos.v[Y] = y;
				collectables[numberOfCollectables].pos.v[Z] = z;

				collectables[numberOfCollectables].type = j;

				collectables[numberOfCollectables].size = 2;
				collectables[numberOfCollectables].active = TRUE;
				collectables[numberOfCollectables].frame->SetPosition(g_lpScene,x,y,z);
				collectables[numberOfCollectables].frame->SetRotation(g_lpScene,(D3DVALUE)0,(D3DVALUE)1,(D3DVALUE)0,(D3DVALUE)0.0003);
				if(numberOfCollectables < MAX_COLLECTABLES)
					numberOfCollectables++;
			}
			continue;
		}
//scenery
		if(strcmp(string,"SCENERY") == 0)
		{
			fscanf(fp,"%s %f %f %f",string,&x,&y,&z);

			for(j = 0;;j++)
			{
				if(strcmp(string,sceneryName[j]) == 0)
					break;
			}

			if(j < NUM_SCENERY_TYPES)
			{
				if(sceneryTexture[j] == NULL)
				{
					sprintf(string,"%s\\graphics\\scenery\\%s.ppm",rootPath,sceneryName[j]);
					g_lpD3DRM->LoadTexture(string,&sceneryTexture[j]);
					sceneryTexture[j]->SetDecalSize(sceneryInfo[j].width,sceneryInfo[j].height);
					sceneryTexture[j]->SetDecalOrigin(sceneryInfo[j].originX,sceneryInfo[j].originY);
					sceneryTexture[j]->SetDecalTransparency(TRUE);
					sceneryTexture[j]->SetDecalTransparentColor(RGB_MAKE(0,0,0));
				}

				g_lpD3DRM->CreateFrame(g_lpScene,&scenicItems[numScenicItems].frame);
				scenicItems[numScenicItems].frame->AddVisual(sceneryTexture[j]);
				
				scenicItems[numScenicItems].visual = sceneryTexture[j];
				scenicItems[numScenicItems].visible = TRUE;

	
  				scenicItems[numScenicItems].frame->SetPosition(g_lpScene,x,y,z); 
				if(numScenicItems < NUM_SCENIC_ITEMS)
					numScenicItems++;
			}
			continue;
		}
		if(strcmp(string,"TELEPORT") == 0)
		{
			fscanf(fp,"%f %f %f %d %d %d %d",&x2,&y2,&z2,&inInt,&inInt2,&i,&j);

			plat = TagToFirstPlatform(inInt2);
			if((plat) || (inInt2 == 0))
			{
				teleport = (TELEPORT *)MyCalloc(1,sizeof(TELEPORT));

				if(inInt2)
					teleport->platform = plat;

				teleport->destination.v[X] = x2;
				teleport->destination.v[Y] = y2;
				teleport->destination.v[Z] = z2;

				teleport->status = ON;
				teleport->flags = inInt;
				teleport->disappearDelay = i;
				teleport->appearDelay = j;
				platform->teleport = teleport;
			}
			else
				dprintf("WARNING!! Platform %d cannot TELEPORT - no such target tag %d\n",platformList.numEntries,inInt2);
			continue;
		}
		if(strcmp(string,"FAN") == 0)
		{
			fscanf(fp,"%f %f %f %f %f %f %f %d",&x2,&y2,&z2,&inFloat[0],&inFloat[1],&inFloat[2],&inFloat[3],&inInt);

			platform->fan = (FAN *)MyCalloc(1,sizeof(FAN));

			platform->fan->windDir.v[X] = x2;
			platform->fan->windDir.v[Y] = y2;
			platform->fan->windDir.v[Z] = z2;

			platform->fan->windStrength = inFloat[0];
			platform->fan->angle = inFloat[1];
			platform->fan->spread = inFloat[2];
			platform->fan->offset = inFloat[3];

			platform->fan->status = inInt;
			MakeUnit(&platform->fan->windDir);
			continue;
		}
		if(strcmp(string,"MAGNET") == 0)
		{
			fscanf(fp,"%f %f %f %f %f %f %f %d",&x2,&y2,&z2,&inFloat[0],&inFloat[1],&inFloat[2],&inFloat[3],&inInt);

			platform->fan = (FAN *)MyCalloc(1,sizeof(FAN));

			platform->fan->windDir.v[X] = x2;
			platform->fan->windDir.v[Y] = y2;
			platform->fan->windDir.v[Z] = z2;

			platform->fan->windStrength = -inFloat[0];
			platform->fan->angle = inFloat[1];
			platform->fan->spread = inFloat[2];
			platform->fan->offset = inFloat[3];

			platform->fan->status = inInt;
			MakeUnit(&platform->fan->windDir);
			continue;
		}
		if(strcmp(string,"WIND") == 0)
		{
			fscanf(fp,"%f %f %f %f %f %f %f %f %f %f %f %d %d",&x,&y,&z,&x2,&y2,&z2,&inFloat[0],&inFloat[1],&inFloat[2],&inFloat[3],&inFloat[4],&inInt,&inInt2);

			wind = (WIND *)MyCalloc(1,sizeof(WIND));

			wind->box.pos.v[X] = x;
			wind->box.pos.v[Y] = y;
			wind->box.pos.v[Z] = z;

			wind->box.size.v[X] = x2;
			wind->box.size.v[Y] = y2;
			wind->box.size.v[Z] = z2;

			wind->windDir.v[X] = inFloat[0];
			wind->windDir.v[Y] = inFloat[1];
			wind->windDir.v[Z] = inFloat[2];

			wind->windStrength = inFloat[3];
			wind->dropOffRange = inFloat[4];
	
			wind->status = inInt;
			wind->tag = inInt2;
			AddWind(wind);
			continue;
		}
//ropeBridges
		if(strcmp(string,"ROPEBRIDGE") == 0)
		{
			fscanf(fp,"%s %c %d %f %f %f %f %f %f %f %f %f %f %f %f %f %d",string,&inChar,&inInt,&x,&y,&z,&x2,&y2,&z2,&inFloat[0],&inFloat[1],&inFloat[2],&inFloat[3],&inFloat[4],&inFloat[5],&inFloat[6],&inInt2);

			sprintf(fileName,"%sgraphics\\%s",rootPath,string);
			meshBuilder = LoadMesh(fileName);

			platformId = (PLATFORMID *)MyCalloc(1, sizeof(PLATFORMID));
			AddPlatformId(platformId);
			sprintf(platformId->name, "%s", fileName);

			platformId->visual = (LPDIRECT3DRMVISUAL)meshBuilder;

			platformId->faceCount = meshBuilder->GetFaceCount();
			meshBuilder->GetFaces(&(platformId->faceArray));

			ropeBridge[rope].minTag = tag;
			ropeBridge[rope].rotVelTransmit = inFloat[0];
			ropeBridge[rope].maxTag = tag + inInt - 1;

			for(j = 0;j < inInt;j++)
			{
				platform = (PLATFORM *)MyCalloc(1,sizeof(PLATFORM));
				AddPlatform(platform);

				platform->id = platformId;
				platform->parent = g_lpScene;
			    g_lpD3DRM->CreateFrame(platform->parent,&(platform->frame));

			    platform->frame->AddVisual((LPDIRECT3DRMVISUAL)meshBuilder);
				platform->visible = TRUE;

				platform->pos.v[X] = x + j*x2;
				platform->pos.v[Y] = y + j*y2;
				platform->pos.v[Z] = z + j*z2;

				platform->scale.v[X] = platform->scale.v[Y] = platform->scale.v[Z] = 1;

				switch(inChar)
				{
					case 'X':
						platform->spinAxis.v[X] = 1;
						platform->rot.v[X] = platform->initRot = platform->rotaim.v[X] = inFloat[4];
						if((inFloat[2] > 0) && ((j == 0) || (j == inInt-1)))
							platform->rAim.v[X] = ON;
						break;
					case 'Y':
						platform->spinAxis.v[Y] = 1;
						platform->rot.v[Y] = platform->initRot = platform->rotaim.v[Y] = inFloat[4];
						if((inFloat[2] > 0) && ((j == 0) || (j == inInt-1)))
							platform->rAim.v[Y] = ON;
						break;
					case 'Z':
						platform->spinAxis.v[Z] = 1;
						platform->rot.v[Z] = platform->initRot = platform->rotaim.v[Z] = inFloat[4];
						if((inFloat[2] > 0) && ((j == 0) || (j == inInt-1)))
							platform->rAim.v[Z] = ON;
						break;
				}
				platform->flags |= TILTING | SEESAW;
				platform->tag = tag++;
				platform->rotInertia = inFloat[1];
				platform->turnSpeed = inFloat[2];
				platform->maxRot = inFloat[3] + min(j,ropeBridge[rope].maxTag - j - ropeBridge[rope].minTag) * inFloat[6];
				platform->maxRotVel = inFloat[5];
				platform->mass = inInt2;
				InitPlatformInfo(platform);
				TransformPlatform(platform);
			}
			if(inFloat[6] == 0)
				ropeBridge[rope].type = 1;
			rope++;
		    RELEASE(meshBuilder);
			continue;
		}
		if(strcmp(string,"ELECTRIC") == 0)
		{
			fscanf(fp,"%d",&inInt);

			platform->electric = inInt;
			platform->flags |= ELECTRICAL;
			continue;
		}
		if(strcmp(string,"SINEBRIDGE") == 0)
		{
			fscanf(fp,"%s %d %f %f %f %f %f %f %f %f %f %f %f %d",string,&inInt,&x,&y,&z,&x2,&y2,&z2,&x3,&y3,&z3,&inFloat[0],&inFloat[1],&inInt2);

			sprintf(fileName,"%sgraphics\\%s",rootPath,string);
			meshBuilder = LoadMesh(fileName);

			platformId = (PLATFORMID *)MyCalloc(1, sizeof(PLATFORMID));
			AddPlatformId(platformId);
			sprintf(platformId->name, "%s", fileName);

			platformId->visual = (LPDIRECT3DRMVISUAL)meshBuilder;

			platformId->faceCount = meshBuilder->GetFaceCount();
			meshBuilder->GetFaces(&(platformId->faceArray));

			for(j = 0;j < inInt;j++)
			{
				platform = (PLATFORM *)MyCalloc(1,sizeof(PLATFORM));
				AddPlatform(platform);

				platform->id = platformId;
				platform->parent = g_lpScene;
			    g_lpD3DRM->CreateFrame(platform->parent,&(platform->frame));

			    platform->frame->AddVisual((LPDIRECT3DRMVISUAL)meshBuilder);
				platform->visible = TRUE;

				platform->scale.v[X] = platform->scale.v[Y] = platform->scale.v[Z] = 1;
				platform->scale.v[X] = 0.4;

				PlatformAddPoint(platform,x + j*x2,y + j*y2,z + j*z2,0);
				PlatformAddPoint(platform,x + j*x2 + x3,y + j*y2 + y3,z + j*z2 + z3,0);

				platform->maxSpeed = inFloat[0];
				platform->accel = inFloat[1];

				platform->direction = 1;

				for(k = 0;k < j*inInt2;k++)
					UpdatePlatform(platform);
			}
			continue;
		}
		if(strcmp(string,"ORBITBRIDGE") == 0)
		{
			fscanf(fp,"%s %d %c %f %f %f %f %f %f %f %f %f %f %d %d",string,&inInt,&inChar,&x,&y,&z,&x2,&y2,&z2,&x3,&y3,&z3,&inFloat[0],&inInt2,&i);

			sprintf(fileName,"%sgraphics\\%s",rootPath,string);
			meshBuilder = LoadMesh(fileName);

			platformId = (PLATFORMID *)MyCalloc(1, sizeof(PLATFORMID));
			AddPlatformId(platformId);
			sprintf(platformId->name, "%s", fileName);

			platformId->visual = (LPDIRECT3DRMVISUAL)meshBuilder;

			platformId->faceCount = meshBuilder->GetFaceCount();
			meshBuilder->GetFaces(&(platformId->faceArray));

			for(j = 0;j < inInt;j++)
			{
				platform = (PLATFORM *)MyCalloc(1,sizeof(PLATFORM));
				AddPlatform(platform);

				platform->id = platformId;
				platform->parent = g_lpScene;
			    g_lpD3DRM->CreateFrame(platform->parent,&(platform->frame));

			    platform->frame->AddVisual((LPDIRECT3DRMVISUAL)meshBuilder);
				platform->visible = TRUE;

				platform->scale.v[X] = platform->scale.v[Y] = platform->scale.v[Z] = 1;
				platform->scale.v[X] = 0.4;

				if(i == -1)
				{
					PlatformAddPoint(platform,x + j*x2 + x3,y + j*y2 + y3,z + j*z2 + z3,0);
					PlatformAddPoint(platform,x + j*x2,y + j*y2,z + j*z2,0);
				}
				else
				{
					k = min(j, inInt - j - 1) + i;
					PlatformAddPoint(platform,x + j*x2 + k*x3,y + j*y2 + k*y3,z + j*z2 + k*z3,0);
					PlatformAddPoint(platform,x + j*x2,y + j*y2,z + j*z2,0);
				}

				platform->orbitSpeed = inFloat[0];

				switch(inChar)
				{
					case 'X':
						platform->orbitAxis.v[X] = 1;
						break;

					case 'Y':
						platform->orbitAxis.v[Y] = 1;
						break;

					case 'Z':
						platform->orbitAxis.v[Z] = 1;
						break;
				}

				for(k = 0;k < j*inInt2;k++)
					UpdatePlatform(platform);
			}
			continue;
		}
		if(strcmp(string,",") == 0)
			continue;

		dprintf("WARNING!!! Unknown string in landscape file: %s\n",string);
	}
	fclose(fp);
/*
	for(j = 0;j < NUMPICKUPS;j++)
		RELEASE(pickupMesh[j]);
*/

	return TRUE;
}

/*	--------------------------------------------------------------------------------
	Function 	: LoadMesh
	Purpose 	: loads a .x file into a meshbuilder
	Parameters 	: filename
	Returns 	: pointer to meshbuilder
	Info 		:
*/
LPDIRECT3DRMMESHBUILDER LoadMesh(char *fName)
{
    LPDIRECT3DRMMESHBUILDER mesh_builder = NULL;
	int rval = 0;

    if (FAILED(g_lpD3DRM->CreateMeshBuilder(&mesh_builder)))
        goto generic_error;

    rval = mesh_builder->Load(fName, NULL, D3DRMLOAD_FROMFILE, LoadTextures, NULL);
    if (rval != D3DRM_OK) 
	{
        Msg("Failed to load file [%s]\n",fName);
        goto ret_with_error;
    }

	mesh_builder->SetPerspective(TRUE);

	return mesh_builder;
generic_error:
    Msg("A failure occured while building the scene.\n");
ret_with_error:
    RELEASE(mesh_builder);
    return NULL;

}

/*	--------------------------------------------------------------------------------
	Function 	: SetPlatformTerrain()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void SetPlatformTerrain(PLATFORM *platform,UBYTE terrainType)
{
	PLANELISTENTRY	*ptr;

	for(ptr = platform->planeList.head.next;ptr != &platform->planeList.head;ptr = ptr->next)
		ptr->plane->terrain = terrainType;
}


/*	--------------------------------------------------------------------------------
	Function 	: PlatformAddPoint()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void PlatformAddPoint(PLATFORM *platform,float x,float y,float z,int counter)
{
	if(platform->numPathPoints < MAXPLATFORMPATHPOINTS)
	{
		if(platform->pathPoint[platform->numPathPoints] == NULL)
		{
			platform->pathPoint[platform->numPathPoints] = (VECTOR *)MyMalloc(sizeof(VECTOR));

			if(counter == 0)
				platform->waitTime[platform->numPathPoints] = counter+1;
			else
				platform->waitTime[platform->numPathPoints] = counter;

			if(platform->waitTime[platform->numPathPoints] == -5)
				platform->waitTime[platform->numPathPoints] = -10;

			platform->pathPoint[platform->numPathPoints]->v[X] = x;
			platform->pathPoint[platform->numPathPoints]->v[Y] = y;
			platform->pathPoint[platform->numPathPoints]->v[Z] = z;

			platform->numPathPoints++;

			if(platform->numPathPoints == 1)
			{
				SetVector(&platform->pos,platform->pathPoint[0]);
				if(platform->parentPlatform)
					AddVector(&platform->pos,&platform->pos,&platform->parentPlatform->pos);
				SetVector(&platform->oldpos,&platform->pos);
				InitPlatformInfo(platform);
				TransformPlatform(platform);
				platform->counter = counter;
				if(platform->counter == 0)
					platform->counter++;
			}
		}
		else
			dprintf("WARNING!! Platform %d cannot follow path - it is an orbiter\n",platformList.numEntries);
	}
	else
	{
		dprintf("WARNING!! Platform %d has too many POINTS!\n",platformList.numEntries);
	}
}
