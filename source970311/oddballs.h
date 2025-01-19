#ifndef __ODDBALLS_H
#define __ODDBALLS_H

#define PI 3.141592654
#define ON 1
#define YES 1
#define OFF 0
#define NO 0

#define SCALINGFACTOR (0.7)
#define TARGETFRAMERATE 30

#include <windows.h>
#include <windowsx.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <search.h>
#include <ddraw.h>
#include <d3d.h>
#include <dinput.h>
#include <direct.h>
#include <d3drmwin.h>
#include <winuser.h>
#include <rpc.h>


#include "keydefs.h"		//aditional key codes

#include "rmstats.h"
//#include "d3dappi.h"
#include "rm.h"
//#include "d3dapp.h"         /* prototypes for D3D helper functions */
//#include "rmdemo.h"         /* prototypes for functions to commumicate with each sample */
//#include "rmfullrc.h"       /* defines constants used in rmfull.rc */
#include <d3drm.h>  /* For D3DRM error codes only */
//#include "lclib.h"


//oddball header files
#include "winmain.h"
#include "directx.h"
#include "error.h"
#include "types.h"
#include "main.h"
#include "debug.h"
#include "memory.h"
#include "input.h"
#include "maths.h"
#include "scene.h"

#include "files.h"
/*
#include "anim.h"
#include "scene.h"
#include "main.h"
#include "debug.h"
#include "actors.h"
#include "input.h"
#include "hand.h"
#include "maths.h"
#include "collision.h"
#include "personal.h"
*/

extern HRESULT dxerror;

extern TIMER_TYPE	timerList[];
#define TIMER__RENDER_LOOP		0
#define TIMER__UPDATE_ACTOR		1
#define TIMER__UPDATE_PLATFORM	2
#define TIMER__UPDATE_SHADOWS	3
#define TIMER__FRAMETIME		4

#endif