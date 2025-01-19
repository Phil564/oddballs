/*
 *  Project:         Oddballs
 *  File:            Puzzles.cpp
 *  Description:     
 *
 *  (c)1996 ISL.
 */

#include "oddballs.h"
#include "platforms.h"
#include "puzzles.h"
#include "enemies.h"
#include "cam.h"
#include "nmebehaviour.h"
#include "wind.h"

PUZZLE_LIST		puzzleList;

#define NUMPUZZLEVALS	100
UBYTE	puzzleVal[NUMPUZZLEVALS];

/*	--------------------------------------------------------------------------------
	Function 	: AddConditionToList()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void AddConditionToList(CONDITION *condition,CONDITION_LIST *conditionList)
{
	if(condition->next == NULL)
	{
		conditionList->numEntries++;
		condition->prev = &conditionList->head;
		condition->next = conditionList->head.next;
		conditionList->head.next->prev = condition;
		conditionList->head.next = condition;
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: AddActionToList()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void AddActionToList(ACTION *action,ACTION_LIST *actionList)
{
	if(action->next == NULL)
	{
		actionList->numEntries++;
		action->next = &actionList->head;
		action->prev = actionList->head.prev;
		actionList->head.prev->next = action;
		actionList->head.prev = action;
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: InitConditionList()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void InitConditionList(CONDITION_LIST *conditionList)
{
	conditionList->numEntries = 0;
	conditionList->head.next = conditionList->head.prev = &conditionList->head;
	conditionList->numTimes = -1;
}

/*	--------------------------------------------------------------------------------
	Function 	: InitActionList()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void InitActionList(ACTION_LIST *actionList)
{
	actionList->numEntries = 0;
	actionList->head.next = actionList->head.prev = &actionList->head;
}

/*	--------------------------------------------------------------------------------
	Function 	: InitPuzzleList()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void InitPuzzleList()
{
	int	j;

	for(j = 0;j < NUMPUZZLEVALS;j++)
		puzzleVal[j] = 0;
	puzzleList.numEntries = 0;
	puzzleList.head.next = puzzleList.head.prev = &puzzleList.head;
}

/*	--------------------------------------------------------------------------------
	Function 	: NewPuzzle()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
PUZZLE *NewPuzzle()
{
	PUZZLE		*puzzle;

	puzzle = (PUZZLE *)MyCalloc(1,sizeof(PUZZLE));
	AddPuzzle(puzzle);
	InitConditionList(&puzzle->conditionList);
	InitActionList(&puzzle->actionList);

	return puzzle;
}

/*	--------------------------------------------------------------------------------
	Function 	: AddPuzzle()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void AddPuzzle(PUZZLE *puzzle)
{
	if(puzzle->next == NULL)
	{
		puzzleList.numEntries++;
		puzzle->prev = &puzzleList.head;
		puzzle->next = puzzleList.head.next;
		puzzleList.head.next->prev = puzzle;
		puzzleList.head.next = puzzle;
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: SubPuzzle()
	Purpose 	: removes a puzzle from the linked list
	Parameters 	: pointer to puzzle
	Returns 	: none
	Info 		:
*/
void SubPuzzle(PUZZLE *puzzle)
{
	if(puzzle->next != NULL)
	{
		puzzle->prev->next = puzzle->next;
		puzzle->next->prev = puzzle->prev;
		puzzleList.numEntries--;
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: CheckPuzzles()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void CheckPuzzles()
{
	PUZZLE	*puzzle;

	for(puzzle = puzzleList.head.next;puzzle != &puzzleList.head;puzzle = puzzle->next)
	{
		if((puzzle->conditionList.numTimes) && (PuzzleEvaluateConditionList(&puzzle->conditionList)))
		{
			if(puzzle->conditionList.numTimes > 0)
				puzzle->conditionList.numTimes--;
			PuzzleStartActionList(&puzzle->actionList);
//			dprintf("!\n");
		}
		if(puzzle->actionList.checkMe)
			PuzzleCheckActionList(&puzzle->actionList);
 	}
}

/*	--------------------------------------------------------------------------------
	Function 	: CheckActionList()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void PuzzleCheckActionList(ACTION_LIST *actionList)
{
	ACTION	*action;
	int num = 0;

	for(action = actionList->head.next;action != &actionList->head;action = action->next)
	{
		if(action->timer != -1)
		{
			if(action->timer == 0)
				PuzzleExecuteAction(action);
			action->timer--;
		}
		else 
			num++;
	}
	if(num == actionList->numEntries)
		actionList->checkMe = NO;
}

/*	--------------------------------------------------------------------------------
	Function 	: StartActionList()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void PuzzleStartActionList(ACTION_LIST *actionList)
{
	ACTION	*action;

	actionList->checkMe = YES;
	for(action = actionList->head.next;action != &actionList->head;action = action->next)
	{
		if(action->timer == -1)
			action->timer = action->delay;
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: ExecuteAction()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void PuzzleExecuteAction(ACTION *action)
{
	PLATFORMLISTENTRY	*platformPtr;
	PLATFORM			*platform;
	WIND				*wind;
	static VECTOR		camVect;
	static int			frameNum;
	int					j;

	switch(action->type)
	{
		case ACTION_SET_VAR:
			puzzleVal[(int)action->value] = YES;
			break;

		case ACTION_CLEAR_VAR:
			puzzleVal[(int)action->value] = NO;
			break;
		
		case ACTION_SET_CAMERA_POS:
			SetVector(&camVect,&action->vect);
			frameNum = frameCount;
			break;

		case ACTION_LOOK_AT_POINT:
			if(cameraInfo.overrideCounter == 0)
			{
				if(frameCount == frameNum)
					SetCameraOverride(camVect.v[X],camVect.v[Y],camVect.v[Z],&action->vect,&zero,action->value);
				else
					SetCameraOverride(camera.pos.v[X],camera.pos.v[Y],camera.pos.v[Z],&action->vect,&zero,action->value);
			}
			break;

		case ACTION_NME_GOTO_ACTION:
			for(j = 0;j < numberOfEnemies;j++)
				if(enemies[j].tag == action->targetTag)
					NMEGetNewInstruction(&enemies[j],action->value,YES);
			break;

		case ACTION_SET_WIND:
			wind = &windList.head;
			do
			{
				wind = FindTaggedWind(wind,action->targetTag);
				if(wind)				
					wind->status = action->value;
			}while(wind);
			break;
 
 		default:
			platformPtr = &platformList.head;
			do
			{
				platformPtr = FindTaggedPlatform(platformPtr,action->targetTag);
				if(platformPtr != NULL)
				{	
					platform = platformPtr->platform;
					if((action->type == ACTION_START_MOVE) || (action->type == ACTION_START_ORBIT)
					 || (action->type == ACTION_START_SPIN) || (action->type == ACTION_SET_NEXT_POINT)
					 || (action->type == ACTION_SET_NEXT_SPIN))
					{
						platform->actionFlags = 0;
						if(action->flags & ACTION_GO_ONE_STEP)
							platform->actionFlags = ACTION_GO_ONE_STEP;
						else if(action->flags & ACTION_GO_ONE_CYCLE)
							platform->actionFlags = ACTION_GO_ONE_CYCLE;
					}
					switch(action->type)
					{
						case ACTION_START_MOVE:
							if((action->value != 0) || (action->flags & ACTION_GO_NOW))
							{
								platform->maxSpeed = action->value;
								if((action->flags & ACTION_GO_NOW) && (platform->speed > platform->maxSpeed))
								{
									ScaleVector(&platform->vel,platform->maxSpeed/platform->speed);
									platform->speed = platform->maxSpeed;
								}
							}
							break;

						case ACTION_START_SPIN:
							if((action->value != 0) || (action->flags & ACTION_GO_NOW))
								platform->spinSpeed = action->value;
							break;

						case ACTION_START_ORBIT:
							if((action->value != 0) || (action->flags & ACTION_GO_NOW))
							{
								platform->orbitSpeed = action->value;
								if(platform->orbitSpeed == 0)
									SetVector(&platform->vel,&zero);
							}
							break;

						case ACTION_SET_NEXT_POINT:
							platform->nextPoint = action->value;
							break;

						case ACTION_SET_SPIN_AXIS:
							platform->spinAxis.v[X] = platform->spinAxis.v[Y] = platform->spinAxis.v[Z] = 0;
							platform->spinAxis.v[(int)action->value] = 1;
							break;

						case ACTION_SET_NEXT_SPIN:
							platform->spinNextPoint = action->value;
							break;
	
						case ACTION_SET_STATE:
							if(action->flags & ACTION_SET_FAN)
								platform->fan->status = action->value;
							if(action->flags & ACTION_SET_TELEPORT)
								platform->teleport->status = action->value;
							if(action->flags & ACTION_SET_TELEPORT)
								platform->catapult->status = action->value;
							if(action->flags & ACTION_SET_ELECTRIC)
								platform->electric = action->value;
							break;

						case ACTION_SET_EXISTANCE:
							if(action->value)
							{
								platform->flags &= -1 - BACKGROUND;
							    platform->frame->AddVisual(platform->id->visual);
							}
							else
							{
								platform->flags |= BACKGROUND;
								RemoveVisuals(platform->frame);
							}
							break;

						case ACTION_LOOK_AT_PLAT:
							if(cameraInfo.overrideCounter == 0)
								SetCameraOverride(action->vect.v[X],action->vect.v[Y],action->vect.v[Z],&platform->pos,&platform->vel,action->value);
							break;
					}
				}
			} while(platformPtr);
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: EvaluateConditionList()
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
BOOL PuzzleEvaluateConditionList(CONDITION_LIST *conditionList)
{
	CONDITION	*condition;
	int			num;

	if(conditionList->andor == CONDITION_AND)
	{
		for(condition = conditionList->head.next;condition != &conditionList->head;condition = condition->next)
		{
			if(PuzzleEvaluateCondition(condition) == FALSE)
				return FALSE;
		}
		return TRUE;
	}
	else
	{
		num = 0;
		for(condition = conditionList->head.next;condition != &conditionList->head;condition = condition->next)
		{
			if(PuzzleEvaluateCondition(condition) == TRUE)
			{
				num++;
				if(num == conditionList->andor)
					return TRUE;
			}
		}
		return FALSE;
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: EvaluateCondition
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
BOOL PuzzleEvaluateCondition(CONDITION *condition)
{
	PLATFORMLISTENTRY	*platformPtr;
	PLATFORM			*platform;
	int					j;

	switch(condition->type)
	{
		case CONDITION_VAR_SET:
			return puzzleVal[condition->value];

		case CONDITION_VAR_NOT_SET:
			return !puzzleVal[condition->value];

		case CONDITION_NME_DEAD:
			if(condition->value)
			{
				for(j = 0;j < numberOfEnemies;j++)
					if((enemies[j].tag == condition->targetTag) && (enemies[j].actor.status != INACTIVE))
						return FALSE;
			}
			else
			{
				for(j = 0;j < numberOfEnemies;j++)
					if((enemies[j].tag == condition->targetTag) && (enemies[j].actor.status != INACTIVE))
						return TRUE;
				return FALSE;
			}
			break;

		default:
			platformPtr = &platformList.head;
			do
			{
				platformPtr = FindTaggedPlatform(platformPtr,condition->targetTag);
				if(platformPtr != NULL)
				{	
					platform = platformPtr->platform;
					switch(condition->type)
					{
						case CONDITION_AT_POINT:
							if((platform->currentPoint != condition->value) || (platform->counter == 0))
								return FALSE;
							break;

						case CONDITION_GOT_TO_POINT:
							if((platform->currentPoint != condition->value) || (platform->counter != platform->waitTime[platform->currentPoint]))
								return FALSE;
							break;

						case CONDITION_AT_SPIN:
							if((((platform->spinNextPoint + platform->spinNumPauses - 1) MOD platform->spinNumPauses) != condition->value) || (platform->spinCounter == 0))
								return FALSE;
							break;

						case CONDITION_GOT_TO_SPIN:
							if((((platform->spinNextPoint + platform->spinNumPauses - 1) MOD platform->spinNumPauses) != condition->value) || (platform->spinCounter != platform->spinPauseTime))
								return FALSE;
							break;

						case CONDITION_AT_ORBIT:
							if((((platform->orbitNextPoint + platform->orbitNumPauses - 1) MOD platform->orbitNumPauses) != condition->value) || (platform->orbitCounter == 0))
								return FALSE;
							break;

						case CONDITION_GOT_TO_ORBIT:
							if((((platform->orbitNextPoint + platform->orbitNumPauses - 1) MOD platform->orbitNumPauses) != condition->value) || (platform->orbitCounter != platform->orbitPauseTime))
								return FALSE;
							break;

						case CONDITION_HAND_ON:
							if(((hand.platform == platform) && (condition->value)) || ((hand.platform != platform) && (condition->value == 0)))
								return TRUE;
							else
								return FALSE;
							break;

						case CONDITION_HAND_GOT_ON:
							if(((condition->value) && (hand.platform == platform) && (hand.oldPlatform != platform)) || ((condition->value == 0) && (hand.oldPlatform == platform) && (hand.platform != platform)))
								return TRUE;
							else
								return FALSE;
							break;

						case CONDITION_BALL_ON:
							if(((ball.platform == platform) && (condition->value)) || ((ball.platform != platform) && (condition->value == 0)))
								return TRUE;
							else
								return FALSE;
							break;

						case CONDITION_BALL_GOT_ON:
							if(((condition->value) && (ball.platform == platform) && (ball.oldPlatform != platform)) || ((condition->value == 0) && (ball.oldPlatform == platform) && (ball.platform != platform)))
								return TRUE;
							else
								return FALSE;
							break;

						case CONDITION_NME_ON:
							if(condition->value)
							{
								for(j = 0;j < numberOfEnemies;j++)
									if(enemies[j].actor.platform == platform)
										return TRUE;
								return FALSE;
							}
							else
							{
								for(j = 0;j < numberOfEnemies;j++)
									if(enemies[j].actor.platform == platform)
										break;
								if(j != numberOfEnemies)
									return FALSE;
							}
							break;

						case CONDITION_NME_GOT_ON:
							if(condition->value)
							{
								for(j = 0;j < numberOfEnemies;j++)
									if((enemies[j].actor.platform == platform) && (enemies[j].actor.oldPlatform != platform))
										return TRUE;
								return FALSE;
							}
							else
							{
								for(j = 0;j < numberOfEnemies;j++)
									if((enemies[j].actor.oldPlatform == platform) && (enemies[j].actor.platform != platform))
										return TRUE;
								return FALSE;
							}
							break;
 
						case CONDITION_HAND_TOUCHING:
							if(((hand.touchPlatform == platform) && (condition->value)) || ((hand.touchPlatform != platform) && (condition->value == 0)))
								return TRUE;
							else
								return FALSE;
							break;

						case CONDITION_HAND_JUST_TOUCHED:
							if(((condition->value) && (hand.touchPlatform == platform) && (hand.oldTouchPlatform != platform)) || ((condition->value == 0) && (hand.oldTouchPlatform == platform) && (hand.touchPlatform != platform)))
								return TRUE;
							else
								return FALSE;
							break;

						case CONDITION_BALL_TOUCHING:
							if(((ball.touchPlatform == platform) && (condition->value)) || ((ball.touchPlatform != platform) && (condition->value == 0)))
								return TRUE;
							else
								return FALSE;
							break;

						case CONDITION_BALL_JUST_TOUCHED:
							if(((condition->value) && (ball.touchPlatform == platform) && (ball.oldTouchPlatform != platform)) || ((condition->value == 0) && (ball.oldTouchPlatform == platform) && (ball.touchPlatform != platform)))
								return TRUE;
							else
								return FALSE;
							break;

						case CONDITION_NME_TOUCHING:
							if(condition->value)
							{
								for(j = 0;j < numberOfEnemies;j++)
									if(enemies[j].actor.touchPlatform == platform)
										return TRUE;
								return FALSE;
							}
							else
							{
								for(j = 0;j < numberOfEnemies;j++)
									if(enemies[j].actor.touchPlatform == platform)
										break;
								if(j != numberOfEnemies)
									return FALSE;
							}
							break;

						case CONDITION_NME_JUST_TOUCHED:
							if(condition->value)
							{
								for(j = 0;j < numberOfEnemies;j++)
									if((enemies[j].actor.touchPlatform == platform) && (enemies[j].actor.oldTouchPlatform != platform))
										return TRUE;
								return FALSE;
							}
							else
							{
								for(j = 0;j < numberOfEnemies;j++)
									if((enemies[j].actor.oldTouchPlatform == platform) && (enemies[j].actor.touchPlatform != platform))
										return TRUE;
								return FALSE;
							}
							break;
 
						case CONDITION_CONFINED:
							if(condition->value < 3)
							{
								if(platform->pos.v[condition->value] != platform->confineBox->pos.v[condition->value])
									return FALSE;
							}
							else if(platform->pos.v[condition->value - 3] != platform->confineBox->pos.v[condition->value - 3] + platform->confineBox->size.v[condition->value - 3])
								return FALSE;

						case CONDITION_JUST_CONFINED:
							if(condition->value < 3)
							{
								if((platform->pos.v[condition->value] != platform->confineBox->pos.v[condition->value]) || (platform->oldpos.v[condition->value] == platform->confineBox->pos.v[condition->value]))
									return FALSE;
							}
							else if((platform->pos.v[condition->value - 3] != platform->confineBox->pos.v[condition->value - 3] + platform->confineBox->size.v[condition->value - 3]) || (platform->oldpos.v[condition->value - 3] == platform->confineBox->pos.v[condition->value - 3] + platform->confineBox->size.v[condition->value - 3]))
								return FALSE;
 					}
				}
			} while(platformPtr);
			break;
	}
	return TRUE;
}