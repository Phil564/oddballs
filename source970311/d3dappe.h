/*
 *  Copyright (C) 1995, 1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File: d3dappe.h
 *
 *  Header to be included in source using D3DApp.  Contains D3DApp function
 *  prototypes and defines.
 *
 *  D3DApp is a collection of helper functions for Direct3D applications.
 *  D3DApp consists of the following files:
 *      d3dapp.h    Main D3DApp header to be included by application
 *      d3dappi.h   Internal header
 *      d3dapp.c    D3DApp functions seen by application.
 *      ddcalls.c   All calls to DirectDraw objects except textures
 *      d3dcalls.c  All calls to Direct3D objects except textures
 *      texture.c   Texture loading and managing texture list
 *      misc.c      Miscellaneous calls
 */

#ifndef __D3DAPPE_H__
#define __D3DAPPE_H__

#define INITGUID

#include <ddraw.h>
#include <d3d.h>

#ifdef __cplusplus
//extern "C" {
#endif

/*
 * DEFINES
 */
#define D3DAPP_WINDOWMINIMUM 50     /* smallest buffer size allowed */
#define D3DAPP_DEFAULTWINDOWDIM 320 /* replaces window size if invalid */
#define D3DAPP_MINBUFFERSIZE 15360  /* minimum "maximum buffer size" for a
                                       D3D driver to be accepted */
#define D3DAPP_MINVERTEXCOUNT 320   /* minimum "maximum vertex count" for a
                                       D3D driver to be accepted */
#define D3DAPP_MAXD3DDRIVERS 5      /* maximum Direct3D drivers ever expected
                                       to find */
#define D3DAPP_MAXTEXTUREFORMATS 50 /* maximum texture formats */
#define D3DAPP_MAXMODES 50          /* maximum display modes ever expected to
                                       be reported by DirectDraw */
#define D3DAPP_MAXTEXTURES 15       /* maximum number of textures that wil be
                                       loaded and managed */
#define D3DAPP_MAXCLEARRECTS 30     /* maximum num. rectangles (ie extents)
                                       for clearing */
#define D3DAPP_BOGUS -100           /* unused parameters accept this */
#define D3DAPP_YOUDECIDE -25        /* Use this for certain parameters to
                                       have D3DApp decide an appropriate
                                       value for you */
#define D3DAPP_USEWINDOW -24        /* Used in place of fullscreen mode */

/*
 * DATA STRUCTURES
 */

/*
 * D3DAppD3DDriver structure
 * Describes a D3D driver
 */
typedef struct tagD3DAppD3DDriver {
    char Name[30];      /* short name of the driver */
    char About[50];     /* short string about the driver */
    D3DDEVICEDESC Desc; /* D3DDEVICEDESC for complete information */
    GUID Guid;          /* it's GUID */
    BOOL bIsHardware;   /* does this driver represent a hardware device? */
    BOOL bDoesTextures; /* does this driver do texture mapping? */
    BOOL bDoesZBuffer;  /* can this driver use a z-buffer? */
    BOOL bCanDoWindow;  /* can it render to Window's display depth? */
} D3DAppD3DDriver;

/*
 * D3DAppTextureFormat stucture
 * Describes a texture format
 */
typedef struct tagD3DAppTextureFormat {
    DDSURFACEDESC ddsd; /* DDSURFACEDESC for complete information */
    BOOL bPalettized;   /* is this format palettized */
    int RedBPP;         /* number of red, */
    int BlueBPP;        /*                blue, */
    int GreenBPP;       /*                      and green bits per pixel */
    int IndexBPP;       /* number of bits in palette index */
} D3DAppTextureFormat;

/*
 * D3DAppMode structure
 * Describes a display mode
 */
typedef struct tagD3DAppMode {
    int     w;                /* width */
    int     h;                /* height */
    int     bpp;              /* bits per pixel */
    BOOL    bThisDriverCanDo; /*can current D3D driver render in this mode?*/
} D3DAppMode;

/*
 * D3DAppInfo structure
 * Contains all the information D3DApp makes available to the application. A
 * pointer to the internal, read only copy is returned by the initializing
 * function.
 */
typedef struct tagD3DAppInfo {
    HWND                    hwnd;          /*handle of window being managed*/
    /*
     * Direct3D objects and information
     */
    LPDIRECT3D              lpD3D;         /* D3D object */
    LPDIRECT3DDEVICE        lpD3DDevice;   /* D3D device */
    LPDIRECT3DVIEWPORT      lpD3DViewport; /* D3D viewport, created by
                                              application */
    int                     NumDrivers;    /* number of D3D drivers avail. */
    int                     CurrDriver;    /* number of curr. D3D driver */
    D3DAppD3DDriver         Driver[D3DAPP_MAXD3DDRIVERS]; /* avail. drivers*/
    D3DAppD3DDriver         ThisDriver;    /* description of this driver,
                                           identical to Driver[CurrDriver] */

    int                     NumTextureFormats; /* num texture formats avail*/
    int                     CurrTextureFormat; /* current texture format
                  will only change if driver changes or when app changes it*/
    D3DAppTextureFormat     TextureFormat[D3DAPP_MAXTEXTUREFORMATS];
                                      /* description of all avail. formats */
    D3DAppTextureFormat     ThisTextureFormat; /* description of this format,
                             identical to TextureFormat[CurrTextureFormat] */

    int                     NumTextures;    /* number of textures in D3DApp's
                                               texture list */
    char                    ImageFile[D3DAPP_MAXTEXTURES][50]; /* files */
    D3DTEXTUREHANDLE        TextureHandle[D3DAPP_MAXTEXTURES]; /* handles */
    LPDIRECTDRAWSURFACE     lpTextureSurf[D3DAPP_MAXTEXTURES]; /* surfaces */
    LPDIRECT3DTEXTURE       lpTexture[D3DAPP_MAXTEXTURES]; /* texture objs */
    int                     NumUsableTextures; /* the number of currently usable
                                                  textures (e.g. for a hardware
                                                  device there may not be enough
                                                  video memory*/
    /*
     * DirectDraw objects and information
     */
    LPDIRECTDRAW            lpDD;          /* DirectDraw object */
    BOOL                    bIsPrimary;    /* Is this the primary DD device?
               If FALSE, we're using a hardware DD device that cannot
               display a window and so only fullscreen modes are available */
    LPDIRECTDRAWSURFACE     lpFrontBuffer; /* front buffer surface */
    LPDIRECTDRAWSURFACE     lpBackBuffer;  /* back buffer surface */
    LPDIRECTDRAWSURFACE     lpZBuffer;     /* z-buffer surface */
    BOOL                    bBackBufferInVideo; /* back buf in video mem? */
    BOOL                    bZBufferInVideo;    /* is Z-buf in video mem? */

    int                     NumModes; /* number of available display modes */
    int                     CurrMode; /* number of current display mode (only
                                         when fullscreen) */
    D3DAppMode              Mode[D3DAPP_MAXMODES]; /* desc avail modes */
    D3DAppMode              ThisMode; /* description of this mode, identical
                                         to Mode[CurrMode] */
    BOOL                    bFullscreen; /* in fullscreen exclusive mode? */
    D3DAppMode              WindowsDisplay; /* current Windows disply mode */

    SIZE                    szClient;         /* dimensions of client win */
    POINT                   pClientOnPrimary; /* position of client win */

    BOOL                    bPaused;           /* the app is paused */
    BOOL                    bAppActive;        /* the app is active */
    BOOL                    bTexturesDisabled; /* textures are disabled */
    BOOL                    bOnlySystemMemory; /* all surfaces forced into
                                                  system memory */
    BOOL                    bOnlyEmulation;    /* no hardware DD or D3D
                                                  devices allowed */
    BOOL                    bMinimized;        /* app window is minimized */
    BOOL                    bRenderingIsOK;    /* All objects etc. necessary
                                                  for rendering are in ok */
} D3DAppInfo;

/*
 * D3DAppRenderState structure
 * The "render state" is the status of this collection of D3D options and
 * variables.  This structure is used to get and set the render state.  The
 * render state will only change during program initialization and when
 * the application sets it.
 */
typedef struct tagD3DAppRenderState {
    BOOL             bZBufferOn;    /* Z buffer is on */
    BOOL             bPerspCorrect; /* perspective correction is on */
    D3DSHADEMODE     ShadeMode;     /* flat, gouraud, phong? */
    D3DTEXTUREFILTER TextureFilter; /* linear or bi-linear texture filter */
    D3DTEXTUREBLEND  TextureBlend;  /* Use shade mode or copy mode? */
    D3DFILLMODE      FillMode;      /* solid, lines or points? */
    BOOL             bDithering;    /* dithering is on */
    BOOL             bSpecular;     /* specular highlights are on */
    BOOL             bAntialiasing; /* anti-aliasing is on */

    BOOL             bFogEnabled;   /* fog is on */
    D3DCOLOR         FogColor;      /* fog color */
    D3DFOGMODE       FogMode;       /* linear, exp. etc. */
    D3DVALUE         FogStart;      /* begining depth */
    D3DVALUE         FogEnd;        /* ending depth */
} D3DAppRenderState;

/*
 * FUNCTION PROTOTYPES
 */


void dpf( LPSTR fmt, ... );

/*
 * D3DAppCreateFromHWND
 *
 * Call this before all other D3DApp functions (except AddTexture).  
 * Initializes all DD and D3D objects necessary for rendering, enumerates the
 * available display modes and drivers and loads textures specified by prior
 * AddTexture() calls.  Caller passes the handle of the window to be manged
 * and callback functions to execute for device creation and destruction.
 * 
 * DeviceCreateCallback is executed AFTER the creation of D3D device and all
 * objects D3DApp created using the device.  This allows an application to
 * reconstruct the scene and create any additional objects.  The callback
 * must create and return (in the variable provided) the DIRECT3DVIEWPORT
 * from the given width and height.  The returned pointer is stored in the
 * D3DAppInfo structure and used for clearing and setting the render state.
 * A NULL pointer is fine if D3DApp is not used for either of these
 * functions. The create callback will always be called before any calls to
 * the destroy callback.  The boolean returned indicates success or failure.
 *
 * DeviceDestroyCallback is executed BEFORE the D3D device and objects
 * created by D3DApp using the device are released.  This allows an
 * application to save data regarding the scene or release any objects
 * created from the device before it is destroyed.  The DIRECT3DVIEWPORT
 * should be released in this callback.  The boolean returned indicates the
 * success or failure.
 *
 * A pointer to the internal D3DAppInfo data structure is returned.  This
 * should be READ ONLY!
 *
 * The DirectDraw device, Direct3D driver, display mode and texture format
 * will all be chosen by D3DApp.  Hardware DD and D3D devices are prefered.
 * Mono lighting D3D drivers are prefered.  Paletted texture formats are
 * prefered.  If possible, the current window size will be used, otherwise
 * a fullscreen mode will be selected.
 *
 * Call AddTexture() to add textures to be loaded upon initialization.
 *
 * Valid flags:
 *    D3DAPP_ONLYSYSTEMMEMORY  Force all surfaces into system memory.  Also
 *                             disables hardware DD and D3D drivers.
 *    D3DAPP_ONLYD3DEMULATION  Disable D3D hardware
 *    D3DAPP_ONLYDDEMULATION   Disable DD hardware
 */
#define D3DAPP_ONLYSYSTEMMEMORY 0x00000001
#define D3DAPP_ONLYD3DEMULATION 0x00000002
#define D3DAPP_ONLYDDEMULATION  0x00000004

#ifdef __cplusplus
//};
#endif

#endif // __D3DAPPE_H__
