#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include <malloc.h>
#include <time.h>
#include <search.h>

#include <crtdbg.h>
#include "debug.h"

#include "memory.h"

extern "C" {

DebugType		debug;

/*	--------------------------------------------------------------------------------
	Function : dprintf
	Purpose : print a string to the debug terminal
	Parameters : as for printf
	Returns : 
	Info : 
*/

void dprintf(char *format, ...)
{
	char	buf[1024];
	va_list	argp;
#ifdef _FIXES
	FILE *out;

	out = fopen("debug.log", "a");
#endif

	va_start(argp, format);
	vsprintf(buf, format, argp);
	va_end(argp);

#ifdef _DEBUG
	_CrtDbgReport(_CRT_WARN, NULL, NULL, "WGS", "%s", buf);
#endif

#ifdef _FIXES
	fprintf(out, "%s", buf);
	fclose(out);
#endif
}


/*	--------------------------------------------------------------------------------
	Function : OpenDebugFile
	Purpose : Open a debug file
	Parameters : leafname
	Returns : handle to debug file, or -1 for failure
	Info :
*/

int OpenDebugFile(char *leaf)
{
	FILE	*fp;
	int		l;

	for(l = 0; l < MAXDEBUGFILES; l++)
		{
		if (debug.dbfname[l] == NULL)
			break;
		}

	if (l == MAXDEBUGFILES)
		{
		dprintf("Too many debug files opened: '%s' failed\n", leaf);
		return -1;
		}

	debug.dbfname[l] = (char*)MyMalloc(256);
	if (debug.dbfname[l] == NULL)
		{
		dprintf("Could not alloc memory for debug filename: '%s' failed\n", leaf);
		return -1;
		}

	strcpy(debug.dbfname[l], leaf);

	fp = fopen(debug.dbfname[l], "w");
	if (fp == NULL)
		{
		dprintf("Could not open debug file: '%s' failed\n", debug.dbfname[l]);
		MyFree((void **)&debug.dbfname[l]);
		debug.dbfname[l] = NULL;
		return -1;
		}

	fprintf(fp, "Debug file '%s' opened\n\n", leaf);
	fclose(fp);

	debug.lasthandle = l;

	return l;
}


/*	--------------------------------------------------------------------------------
	Function : dbfprintf
	Purpose : print a string to the last opened debug file
	Parameters : as for printf
	Returns : 
	Info :
*/

void dbfprintf(char *format, ...)
{
	va_list	argp;
	FILE	*fp;

	if ((debug.lasthandle == -1) || (debug.dbfname[debug.lasthandle] == NULL))
		return;

	fp = fopen(debug.dbfname[debug.lasthandle], "a");
	if (fp == NULL)
		{
		dprintf("dbfprintf could not open '%s'\n", debug.dbfname[debug.lasthandle]);
		return;
		}
	
	va_start(argp, format);
	vfprintf(fp, format, argp);
	va_end(argp);

	fclose(fp);
}


/*	--------------------------------------------------------------------------------
	Function : ToDebugFile
	Purpose : print a string to a given debug file
	Parameters : file handle, as for printf
	Returns : 
	Info :
*/

void ToDebugFile(int handle, char *format, ...)
{
	va_list	argp;
	FILE	*fp;

	if ((handle == -1) || (debug.dbfname[handle] == NULL))
		return;

	fp = fopen(debug.dbfname[handle], "a");
	if (fp == NULL)
		{
		dprintf("dbfprintf could not open '%s'\n", debug.dbfname[handle]);
		return;
		}
	
	va_start(argp, format);
	vfprintf(fp, format, argp);
	va_end(argp);

	fclose(fp);
}

/*	--------------------------------------------------------------------------------
	Function : ToLastDebugFile
	Purpose : print a string to the last opened debug file
	Parameters : as for printf
	Returns : 
	Info :
*/

void ToLastDebugFile(char *format, ...)
{
	va_list	argp;
	FILE	*fp;

	if ((debug.lasthandle == -1) || (debug.dbfname[debug.lasthandle] == NULL))
		return;

	fp = fopen(debug.dbfname[debug.lasthandle], "a");
	if (fp == NULL)
		{
		dprintf("dbfprintf could not open '%s'\n", debug.dbfname[debug.lasthandle]);
		return;
		}
	
	va_start(argp, format);
	vfprintf(fp, format, argp);
	va_end(argp);

	fclose(fp);
}


/*	--------------------------------------------------------------------------------
	Function : CloseDebugFile
	Purpose : close a debug file
	Parameters : file handle
	Returns : 
	Info :
*/

void CloseDebugFile(int handle)
{
	if (handle == -1)
		return;

	if (debug.dbfname[handle] == NULL)
		return;

	MyFree((void **)&debug.dbfname[handle]);
	debug.dbfname[handle] = NULL;
}


/*	--------------------------------------------------------------------------------
	Function : CloseLastDebugFile
	Purpose : close the last debug file opened
	Parameters : 
	Returns : 
	Info :
*/

void CloseLastDebugFile()
{
	if (debug.lasthandle == -1)
		return;

	if (debug.dbfname[debug.lasthandle] == NULL)
		{
		debug.lasthandle = -1;
		return;
		}

	MyFree((void **)&debug.dbfname[debug.lasthandle]);
	debug.dbfname[debug.lasthandle] = NULL;
	debug.lasthandle = -1;
}


}
