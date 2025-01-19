/*
 *	Project:		Oddballs
 *	File:			memory.h
 *	Description:	malloc etc
 */


#ifndef __MEMORY_H
#define __MEMORY_H

void MyFree(void **ptr);
void *MyMalloc(int size);
void *MyCalloc(int num, int size);


extern int numMallocs;
extern int totalMalloced;


#endif