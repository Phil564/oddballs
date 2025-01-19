/*
 *	Project:		oddballs
 *	File:			NmeBehaviour.cpp
 *	Description:	controls enemy behaviour
 *
 *	(c)1996 ISL.
 */


#include "oddballs.h"
#include "nmebehaviour.h"
#include "anim.h"
#include "hand.h"
#include "actors.h"
#include "enemies.h"
#include "cam.h"
#include "collectables.h"
#include "sound.h"
#include "bullets.h"


static BOOL	parrotTrigger = FALSE;

extern char *actorName[] = {"CAMERA","HAND","BALL","DEBRIS","BAT","CHEST","LOBSTER",
							"CUTLASS","PARROT","SHARKFIN","BIGBALL","CYLON","DROID","FIGHTER",
							"SNEEZY"};

/*	--------------------------------------------------------------------------------
	Function 	: NMEDetermineConditionType
	Purpose 	: converts a string into the correct condition type number
	Parameters 	: char *string
	Returns 	: char
	Info 		:
*/
char NMEDetermineConditionType(char * string)
{
	if(strcmp(string,"IF_BALL_IN_RANGE") == 0)
		return IF_BALL_IN_RANGE;
	if(strcmp(string,"IF_BALL_IN_RANGE_2D") == 0)
		return IF_BALL_IN_RANGE_2D;
	if(strcmp(string,"IF_HAND_IN_RANGE") == 0)
		return IF_HAND_IN_RANGE;
	if(strcmp(string,"IF_HAND_IN_RANGE_2D") == 0)
		return IF_HAND_IN_RANGE_2D;
	if(strcmp(string,"IF_CLOSEST_IN_RANGE") == 0)
		return IF_CLOSEST_IN_RANGE;
	if(strcmp(string,"IF_CLOSEST_IN_RANGE_2D") == 0)
		return IF_CLOSEST_IN_RANGE_2D;
	if(strcmp(string,"IF_CANSEE_BALL") == 0)
		return IF_CANSEE_BALL;
	if(strcmp(string,"IF_CANSEE_HAND") == 0)
		return IF_CANSEE_HAND;
	if(strcmp(string,"IF_CANSEE_CLOSEST") == 0)
		return IF_CANSEE_CLOSEST;
	if(strcmp(string,"EVERY_NUM_FRAMES") == 0)
		return EVERY_NUM_FRAMES;
	if(strcmp(string,"BETWEEN_SPIN_VALS") == 0)
		return BETWEEN_SPIN_VALS;
	if(strcmp(string,"IF_HAND_HOLDING_BALL") == 0)
		return IF_HAND_HOLDING_BALL;
	if(strcmp(string,"IF_HAND_NOT_HOLDING_BALL") == 0)
		return IF_HAND_NOT_HOLDING_BALL;
	if(strcmp(string,"IF_IM_HOLDING_BALL") == 0)
		return IF_IM_HOLDING_BALL;
	if(strcmp(string,"IF_IM_NOT_HOLDING_BALL") == 0)
		return IF_IM_NOT_HOLDING_BALL;
	if(strcmp(string,"IF_IM_HOLDING_HAND") == 0)
		return IF_IM_HOLDING_HAND;
	if(strcmp(string,"IF_IM_NOT_HOLDING_HAND") == 0)
		return IF_IM_NOT_HOLDING_HAND;
	if(strcmp(string,"IF_TOUCHING_BALL") == 0)
		return IF_TOUCHING_BALL;
	if(strcmp(string,"IF_TOUCHING_HAND") == 0)
		return IF_TOUCHING_HAND;





	if(strcmp(string,"ALWAYS") == 0)
		return ALWAYS;
	if(strcmp(string,"NEVER") == 0)
		return NEVER;
	if(strcmp(string,"SOMETIMES") == 0)
		return SOMETIMES;

	dprintf("NMEDetermineConditionType:error, unknown condition type\n");
	return -1;
}

/*	--------------------------------------------------------------------------------
	Function 	: NMEInitInstruction
	Purpose 	: adds an instruction to one of the instruction lists (can be any type)
	Parameters 	: FILE*, NME_INSTRUCTION **
	Returns 	: none
	Info 		:
*/
void NMEInitInstruction(FILE *fp, NME_INSTRUCTION **instruction)
{
	int				instructionType;
	int				time;
	universalType	ival1, ival2;
	short			condition;
	universalType	cval1, cval2;
	char			string[256];

	fscanf(fp," %s %d",string, &time);

	
	*instruction = (NME_INSTRUCTION *)calloc(1, sizeof(NME_INSTRUCTION));
	(*instruction)->time = time;

//	dprintf("NMEInitInstruction:found instruction %s\n", string);


	if(strcmp(string,"NME_MOVETO") == 0)
	{
		float x, y, z, c1, c2, i;int c, f;

		fscanf(fp, "%f %f %f %f %d %s %f %f", &x, &y, &z, &i, &f, string, &c1, &c2);
		(*instruction)->val1.v.v[X] = x;
		(*instruction)->val1.v.v[Y] = y;
		(*instruction)->val1.v.v[Z] = z;
		(*instruction)->val2.f = i;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_MOVETO;
		return;
	}
	else if(strcmp(string,"NME_MOVE_VECTOR") == 0)
	{
		float x, y, z, c1, c2, i;int c, f;

		fscanf(fp, "%f %f %f %f %d %s %f %f", &x, &y, &z, &i, &f, string, &c1, &c2);
		(*instruction)->val1.v.v[X] = x;
		(*instruction)->val1.v.v[Y] = y;
		(*instruction)->val1.v.v[Z] = z;
		(*instruction)->val2.f = i;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_MOVE_VECTOR;
		return;
	}
	else if(strcmp(string,"NME_MOVETO2D") == 0)
	{
		float x, y, z, c1, c2, i;int c, f;

		fscanf(fp, "%f %f %f %f %d %s %f %f", &x, &y, &z, &i, &f, string, &c1, &c2);
		(*instruction)->val1.v.v[X] = x;
		(*instruction)->val1.v.v[Y] = y;
		(*instruction)->val1.v.v[Z] = z;
		(*instruction)->val2.f = i;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_MOVETO2D;
		return;
	}
	else if(strcmp(string,"NME_FACETO") == 0)
	{
		float x, y, z, c1, c2;int i, c, f;

		fscanf(fp, "%f %f %f %d %d %s %f %f", &x, &y, &z, &i, &f, string, &c1, &c2);
		(*instruction)->val1.v.v[X] = x;
		(*instruction)->val1.v.v[Y] = y;
		(*instruction)->val1.v.v[Z] = z;
		(*instruction)->val2.i = i;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_FACETO;
		return;
	}
	else if(strcmp(string,"NME_RANDOMMOVE") == 0)
	{
		float x, y, z, x1, y1, z1, c1, c2;int c, f, r;

		fscanf(fp, "%f %f %f %f %f %f %d %d %s %f %f", &x, &y, &z, &x1, &y1, &z1, &r, &f, string, &c1, &c2);
		(*instruction)->val1.b.pos.v[X] = x;
		(*instruction)->val1.b.pos.v[Y] = y;
		(*instruction)->val1.b.pos.v[Z] = z;
		(*instruction)->val1.b.size.v[X] = x1;
		(*instruction)->val1.b.size.v[Y] = y1;
		(*instruction)->val1.b.size.v[Z] = z1;
		(*instruction)->val2.i = r;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_RANDOMMOVE;
		return;
	}
	else if(strcmp(string,"NME_WAIT") == 0)
	{
		float c1, c2;int i, j, f;

		fscanf(fp, "%d %d %d %s %f %f", &i, &j, &f, string, &c1, &c2);
		(*instruction)->val1.i = i;
		(*instruction)->val2.i = j;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_WAIT;
		return;
	}
	else if(strcmp(string,"NME_FIRE1") == 0)
	{
		float c1, c2;int i, j, f;

		fscanf(fp, "%d %d %d %s %f %f", &i, &j, &f, string, &c1, &c2);
		(*instruction)->val1.i = i;
		(*instruction)->val2.i = j;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_FIRE1;
		return;
	}
	else if(strcmp(string,"NME_FIRE2") == 0)
	{
		float c1, c2;int i, j, f;

		fscanf(fp, "%d %d %d %s %f %f", &i, &j, &f, string, &c1, &c2);
		(*instruction)->val1.i = i;
		(*instruction)->val2.i = j;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_FIRE2;
		return;
	}
	else if(strcmp(string,"NME_SPECIAL") == 0)
	{
		float c1, c2;int i, j, f;

		fscanf(fp, "%d %d %d %s %f %f", &i, &j, &f, string, &c1, &c2);
		(*instruction)->val1.i = i;
		(*instruction)->val2.i = j;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_SPECIAL;
		return;
	}
	else if(strcmp(string,"NME_FACEPLAYER") == 0)
	{
		float c1, c2, i;int j, f;

		fscanf(fp, "%f %d %d %s %f %f", &i, &j, &f, string, &c1, &c2);
		(*instruction)->val1.f = i;
		(*instruction)->val2.i = j;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_FACEPLAYER;
		return;
	}
	else if(strcmp(string,"NME_FOLLOWPLAYER") == 0)
	{
		float x, y, z, c1, c2, i;int j, f;

		fscanf(fp, "%f %f %f %f %d %s %f %f", &x, &y, &z, &i, &f, string, &c1, &c2);
		(*instruction)->val1.v.v[X] = x;
		(*instruction)->val1.v.v[Y] = y;
		(*instruction)->val1.v.v[Z] = z;
		(*instruction)->val2.f = i;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_FOLLOWPLAYER;
		return;
	}
	else if(strcmp(string,"NME_HOLDPLAYER") == 0)
	{
		float x, y, z, c1, c2;int i, j, f;

		fscanf(fp, "%f %f %f %d %d %s %f %f", &x, &y, &z, &j, &f, string, &c1, &c2);
		(*instruction)->val1.v.v[X] = x;
		(*instruction)->val1.v.v[Y] = y;
		(*instruction)->val1.v.v[Z] = z;
		(*instruction)->val2.i = j;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_HOLDPLAYER;
		return;
	}
	else if(strcmp(string,"NME_DROPPLAYER") == 0)
	{
		float x, y, z, c1, c2;int i, j, f;

		fscanf(fp, "%f %f %f %d %d %s %f %f", &x, &y, &z, &j, &f, string, &c1, &c2);
		(*instruction)->val1.v.v[X] = x;
		(*instruction)->val1.v.v[Y] = y;
		(*instruction)->val1.v.v[Z] = z;
		(*instruction)->val2.i = j;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_DROPPLAYER;
		return;
	}
	else if(strcmp(string,"NME_FLEEPLAYER") == 0)
	{
		float c1, c2;int i, j, f;

		fscanf(fp, "%d %d %d %s %f %f", &i, &j, &f, string, &c1, &c2);
		(*instruction)->val1.i = i;
		(*instruction)->val2.i = j;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_FLEEPLAYER;
		return;
	}
	else if(strcmp(string,"NME_CIRCLE") == 0)
	{
		float x, y, z, c1, c2, i, j;int f;

		fscanf(fp, "%f %f %f %f %d %s %f %f", &x, &y, &z, &j, &f, string, &c1, &c2);
		(*instruction)->val1.v.v[X] = x;
		(*instruction)->val1.v.v[Y] = y;
		(*instruction)->val1.v.v[Z] = z;
		(*instruction)->val2.f = j;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_CIRCLE;
		return;
	}
	else if(strcmp(string,"NME_ATTACK") == 0)
	{
		float c1, c2;int i, j, f;

		fscanf(fp, "%d %d %d %s %f %f", &i, &j, &f, string, &c1, &c2);
		(*instruction)->val1.i = i;
		(*instruction)->val2.i = j;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_ATTACK;
		return;
	}
	else if(strcmp(string,"NME_DONTATTACK") == 0)
	{
		float c1, c2;int i, j, f;

		fscanf(fp, "%d %d %d %s %f %f", &i, &j, &f, string, &c1, &c2);
		(*instruction)->val1.i = i;
		(*instruction)->val2.i = j;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_DONTATTACK;
		return;
	}
	else if(strcmp(string,"NME_ENDATTACK") == 0)
	{
		float c1, c2;int i, j, f;

		fscanf(fp, "%d %d %d %s %f %f", &i, &j, &f, string, &c1, &c2);
		(*instruction)->val1.i = i;
		(*instruction)->val2.i = j;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_ENDATTACK;
		return;
	}
	else if(strcmp(string,"NME_CHARGE") == 0)
	{
		float c1, c2, i;int j, f;

		fscanf(fp, "%f %d %d %s %f %f", &i, &j, &f, string, &c1, &c2);
		(*instruction)->val1.f = i;
		(*instruction)->val2.i = j;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_CHARGE;
		return;
	}
	else if(strcmp(string,"NME_BRANCH") == 0)
	{
		float c1, c2;int i, j, f;

		fscanf(fp, "%d %d %d %s %f %f", &i, &j, &f, string, &c1, &c2);
		(*instruction)->val1.i = i;
		(*instruction)->val2.i = j;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_BRANCH;
		return;
	}
	else if(strcmp(string,"NME_RETURN") == 0)
	{
		float c1, c2;int i, j, f;

		fscanf(fp, "%d %d %d %s %f %f", &i, &j, &f, string, &c1, &c2);
		(*instruction)->val1.i = i;
		(*instruction)->val2.i = j;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_RETURN;
		return;
	}
	else if(strcmp(string,"NME_LOOP") == 0)
	{
		float c1, c2;int i, j, f;

		fscanf(fp, "%d %d %d %s %f %f", &i, &j, &f, string, &c1, &c2);
		(*instruction)->val1.i = i;
		(*instruction)->val2.i = j;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_LOOP;
		return;
	}
	else if(strcmp(string,"NME_LOADCOUNTER") == 0)
	{
		float c1, c2;int i, j, f;

		fscanf(fp, "%d %d %d %s %f %f", &i, &j, &f, string, &c1, &c2);
		(*instruction)->val1.i = i;
		(*instruction)->val2.i = j;
		(*instruction)->flags = f;
		(*instruction)->condition.type = NMEDetermineConditionType(string);
		(*instruction)->condition.val1.f = c1;
		(*instruction)->condition.val2.f = c2;
		(*instruction)->type = NME_LOADCOUNTER;
		return;
	}
	else
	{
		dprintf("NMEInitInstruction:error, unrecognised instruction %s\n", string);
	}

	
}


/*	--------------------------------------------------------------------------------
	Function 	: NMEAddNormalInstruction
	Purpose 	: calls NMEInitInstruction with address of pointer of correct instruction
	Parameters 	: FILE *
	Returns 	: none
	Info 		:
*/
void NMEAddNormalInstruction(FILE *fp)
{
	NME_SCRIPT *script = &enemies[numberOfEnemies - 1].script;
	int numInst = script->numNormalInstructions;

	NMEInitInstruction(fp, &script->instructionList[numInst]);
	script->numNormalInstructions++;
}

/*	--------------------------------------------------------------------------------
	Function 	: NMEAddConditionalInstruction                                                
	Purpose 	: calls NMEInitInstruction with address of pointer of correct instruction
	Parameters 	: FILE *                                                                 
	Returns 	: none                                                                   
	Info 		:
*/
void NMEAddConditionalInstruction(FILE *fp)
{
	NME_SCRIPT *script = &enemies[numberOfEnemies - 1].script;
	int numInst = script->numConditionalInstructions;

	NMEInitInstruction(fp, &script->conditionalList[numInst]);
	script->numConditionalInstructions++;
}

/*	--------------------------------------------------------------------------------
	Function 	: NMEAddAttackInstruction                                                
	Purpose 	: calls NMEInitInstruction with address of pointer of correct instruction
	Parameters 	: FILE *                                                                 
	Returns 	: none                                                                   
	Info 		:
*/
void NMEAddAttackInstruction(FILE *fp)
{
	NME_SCRIPT *script = &enemies[numberOfEnemies - 1].script;
	int numInst = script->numAttackInstructions;

	NMEInitInstruction(fp, &script->attackList[numInst]);
	script->numAttackInstructions++;
}




/*	--------------------------------------------------------------------------------
	Function 	: NMEGetNewInstruction
	Purpose 	: copies the correct instruction from the active instructionlist into the current
				  instruction
	Parameters 	: ENEMY_TYPE*, short
	Returns 	: none
	Info 		:
*/
void NMEGetNewInstruction(ENEMY_TYPE *enemy, short instructionNum, BOOL overrideConditions)
{
	NME_SCRIPT *script = &enemy->script;


	if(script->currentInstruction.type == NME_CIRCLE)
		enemy->actor.rotaim.v[X] =0;


	if(!overrideConditions)
	{
		while(NMEConditionsMet(enemy, script->activeInstructionList[instructionNum]) == FALSE)
		{
			instructionNum++;
		}
	}

	script->currentInstructionNum = instructionNum;
//	script->timer = script->activeInstructionList[instructionNum].time;		

	memcpy(&script->currentInstruction, script->activeInstructionList[instructionNum], sizeof(NME_INSTRUCTION));
	script->timer = script->currentInstruction.time;		
	script->currentInstructionTime = 0;
}



/*	--------------------------------------------------------------------------------
	Function 	: NMEProcessInstruction
	Purpose 	: acts upon the current instruction, requests new one if neccessary
	Parameters 	: ENEMY_TYPE*
	Returns 	: none
	Info 		:
*/
void NMEProcessInstruction(ENEMY_TYPE *enemy)//, NME_INSTRUCTION *instructions)
{
	short instructionNum = enemy->script.currentInstructionNum;
	NME_SCRIPT *script = &enemy->script;
	NME_INSTRUCTION *instruction = &script->currentInstruction;
	BOOL needNewInstruction = FALSE;
	char newInstructionNum;
	VECTOR *tempVectPtr;
	VECTOR tempVect;
	ACTOR *tempActor;
	BOOL	res;
//act on instruction
	switch(instruction->type)
	{
		case NME_MOVE_VECTOR:
			AddOneScaledVector(&tempVect,&enemy->actor.pos,&instruction->val1.v,10);
			if((NMEActionFaceTo(enemy,&tempVect) == TRUE) || (instruction->flags & NMEFLAGS_DONT_FACE_DIR))
				NMEActionMoveTo(enemy,&tempVect,0,instruction->flags);
			break;

		case NME_MOVETO:
				if(NMEActionMoveTo(enemy, &instruction->val1.v, instruction->val2.f, instruction->flags) == TRUE)
				{
					needNewInstruction = TRUE;
					newInstructionNum = script->currentInstructionNum + 1;
				}
				break;
		case NME_MOVETO2D:
				if(script->currentInstructionTime == 0)
				{
					AnimateActor(&enemy->actor, NMEANIM_START, NO, NO, 0);
					AnimateActor(&enemy->actor, NMEANIM_WALK, YES, YES, 0);
				}
				if(NMEActionMoveTo2D(enemy, &instruction->val1.v, instruction->val2.f, instruction->flags) == TRUE)
				{
					needNewInstruction = TRUE;
					newInstructionNum = script->currentInstructionNum + 1;

					AnimateActor(&enemy->actor, NMEANIM_STOP, NO, NO, 0);
					AnimateActor(&enemy->actor, NMEANIM_IDLE, YES, YES, 0);
				}
				break;
		case NME_FACETO:
				if(NMEActionFaceTo(enemy, &instruction->val1.v) == TRUE)
				{
					needNewInstruction = TRUE;
					newInstructionNum = script->currentInstructionNum + 1;
				}
				break;
		case NME_RANDOMMOVE:
				NMEActionRandomMove(enemy);
				break;
		case NME_WAIT:
				NMEActionWait(enemy);
				break;
		case NME_FIRE1:
				NMEActionFire1(enemy);
				dprintf("Fire! %d\n", script->timer);
				needNewInstruction = TRUE;
				newInstructionNum = script->currentInstructionNum + 1;
				break;
		case NME_SPECIAL:
				AnimateActor(&enemy->actor, NMEANIM_SPECIAL1 + instruction->val1.i, NO, NO, 0);
				needNewInstruction = TRUE;
				newInstructionNum = script->currentInstructionNum + 1;
				break;
				
		case NME_FIRE2:
				NMEActionFire1(enemy);
				dprintf("Fire2! %d\n", script->timer);
				needNewInstruction = TRUE;
				newInstructionNum = script->currentInstructionNum + 1;
				break;
		case NME_FACEPLAYER:
				if(NMEActionFacePlayer(enemy, script->currentInstruction.flags) == TRUE)
				{
					if(script->timer < 0)
					{
						needNewInstruction = TRUE;
						newInstructionNum = script->currentInstructionNum + 1;
					}
				}
				break;
		case NME_FOLLOWPLAYER:
				if(NMEActionFollowPlayer(enemy) == TRUE)
				{
//					if(script->timer < 0)
					{
						needNewInstruction = TRUE;
						newInstructionNum = script->currentInstructionNum + 1;
					}
				}
				break;
		case NME_HOLDPLAYER:

				if(IsHandHoldingBall() == TRUE)
				{
					hand.flags &= -1 - LO_GRAVITY;
					ball.owner = NULL;
					ball.pos.v[X] += 0.1;
					ApplyAction(HAND_IDLE);
					handControl.newActionDelay = 20;
					if(cameraInfo.positionToTrack == &ball.pos)
					{
						cameraInfo.positionToTrack = &hand.pos;
						cameraInfo.velToTrack = &hand.vel;
					}

				}


				if(instruction->flags & NMEFLAGS_SPECIFY_BALL)
				{
					ball.owner = &enemy->actor;
					ballControl.status = BALL_HELD;
					ballControl.holdingActor = &enemy->actor;
					SetVector(&ballControl.holdingOffset, &instruction->val1.v);
				}
				if(instruction->flags & NMEFLAGS_SPECIFY_HAND)
				{
					hand.owner = &enemy->actor;
					handControl.status = HAND_HELD;
					handControl.holdingActor = &enemy->actor;
					SetVector(&handControl.holdingOffset, &instruction->val1.v);

				}

				dprintf("holdball\n");
				needNewInstruction = TRUE;
				newInstructionNum = script->currentInstructionNum + 1;
				break;
		case NME_DROPPLAYER:
				if(instruction->flags & NMEFLAGS_SPECIFY_BALL)
				{
					if(ball.owner == &enemy->actor)
						DropBall();
					RotateVector2D(&tempVect, &instruction->val1.v, enemy->actor.rot.v[Y]);
					ScaleVector2D(&tempVect, 0.5);
					SetVector(&ball.vel, &tempVect);
				}
				if(instruction->flags & NMEFLAGS_SPECIFY_HAND)
				{
					if(hand.owner == &enemy->actor)
						DropHand();
					RotateVector2D(&tempVect, &instruction->val1.v, enemy->actor.rot.v[Y]);
					SetVector(&hand.vel, &tempVect);
//					ActorAddVelocity(&hand, &instruction->val1.v);
				}

				dprintf("dropball\n");
				needNewInstruction = TRUE;
				newInstructionNum = script->currentInstructionNum + 1;
				break;
		case NME_FLEEPLAYER:
				NMEActionFleePlayer(enemy);
				break;
		case NME_CIRCLE:
				NMEActionCircle(enemy);
				break;
		case NME_ATTACK:
				if(script->numAttackInstructions < 1)
					dprintf("error:no attack instructions present\n");
				else
				{
					script->attacking = TRUE;
//					if(script->hasBeenOverridden == FALSE)
						script->lastNormalInstructionNum = script->currentInstructionNum + 1;
//					else
//						script->lastNormalInstructionNum = script->currentInstructionNum;
						
					script->currentInstructionNum = 0;
					script->hasBeenOverridden = FALSE;
					script->activeInstructionList = script->attackList;
					needNewInstruction = TRUE;
					newInstructionNum = 0;
				}
				break;
		case NME_ENDATTACK:
				script->attacking = FALSE;
				script->currentInstructionNum = script->lastNormalInstructionNum;
				script->activeInstructionList = script->instructionList;
				needNewInstruction = TRUE;
				newInstructionNum = script->currentInstructionNum;
				script->blockAttack = instruction->val1.i;
				AnimateActor(&enemy->actor, NMEANIM_ENDATTACK, NO, YES, 0);
				break;
		case NME_DONTATTACK:
				dprintf("dont attack\n");
				script->blockAttack = instruction->val1.i;
				needNewInstruction = TRUE;
				newInstructionNum = script->currentInstructionNum + 1;
				break;
		case NME_CHARGE:
				NMEActionCharge(enemy);
//				dprintf("charge\n");
				break;
		case NME_BRANCH:
				needNewInstruction = TRUE;
				newInstructionNum = instruction->val1.i;
				break;
		case NME_RETURN:
				dprintf("return\n");
				break;
		case NME_LOOP:
				dprintf("loop\n");
				if(script->loopCounter > 0)
				{
					script->loopCounter--;
					needNewInstruction = TRUE;
					newInstructionNum = instruction->val1.i;
				}
				else
				{
					needNewInstruction = TRUE;
					newInstructionNum = instructionNum + 1;
				}
				break;
		case NME_LOADCOUNTER:
				dprintf("load counter %d\n", instruction->val1.i);
				script->loopCounter = instruction->val1.i;
				needNewInstruction = TRUE;
				newInstructionNum = instructionNum + 1;
				break;
	
	
	}			

	script->currentInstructionTime++;

	if(script->blockAttack > 0)
		script->blockAttack--;

//see if current action has run out of time
	if((script->timer > 0) && (!needNewInstruction))
	{
		if(--script->timer == 0)
		{
			needNewInstruction = TRUE;
			newInstructionNum = instructionNum+1;
		}
	}

//see if one of the above actions has requested a new instruction
	if(needNewInstruction == TRUE)
	{
		if(script->hasBeenOverridden == TRUE)
		{
			script->activeInstructionList = script->instructionList;
			script->currentInstructionNum = script->lastNormalInstructionNum;
			script->hasBeenOverridden = FALSE;
			newInstructionNum = script->currentInstructionNum;
		}

		//get next instruction
		NMEGetNewInstruction(enemy, newInstructionNum, NO);
		NMEProcessInstruction(enemy);
	}

}

/*	--------------------------------------------------------------------------------
	Function 	: NMEConditionsMet
	Purpose 	: every instruction has a condition, check if it is true.
	Parameters 	: ACTOR*, NME_INSTRUCTION*
	Returns 	: T/F
	Info 		:
*/
BOOL NMEConditionsMet(ENEMY_TYPE *enemy, NME_INSTRUCTION *instruction)
{
	ACTOR *actor = &enemy->actor;
	float temp, temp2, dist;
	ACTOR *closestActor;
	ACTOR *closestActor2D;
	float handDist, ballDist, closestDist;
	float handDist2D, ballDist2D, closestDist2D;

//	if(enemy->script.currentInstruction.type == instruction->type)
//		return FALSE;

	if((instruction->type == NME_HOLDPLAYER) && (ballControl.holdingActor == &enemy->actor)
	   && (instruction->flags & NMEFLAGS_SPECIFY_BALL))
		return FALSE;
	if((instruction->type == NME_HOLDPLAYER) && (handControl.holdingActor == &enemy->actor)
	   && (instruction->flags & NMEFLAGS_SPECIFY_HAND))
		return FALSE;
	//if((instruction->type == NME_HOLDPLAYER) && (IsHandHoldingBall()==TRUE)
	//   && (instruction->flags & NMEFLAGS_SPECIFY_BALL))
	//	return FALSE;
	if((instruction->type == NME_ATTACK) && (enemy->script.blockAttack))
		return FALSE;


//deternime closest actor
	ballDist = DistanceBetweenPoints(&ball.pos, &actor->pos);
	handDist = DistanceBetweenPoints(&hand.pos, &actor->pos);
	ballDist -= ball.radius + actor->radius;
	handDist -= hand.radius + actor->radius;
	closestActor = ballDist < handDist ? &ball : &hand;
	closestDist = ballDist < handDist ? ballDist : handDist;	
//determine closest actor 2D
	ballDist2D = DistanceBetweenPoints2D(&ball.pos, &actor->pos);
	handDist2D = DistanceBetweenPoints2D(&hand.pos, &actor->pos);
	ballDist2D -= ball.radius + actor->radius;
	handDist2D -= hand.radius + actor->radius;
	closestActor2D = ballDist2D < handDist2D ? &ball : &hand;
	closestDist2D = ballDist2D < handDist2D ? ballDist2D : handDist2D;	



	switch(instruction->condition.type)
	{
		case IF_HAND_IN_RANGE:
				if((handDist >= instruction->condition.val1.f) && (handDist < instruction->condition.val2.f))
					return TRUE;
				break;
		case IF_HAND_IN_RANGE_2D:
				if((handDist2D >= instruction->condition.val1.f) && (handDist2D < instruction->condition.val2.f))
					return TRUE;
				break;
		case IF_BALL_IN_RANGE:
				if((ballDist >= instruction->condition.val1.f) && (ballDist < instruction->condition.val2.f))
					return TRUE;
				break;
		case IF_BALL_IN_RANGE_2D:
				if((ballDist2D >= instruction->condition.val1.f) && (ballDist2D < instruction->condition.val2.f))
					return TRUE;
				break;
		case IF_CLOSEST_IN_RANGE:
				if((closestDist >= instruction->condition.val1.f) && (closestDist < instruction->condition.val2.f))
					return TRUE;
				break;
		case IF_CLOSEST_IN_RANGE_2D:
				if((closestDist2D >= instruction->condition.val1.f) && (closestDist2D < instruction->condition.val2.f))
					return TRUE;
				break;
		case IF_CANSEE_BALL:
				if(IsActorFacingActor(actor, &ball, instruction->condition.val1.f) == TRUE)
					return TRUE;
				break;
		case IF_CANSEE_HAND:
				if(IsActorFacingActor(actor, &hand, instruction->condition.val1.f) == TRUE)
					return TRUE;
				break;
		case IF_CANSEE_CLOSEST:
				if(IsActorFacingActor(actor, closestActor, instruction->condition.val1.f) == TRUE)
				break;
		case EVERY_NUM_FRAMES:
				float temp;
				temp = frameCount % (int)instruction->condition.val1.f;
				if(temp < 0.01)
					return TRUE;
				break;
		case BETWEEN_SPIN_VALS:
				if((actor->rot.v[X] > instruction->condition.val1.f) && (actor->rot.v[X] < instruction->condition.val2.f))
					if(!(frameCount % instruction->val1.i))
						return TRUE;
				break;
		case IF_HAND_HOLDING_BALL:
				return IsHandHoldingBall();
				break;
		case IF_HAND_NOT_HOLDING_BALL:
				return !IsHandHoldingBall();
				break;
		case IF_IM_HOLDING_BALL:
				if(&enemy->actor == ball.owner)
					return TRUE;
				else
					return FALSE;
		case IF_IM_NOT_HOLDING_BALL:
				if(&enemy->actor != ball.owner)
					return TRUE;
				else
					return FALSE;
		case IF_IM_HOLDING_HAND:
				if(&enemy->actor == hand.owner)
					return TRUE;
				else
					return FALSE;
		case IF_IM_NOT_HOLDING_HAND:
				if(&enemy->actor != hand.owner)
					return TRUE;
				else
					return FALSE;
				break;
		case IF_TOUCHING_BALL:
				if(ballDist == 0)
					return TRUE;
				break;
		case IF_TOUCHING_HAND:
				if(handDist == 0)
					return TRUE;
				break;
		case ALWAYS:
				return TRUE;
				break;
		case NEVER:
				return FALSE;
				break;
		case SOMETIMES:
				temp = rand() % 1000;
				if(temp <= instruction->condition.val1.f)
					return TRUE;
				break;

		default:
				return FALSE;
				break;
	}
	return FALSE;

}

/*	--------------------------------------------------------------------------------
	Function 	: NMEAnalyseConditionalList
	Purpose 	: constantly check if any of the conditional instructions are true
	Parameters 	: ENEMY_TYPE*
	Returns 	: T/F
	Info 		:
*/
BOOL NMEAnalyseConditionalList(ENEMY_TYPE *enemy)//, NME_INSTRUCTION *instructions)
{
	NME_SCRIPT *script = &enemy->script;

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// i need to make sure that the conditional instruction is not constantly being given

//these are things that can happen at any time during the normal instruction list

	for(short x = 0; x < script->numConditionalInstructions; x++)
	{
		if((int)script->activeInstructionList[script->currentInstructionNum] == (int)script->conditionalList[x])
			continue;
 
 		if(NMEConditionsMet(enemy, script->conditionalList[x]) == TRUE)
		{
			//yes, a condition has been met.... make this the current instruction (if it is an action)

			if(script->conditionalList[x]->flags & NMEFLAGS_DONT_INTERUPT)
			{

				switch(script->conditionalList[x]->type)
				{
					case NME_FIRE1:
							NMEActionFire1(enemy);
							break;
					case NME_FIRE2:
							NMEActionFire1(enemy);
							break;
					case NME_FACEPLAYER:
							NMEActionFacePlayer(enemy, script->conditionalList[x]->flags);
							break;
					default:
							break;
				};

			}
			else
			{
				//if the script was performing a normal instruction, record this to revert to.
				if(script->hasBeenOverridden == FALSE)
					script->lastNormalInstructionNum = script->currentInstructionNum;
				script->activeInstructionList = script->conditionalList;
				script->currentInstructionNum = x;
				script->hasBeenOverridden = TRUE;

				NMEGetNewInstruction(enemy, x, YES);
				return TRUE;
			}
		}		

	}
	return FALSE;
}

/*	--------------------------------------------------------------------------------
	Function 	: ProcessActorScript()
	Purpose 	: controls enemies
	Parameters 	: ENEMY_TYPE*
	Returns 	: none
	Info 		:
*/														
void NMEProcessActorScript(ENEMY_TYPE *enemy)
{
	NME_SCRIPT *script = &enemy->script;

	if(script->numNormalInstructions == 0)
		return;








	if(script->disabledCount)
	{
		script->disabledCount--;
	}
	else
	{
		if(script->attacking == FALSE)
		{
			BOOL lastResult = script->hasBeenOverridden;

	//has normal script been overridden?
			NMEAnalyseConditionalList(enemy);//, actor->actorScript.conditionalList);
			NMEProcessInstruction(enemy);
		}
		else
			NMEProcessInstruction(enemy);
	}


	switch(enemy->actor.type)
	{
		case DROID:
			NMEProcessDroid(enemy);
			break;
		case CYLON:
			NMEProcessCylon(enemy);
			break;
		case FIGHTER:
			NMEProcessFighter(enemy);
			break;
		case SNEEZY:
			NMEProcessSneezy(enemy);
	}





}



//***************************************************************************************
//*					Specific enemy handling code										*
//*					Hopfully this section will not be tooo big!							*
//***************************************************************************************

/*	--------------------------------------------------------------------------------
	Function 	: NMEProcessDroid
	Purpose 	: process special cases for the droid
	Parameters 	: ENEMY_TYPE *
	Returns 	: none
	Info 		:
*/
void NMEProcessDroid(ENEMY_TYPE *enemy)
{
	static BOOL	whichEngine = 0;
	VECTOR		tempVect, tempVect2;
	NME_SCRIPT	*script = &enemy->script;
	static short	dizzyCounter = 0;

	//handle puffs of smoke from engines
	if(!(frameCount % 3))
	{
		VECTOR pos1;

		whichEngine = 1 - whichEngine;
		if(whichEngine)
			tempVect.v[X] = -1.74;
		else
			tempVect.v[X] = 1.74;
		tempVect.v[Y] = 0.87;
		tempVect.v[Z] = 0.56;

		RotateVector2D(&tempVect, &tempVect, enemy->actor.rot.v[Y]);
		AddVector(&pos1, &tempVect, &enemy->actor.pos);

		tempVect2.v[X] = tempVect2.v[Z] = 0;
		tempVect2.v[Y] = -0.3;
		AddDebris(DUST, &pos1, &tempVect2);
	}

	enemy->actor.vel.v[Y] += SineWave(3,0) / 50;
	enemy->actor.accel = 0.4;


	if((AreActorsTouching(&ball, &enemy->actor)) && (Magnitude(&ball.vel) > 0.1))
	{
		dizzyCounter = 100;
		enemy->script.disabledCount = 100;
	}

	if(dizzyCounter)
	{
		float dizzyFactor = (float)dizzyCounter / 100;
		dizzyCounter--;
		enemy->actor.rotaim.v[Y] += 0.4 * dizzyFactor;
		enemy->actor.turnSpeed = 1.8;
		enemy->actor.rotaim.v[Z] = (SineWave(10,0)/2) * dizzyFactor;
		AccelerateActor(&enemy->actor, 0.1 * dizzyFactor);
	}
	else
	{
		enemy->actor.rotaim.v[Z] = 0;
		enemy->actor.turnSpeed = 0.25;
		//make droid block hands path
		if(DistanceBetweenPoints(&hand.pos, &enemy->actor.pos) < 20)
		{
			script->disabledCount = 1;			
			tempVect.v[Y] = hand.pos.v[Y] + 1.8;
			if(enemy->flags & SPECIFY_X)
			{
				tempVect.v[X] = hand.pos.v[X];
				tempVect.v[Z] = enemy->originalPosition.v[Z];
			}
			else
			{
				tempVect.v[Z] = hand.pos.v[Z];
				tempVect.v[X] = enemy->originalPosition.v[X];
			}
			NMEActionMoveTo(enemy, &tempVect, 0.8, 4102);
			NMEActionFaceTo(enemy, &hand.pos);

			if(DistanceBetweenPoints(&hand.pos, &enemy->actor.pos) < 8)
			{
				NMEActionFire1(enemy);
			}
		}
		else
		{
			enemy->actor.accel = 0.1;
		}
	}

}


/*	--------------------------------------------------------------------------------
	Function 	: NMEProcessCylon
	Purpose 	: process special cases for the cylon
	Parameters 	: ENEMY_TYPE *
	Returns 	: none
	Info 		:
*/
void NMEProcessCylon(ENEMY_TYPE *enemy)
{
	VECTOR			tempVect, tempVect2;
	NME_SCRIPT		*script = &enemy->script;
	ACTOR			*actor = &enemy->actor;
	NME_INSTRUCTION	*instruction = &script->currentInstruction;
	float			angle;

//make sure cylon is upright after chase
	if(instruction->type != NME_FOLLOWPLAYER)
		enemy->actor.rotaim.v[Z] = 0;

//do suprised look
/*
	if(instruction->type == NME_SPECIAL)
	{
		if(script->currentInstructionTime == 0)
			AnimateActor(actor, NMEANIM_RUN, YES, YES, 0);
	}
*/
	if((instruction->type == NME_FOLLOWPLAYER) && (script->currentInstructionTime == 1))
	{
	 	AnimateActor(actor, NMEANIM_RUN, YES, YES, 0);
	}

//if cylon is turning at a right angle, play anim and snap rotation.  otherwise rotate as normal
	if(instruction->type == NME_FACETO)
	{
		if(script->currentInstructionTime == 1)
		{
			angle = FindShortestAngleSigned(actor->rotaim.v[Y], actor->rot.v[Y]);
			if(fabs(angle) - (PI/2) < 0.1)
			{
				if(angle < 0)
				{
					AnimateActor(actor, NMEANIM_SPECIAL1, NO, NO, 0);
				}
				else
				{
					AnimateActor(actor, NMEANIM_SPECIAL2, NO, NO, 0);
				}
			  	AnimateActor(actor, NMEANIM_IDLE, YES, YES, 0);
			}
		}
	
		if((actor->currentAnimation == NMEANIM_SPECIAL1) || (actor->currentAnimation == NMEANIM_SPECIAL2))
		{
			if(actor->reachedEndOfAnimation == FALSE)
				actor->rotaim.v[Y] = actor->rot.v[Y];
			else
			{
				actor->rot.v[Y] = actor->rotaim.v[Y];
				TransformActor(actor);
			}
		}
	}


}

/*	--------------------------------------------------------------------------------
	Function 	: NMEProcessFighter
	Purpose 	: control fighter
	Parameters 	: ENEMY_TYPE *
	Returns 	: none
	Info 		:
*/
void NMEProcessFighter(ENEMY_TYPE *enemy)
{
	ACTOR *actor = &enemy->actor;

	return;

	if(enemy->script.currentInstruction.type == NME_CIRCLE)
	{
		if((actor->rot.v[X] > 4.7) && (actor->rot.v[X] < 6))
		{
			if(!(frameCount % 3))
			{
				NMEActionFire1(enemy);
			}

		}
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: NMEProcessSneezy
	Purpose 	: special actions for sneezy
	Parameters 	: ENEMY_TYPE *
	Returns 	: none
	Info 		:
*/
void NMEProcessSneezy(ENEMY_TYPE *enemy)
{
	int val = rand()%100;
	VECTOR tempVect, tempVect2;


	if((enemy->script.currentInstruction.type == NME_RANDOMMOVE)
	   && (enemy->actor.currentAnimation == NMEANIM_WALK)
	  )
	{
		if(val < 4)
		{
			//sneeze!
			if((val < 2) || ((val < 10) && (DistanceBetweenPoints(&hand.pos, &enemy->actor.pos) < 10)))
			{
				if((frameCount & 1) && (enemy->actor.currentAnimation != NMEANIM_ATTACK1))
				{

					enemy->script.disabledCount = 120;
					AnimateActor(&enemy->actor, NMEANIM_ATTACK1, NO, NO, 0);
					AnimateActor(&enemy->actor, NMEANIM_WALK, YES, YES, 0);
					enemy->actor.rotaim.v[Y] = enemy->actor.rot.v[Y];

				}
			}
			else
			{
			//sniff
				AnimateActor(&enemy->actor, NMEANIM_SPECIAL1, NO, NO, 0);
				AnimateActor(&enemy->actor, NMEANIM_WALK, YES, YES, 0);
			}
		}

	}
	if(enemy->actor.currentAnimation == NMEANIM_ATTACK1)
	{
		if((enemy->actor.callBack->time >= 43) && (enemy->actor.callBack->time < 43.8))
		{
			FireBulletFromActor(&enemy->actor, SNEEZY_BULLET);
		}
	}
}