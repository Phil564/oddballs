/*
 *  Project:         oddballs
 *  File:            sound.cpp
 *  Description:     holds all the sound / CDDA stuff
 *
 *  (c)1996 ISL.
 */

#include "oddballs.h"
#include "sound.h"
#include "personal.h"

// Plays a given CD audio track using MCI_OPEN, MCI_PLAY. Returns as 
// soon as playback begins. The window procedure function for the given
// window will be notified when playback is complete. Returns 0L on
// success; otherwise, returns an MCI error code.

UINT wDeviceID;
DWORD dwReturn;
MCI_OPEN_PARMS mciOpenParms;
MCI_SET_PARMS mciSetParms;
MCI_PLAY_PARMS mciPlayParms;

DXLInfo_SND		dsi;		// DirectSound info

DXLSampleType	sounds[NUMSOUNDS];
int musicPlaying;
int	soundOK;

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
DWORD OpenCDDevice(HWND hWndNotify)
{
    // Open the CD audio device by specifying the device name.
    mciOpenParms.lpstrDeviceType = "cdaudio";
    if (dwReturn = mciSendCommand(NULL, MCI_OPEN,
        MCI_OPEN_TYPE, (DWORD)(LPVOID) &mciOpenParms))
    {
        // Failed to open device. Don't close it; just return error.
		dprintf("OpenCDDevice:Cant open CD Device\n");
        return (dwReturn);
    }
    return (0L);
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
DWORD CloseCDDevice()
{
	StopCDTrack();
    wDeviceID = mciOpenParms.wDeviceID;

    mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
    return (dwReturn);
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
DWORD StopCDTrack()
{
    wDeviceID = mciOpenParms.wDeviceID;

    mciSendCommand(wDeviceID, MCI_STOP, 0, NULL);
    return (dwReturn);
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
DWORD playCDTrack(HWND hWndNotify, BYTE bTrack)
{
 
    // The device opened successfully; get the device ID.
    wDeviceID = mciOpenParms.wDeviceID;

    // Set the time format to track/minute/second/frame (TMSF).
    mciSetParms.dwTimeFormat = MCI_FORMAT_TMSF;
    if (dwReturn = mciSendCommand(wDeviceID, MCI_SET, 
        MCI_SET_TIME_FORMAT, (DWORD)(LPVOID) &mciSetParms))
    {
        mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
		dprintf("WARNING!! playCDTrack:cant set time format\n");
        return (dwReturn);
    }  
    // Begin playback from the given track and play until the beginning 
    // of the next track. The window procedure function for the parent 
    // window will be notified with an MM_MCINOTIFY message when 
    // playback is complete. Unless the play command fails, the window 
    // procedure closes the device.
    mciPlayParms.dwFrom = 0L;
    mciPlayParms.dwTo = 0L;
    mciPlayParms.dwFrom = MCI_MAKE_TMSF(bTrack, 0, 0, 0);
    mciPlayParms.dwTo = MCI_MAKE_TMSF(bTrack + 1, 0, 0, 0);
    mciPlayParms.dwCallback = (DWORD) hWndNotify;

    if (dwReturn = mciSendCommand(wDeviceID, MCI_PLAY,
        MCI_FROM | MCI_TO | MCI_NOTIFY, (DWORD)(LPVOID) &mciPlayParms))
    {
		dprintf("WARNING!! playCDTrack:Play error\n");
        mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
        return (dwReturn);
    }
    return (0L);

    return (0L);
}
 
/*	--------------------------------------------------------------------------------
	Function : dxlDSErrorToString
	Purpose : Convert a DirectSound error code to a descriptive string
	Parameters : error code
	Returns : string
	Info :
*/
char *DxlDSErrorToString(HRESULT res)
{
	if (res == DS_OK)
		return "The request completed successfully";

	if (res == DSERR_ALLOCATED)
		return "The request failed because resources, such as a priority level, were already in use by another caller";

	if (res == DSERR_ALREADYINITIALIZED)
		return "The object is already initialized";

	if (res == DSERR_BADFORMAT)
		return "The specified wave format is not supported";

	if (res == DSERR_BUFFERLOST)
		return "The buffer memory has been lost and must be restored";

	if (res == DSERR_CONTROLUNAVAIL)
		return "The control (volume, pan, and so forth) requested by the caller is not available";

	if (res == DSERR_GENERIC)
		return "An undetermined error occurred inside the DirectSound subsystem";

	if (res == DSERR_INVALIDCALL)
		return "This function is not valid for the current state of this object";

	if (res == DSERR_INVALIDPARAM)
		return "An invalid parameter was passed to the returning function";

	if (res == DSERR_NOAGGREGATION)
		return "The object does not support aggregation";

	if (res == DSERR_NODRIVER)
		return "No sound driver is available for use";

	if (res == DSERR_OTHERAPPHASPRIO)
		return "This value is obsolete and is not used";

	if (res == DSERR_OUTOFMEMORY)
		return "The DirectSound subsystem could not allocate sufficient memory to complete the caller's request";

	if (res == DSERR_PRIOLEVELNEEDED)
		return "The caller does not have the priority level required for the function to succeed";

	if (res == DSERR_UNINITIALIZED)
		return "The IDirectSound::Initialize method has not been called or has not been called successfully before other methods were called";

	if (res == DSERR_UNSUPPORTED)
		return "The function called is not supported at this time";

	return "Undefined error";
}


/*	--------------------------------------------------------------------------------
	Function : dxlDeleteSample
	Purpose : Delete a sample from memory
	Parameters : sample ptr
	Returns : 1/0
	Info :
*/

void DxlDeleteSample(DXLSampleType *sbuf)
{
	while (sbuf->instances != NULL)
		DxlStopSampleInstance(sbuf->instances);
	if(sbuf->buffer != NULL)
		sbuf->buffer->Release();
}


/*	--------------------------------------------------------------------------------
	Function : dxlLoadSample
	Purpose : Load a sample
	Parameters : filename, sample ptr
	Returns : 1/0
	Info :
*/

int DxlLoadSample(char *fname, DXLSampleType *sbuf)
{
	DSBUFFERDESC	bdesc;
	WAVEFORMATEX	wform;
	FILE			*fp;
	unsigned char	*buf, *sdata, *sbufptr, *sbuf2;
	int				len, sdatalen;
	unsigned long	byteslocked;
	DWORD			*dwp, riff,slen,type,*end;

	INIT(bdesc);
	
	fp = fopen(fname, "rb");
	if (fp == NULL)
		{
		dprintf("\nWARNING!! dxlLoadSample: Could not open file '%s'\n\n", fname);
		return 0;
		}
	fseek(fp, 0L, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
//	buf = (uchar*)dxlMalloc(len, "sample buffer");
	buf = (uchar*)MyMalloc(len);
	if (buf == NULL)
		{
		dprintf("dxlLoadSample: Could not alloc buffer (%d bytes)\n", len);
		fclose(fp);
		return 0;
		}
	fread(buf, 1,len, fp);
	fclose(fp);

	dwp = (DWORD*)buf;
    riff = *dwp++;
    slen = *dwp++;
    type = *dwp++;
	
    if (riff != mmioFOURCC('R', 'I', 'F', 'F'))
		{
		MyFree((void**)&buf);
		buf = 0;
//		dxlFree(buf);
		dprintf("dxlLoadSample: File not RIFF format\n");
		return 0;
		}

    if (type != mmioFOURCC('W', 'A', 'V', 'E'))
		{
//		dxlFree(buf);
		MyFree((void **)&buf);
		buf = 0;
		dprintf("dxlLoadSample: File not WAVE format\n");
		return 0;
		}

    end = (DWORD*)((BYTE*)dwp+slen-4);

    while (dwp < end)
	    {
        type = *dwp++;
        slen = *dwp++;

        switch (type)
	        {
		    case mmioFOURCC('f', 'm', 't', ' '):
                if (slen < sizeof(WAVEFORMAT))
					{
//					dxlFree(buf);
					MyFree((void **)&buf);
					buf = 0;
					dprintf("dxlLoadSample: File does not appear to be a WAV\n");
					return 0;
					}
		        wform = *((WAVEFORMATEX*)dwp);
	            break;

	        case mmioFOURCC('d', 'a', 't', 'a'):
				sdata = (uchar*)dwp;
			    sdatalen = slen;
				break;
	        }

        dwp = (DWORD*)((BYTE*)dwp + ((slen+1)&~1));
		}

	bdesc.dwFlags = DSBCAPS_CTRLDEFAULT;
	bdesc.dwBufferBytes = sdatalen;
	bdesc.lpwfxFormat = &wform;

//	dprintf("dxlLoadSample: Found sample '%s', length %d\n", fname, sdatalen);

	dxerror = dsi.dsound->CreateSoundBuffer(&bdesc, &(sbuf->buffer), NULL);
	if (dxerror != DS_OK)
		{
		dprintf("dxlLoadSample: CreateSoundBuffer failure '%s'\n", DxlDSErrorToString(dxerror));
		return 0;
		}

	dxerror = (sbuf->buffer)->Lock(0, sdatalen, &sbufptr, &byteslocked, &sbuf2,NULL, 0);
	if (dxerror != DS_OK)
		{
		(sbuf->buffer)->Release();
//		dxlFree(buf);
		MyFree((void **)&buf);
		buf = 0;
		dprintf("dxlLoadSample: Lock failure '%s'\n", DxlDSErrorToString(dxerror));
		return 0;
		}

	if ((int)byteslocked != sdatalen)
		{
		(sbuf->buffer)->Unlock(sbufptr, 0, sbuf2,0);
		(sbuf->buffer)->Release();
//		dxlFree(buf);
		MyFree((void **)&buf);
		buf = 0;
		dprintf("dxlLoadSample: Could only lock %d of %d bytes\n", byteslocked, sdatalen);
		return 0;
		}

	memcpy(sbufptr, sdata, sdatalen);
	(sbuf->buffer)->Unlock(sbufptr, sdatalen, sbuf2,0);
//	dxlFree(buf);
	MyFree((void **)&buf);
	buf = 0;
	
	sbuf->instances = NULL;

	return 1;
}


/*	--------------------------------------------------------------------------------
	Function : dxlInitSound
	Purpose : Initialise sound stuff
	Parameters : window handle
	Returns : 1/0
	Info :
*/

BOOL dsenumcallback(GUID FAR * lpGuid, LPSTR lpstrDescription, LPSTR lpstrModule, LPVOID lpContext)
{
	dsi.guid = lpGuid;
	strcpy(dsi.lpstrDescription, lpstrDescription);
	strcpy(dsi.lpstrModule, lpstrModule);

	dprintf("dxlInitSound: Found sound device '%s' '%s'\n", lpstrDescription, lpstrModule);

	return FALSE;
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
int DxlInitSound(HWND whand)
{
//	char	buf[256];

/*	dxerror = DirectSoundEnumerate((LPDSENUMCALLBACK)dsenumcallback, NULL);
	if (dxerror != DS_OK)
		{
		dprintf("dxlInitSound: DirectSoundEnumerate failure '%s'\n", DxlDSErrorToString(dxerror));
		}  
*/
	dsi.dsound = NULL;
	dxerror = DirectSoundCreate(NULL /*dsi.guid*/, &dsi.dsound, NULL);
	if (dxerror != DS_OK)
		{
		dprintf("dxlInitSound: DirectSoundCreate failure '%s'\n", DxlDSErrorToString(dxerror));
		return 0;
		}

	dxerror = dsi.dsound->SetCooperativeLevel(whand, DSSCL_EXCLUSIVE);
	if (dxerror != DS_OK)
		{
		dprintf("dxlInitSound: SetCooperativeLevel failure '%s'\n", DxlDSErrorToString(dxerror));
		return 0;
		}

	return 1;
}


/*	--------------------------------------------------------------------------------
	Function : dxlShutdownSound
	Purpose : Shutdown sound stuff
	Parameters : 
	Returns : 
	Info :
*/

void DxlShutdownSound()
{
	if (dsi.dsound != NULL)
		dsi.dsound->Release();
}


/*	--------------------------------------------------------------------------------
	Function : dxlPlaySampleDirect
	Purpose : play a sound directly from it's sample
	Parameters : sample, loop it?
	Returns : 
	Info :
*/

void DxlPlaySampleDirect(DXLSampleType *sptr, int loop)
{
	dxerror = sptr->buffer->Play(0, 0, (loop) ? DSBPLAY_LOOPING : 0);
	if (dxerror != DS_OK)
		dprintf("PlaySample: Play failure '%s'\n", DxlDSErrorToString(dxerror));
}


/*	--------------------------------------------------------------------------------
	Function : dxlStopSampleDirect
	Purpose : stop a sound directly from it's sample
	Parameters : sound
	Returns : 
	Info :
*/

void DxlStopSampleDirect(DXLSampleType *sptr)
{
	dxerror = sptr->buffer->Stop();
	if (dxerror != DS_OK)
		dprintf("StopSample: Stop failure '%s'\n", DxlDSErrorToString(dxerror));
}


/*	--------------------------------------------------------------------------------
	Function : dxlPlaySampleInstance
	Purpose : create and play a sample instance
	Parameters : sample, loop it?
	Returns : instance ptr or NULL
	Info :
*/

DXLSampleInstType *DxlPlaySampleInstance(DXLSampleType *sptr, int loop)
{
	DXLSampleInstType	*inst, *next, *ninst;
	DWORD				status;
	int					first;

	if (sptr->instances != NULL)
		{
		first = 1;
		inst = sptr->instances;
		while ((inst != sptr->instances) || (first))
			{
			first = 0;
			next = inst->next;
			inst->buffer->GetStatus(&status);
			if (!(status & DSBSTATUS_PLAYING))
				{
				DxlStopSampleInstance(inst);
				if (sptr->instances == NULL)
					break;
				}
			inst = next;
			}
		}

	// Play master buffer if possible
	sptr->buffer->GetStatus(&status);
	if (!(status & DSBSTATUS_PLAYING))
		{
		dxerror = sptr->buffer->Play(0, 0, (loop) ? DSBPLAY_LOOPING : 0);
		if (dxerror != DS_OK)
			dprintf("PlaySample: Play failure '%s'\n", DxlDSErrorToString(dxerror));
		return NULL;
		}		

	// Master not available, so create temporary instance
//	ninst = (DXLSampleInstType*)dxlMalloc(sizeof(DXLSampleInstType), "Sample instance");
	ninst = (DXLSampleInstType*)MyMalloc(sizeof(DXLSampleInstType));
	if (ninst == NULL)
		{
		dprintf("dxlPlaySampleInstance: Out of memory\n");
		return NULL;
		}

	dxerror = dsi.dsound->DuplicateSoundBuffer(sptr->buffer, &(ninst->buffer));
	if (dxerror != DS_OK)
		{
//		dxlFree(ninst);
		MyFree((void **)&ninst);
		ninst = 0;
		dprintf("dxlPlaySampleInstance: DuplicateSoundBuffer failure '%s'\n", DxlDSErrorToString(dxerror));
		return NULL;
		}

	if (sptr->instances == NULL)
		{
		sptr->instances = ninst;
		ninst->next = ninst;
		ninst->prev = ninst;
		}
	else
		{
		ninst->prev = sptr->instances->prev;
		ninst->prev->next = ninst;
		sptr->instances->prev = ninst;
		ninst->next = sptr->instances;
		}
	ninst->parent = sptr;

	dxerror = ninst->buffer->Play(0, 0, (loop) ? DSBPLAY_LOOPING : 0);
	if (dxerror != DS_OK)
		dprintf("dxlPlaySampleInstance: Play failure '%s'\n", DxlDSErrorToString(dxerror));

	return ninst;
}


/*	--------------------------------------------------------------------------------
	Function : dxlStopSampleInstance
	Purpose : stop/delete a sample instance
	Parameters : instance ptr
	Returns : 
	Info :
*/

void DxlStopSampleInstance(DXLSampleInstType *inst)
{
	DWORD	status;

	inst->buffer->GetStatus(&status);
	if (status & DSBSTATUS_PLAYING)
		{
		dxerror = inst->buffer->Stop();
		if (dxerror != DS_OK)
			dprintf("StopSample: Stop failure '%s'\n", DxlDSErrorToString(dxerror));
		}

	inst->prev->next = inst->next;
	inst->next->prev = inst->prev;
	if ((inst->parent->instances == inst) && (inst->next == inst))
		inst->parent->instances = NULL;
	else if (inst->parent->instances == inst)
		inst->parent->instances = inst->next;
	inst->buffer->Release();
//	dxlFree(inst);
	MyFree((void**)&inst);
	inst = 0;
}


/*	--------------------------------------------------------------------------------
	Function : dxlSetSampleFrequency
	Purpose : set's the frequency a sound is played at
	Parameters : sound, frequency (in Hz)
	Returns : 
	Info :
*/

void DxlSetSampleFrequency(DXLSampleType *sptr, int freq)
{
	dxerror = sptr->buffer->SetFrequency(freq);
	if (dxerror != DS_OK)
		dprintf("SetSampleFrequency: SetFrequency failure '%s'\n", DxlDSErrorToString(dxerror));
}


/*	--------------------------------------------------------------------------------
	Function : dxlSetSampleInstanceFrequency
	Purpose : set's the frequency a sample instance is played at
	Parameters : instance, frequency (in Hz)
	Returns : 
	Info :
*/

void DxlSetSampleInstanceFrequency(DXLSampleInstType *inst, int freq)
{
	dxerror = inst->buffer->SetFrequency(freq);
	if (dxerror != DS_OK)
		dprintf("SetSampleFrequency: SetFrequency failure '%s'\n", DxlDSErrorToString(dxerror));
}


/*	--------------------------------------------------------------------------------
	Function : dxlSetSampleVolume
	Purpose : set's the volume a sound is played at
	Parameters : sound, volume (in dB, 0 = normal, -10 = 1/2, -20 = 1/4)
	Returns : 
	Info :
*/

void DxlSetSampleVolume(DXLSampleType *sptr, int vol)
{
	dxerror = sptr->buffer->SetVolume(vol);
	if (dxerror != DS_OK)
		dprintf("SetSampleVolume: SetVolume failure '%s'\n", DxlDSErrorToString(dxerror));
}


/*	--------------------------------------------------------------------------------
	Function : dxlSetSampleInstanceVolume
	Purpose : set's the volume a sample instance is played at
	Parameters : instance, volume (in dB, 0 = normal, -10 = 1/2, -20 = 1/4)
	Returns : 
	Info :
*/

void DxlSetSampleInstanceVolume(DXLSampleInstType *sptr, int vol)
{
	dxerror = sptr->buffer->SetVolume(vol);
	if (dxerror != DS_OK)
		dprintf("SetSampleVolume: SetVolume failure '%s'\n", DxlDSErrorToString(dxerror));
}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void InitSound()
{
	char	temp[512];
	int		j;


	for(j = 0;j < NUMSOUNDS;j++)
		sounds[j].buffer = NULL;

	if(detailFlags & DF_NOSOUND)
		soundOK = NO;
	else soundOK = DxlInitSound(g_hWnd);

	if(soundOK == NO)
		return;

	sprintf(temp,"%ssound\\whack.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_HANDWHACK]);
	sprintf(temp,"%ssound\\handland.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_HANDLAND]);
	sprintf(temp,"%ssound\\jump1.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_HANDJUMP]);
	sprintf(temp,"%ssound\\bounce.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_HANDBOUNCE]);
	sprintf(temp,"%ssound\\h2odeath.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_HANDDROWN]);

	sprintf(temp,"%ssound\\plat2.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_SPIKESWITCH]);
	sprintf(temp,"%ssound\\switch02.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_SWITCH]);
	sprintf(temp,"%ssound\\pain2.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_HEAVYLAND]);
	sprintf(temp,"%ssound\\medplat1.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_GATEOPEN]);
	sprintf(temp,"%ssound\\ax1.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_SPIKE]);
	sprintf(temp,"%ssound\\splash.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_SPLASH1]);
	sprintf(temp,"%ssound\\snappy.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_SNAPPY]);


/*	sprintf(temp,"%ssound\\splish.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_SPLASH2]);
*/
	sprintf(temp,"%ssound\\lobster.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_LOBSTER]);
	sprintf(temp,"%ssound\\chest.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_CHEST]);
	sprintf(temp,"%ssound\\cutlass.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_CUTLASS]);

	sprintf(temp,"%ssound\\poolball.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_BIGBALL]);

	sprintf(temp,"%ssound\\swish.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_SWISH]);
	sprintf(temp,"%ssound\\motor.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_MOTOR]);
	sprintf(temp,"%ssound\\lobdeath.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_SQUASH]);

	sprintf(temp,"%ssound\\teleport.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_TELEPORT]);
	sprintf(temp,"%ssound\\burst.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_BALLBURST]);

	sprintf(temp,"%ssound\\timegate.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_TIMEGATE]);
	sprintf(temp,"%ssound\\collect.wav",rootPath);
	DxlLoadSample(temp,&sounds[SFX_COLLECTABLE]);

}

/*	--------------------------------------------------------------------------------
	Function 	: 
	Purpose 	: 
	Parameters 	: 
	Returns 	: 
	Info 		:
*/
void FreeSamples()
{
	int	j;

	for(j = 0;j < NUMSOUNDS;j++)
		DxlDeleteSample(&sounds[j]);
}



void PlaySample(int sampleNum, BOOL loop, VECTOR *source, int volume)
{
	double realVolume = 0;
	VECTOR tempVect;
	double temp;
	DXLSampleInstType *sInst;

	if(soundOK == 0)
		return;

	if(source)
	{
		SubVector(&tempVect, &camera.pos, source);
		temp = Magnitude(&tempVect) * 20;
		realVolume = 0 - temp;

	}
	if(realVolume > -2000)
	{
		realVolume += 2000;
		realVolume *= volume/100;
		realVolume -= 2000;

		sInst = DxlPlaySampleInstance(&sounds[sampleNum],loop);
		if(sInst)
			DxlSetSampleInstanceVolume(sInst, (int)realVolume);
		else
			DxlSetSampleVolume(&sounds[sampleNum], (int)realVolume);
	}

}

