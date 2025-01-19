/*==========================================================================
 *
 *  Copyright (C) 1995, 1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File: rmstats.cpp
 *
 ***************************************************************************/
#include "oddballs.h"
#include "graphics.h"
#include "personal.h"
#include "rmstats.h"
#include "winmain.h"
#include "collectables.h"
#include "hand.h"
/*
 * GLOBAL VARIABLES
 */
//extern D3DAppInfo* d3dapp;          /* Pointer to read only collection of DD and D3D
//                                       objects maintained by D3DApp */
//extern rmfullglobals myglobs;       /* collection of global variables */

HFONT scoreFont;


USER_SURFACE userSurface[MAX_USER_SURFACES];


/*	--------------------------------------------------------------------------------
	Function 	: CreateSurface()
	Purpose 	: creates a user surface
	Parameters 	: surface pointer
	Returns 	: true/false
	Info 		:
*/
BOOL CreateSurface(USER_SURFACE *surface)
{
	int x;
    DDCOLORKEY          ddck;
    DDSURFACEDESC       ddsd;
    HRESULT ddrval;
	char				temp[256];
	SIZE				size;	

	
	RELEASE(surface->buffer);
	if(surface->active == TRUE)
	{
		size.cx = size.cy = 0;
		x = 1;
		while(1)
		{
			sprintf(temp, "%s%04d.bmp", surface->filename, x);
			if(gfxReturnBMPSize(temp, &size.cx, &size.cy) == TRUE)
			{
				surface->frameSize.cx = size.cx;
				surface->frameSize.cy = size.cy;

				surface->size.cx += size.cx;
				surface->size.cy = size.cy;

				if(x > 1)
				{
					surface->animating = TRUE;
					surface->animSpeed = 1;
					surface->numberOfFrames = x;
				}
				x++;
			}
			else
			{
				if(x == 1)
				{
					surface->active = FALSE;
					return FALSE;
				}
			 	break;
			}
		}

//		gfxReturnBMPSize(surface->filename, &(surface->size.cx), &(surface->size.cy));
					
		memset( &ddsd, 0, sizeof( ddsd ) );
		ddsd.dwSize = sizeof( ddsd );
//		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
//		ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_ALLOCONLOAD | DDSCAPS_OFFSCREENPLAIN;
   
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		ddsd.dwHeight = surface->size.cy;
		ddsd.dwWidth = surface->size.cx;
		ddrval = D3DAppICreateSurface(&ddsd, &surface->buffer);
		if (ddrval != DD_OK) 
		{
			Msg("Could not create user surface buffer.\n%s", D3DAppErrorToString(ddrval));    
			goto exit_with_error;
		}
		memset(&ddck, 0, sizeof(ddck));
		surface->buffer->SetColorKey(DDCKEY_SRCBLT, &ddck);

		x = 1;
		size.cx = size.cy = 0;
		while(1)
		{
			sprintf(temp, "%s%04d.bmp", surface->filename, x++);
			if(FileExists(temp))
			{
				gfxLoadBMPIntoSurface(temp, surface->buffer, size.cx, size.cy);
				size.cx += surface->frameSize.cx;
			}
			else
				break;
		}
	}

	return TRUE;
exit_with_error:
	RELEASE(surface->buffer);
	return FALSE;

}


/*	--------------------------------------------------------------------------------
	Function 	: InitUserSurfaces
	Purpose 	: initialise direct draw surfaces
	Parameters 	: none
	Returns 	: BOOL
	Info 		:
*/
BOOL
InitUserSurfaces(void)
{
    DDCOLORKEY          ddck;
    DDSURFACEDESC       ddsd;
    HDC hdc;
    HRESULT ddrval;
   	int x;
	USER_SURFACE *surface;
	static int timesCalled = 0;

	if(timesCalled++ > 0)
		return TRUE;

//	for(x = 0; x < MAX_USER_SURFACES; x++)
//		memset(&userSurface[x], 0, sizeof(USER_SURFACE));

	for(x = 0; x < MAX_USER_SURFACES; x++)
	{

		surface = &userSurface[x];
		CreateSurface(surface);

	}

	dprintf("%d User surfaces initialised\n", x);




	return TRUE;
exit_with_error:
	RELEASE(surface->buffer);
	return FALSE;

}


/*************************************************************************
  Frame rate output.
 *************************************************************************/

#define INTERVAL 30

char StatTxt[100];
int StatTxtLen;
int count = 0;
int last_polygons = 0;
int this_frames = 0;
time_t last_time;
float fps;
long tps;


/*
 * DisplayFrameRate
 * Outputs frame rate info and screen mode to back buffer when appropriate.
 */
BOOL
DisplayUserSurfaces(int* count, LPD3DRECT lpExtents )
{
    RECT rc;
    int x, y;
    HRESULT ddrval = DD_OK;
	USER_SURFACE *surface;
	int cx;
	SIZE offset;

	for(cx = 0; cx < MAX_USER_SURFACES; cx++)
	{

		surface = &userSurface[cx];

		if(surface->display == FALSE)
			continue;

	  	if (!myglobs.bSingleStepMode && surface->frameSize.cx > 0 && surface->frameSize.cy > 0 &&
			surface->frameSize.cx < szClient.cx && surface->frameSize.cy < szClient.cy) 
		{
			SetRect(&rc, 0, 0, surface->frameSize.cx, surface->frameSize.cy);
			x = surface->position.cx;
			y = surface->position.cy;

			offset.cx = 0;
			offset.cy = 0;
			if(surface->animating)
			{
				surface->currentFrame += surface->animSpeed;
				if(surface->currentFrame >= surface->numberOfFrames)
					surface->currentFrame = 0;
				offset.cx = surface->frameSize.cx * surface->currentFrame;
			}
	
			if(cx == LIVES_ICON)
			{
				for(x = 0; x < player.numLives; x++)
				{
					ddrval = BlitArea(surface->buffer, g_lpBackBuffer, offset.cx, offset.cy, surface->frameSize.cx + offset.cx, surface->frameSize.cy + offset.cy, surface->position.cx + (x * (surface->frameSize.cx + 8)), surface->position.cy, surface->position.cx + surface->frameSize.cx + (x * (surface->frameSize.cx + 8)), surface->position.cy + surface->frameSize.cy, TRUE);

				}
			}
			else if (surface->buffer)
				ddrval = BlitArea(surface->buffer, g_lpBackBuffer, offset.cx, offset.cy, surface->frameSize.cx + offset.cx, surface->frameSize.cy + offset.cy, surface->position.cx, surface->position.cy, surface->position.cx + surface->frameSize.cx, surface->position.cy + surface->frameSize.cy, TRUE);
//				ddrval = d3dapp->lpBackBuffer->BltFast(x, y, surface->buffer, &rc,
//													 DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
			if (ddrval != DD_OK) 
			{
				//
				// Blting the frame rate has failed. Since it is not vital, we
				// aren't going to report an error.  Check to see if the surfaces
				// have been lost and then return.
				//
				if (ddrval == DDERR_SURFACELOST) 
				{
					g_lpBackBuffer->Restore();
					surface->buffer->Restore();
				}
			}
			SetRect((LPRECT)(lpExtents), x, y, surface->size.cx + x, surface->size.cy + y);
			++(*count);
			++lpExtents;
		}


	}
	return TRUE;

}




inline int BlitArea(LPDIRECTDRAWSURFACE ssurf, LPDIRECTDRAWSURFACE dsurf,
					 long sx0, long sy0, long sx1, long sy1,
					 long dx0, long dy0, long dx1, long dy1,
					 int trans)
{
	RECT	sarea = {sx0,sy0,sx1,sy1},
		darea = {dx0,dy0,dx1,dy1};
	DDBLTFX	bltFX;
	HRESULT	dxerror;

	if (trans)
	{
		INIT(bltFX);
		bltFX.ddckSrcColorkey.dwColorSpaceLowValue = 0;
		bltFX.ddckSrcColorkey.dwColorSpaceHighValue = 0;

		dxerror = dsurf->Blt(&darea, ssurf, &sarea, 
					DDBLT_WAIT|DDBLT_KEYSRCOVERRIDE, &bltFX);
	}
	else
	{
		dxerror = dsurf->Blt(&darea, ssurf, &sarea, DDBLT_WAIT, NULL);
	}

	return dxerror;
}

