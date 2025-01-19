#ifndef __RMSTATS_H
#define __RMSTATS_H


//-----------------------------------------------
//user surfaces for icons etc!
#define MAX_USER_SURFACES	10


typedef struct
{
    LPDIRECTDRAWSURFACE buffer;      /* window info surface */
    SIZE size;						//size of icon buffer
	SIZE position;					//position of buffer on screen
	SIZE frameSize;
	char filename[256];
	BOOL animating;
	int	 numberOfFrames;
	int  currentFrame;
	int  animSpeed;
	BOOL display;
	BOOL active;

}USER_SURFACE;

extern USER_SURFACE userSurface[];
extern HFONT scoreFont;







BOOL InitFontAndTextBuffers(void);
BOOL WriteInfoBuffer(char *buf, int x, int y);
BOOL WriteFrameRateBuffer(float fps, long tps);
void ResetFrameRate(void);
BOOL CalculateFrameRate();
BOOL DisplayFrameRate(int* count, LPD3DRECT lpExtents );
BOOL InitUserSurfaces(void);
BOOL DisplayUserSurfaces(int* count, LPD3DRECT lpExtents );
BOOL CreateSurface(USER_SURFACE *surface);
inline int BlitArea(LPDIRECTDRAWSURFACE ssurf, LPDIRECTDRAWSURFACE dsurf,
					 long sx0, long sy0, long sx1, long sy1,
					 long dx0, long dy0, long dx1, long dy1, int trans);


#endif