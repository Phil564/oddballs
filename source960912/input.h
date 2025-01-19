#ifndef __INPUT_H
#define __INPUT_H


typedef struct
{
	double	xPos, yPos, xOrig, yOrig;
	char	button[2];
}joystickType;





extern JOYINFOEX	joystickinfo;
extern joystickType joystick;


void ProcessKeys(WPARAM key);
BOOL InitJoystick();
void ReadJoystick();




#endif