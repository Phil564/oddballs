#ifndef __PUZZLES_H__
#define __PUZZLES_H__

//---------------------------------------------------------------------------------
enum
{
	ACTION_START_MOVE,
	ACTION_START_SPIN,
	ACTION_START_ORBIT,
	ACTION_SET_NEXT_POINT,
	ACTION_SET_NEXT_SPIN,
	ACTION_SET_SPIN_AXIS,
	ACTION_SET_STATE,
	ACTION_SET_EXISTANCE,
	ACTION_SET_VAR,
	ACTION_CLEAR_VAR,
	ACTION_LOOK_AT_PLAT,
	ACTION_LOOK_AT_POINT,
	ACTION_SET_CAMERA_POS,
	ACTION_NME_GOTO_ACTION,
	ACTION_SET_WIND,
	NUM_ACTION_TYPES
};

enum
{
	CONDITION_AT_POINT,
	CONDITION_AT_SPIN,
	CONDITION_AT_ORBIT,
	CONDITION_GOT_TO_POINT,
	CONDITION_GOT_TO_SPIN,
	CONDITION_GOT_TO_ORBIT,
	CONDITION_HAND_ON,
	CONDITION_HAND_GOT_ON,
	CONDITION_BALL_ON,
	CONDITION_BALL_GOT_ON,
	CONDITION_NME_ON,
	CONDITION_NME_GOT_ON,
	CONDITION_HAND_TOUCHING,
	CONDITION_HAND_JUST_TOUCHED,
	CONDITION_BALL_TOUCHING,
	CONDITION_BALL_JUST_TOUCHED,
	CONDITION_NME_TOUCHING,
	CONDITION_NME_JUST_TOUCHED,
	CONDITION_VAR_SET,
	CONDITION_VAR_NOT_SET,
	CONDITION_CONFINED,
	CONDITION_JUST_CONFINED,
	CONDITION_NME_DEAD,
	NUM_CONDITION_TYPES
};

#define CONDITION_AND	0
#define CONDITION_OR	1

#define ACTION_GO_ONE_CYCLE		(1<<0)
#define ACTION_GO_ONE_STEP		(1<<1)
#define ACTION_GO_NOW			(1<<2)
#define	ACTION_SET_FAN			(1<<3)
#define ACTION_SET_TELEPORT		(1<<4)
#define ACTION_SET_CATAPULT		(1<<5)
#define ACTION_SET_ELECTRIC		(1<<6)

typedef struct TAGCONDITION
{
	struct	TAGCONDITION	*prev,*next;
	UBYTE	type;
	int		targetTag;
	int		value;
}CONDITION;

typedef struct TAGACTION
{
	struct	TAGACTION	*prev,*next;
	short	timer;
	short	delay;
	UBYTE	type;
	int		targetTag;
	float	value;
	VECTOR	vect;
	int		flags;
}ACTION;

typedef struct
{
	ACTION	head;
	int		numEntries;
	UBYTE	checkMe;
}ACTION_LIST;

typedef struct
{
	CONDITION	head;
	int			numEntries;
	UBYTE		andor;
	short		numTimes;
}CONDITION_LIST;

typedef struct TAGPUZZLE
{
	struct TAGPUZZLE	*prev,*next;
	CONDITION_LIST		conditionList;
	ACTION_LIST			actionList;
}PUZZLE;

typedef struct
{
	PUZZLE	head;
	int		numEntries;
}PUZZLE_LIST;

//---------------------------------------------------------------------------------


void AddConditionToList(CONDITION *condition,CONDITION_LIST *conditionList);
void AddActionToList(ACTION *action,ACTION_LIST *actionList);
void InitConditionList(CONDITION_LIST *conditionList);
void InitActionList(ACTION_LIST *actionList);
void InitPuzzleList();
void AddPuzzle(PUZZLE *puzzle);
void SubPuzzle(PUZZLE *puzzle);
void CheckPuzzles();
BOOL PuzzleEvaluateConditionList(CONDITION_LIST *conditionList);
BOOL PuzzleEvaluateCondition(CONDITION *condition);
PUZZLE *NewPuzzle();
void PuzzleStartActionList(ACTION_LIST *actionList);
void PuzzleCheckActionList(ACTION_LIST *actionList);
void PuzzleExecuteAction(ACTION *action);


#endif