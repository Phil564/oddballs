/*
 *	Project:		Oddballs
 *	File:			types.h
 *	Description:	type declarations
 */

#ifndef __TYPES_H
#define __TYPES_H

#define JOYSTICK_MAX 17000

#define INACTIVE	0
#define ACTIVE		1
#define DEAD		2

enum
{
	SUCTION,
	GAUNTLET,
	SPEEDUP,
	HEAVYBALL,
	LIGHTBALL,
	NUMPICKUPS
};

enum
{
	STONE,
	GRASS,
	ICE,
	MUD,
	SAND,
	SHALLOW_WATER,
	DEEP_WATER,
	NUM_TERRAIN_TYPES
};

typedef struct
{
	float	slidy;
	float	bouncy;
}TERRAIN_TYPE;

#define uchar	unsigned char
#define ushort	unsigned short
#define LONG	int
#define ULONG	unsigned int
#define SHORT	short
#define USHORT	unsigned short
#define UBYTE	unsigned char
#define MYBYTE	signed char

#define TRACK_DIRECTION	0
#define TRACK_VELOCITY	1
#define TRACK_POSITION	2

#define	X	0
#define	Y	1
#define	Z	2

#define INIT(VAR)			{ memset(&(VAR), 0, sizeof(VAR)); (VAR).dwSize = sizeof(VAR); }

#define MOD	%

#define GRAVITY			(1<<0) 
#define ANIMATING		(1<<1)
#define COLLISION		(1<<2)
#define ROLLING			(1<<3)
#define DECELERATE		(1<<4)
#define LO_GRAVITY		(1<<5) 
#define DESTROYED		(1<<6)
#define DIESONCONTACT	(1<<7)
#define DIESAFTERTIME	(1<<8)
#define HASFIRED		(1<<9)
#define HASFIRED2		(1<<10)
#define PROJECTILE		(1<<11)
#define SHADOW			(1<<12)

#define TILTING			(1<<13)
#define SEESAW			(1<<14)
#define BACKGROUND		(1<<15)
#define REVERSING		(1<<16)
#define TAKESPIN		(1<<17)
#define DUMMY			(1<<18)
#define DESTRUCTIBLE	(1<<19)
#define DESTROYER		(1<<20)
#define CONVEYOR		(1<<21)
#define MAGNETIC		(1<<22)
#define BLOWN_BY_WIND	(1<<23)
#define BEING_BLOWN		(1<<24)
#define CATAPULTING		(1<<25)
#define LOOK_AT_HAND	(1<<26)
#define LOOK_AT_BALL	(1<<27)
#define LOOK_AT_POINT	(1<<28)
#define GO_FORWARDS		(1<<29)
#define ELECTRICAL		(1<<30)

typedef struct {
    LPDIRECT3DRMANIMATIONSET	animset;
    D3DVALUE					time;
} animationCallbackArgs;

typedef struct {
	LPDIRECT3DRMANIMATIONSET	animationSet;
	short						numberOfFrames;
	double						speed;
} animation;


typedef struct
{
	double	v[3];
}VECTOR;

typedef struct
{
	MYBYTE	v[3];
}BYTEVECTOR;

/*
typedef struct
{
	int command;
	VECTOR param;
} instruction;

typedef struct
{
	int currentCommand;
	int numberOfCommands;
	instruction *commands;
} SCRIPT;
*/

typedef struct tagBMPHeaderType {				// BMP file header
	long 	filesize;
	short	reserved[2];
	long 	headersize;
	long 	infoSize;
	long 	width;
	long 	depth;
	short	biPlanes;
	short 	bits;
	long 	biCompression;
	long 	biSizeImage;
	long 	biXPelsPerMeter;
	long 	biYPelsPerMeter;
	long 	biClrUsed;
	long 	biClrImportant;
} BMPHeaderType;

/*
typedef struct
{
	short	action;
	short	countto;
//	int		count;
	double	val;
	double	val2;
	int		flags;
	VECTOR	pos;
	double	genCounter;
}NME_INSTRUCTION;


typedef struct
{
	short	currentCommand;
//	int 	commandCount;
//	int		commandCountTo;
//	int		numberOfCommands;
	NME_INSTRUCTION *instruction;
	NME_INSTRUCTION currentInstruction;
}NME_SCRIPT;
*/
typedef struct
{
	float	x;
	float	z;
}POINT2D;


typedef struct
{
	VECTOR	pos;
	VECTOR	size;
}BOX;

typedef struct
{
	VECTOR	normal;
	double	J;
	UBYTE	i[2];
	DWORD	numVertices;
	VECTOR	vertex[4];
	double	u1,u2;
	double	v1,v2;
	int		index;
	UBYTE	terrain;
}PLANE;

typedef struct TAGPLANELISTENTRY
{
	TAGPLANELISTENTRY	*next,*prev;
	PLANE				*plane;
}PLANELISTENTRY;

typedef struct 
{
	PLANELISTENTRY	head;
	int				numEntries;
}PLANELIST;




//-------------------------------------------------------
// brand new enemy controller stuff
#define MAX_NME_INSTRUCTIONS (30)

union universalType
{
		int		i;
		float	f;
		VECTOR	v;
		BOX		b;
};

typedef struct
{
	char	type;
	universalType val1;
	universalType val2;
}NME_CONDITION;

typedef struct
{
	char	type;
	short	time;
	universalType val1;
	universalType val2;
	int		flags;
	NME_CONDITION	condition;

}NME_INSTRUCTION;

typedef struct
{
	NME_INSTRUCTION		currentInstruction;
	BOOL				hasBeenOverridden;

	NME_INSTRUCTION		*instructionList[MAX_NME_INSTRUCTIONS];
	NME_INSTRUCTION		*conditionalList[MAX_NME_INSTRUCTIONS];
	NME_INSTRUCTION		*attackList[MAX_NME_INSTRUCTIONS];

	short				blockAttack;
	char				numNormalInstructions;
	char				numConditionalInstructions;
	char				numAttackInstructions;
	NME_INSTRUCTION		**activeInstructionList;
	char				currentInstructionNum;
	char				lastNormalInstructionNum;
	char				instructionStack;
	BOOL				attacking;
	char				loopCounter;
	short				timer;
	short				currentInstructionTime;
	VECTOR				tempDest;
	int					tempInt;
	float				tempFloat;
	float				tempFloat2;

	short				disabledCount;
}NME_SCRIPT;

//-------------------------------------------------------

#define TELEPORT_BALL			(1<<0)
#define TELEPORT_HAND			(1<<1)
#define TELEPORT_NME			(1<<2)
#define TELEPORT_JOINED			(1<<3)
#define TELEPORT_ZEROVEL		(1<<4)
#define TELEPORT_GOTOPLATFORM	(1<<5)

typedef struct TAGTELEPORT
{
	VECTOR				destination;
	int					flags;
	BOOL				status;
	short				disappearDelay;
	short				appearDelay;
	struct TAGPLATFORM	*platform;
} TELEPORT;

typedef struct TAGFAN
{
	VECTOR	windDir;
	BOOL	status;
	double	windStrength;
	double	angle;
	double	spread;
	short	tag;
	double	offset;
}FAN;

typedef struct 
{
	VECTOR	activateOffset;
	float	strengthXZ;
	float	strengthY;
	BOOL	status;
//	short	duration;
	short	durationCounter;
	short	delayCounter;
	short	catCounter;
	float	rotSpeed;
	short	rotTime;
}CATAPULT;

#define MAXPLATFORMPATHPOINTS	6

#define TOPPLE_POS	(1<<0)
#define TOPPLE_NEG	(1<<1)

typedef struct TAGPLATFORM
{
	double		radius;
	double		initRadius;
	PLANELIST	planeList;
	int			flags;
	UBYTE		actionFlags;
	VECTOR		pos;
	VECTOR		oldpos;
	VECTOR		vel;
	double		speed;
	double		maxSpeed;
	double		accel;
	VECTOR		rot;
	VECTOR		oldrot;
	VECTOR		rotvel;
	VECTOR		rotaim;
	BYTEVECTOR	rAim;
	VECTOR		*pathPoint[MAXPLATFORMPATHPOINTS];
	short		waitTime[MAXPLATFORMPATHPOINTS];
	MYBYTE		direction;
	short		counter;
	UBYTE		numPathPoints;
	UBYTE		nextPoint;
	UBYTE		currentPoint;
	double		mass;
	struct TAGPLATFORM	*parentPlatform;

	double		orbitSpeed;
	BYTEVECTOR	orbitAxis;
	short		orbitPauseTime;
	UBYTE		orbitNumPauses;
	UBYTE		orbitNextPoint;
	short		orbitCounter;
	double		orbitFlipFlop;
	double		initOrbitRot;

	double		spinSpeed;
	BYTEVECTOR	spinAxis;
	UBYTE		spinSymmetry;
	short		spinPauseTime;
	UBYTE		spinNumPauses;
	UBYTE		spinNextPoint;
	short		spinCounter;
	double		spinFlipFlop;
	float		pendulumSwing;
	double		turnSpeed;
	int			noActorInContactCount;
	UBYTE		toppleFlags;
	double		toppleRot;
	double		maxRot;
	double		maxRotVel;
	double		initRot;
	double		bank;

	VECTOR		conveyorSpeed;

	BYTEVECTOR	pushDir;

	VECTOR		targetSpeed;
	double		spinToMove;
	double		spinToSpin;
	VECTOR		moveBySpeed;
	BOX			*confineBox;

	VECTOR		scale;
	VECTOR		initScale;
	VECTOR		stretchScale;
	short		stretchTime;
	short		stretchCounter;
	short		stretchWait;

	short		crumbleCounter;
	short		crumbleLimit;
	short		crumbleGravityDelay;
	double		crumbleGravityMultiplier;

	BOOL		electric;

	FAN			*fan;
	TELEPORT	*teleport;
	CATAPULT	*catapult;
	VECTOR		size;
	int			tag;
	int			targetTag;
	double		bounce;
	UBYTE		bounceDir[3];
	struct TAGPLATFORMLISTENTRY  	*listPointer;
	LPDIRECT3DRMFRAME frame;
	LPDIRECT3DRMFRAME parent;
	BOOL				visible;
	short		timeBonus;

	double		rotInertia;
	double		velInertia;
	int			lastCalcFrame;
	struct		TAGPLATFORMID *id;
}PLATFORM;

typedef struct TAGPLATFORMLISTENTRY
{
	TAGPLATFORMLISTENTRY	*next,*prev;
	PLATFORM				*platform;
}PLATFORMLISTENTRY;

typedef struct
{
	int					numEntries;
	PLATFORMLISTENTRY	head;
}PLATFORMLIST;

typedef struct TAGPLATFORMID
{
	struct TAGPLATFORMID	*next,*prev;
	char					name[256];
	LPDIRECT3DRMVISUAL		visual;
	int						faceCount;
	LPDIRECT3DRMFACEARRAY	faceArray;
}PLATFORMID;

typedef struct
{
	int		  	numEntries;
	PLATFORMID	head;
}PLATFORMIDLIST;

#define NUMACTORPLANES 5

typedef struct
{
	VECTOR	offset;
	double	radius;
	int		spinVal;
}COLLSPHERE;

typedef struct TAGACTOR
{
	LPDIRECT3DRMFRAME			parent;
	LPDIRECT3DRMFRAME			frame;
	LPDIRECT3DRMVISUAL			Shadow;
	LPDIRECT3DRMFRAME			shadowFrame;
	short						type;
	VECTOR						pos;
	VECTOR						oldpos;
	VECTOR						vel;
	VECTOR						oldVel;
	double						accel;
	double						speed;
	double						maxSpeed;
	double						maxAccelSpeed;
	double						stopSpeed;
	double						slideLimit;
	BOOL						onGround;
	BOOL						flying;
	VECTOR						rot;
	VECTOR						rotvel;
	VECTOR						rotaim;
	int							faceCount;
	LPDIRECT3DRMFACEARRAY		faceArray;
	double						velInertia;
	double						normInertia;
	double						rotInertia;
	double						bounce;
	BYTEVECTOR					rAim;
	double						turnSpeed;
	int							flags;
	struct TAGACTORLISTENTRY  	*listPointer;
	PLANE						*plane[NUMACTORPLANES];
	VECTOR						nearestPoint[NUMACTORPLANES];
	int							contactTime[NUMACTORPLANES];
	char						rolling[NUMACTORPLANES];
	double						radius;
	double						radiusAim;
	COLLSPHERE					collisionSphere[4];
	UBYTE						numExtraCollisionSpheres;
	VECTOR						centreOfGravity;
	double						maxRadius;
	VECTOR						scale;
	short						lifetime;
	TAGACTOR					*owner;
	short						health;
	short						name;
	short						damage;
	UBYTE						trackMode;
	short						status;
	PLATFORM					*platform;
	PLATFORM					*oldPlatform;
	PLATFORM					*touchPlatform;
	PLATFORM					*oldTouchPlatform;
	BOOL						touchingElectric;
	short						enemyNum;
	double						mass;

	short						currentAnimation;
	BOOL						reachedEndOfAnimation;
	BOOL						loopAnimation;
	double						speedAnimation;
	short						queueAnimation[5];
	BOOL						queueLoopAnimation[5];
	double						queueSpeedAnimation[5];
	UBYTE 						numberQueued;
	animation					*anims;
	animationCallbackArgs		*callBack;

	short						action;
	short						lastAction;
	short						teleportCounter;
	TELEPORT					*teleport;
	void						(*destroyCallBack)(TAGACTOR *);
	BOOL						(*maintainCallBack)(TAGACTOR *);
} ACTOR;

typedef struct TAGACTORLISTENTRY
{
	TAGACTORLISTENTRY	*next,*prev;
	ACTOR				*actor;
}ACTORLISTENTRY;

typedef struct
{
	ACTORLISTENTRY	head;
	int				numEntries;
}ACTORLIST;

typedef struct
{
//	BOX			box;
	PLANELIST	planeList;
}SPACE_PARTITION;

#define NUMLANDSCAPEOBJECTS	100

typedef struct
{
	int		numObjects;
	int		numPlanes;
	ACTOR	object[NUMLANDSCAPEOBJECTS];
	PLANE	*planes;
}LANDSCAPE;

typedef struct
{
	int score;
	int numLives;
	int timer;
	int pickupTimer[NUMPICKUPS];
	int	messageTimer;
}PLAYER_TYPE;

#define MAX_ENEMY_SPHERES 10

typedef struct
{
	LPDIRECT3DRMFRAME	frame;
	VECTOR				offset;
}SPHERE_TYPE;

typedef struct
{
	ACTOR		actor;
	NME_SCRIPT	script;
	BOOL		processScript;
	int			flags;
	short		tag;
	SPHERE_TYPE	*sphere[MAX_ENEMY_SPHERES];

	VECTOR		originalPosition;
	
	char		numGuns;
	char		firingType;
	BOOL		whichGun;
	float		bankMultiplier;

}ENEMY_TYPE;

#define Max(p1,p2)	(((p1) > (p2)) ? (p1) : (p2))
#define Min(p1,p2)	(((p1) > (p2)) ? (p2) : (p1))


typedef struct
{
	LARGE_INTEGER	count;
	int		time;
	char	procName[20];
	BOOL	active;
}TIMER_TYPE;


#endif