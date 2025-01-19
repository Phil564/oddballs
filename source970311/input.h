#ifndef __INPUT_H
#define __INPUT_H


typedef struct
{
	double	xPos, yPos, xOrig, yOrig;
	double	magnitude;
	double	lastXPos, lastYPos;
	double	direction;
	char	button[6];
	char	lastButton[6];
	char	debounce[6];
	int		buttonStateTime[6];
	int		buttonLastStateTime[6];
	char	buttonState[6];
	char	buttonLastState[6];
	int		timeSincePress[6];
	short	numButtons;
}joystickType;


extern int JS_BUTTON_A;
extern int JS_BUTTON_B;
extern int JS_BUTTON_Z;


extern BOOL	keyboardShift;

extern JOYINFOEX	joystickinfo;
extern joystickType joystick;


void ProcessKeys(WPARAM key);
BOOL InitJoystick();
void ReadJoystick();
void ProcessInput();




#endif