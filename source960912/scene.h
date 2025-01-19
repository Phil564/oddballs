#ifndef __SCENE_H
#define __SCENE_H

#define	X	0
#define	Y	1
#define	Z	2

typedef struct
{
	double	v[3];
}VECTOR;


typedef struct
{
	LPDIRECT3DRMFRAME		parent;
	LPDIRECT3DRMFRAME		frame;
	VECTOR					pos;
	VECTOR					vel;
	double					speed;
	VECTOR					rot;
	VECTOR					rotvel;
	VECTOR					rotaim;
	int						faceCount;
	LPDIRECT3DRMFACEARRAY	faceArray;
	double					inertia;
} ACTOR;

BOOL LoadMeshIntoActor(ACTOR *actor,LPDIRECT3DRMFRAME parent,char *filename);

extern ACTOR	hand;
extern ACTOR	ball;
extern ACTOR	camera;

#endif