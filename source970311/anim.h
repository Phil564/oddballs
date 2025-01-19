#ifndef __H_ANIMS
#define __H_ANIMS

#define STAND 0
#define WALK 1
#define FLY 2

#define BAT_WALK 0
#define BAT_FLY 1
#define BAT_TAKEOFF 2
#define BAT_LAND 3

#define RESET 0
#define NORESET 1

#define WALK_TO 0
#define FLY_TO 1


void ACB(LPDIRECT3DRMFRAME obj, void* arg, D3DVALUE delta);

LPDIRECT3DRMFRAME LoadMesh(LPDIRECT3DRMFRAME scene,char *filename);
LPDIRECT3DRMFRAME LoadFrame(LPDIRECT3DRMFRAME scene,char *filename);
BOOL LoadActor(ACTOR *myActor,LPDIRECT3DRMFRAME scene,char *filename);
BOOL LoadAnimation(animation *anim,int noFrames, double speed, char *filename, ACTOR *actor);
BOOL SetAnimCallBack(ACTOR *myActor, LPDIRECT3DRMANIMATIONSET animset);
void StopAnimationCallBack(ACTOR *myActor);
void AttachAnimations(ACTOR *myActor,animation *anim);
void AnimationCallback(LPDIRECT3DRMFRAME obj,void *myActor, D3DVALUE delta);
void InitActor(ACTOR *myactor);
void StopAnimateActor(ACTOR *myActor,int mode);
void GoTo(ACTOR *myActor,double x,double y,double z,double speed,double inertia);
void AutoAnimator(ACTOR *myActor); 
void InitScript(NME_SCRIPT *myScript, NME_INSTRUCTION *myInstructions, int numberOfInstuctions);
void FlushQueue(ACTOR *myActor);
void EndAnimationLoop(ACTOR *myActor);
void ProcessScript(ACTOR *myActor);
void AnimateActor(ACTOR *myActor,int anim,BOOL loop,BOOL queue,double speed);
void SetAnimationSpeed(ACTOR *myActor, double speed);

#endif