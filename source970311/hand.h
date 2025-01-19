#ifndef __HAND_H
#define __HAND_H


#define DEFAULT_HAND_MAXSPEED	0.55
#define CRAWL_HAND_MAXSPEED		1.7
#define SPEEDUP_HAND_MAXSPEED	1
#define NORMALBALL_MAXSPEED		1.3
#define WALKBALL_MAXSPEED		0.4

#define BALL_NORMAL_RADIUS	(0.9)

enum
{
	HURT_GO_BACKWARDS,
	HURT_GO_TO_SAFE
};

//------------------------------------------------------
//hand animations
enum
{
	HANDANIM_TIPTOE,	
	HANDANIM_WALK,	
	HANDANIM_RUN,	
	HANDANIM_SKID,
	HANDANIM_STAND,		
	HANDANIM_ST2CUP,		
	HANDANIM_CUP2ST,		
	HANDANIM_WHACK1,		
	HANDANIM_WHACK2,		
	HANDANIM_ST2DUCK,	
	HANDANIM_DUCK2ST,	
	HANDANIM_ROLL,
	HANDANIM_DUCK2ROLL,
	HANDANIM_ROLL2DUCK,
	HANDANIM_JUMP,
	HANDANIM_LONGJUMP,
	HANDANIM_THROWAIM,
	HANDANIM_THROW,
	HANDANIM_BNCE2THROWAIM,
	HANDANIM_THROW2CUP,
	HANDANIM_THROW2LAND,
	HANDANIM_CUP2BOUNCE,
	HANDANIM_BOUNCE,
	HANDANIM_BOUNCE2,
	HANDANIM_BOUNCE3,
	HANDANIM_BOUNCE2CUP,
	HANDANIM_SLAM,
	HANDANIM_SLAM2ST,
	HANDANIM_PUNCH,
	HANDANIM_SLAP,
	HANDANIM_PUNCH2ST,
	HANDANIM_IDLE1,
	HANDANIM_IDLE2,
	HANDANIM_IDLE3,
	HANDANIM_SPECIAL1,
	HANDANIM_SPECIAL2,
	HANDANIM_STUN,
	HANDANIM_PAT,
	HANDANIM_CARTWHEEL,
	HANDANIM_CLING,
	HANDANIM_CLIMBDOWN,
	HANDANIM_CLIMBDOWNIDLE,
	HANDANIM_CLIMBUP,
	MAX_HANDANIMS

};

//------------------------------------------------------
//ball animations
enum
{
	BALLANIM_BOUNCE,	
	BALLANIM_BURST,	
	MAX_BALLANIMS

};


//------------------------------------------------------
//hand actions
enum
{
	HAND_IDLE,	  
	HAND_WALK,	  
	HAND_JUMP,
	HAND_JOINED,	  
	HAND_THROW,	  
	HAND_PREWHACK,	  
	HAND_WHACK,	  
	HAND_SKIDDING, 
	HAND_DUCK,
	HAND_RELEASE,
	HAND_GETUP,
	HAND_THROWAIM,
	HAND_THROW2CUP,
	HAND_FALLING,
	HAND_BOUNCE,
	HAND_BOUNCE2CUP,
	HAND_AIRBOURNEWHACK,
	HAND_THROW2LAND,
	HAND_SLAM,
	HAND_SLAM2ST,
	HAND_PUNCH,
	HAND_PUNCH2ST,
	HAND_SLAP,
	HAND_BALLWALK,
	HAND_CRAWL,
	HAND_CARTWHEEL,
	HAND_CLIMBUP,
	HAND_CLIMBDOWN,
	HAND_CLING,
	HAND_DROPDOWN,
	HAND_HURT,
	HAND_PUSH,
	MAX_HAND_ACTIONS
};



//------------------------------------------------------
//ball states
enum
{
	BALL_NORMAL,
	BALL_HELD,
	BALL_BURST,
	BALL_OVERRIDE_CONTROL,
	BALL_TELEPORTING,
};
//hand states

enum
{
	HAND_NORMAL,
	HAND_OVERRIDE_CONTROL,
	HAND_TELEPORTING,
	HAND_HELD,
};


enum
{
	HANDWALK_TIPTOE,
	HANDWALK_WALK,
	HANDWALK_RUN,
	HANDWALK_ONBALL
};

//------------------------------------------------------
//structures
typedef struct
{
	float		altitude;
	PLANE		*altPlane;
	short		newActionDelay;
	float		whackStrength;
	short		whackDelay;
	BOOL		bounceMe;
	BOOL		bounceAnimate;
	short		bounceIdleCount;
	short		actionLastFrame;
	short		currentActionTime;
	char		walkMode;
	char		lastWalkMode;
	short		status;
	BOOL		lastOnGround;
	short		inputDisabledTimer;
	short		idleTime;
	short		lastTimeGate;
	char		jumpStrength;
	char		releaseTimer;	
				
	short		lastRunTimer;
	short		fallCounter;
 	BOOL		longJump;
				
	ACTOR		verticalEdgeChecker;
	ACTOR		frontProbe[2];
	float		frontProbeAltitude[2];
				
	VECTOR		handForcedPosition;
	VECTOR		handAccel;
	short		pushCount;
	PLATFORM	*catPlat;
	PLATFORM	*sitPlat;
	VECTOR		safePos;
	PLATFORM	*safePlat;
				
	ACTOR		*holdingActor;
	VECTOR		holdingOffset;
}HAND_CONTROL_TYPE;


typedef struct
{
	int		status;
	int		ballType;
	int		timer;
	ACTOR	*holdingActor;
	VECTOR	holdingOffset;
//	int		numLives;
	double	altitude;
	BOOL	lastOnGround;
}BALL_CONTROL_TYPE;



#define HAND_ACCELERATION (0.3)
#define BALL_ACCELERATION 0.5
#define BALL_BOUNCE 0.8
#define BALL_MASS 10


//-------------------------externs
extern HAND_CONTROL_TYPE handControl;
extern BALL_CONTROL_TYPE ballControl;

//-------------------------declerations
void BurstBall();
void MoveBall();
void MoveHand();
BOOL IsHandHoldingBall();
void DropBall();
void DropHand();
void ApplyAction(int action);
void LoseLife();
double BallBounceMaxSpeed();
void DisableControl(short time);
BOOL HandOverstepsEdge(ACTOR *probe, PLANE *plane);
void MaintainAction();
void CheckHandCatapult();
void CheckHandPlatform();
void HurtHand(int type);


#endif