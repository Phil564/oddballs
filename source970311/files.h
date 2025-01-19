#ifndef __FILES_H
#define __FILES_H

enum
{
	PTREE1,
	PTREE2,
	GRASS1,
	NUM_SCENERY_TYPES
};

BOOL FileExists(char *filename);
BOOL ParseLandscapeFile(char *file);
LPDIRECT3DRMMESHBUILDER LoadMesh(char *fName);
void SetPlatformTerrain(PLATFORM *platform,UBYTE terrainType);
void PlatformAddPoint(PLATFORM *platform,float x,float y,float z,int counter);

#endif

