#ifndef __CAMERA_H
#define __CAMERA_H



typedef struct
{
	double	rotation;
	double	distanceFromHand;
}CAMERA_INFO_TYPE;


extern CAMERA_INFO_TYPE cameraInfo;





void MoveCamera();

#endif