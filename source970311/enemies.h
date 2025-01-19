#ifndef __ENEMIES_H
#define __ENEMIES_H

#define MAX_ENEMIES (15)

#define HURT_BY_SLAM	(1 << 1)
#define HURT_BY_FLICK	(1 << 2)
#define HURT_BY_SLAP	(1 << 3)
#define HURN_BY_BALL	(1 << 4)
#define DEADLY_TO_BALL	(1 << 5)
#define DEADLY_TO_HAND	(1 << 6)
#define SPECIFY_X		(1 << 7)


extern int numberOfEnemies;
extern ENEMY_TYPE enemies[];
extern animation enemyAnims[MAX_ENEMIES][20];


void LoadEnemy(int enemyType, double xpos, double ypos, double zpos, double mass,int flags,int tag);



//enemy actions
BOOL NMEActionMoveTo(ENEMY_TYPE *enemy, VECTOR *dest, float proximity, int flags);
BOOL NMEActionMoveTo2D(ENEMY_TYPE *enemy, VECTOR *dest, float proximity, int flags);
void NMEActionWait(ENEMY_TYPE *enemy);
BOOL NMEActionFaceTo(ENEMY_TYPE *enemy, VECTOR *dest);
void NMEActionRandomMove(ENEMY_TYPE *enemy);
void NMEActionRandomMove(ENEMY_TYPE *enemy);
BOOL NMEActionFacePlayer(ENEMY_TYPE *enemy, int flags);
BOOL NMEActionFollowPlayer(ENEMY_TYPE *enemy);
BOOL NMEActionFleePlayer(ENEMY_TYPE *enemy);
BOOL NMEActionCharge(ENEMY_TYPE *enemy);
BOOL NMEActionCircle(ENEMY_TYPE *enemy);
void NMEActionFire1(ENEMY_TYPE *enemy);






#endif