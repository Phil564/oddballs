/*
 *	Project:		Oddballs
 *	File:			personal.h
 *	Description:	personal options
 */

#ifndef __PERSONAL_H
#define __PERSONAL_H

typedef struct
{
	ACTOR	actor[8];
	BOX		box;
}HELPER;

extern HELPER helper;

extern VECTOR	ballStart,handStart;
extern long detailFlags;
extern LPDIRECT3DRMTEXTURE	sphereTexture[10];

extern animation batanims[];
extern char outputMessage[];
extern char rootPath[];
extern char levelToLoad[];
extern VECTOR mapRes;
extern int camMode;

void ResetCamera();
void ResetHand();
void ResetBall(ACTOR *ball);

void InitScene(LPDIRECT3DRMFRAME scene);
void ProcessPersonalKeys(WPARAM key);
void ProcessPersonalLoop();
void RestartHand();
void RestartBall();
void RestartCamera();
void InitPlayer();


#endif
