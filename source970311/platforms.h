#ifndef __PLATFORMS_H__
#define __PLATFORMS_H__

#define NUMROPEBRIDGES	10

typedef struct
{
	int		minTag;
	int		maxTag;
	double	rotVelTransmit;
	int		type;
}ROPEBRIDGE;

extern PLATFORMIDLIST	platformIdList;


extern PLATFORMLIST	platformList;
extern ROPEBRIDGE ropeBridge[];

void InitPlatformList();
void ClearPlatformList();
void AddPlatform(PLATFORM *platform);
void SubPlatform(PLATFORM *platform);
void TransformPlatform(PLATFORM *platform);
void UpdatePlatform(PLATFORM *platform);
void AddPlaneToList(PLANE *plane,PLANELIST *pl);
void InitPlaneList(PLANELIST *pl);
void InitPlatformInfo(PLATFORM *platform);
void RecalculatePlatformInfo(PLATFORM *platform);
PLATFORM *TagToFirstPlatform(int tag);
PLATFORMLISTENTRY *FindTaggedPlatform(PLATFORMLISTENTRY *startPlatform, int tag);
void UpdateRopeBridges();
void KillPlatform(PLATFORM *platform);
void AddPlatformId(PLATFORMID *platformId);
void MovePlatform(PLATFORM *platform);


void InitPlatformIdList();
void AddTexture(PLATFORMID *platformId);
BOOL LoadMeshIntoPlatform(PLATFORM *platform,LPDIRECT3DRMFRAME parent,char *fName);

#endif