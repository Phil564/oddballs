#ifndef __BULLETS_H__
#define __BULLETS_H__


enum BULLET_DEF
{
	FIGHTER_BULLET,
	DROID_BULLET,
	SNEEZY_BULLET,
	NUM_BULLETS,
};

#define MAX_ACTOR_BULLETS 6

typedef struct
{
	VECTOR	originalVel[MAX_ACTOR_BULLETS];
	VECTOR	offsetFromActor[MAX_ACTOR_BULLETS];
	char	numGuns;
	float	mass;
	short	type;
	short	lifetime;
	int		flags;
	float	maxSpeed;
	float	velInertia;
	float	normInertia;
	float	radius;
	char	dirFromActor;		// 0 takes velocity
								// 1 takes actor dir only in y direction
								// 2 takes actor dir completely
	LPDIRECT3DRMVISUAL	visual;
	void	(*destroyCallBack)(TAGACTOR *);
	BOOL	(*maintainCallBack)(TAGACTOR *);
}BULLET_TYPE;







//void FireBulletFromActor(ACTOR *actor);
void FireBulletFromActor(ACTOR *actor, short type);
void FireSlapBullet();
void InitBulletStructures();

//bullet destroy functions
void DestroyBulletFighter(ACTOR *actor);
void DestroyBulletDroid(ACTOR *actor);

//bullet maintain functions
BOOL MaintainBulletDroid(ACTOR *actor);
BOOL MaintainGenericBullet(ACTOR *actor);




#endif
