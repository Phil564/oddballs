#ifndef __NMEBEHAVIOUR_H
#define __NMEBEHAVIOUR_H

//enemy actions
enum
{
	
	NME_MOVETO,
	NME_MOVETO2D,
	NME_FACETO,
	NME_RANDOMMOVE,
	NME_WAIT,
	NME_FIRE1,
	NME_FIRE2,
	NME_SPECIAL,
	NME_FACEPLAYER,
	NME_FOLLOWPLAYER,
	NME_HOLDPLAYER,
	NME_DROPPLAYER,
	NME_FLEEPLAYER,
	NME_CIRCLE,
	NME_DONTATTACK,
	NME_ATTACK,
	NME_ENDATTACK,
	NME_CHARGE,
	NME_BRANCH,
	NME_RETURN,
	NME_LOOP,
	NME_LOADCOUNTER,
	NME_MOVE_VECTOR

};

//enemy flags
#define	NMEFLAGS_MOVE_ACCEL				(1<<0)	//1	signals to accellerate enemies
#define NMEFLAGS_MOVE_VEL				(1<<1)	//2	signals to add to velocity
#define NMEFLAGS_DONT_SLOW_WHEN_NEAR	(1<<2)	//4	used with moveto's
#define NMEFLAGS_USE_X_AXIS				(1<<3)	//8	used for circling
#define NMEFLAGS_USE_Y_AXIS				(1<<4)	//16	used for circling
#define NMEFLAGS_USE_Z_AXIS				(1<<5)	//32	used for circling
#define NMEFLAGS_SPECIFY_HAND			(1<<6)	//64	used for follow, flee, face
#define NMEFLAGS_SPECIFY_BALL			(1<<7)	//128	used for follow, flee, face
#define NMEFLAGS_SPECIFY_CLOSEST		(1<<8)	//256	used for follow, flee, face
#define NMEFLAGS_DONT_CHANGE_ROT		(1<<9)	//512	used for circling
#define NMEFLAGS_ANTICLOCKWISE			(1<<10)	//1024	used to specify circling direction
#define NMEFLAGS_DONT_INTERUPT			(1<<11)	//2048	if a conditional instruction is allowed to happen
												//	act on it, but dont interupt normal instr list.
#define NMEFLAGS_DONT_FACE_DIR			(1<<12)	//4096	dont face in the direction of travel
#define NMEFLAGS_BANK					(1<<13)	//8192	allow enemy to bank when he turns
#define NMEFLAGS_DONTANIMATE			(1<<14) //16384 dont animate action


//enemy action conditions
enum
{
	IF_BALL_IN_RANGE,
	IF_BALL_IN_RANGE_2D,
	IF_HAND_IN_RANGE,
	IF_HAND_IN_RANGE_2D,
	IF_CLOSEST_IN_RANGE,
	IF_CLOSEST_IN_RANGE_2D,
	IF_CANSEE_BALL,
	IF_CANSEE_HAND,
	IF_CANSEE_CLOSEST,
	EVERY_NUM_FRAMES,
	BETWEEN_SPIN_VALS,
	IF_HAND_HOLDING_BALL,
	IF_HAND_NOT_HOLDING_BALL,
	IF_IM_HOLDING_BALL,
	IF_IM_NOT_HOLDING_BALL,
	IF_IM_HOLDING_HAND,
	IF_IM_NOT_HOLDING_HAND,
	IF_TOUCHING_BALL,
	IF_TOUCHING_HAND,
	ALWAYS,
	NEVER,
	SOMETIMES
};

//enemy action flags
//enum
/*
#define		ATTACK_BALL_IF_NEAR		(1 << 0)
#define 	ATTACK_HAND_IF_NEAR		(1 << 1)
#define 	ATTACK_NME_IF_NEAR		(1 << 2)
#define 	RETREAT_BALL_IF_NEAR	(1 << 3)
#define 	RETREAT_HAND_IF_NEAR	(1 << 4)
#define 	RETREAT_NME_IF_NEAR		(1 << 5)

#define 	DEFEND_BALL_IF_NEAR		(1 << 6)
#define 	DEFEND_HAND_IF_NEAR		(1 << 7)
#define 	DEFEND_NME_IF_NEAR		(1 << 8)

#define 	DROP_BALL_ON_ARRIVAL	(1 << 9)
#define 	DONT_PICK_UP_BALL		(1 << 10)

#define 	DONT_ATTACK				(1 << 11)

#define 	FACE_DIRECTION			(1 << 12)
#define 	FACE_POSITION			(1 << 13)

#define 	DONT_FACE_DIR			(1 << 14)
*/




//standard enemy animations
enum
{
	NMEANIM_IDLE,
	NMEANIM_WALK,
	NMEANIM_RUN,
	NMEANIM_START,
	NMEANIM_STOP,
	NMEANIM_ATTACK1,
	NMEANIM_ATTACK2,
	NMEANIM_SPECIAL1,
	NMEANIM_SPECIAL2,
	NMEANIM_SPECIAL3,
	NMEANIM_SPECIAL4,

	NMEANIM_ENDATTACK,
//	NMEANIM_TAKEOFF,
//	NMEANIM_LAND,
//	NMEANIM_FLY,
	NMEANIM_DEATH
};

//actor types
enum
{
	CAMERA,
	HAND,
	BALL,
	DEBRIS,
	BAT,
	CHEST,
	LOBSTER,
	CUTLASS,
	PARROT,
	SHARKFIN,
	BIGBALL,
	CYLON,
	DROID,
	FIGHTER,
	SNEEZY,
	BULLET
};



//-------------------------------------------------------

extern char *actorName[];


#define NORTH		(0)
#define EAST		(PI/2)
#define SOUTH		(PI)
#define WEST		(PI+PI/2)


extern NME_INSTRUCTION *nmeInstruction[];

/*
void ProcessActorScript(ACTOR *);
void InitActorScript(ACTOR *actor);
void ControlEnemy(int enemyNumber);
void NmeGetNextInstruction(ACTOR *actor, int instructionNum);
*/
void SetupTestScript();
void NMEProcessActorScript(ENEMY_TYPE *enemy);
void NMEAddNormalInstruction(FILE *fp);
void NMEAddConditionalInstruction(FILE *fp);
void NMEAddAttackInstruction(FILE *fp);
void NMEGetNewInstruction(ENEMY_TYPE *enemy, short instructionNum, BOOL overrideConditions);
BOOL NMEConditionsMet(ENEMY_TYPE *enemy, NME_INSTRUCTION *instruction);

void NMEProcessDroid(ENEMY_TYPE *enemy);
void NMEProcessCylon(ENEMY_TYPE *enemy);
void NMEProcessFighter(ENEMY_TYPE *enemy);
void NMEProcessSneezy(ENEMY_TYPE *enemy);


#endif