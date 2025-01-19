#ifndef __COLLECTABLES_H
#define __COLLECTABLES_H


enum
{
	LIVES_ICON,
	SUCTION_ICON,
	GAUNTLET_ICON,
	SPEEDUP_ICON,
	ISL_ICON,
	HEAVYBALL_ICON,
	LIGHTBALL_ICON,
	TIMEOUT_ICON,
	GAMEWON_ICON,
	LIVESGONE_ICON,
};


#define MAX_COLLECTABLES	50
#define MAX_DEBRIS			50


//debris types
enum
{
	DUST,
	STARS,
	PLATBIT,
	SPLASH,
	FIREBALL,
	MAGIC,
	SPARK,
	NUM_DEBRIS_TYPES

};

//number of animation frames
#define NUM_STAR_FRAMES 3
#define NUM_DUST_FRAMES 8
#define NUM_PLAT_FRAMES 4
#define NUM_SPLASH_FRAMES 8
#define NUM_FIREBALL_FRAMES 5
#define NUM_MAGIC_FRAMES 12
#define NUM_SPARK_FRAMES 12

//debris animation types
#define DEBRIS_ANIM_NONE	0
#define DEBRIS_ANIM_CYCLE	1
#define DEBRIS_ANIM_ONESHOT 2

//collectable structure
typedef struct
{
	LPDIRECT3DRMFRAME	frame;
	VECTOR				pos;
	int					type;
	double				size;
	BOOL				active;
	int					delay;
}COLLECTABLE_TYPE;

//debris structure
typedef struct
{
	ACTOR *actor;
	int		active;
	BOOL	flash;
	int		currentFrame;
	int		animationType;
	int		numFrames;
	int		type;
	LPDIRECT3DRMTEXTURE	*texture;
}DEBRIS_TYPE;



extern DEBRIS_TYPE debris[];
extern COLLECTABLE_TYPE collectables[];
extern int numberOfCollectables;
extern LPDIRECT3DRMMESHBUILDER pickupMesh[];
extern char *pickupName[];

void InitCollectables();
void ProcessCollectables();
void ActivateCollectable(int type);
void DeactivateCollectable(int type);
void AddDebris(int debrisType, VECTOR *position, VECTOR *velocity);
void UpdateDebris();
void CreateDustCircle(VECTOR *pos, int numStars, double verticalVelocity);
//void CreateDustCircle(VECTOR *pos, int numStars);
void CreateStarCircle(VECTOR *pos, int numStars);
void CreateStarCircleXYZ(double x, double y, double z, int numStars);
void CreateSplashCircle(VECTOR *pos, int numStars, double verticalVelocity);
void ResetCollectables();



#endif