#ifndef __SOUND_H
#define __SOUND_H

typedef struct tagDXLSampleInstType {			// a DXL sample instance
	LPDIRECTSOUNDBUFFER		buffer;				// duplicate buffer
	tagDXLSampleInstType	*next,*prev;		// links
	struct tagDXLSampleType	*parent;			// parent buffer
} DXLSampleInstType;

typedef struct tagDXLSampleType {				// info on a sound sample under DXL control
	LPDIRECTSOUNDBUFFER		buffer;				// primary sample buffer
	DXLSampleInstType		*instances;			// secondary instances
} DXLSampleType;

typedef struct tagDXLInfo_SND {
	LPDIRECTSOUND		dsound;					// direct sound object
	GUID				*guid;					// DirectSound object GUID ptr
	char				lpstrDescription[256];	// DirectSound object description
	char				lpstrModule[256];		// DirectSound driver name
} DXLInfo_SND;






#define NUMSOUNDS 30

enum
{
	SFX_FOOTSTEP1,
	SFX_FOOTSTEP2,
	SFX_SPLASH1,
	SFX_SPLASH2,
	SFX_HANDBOUNCE,
	SFX_HANDJUMP,
	SFX_HANDWHACK,
	SFX_HANDLAND,
	SFX_SWITCH,
	SFX_SPIKESWITCH,
	SFX_GATEOPEN,
	SFX_HEAVYLAND,
	SFX_SPIKE,
	SFX_HANDDROWN,

	SFX_LOBSTER,
	SFX_CHEST,
	SFX_CUTLASS,
	SFX_BIGBALL,

	SFX_SWISH,
	SFX_MOTOR,
	SFX_SQUASH,
	SFX_SNAPPY,
	SFX_TELEPORT,
	SFX_BALLBURST,
	SFX_TIMEGATE,
	SFX_COLLECTABLE,
};

DWORD playCDTrack(HWND hWndNotify, BYTE bTrack);
DWORD OpenCDDevice(HWND hWndNotify);
DWORD CloseCDDevice();
DWORD StopCDTrack();
void PlaySample(int sampleNum, BOOL loop, VECTOR *source, int volume);

char *DxlDSErrorToString(HRESULT res);
void DxlDeleteSample(DXLSampleType *sbuf);
int DxlLoadSample(char *fname, DXLSampleType *sbuf);
BOOL dsenumcallback(GUID FAR * lpGuid, LPSTR lpstrDescription, LPSTR lpstrModule, LPVOID lpContext);
int DxlInitSound(HWND whand);
void DxlShutdownSound();
void DxlPlaySampleDirect(DXLSampleType *sptr, int loop);
void DxlStopSampleDirect(DXLSampleType *sptr);
DXLSampleInstType *DxlPlaySampleInstance(DXLSampleType *sptr, int loop);
void DxlStopSampleInstance(DXLSampleInstType *inst);
void DxlSetSampleFrequency(DXLSampleType *sptr, int freq);
void DxlSetSampleInstanceFrequency(DXLSampleInstType *inst, int freq);
void DxlSetSampleVolume(DXLSampleType *sptr, int vol);
void DxlSetSampleInstanceVolume(DXLSampleInstType *sptr, int vol);
void InitSound();
void FreeSamples();


extern DXLInfo_SND		dsi;		// DirectSound info
extern DXLSampleType	sounds[];
extern int musicPlaying;


#endif