#ifndef __GRAPHICS_H
#define __GRAPHICS_H


int gfxLoadBMPIntoSurface(char *fname, LPDIRECTDRAWSURFACE surf, int topx,int topy);
BOOL gfxReturnBMPSize(char *fname,long *xdim, long *ydim);
void DetectColourMode();


#endif