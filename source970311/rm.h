/*==========================================================================
 *
 *  Copyright (C) 1995, 1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File: rm.h
 *
 ***************************************************************************/

#ifndef __RM_H_
#define __RM_H_

// Includes....
#include "d3drmwin.h"

// Defines....
#undef ATTEMPT
#define ATTEMPT(x) if (!(x)) goto exit_with_error
#undef RELEASE
#define RELEASE(x) if (x) { x->Release(); x = NULL; }
#undef MAX
#define MAX(x, y) ((x) > (y)) ? (x) : (y)
#undef MIN
#define MIN(x, y) ((x) > (y)) ? (y) : (x)
#undef ZEROMEM
#define ZEROMEM(x) memset(&x, 0, sizeof(x))

#define NUM_DEBRIS      25
#define DEBRIS_LIFE     75
#define NUM_HIT_DEBRIS  4

// Structures
struct Debris
{
    LPDIRECT3DRMFRAME           m_pFrame;
    LPDIRECT3DRMMESHBUILDER     m_pMeshBuilder;
    BOOL                        m_bInUse;
    D3DVECTOR                   m_vel;
    D3DVECTOR                   m_acc;
    int                         m_life;
};

extern LPDIRECT3DRMFRAME               g_lpScene;       // Scene frame
extern LPDIRECT3DRMFRAME               g_lpCamera;      // Camera frame
extern LPDIRECT3DRMFRAME               g_lpArena;       // Arena frame
extern LPDIRECT3DRMFRAME               g_lpBackground;  // Background frame
extern LPDIRECT3DRMFRAME               g_lpWorld;  // Background frame

extern BOOL                     g_bShowStats;           // Defined in WINMAIN.CPP
extern BOOL                     g_bHardware3D;          // Defined in DIRECTX.CPP
extern LPDIRECT3DRM             g_lpD3DRM;              // Defined in DIRECTX.CPP
extern LPDIRECT3DRMVIEWPORT     g_lpD3DRMViewport;      // Defined in DIRECTX.CPP
extern LPDIRECT3DRMDEVICE       g_lpD3DRMDevice;        // Defined in DIRECTX.CPP
extern LPDIRECTDRAWSURFACE      g_lpPrimary;            // Defined in DIRECTX.CPP
extern LPDIRECTDRAWSURFACE      g_lpBackBuffer;         // Defined in DIRECTX.CPP
extern LPDIRECTDRAWSURFACE      g_lpZBuffer;            // Defined in DIRECTX.CPP
extern DWORD                    g_vidModeX;             // Defined in DIRECTX.CPP
extern DWORD                    g_vidModeY;             // Defined in DIRECTX.CPP
extern DWORD                    g_vidModeBIT;           // Defined in DIRECTX.CPP
extern DWORD                    g_dwFontHeight;         // Defined in DIRECTX.CPP
extern DWORD                    g_dwAveCharWidth;       // Defined in DIRECTX.CPP

extern DWORD                    g_player1health;        // Defined in CONTROL.CPP
extern DWORD                    g_player2health;        // Defined in CONTROL.CPP

extern DWORD                    g_lbar1;                // Defined in DIRECTX.CPP
extern DWORD                    g_wbar1;                // Defined in DIRECTX.CPP
extern DWORD                    g_lbar2;                // Defined in DIRECTX.CPP
extern DWORD                    g_wbar2;                // Defined in DIRECTX.CPP
extern DWORD                    g_hbar1;                // Defined in DIRECTX.CPP
extern DWORD                    g_hbar2;                // Defined in DIRECTX.CPP

typedef struct tagrmfullglobals 
{
//    HWND hWndMain;          /* application window handle */
//    HINSTANCE hInstApp;     /* application instance for dialog boxes */

//    LPDIRECT3DRMDEVICE dev;     /* Direct3DRM device */
//    LPDIRECT3DRMVIEWPORT view;  /* Direct3DRM viewport through which we view
//                                   the scene */
//    LPDIRECT3DRMFRAME scene;    /* Master frame in which others are placed */

    BOOL bSingleStepMode;        /* render one frame at a time */
    BOOL bDrawAFrame;            /* render on this pass of the main loop */
    BOOL bShowFrameRate;         /* show the frame rate at the top */
    BOOL bShowInfo;              /* show window information at the bottom */

//    BOOL bResized; /* the window has resized or some other drastic change, the
//                      entire client area should be cleared */
    BOOL bQuit;    /* program is about to terminate */

//    BOOL bNoTextures;           /* this sample doesn't use any textures */
//    BOOL bConstRenderQuality;   /* this sample is not constructed with
//                                   MeshBuilders and so the RenderQuality
//                                   cannot be changed */
    D3DRMRENDERQUALITY RenderQuality;   /* current shade mode, fill mode and
                                           lighting state */
    D3DRMTEXTUREQUALITY TextureQuality; /* current texture interpolation */
    BOOL bDithering;                    /* is dithering on? */
    BOOL bAntialiasing;                 /* is antialiasing on? */

    int mouse_buttons;          /* mouse button state */
    int mouse_x;                /* mouse cursor x position */
    int mouse_y;                /* mouse cursor y position */

    LPDIRECTDRAWSURFACE lpFrameRateBuffer; /* frame rate surface */
    LPDIRECTDRAWSURFACE lpInfoBuffer;      /* window info surface */


} rmfullglobals;

extern rmfullglobals myglobs;      /* collection of global variables */

extern DWORD    g_dwLastTime;
extern DWORD    g_dwCurTime;
extern DWORD    g_dwFpsTime;
extern DWORD    g_dwDeltaTime;
extern DWORD    g_dwFramesRendered;
extern DWORD    g_dwFps;



//extern AnimArgs                 g_player1AnimArgs;
//extern AnimArgs                 g_player2AnimArgs;

// Prototypes....
BOOL InitScene();
void TermScene();

BOOL RenderScene();
void RemoveVisuals(LPDIRECT3DRMFRAME frame);

HRESULT LoadTextures(char *sName, void *pArg, LPDIRECT3DRMTEXTURE *hTexture);
void Msg( LPSTR fmt, ... );
static BOOL AppPause(BOOL f);
BOOL D3DAppPause(BOOL flag);

#endif
   
