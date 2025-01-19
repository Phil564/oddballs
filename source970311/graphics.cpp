/*
 *  Project:         oddballs
 *  File:            graphics.cpp
 *  Description:     handles all graphics loading stuff etc
 *
 *  (c)1996 ISL.
 */

#include "oddballs.h"
#include "graphics.h"


BOOL rgb555 = FALSE;

void DetectColourMode()
{


	DDSURFACEDESC	sdesc;
	int l;

	INIT(sdesc);
	g_lpPrimary->GetSurfaceDesc(&sdesc);
	l = (int)sdesc.ddpfPixelFormat.dwGBitMask;
	while ((l&1) == 0)
		l >>= 1;
	rgb555 = (l == 31);



}



/*	--------------------------------------------------------------------------------
	Function : gfxLoadBMPIntoSurface
	Purpose : Load a BMP file into a surface
	Parameters : filename, surface, topx,topy
	Returns : 1/0
	Info : the surface MUST be big enough to receive the BMP image
*/

int gfxLoadBMPIntoSurface(char *fname, LPDIRECTDRAWSURFACE surf, int topx,int topy)
{
	int				bpp,pitch, xdim,ydim,fbpp, l, r,g,b, xc,yc, sxdim,sydim;
	BMPHeaderType	header;
	DDSURFACEDESC	sdesc;
	FILE			*fp;
	uchar	*cp, pal[768],*pp;
	ushort	*sp,*sp2;

	INIT(sdesc);
    surf->GetSurfaceDesc(&sdesc);

	bpp = sdesc.ddpfPixelFormat.dwRGBBitCount;
	pitch = sdesc.lPitch;

	
	sxdim = sdesc.dwWidth;
	sydim = sdesc.dwHeight;

	fp = fopen(fname, "rb");
	if (fp == NULL)
	{
		dprintf("LoadBMPIntoSurface: Could not open file '%s'\n", fname);
		return 0;
	}
	fseek(fp, 2L, SEEK_SET);
	fread((char*)&header, 1,sizeof(BMPHeaderType), fp);
	xdim = header.width;
	ydim = header.depth;
	fbpp = header.bits;
	if (fbpp == 8)
	{
		cp = pal;
		for(l = 0; l < 256; l++)
		{
			pal[2] = getc(fp);
			pal[1] = getc(fp);
			pal[0] = getc(fp);
			(void)getc(fp);
		}
	}

	dxerror = surf->Lock(NULL, &sdesc, 0, NULL);
	if (dxerror != DD_OK)
	{
		fclose(fp);
		dprintf("LoadBMPIntoSurface: Lock failed %s\n", D3DAppErrorToString(dxerror));
//		dprintf("LoadBMPIntoSurface: Lock failed '%s'\n", dxlDDErrorToString(dxerror));
		return 0;
	}

	if (bpp == 8)
	{
		cp = (uchar*)sdesc.lpSurface;
		cp += topx+((topy+ydim-1)*pitch);
		if (fbpp == 24)
		{
		}
		else
		{
			for(yc = 0; yc < ydim; yc++)
			{
				for(xc = 0; xc < xdim; xc++)
					fread(cp, 1,xdim, fp);
				cp -= pitch;
			}
		}
	}
	else
	{
		sp = (ushort*)sdesc.lpSurface;
		sp += topx+((topy+ydim-1)*(pitch>>1));
		if (fbpp == 24)
		{
			for(yc = 0; yc < ydim; yc++)
			{
				sp2 = sp;
				for(xc = 0; xc < xdim; xc++)
				{
					b = getc(fp);
					g = getc(fp);
					r = getc(fp);

					if((r <= 32) && (g <= 32) && (b <= 32))
					{
						r = g = b = 33;
					}

					if((b == 255) && (g == 0) && (r == 255))
					{
						r = g = b = 0;
					}
					if(rgb555 == TRUE)
						*sp2++ = (b>>3) | ((g>>3)<<5) | ((r>>3)<<10);
					else
						*sp2++ = (b>>3) | ((g>>2)<<5) | ((r>>3)<<11);
				}
				sp -= (pitch>>1);
			}
		}
		else
		{
			for(yc = 0; yc < ydim; yc++)
			{
				sp2 = sp;
				for(xc = 0; xc < xdim; xc++)
				{
					l = getc(fp);
					pp = pal+l+l+l;
					r = pal[0]; g = pal[1]; b = pal[2];
					*sp2++ = (b>>3) | ((g>>2)<<5) | ((r>>3)<<11);
				}
				sp -= (pitch>>1);
			}
		}
	}
	fclose(fp);
	surf->Unlock(NULL);

	return 1;
}


/*	--------------------------------------------------------------------------------
	Function : gfxLoadBMPIntoSurface
	Purpose : Load a BMP file into a surface
	Parameters : filename, surface, topx,topy
	Returns : 1/0
	Info : the surface MUST be big enough to receive the BMP image
*/

BOOL gfxReturnBMPSize(char *fname,long *xdim, long *ydim)
{
	BMPHeaderType	header;
	FILE			*fp;


	fp = fopen(fname, "rb");
	if (fp == NULL)
	{
		dprintf("LoadBMPIntoSurface: Could not open file '%s'\n", fname);
		return FALSE;
	}
	fseek(fp, 2L, SEEK_SET);
	fread((char*)&header, 1,sizeof(BMPHeaderType), fp);
	*xdim = header.width;
	*ydim = header.depth;
	fclose(fp);
	return TRUE;
}
