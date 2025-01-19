#ifndef __SPIKES_H
#define __SPIKES_H

//structures--------------------------
typedef struct
{
	LPDIRECT3DRMFRAME	frame;
	VECTOR				actualPos;
	VECTOR				pos[4];
	int					posDelay[4];
	int					numPositions;
	int					currentPosition;
	int					counter;
	double				speed[4];
	int					tag;
	struct TAGSPIKELISTENTRY  	*listPointer;
}SPIKE;

typedef struct TAGSPIKELISTENTRY
{
	TAGSPIKELISTENTRY	*next,*prev;
	SPIKE				*spike;
}SPIKELISTENTRY;

typedef struct
{
	int					numEntries;
	SPIKELISTENTRY		head;
}SPIKELIST;


//externs-------------------------------
extern SPIKELIST spikeList;
extern LPDIRECT3DRMTEXTURE spikeTexture;



//definitions---------------------------
void InitSpikes();
void InitSpikeList();
void TranslateSpikes();
void AddSpike(SPIKE *spike);
void UpdateSpikes();
void SubSpike(SPIKE *spike);
SPIKELISTENTRY *FindTaggedSpike(SPIKELISTENTRY *startSpike, int tag);
void InitSpikePuzzle();




#endif