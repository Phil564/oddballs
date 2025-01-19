#ifndef __MAIN_H
#define __MAIN_H


#define CONTROL_HAND	0
#define CONTROL_CAMERA	1

#define CAMERA_ACCELERATION 0.4
//------------------------------------------------
//detail flags
#define DF_TEXTURED		(1<<0)
#define DF_GOURAUD		(1<<1)
#define DF_FULLSCREEN	(1<<2)
#define DF_WOODY		(1<<3)
#define DF_TONY			(1<<4)
#define DF_HARDWARE		(1<<5)
#define DF_STEVE		(1<<6)
#define DF_CDAUDIO		(1<<7)
#define DF_CHEAT		(1<<8)
#define DF_NOSOUND		(1<<9)
#define DF_TIMERS		(1<<10)


//------------------------------------------------
//externs
extern LPDIRECT3DRMVISUAL lpShadowVisual;
extern char message[];
extern char controlMethod;
extern ACTORLIST	actorList;
extern int frameCount;
extern PLAYER_TYPE player;
//---------------------------------------------------
//defenitions
void MainLoop();
void InitShadow(ACTOR *actor);
void InitTimerList();
void StartTiming(char entryNum);
void EndTiming(char entryNum);




#endif