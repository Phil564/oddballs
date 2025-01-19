#ifndef __SCENE_H
#define __SCENE_H

#define NUM_SCENIC_ITEMS 50

//structures--------------------------
typedef struct
{
	char						name[256];
	LPDIRECT3DRMTEXTURE			texture;
	struct TAGTEXTURELISTENTRY 	*listPointer;
}TEXTURE;

typedef struct TAGTEXTURELISTENTRY
{
	TAGTEXTURELISTENTRY	*next,*prev;
	TEXTURE				*texture;
}TEXTURELISTENTRY;

typedef struct
{
	int					numEntries;
	TEXTURELISTENTRY		head;
}TEXTURELIST;




typedef struct
{
	LPDIRECT3DRMFRAME	frame;
	LPDIRECT3DRMVISUAL	visual;
	BOOL				visible;
}SCIT;




extern int	numScenicItems;
extern SCIT	scenicItems[];
extern ACTOR		hand;
extern ACTOR		ball;
extern ACTOR		camera;
extern LANDSCAPE	landscape;

extern LPDIRECT3DRMLIGHT dummyLight;
extern LPDIRECT3DRMFRAME dummyFrame;
//extern LPDIRECT3DRMFRAME scenicItems[];



BOOL LoadMeshIntoActor(ACTOR *actor,LPDIRECT3DRMFRAME parent,char *filename);
BOOL BuildScene(LPDIRECT3DRMDEVICE dev, LPDIRECT3DRMVIEWPORT view,LPDIRECT3DRMFRAME scene, LPDIRECT3DRMFRAME camera);
void InitScene(LPDIRECT3DRMFRAME scene,char *file);
void InitTextureList();
void AddTexture(TEXTURE *texture);


#endif