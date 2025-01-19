/*
 *	Project:		Oddballs
 *	File:			memory.cpp
 *	Description:	malloc etc
 */

#include "oddballs.h"
#include "memory.h"
#include "malloc.h"

int numMallocs = 0;
int totalMalloced = 0;

/*	--------------------------------------------------------------------------------
	Function 	: MyFree()
	Purpose 	: frees memory and sets pointer to 0
	Parameters 	: pointer to free
	Returns 	: none
	Info 		:
*/
void MyFree(void **ptr)
{
	if(*ptr)
	{
		numMallocs--;
		totalMalloced -= _msize(*ptr);
		free(*ptr);
		*ptr = 0;
	}
}

/*	--------------------------------------------------------------------------------
	Function 	: MyMalloc()
	Purpose 	: guess
	Parameters 	: 
	Returns 	: none
	Info 		:
*/
void *MyMalloc(int size)
{
	void *ptr;

	ptr = malloc(size);
	numMallocs++;
	totalMalloced += _msize(ptr);
	return ptr;
}

/*	--------------------------------------------------------------------------------
	Function 	: MyCalloc()
	Purpose 	: guess
	Parameters 	: 
	Returns 	: none
	Info 		:
*/
void *MyCalloc(int num, int size)
{
	void *ptr;
	ptr = calloc(num, size);
	numMallocs++;
	totalMalloced += _msize(ptr);
	return ptr;
}
