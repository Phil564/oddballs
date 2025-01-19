#ifndef __CAMERA_H
#define __CAMERA_H



//special case camera work

typedef struct TAGCAMERA_ACTION
{
	struct	TAGCAMERA_ACTION	*prev,*next;
	char			type;
	universalType	value1;
	universalType	value2;
}CAMERA_ACTION;

typedef struct
{
	CAMERA_ACTION	head;
	int		numEntries;
}CAMERA_ACTION_LIST;







typedef struct
{
	char			type;
	universalType	value1;
	universalType	value2;
}CAMERA_CONDITION;
/*
typedef struct
{
	char			type;
	universalType	value;
}CAMERA_ACTION;
*/

typedef struct TAGCAMERACASE
{
	struct TAGCAMERACASE	*prev,*next;
	CAMERA_CONDITION	condition;
	CAMERA_ACTION_LIST	actionList;
}CAMERACASE;

typedef struct
{
	CAMERACASE	head;
	int		numEntries;
}CAMERACASE_LIST;




//actions
enum
{
	CAM_HEIGHT,
	CAM_DISTANCE,
	CAM_PARALLEL,
	CAM_FIXEDPOS,
	CAM_FIXEDROT,
	CAM_CORNER,
	CAM_MAXACTIONS,
};
//conditions
enum
{
	CAM_HAND_CROSSES_LINE,
	CAM_HAND_IN_BOX,
	CAM_HAND_IN_RANGE,
	CAM_BALL_IN_BOX,
	CAM_BALL_IN_RANGE,
	CAM_CAM_IN_BOX,
	CAM_CAM_IN_RANGE,
	CAM_ALWAYS,
	CAM_NEVER,
};




typedef struct
{
	double				distanceFromHand;
 //	LPDIRECT3DRMFRAME	dummyFrame;
	ACTOR				garib;
	double				garibHeight;
	double				garibHeightAim;
	VECTOR				positionBias;
	char				type;
//	ACTOR				*actorToTrack;
	VECTOR				*positionToTrack;		//vector to look at
	VECTOR				*lastPositionToTrack;	//store normal position for override to restore
	VECTOR				*velToTrack;			//vector to look at
	VECTOR				*lastVelToTrack;		//store normal position for override to restore
	BOOL				lastPositionLocked;		//was the position locked before override
	VECTOR				lastLockedPosition;		//what was it
	int					overrideCounter;		//how long to override
	BOOL				positionLocked;			//is the cameras position locked
	VECTOR				lockedPosition;			//where to.
	VECTOR				lastPosition;
	VECTOR				lastTimeGatePos;
	VECTOR				garibLastTimeGatePos;

	BOOL				overrideRotation;
	float				overriddenRotation;
	float				overrideRotationSpeed;

	float				preferedRotation;
	float				additionalDistance;
	float				additionalHeight;

	BOOL				swingBehind;

	short				cornerCounter;
	float				cornerAngle;

	float				slowGaribSpeed;
	short				slowGaribTimer;
}CAMERA_INFO_TYPE;

//------------------------------------------------------------------
extern CAMERA_INFO_TYPE cameraInfo;
extern BOOL cameraFollow;
extern CAMERACASE_LIST cameraCaseList;





void MoveCam();
void SetCameraOverride(double, double, double, VECTOR *camTargetPos,VECTOR *camtargetVel, int timer);

void InitCameraCaseList();
void AddCameraCase(CAMERACASE *cameraCase);
CAMERACASE *NewCameraCase(FILE *);
void AddCameraAction(FILE *fp, CAMERACASE *cameraCase);

#endif