/*==========================================================================
 *
 *  Copyright (C) 1995, 1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File: winmain.cpp
 *
 ***************************************************************************/

// Defines....
#define CLASS_NAME                              "Whack_n_Roll_Class"

// Includes....
#include "oddballs.h"
#include "windows.h"
#include "resource.h"
#include "winmain.h"
#include "scene.h"
#include "directx.h"
#include "rm.h"
#include "personal.h"
//#include "control.h"
#include "sound.h"
#include "anim.h"
#include "input.h"

//#include "midi.h"

// Globals....
HINSTANCE       g_hInst = NULL;
HWND            g_hWnd  = NULL;

SIZE			szClient; //Size of window
BOOL            g_bActive       = FALSE;
BOOL            g_bFirstActive  = TRUE;
BOOL            g_bErrorOccured = FALSE;
char            g_sError[2048];
BOOL            g_bShowStats    = FALSE;
BOOL            g_bMusicPaused  = FALSE;
BOOL            g_bSoundPaused  = FALSE;
HRESULT			dxerror;

// Externals....
extern DWORD    g_dwCurrMode; // Defined in DIRECTX.CPP

//------------------------------------------------------------------
// 
// Function     : RegError()
//
// Purpose      : Registers an error
//
//------------------------------------------------------------------

void RegError(char *sErr)
{
    sprintf(g_sError, "%s\n", sErr);
	dprintf(g_sError);
//    g_bErrorOccured = TRUE;
}


//------------------------------------------------------------------
// 
// Function     : InitClass()
//
// Purpose      : Initialises and registers window class
//
//------------------------------------------------------------------

BOOL InitClass(HINSTANCE hInst)
{
    WNDCLASS    wndClass;

    // Fill out WNDCLASS info
    wndClass.style              = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc        = WndProc;
    wndClass.cbClsExtra         = 0;
    wndClass.cbWndExtra         = 0;
    wndClass.hInstance          = hInst;
    wndClass.hIcon              = LoadIcon(hInst, "ROCKEM3D");
    wndClass.hCursor            = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground      = GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName       = NULL;
    wndClass.lpszClassName      = CLASS_NAME;
    
    if (!RegisterClass(&wndClass)) return FALSE;

    // Everything's perfect
    return TRUE;
}

//------------------------------------------------------------------
// 
// Function     : InitWindow()
//
// Purpose      : Initialises and creates the main window
//
//------------------------------------------------------------------

BOOL InitWindow(HINSTANCE hInst, int nCmdShow)
{
    // Create a window
    g_hWnd = CreateWindowEx(WS_EX_APPWINDOW,
                            CLASS_NAME, 
                            "Whack 'N Roll Demo",
                            WS_POPUP | WS_SYSMENU,
                            0, 0,
                            GetSystemMetrics(SM_CXSCREEN),
                            GetSystemMetrics(SM_CYSCREEN),
                            NULL,
                            NULL,
                            hInst,
                            NULL);

    // Return false if window creation failed
    if (!g_hWnd) return FALSE;
    
    // Show the window
    ShowWindow(g_hWnd, SW_SHOWNORMAL);

    // Update the window
    UpdateWindow(g_hWnd);
    
    // Everything's perfect
    return TRUE;
}

//------------------------------------------------------------------
// 
// Function     : WndProc()
//
// Purpose      : Windows procedure to handle messages
//
//------------------------------------------------------------------

long FAR PASCAL WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
        // D3DVECTOR used for 3D position of sound
        static D3DVECTOR d3dvPos= {D3DVAL(0), D3DVAL(0), D3DVAL(0)};
        
    // Handle messages
    switch (message)
    {
		case MM_MCINOTIFY:
			switch((WORD)wParam)
			{
				case MCI_NOTIFY_SUCCESSFUL:
					playCDTrack(g_hWnd,2);
					break;
			}
			break;

        case WM_KEYDOWN:
        {

			ProcessKeys(wParam);
			ProcessPersonalKeys(wParam);

            switch (wParam)
            {
                case VK_ESCAPE:
                {                                                       
                    // Time to quit....
                    PostMessage(g_hWnd, WM_CLOSE ,0 ,0);
                }
                break;

				case VK_SHIFT:
				keyboardShift = TRUE;

				break;
            }
        }
        break;
        case WM_KEYUP:
        {
            switch (wParam)
            {
				case VK_SHIFT:
				keyboardShift = FALSE;
				break;
            }
        }
        break;


        case WM_SYSCOMMAND:
        {
            switch (wParam)
            {
                // Trap ALT so it doesn't pause the app
                case SC_KEYMENU :
                {
                    return 0;
                }
                break;
            }
        }       
        
        case WM_ACTIVATEAPP:
        {
            // Determine whether app is being activated or not
            g_bActive = (BOOL)wParam ? TRUE : FALSE;

            if (g_bActive)
            {
                while (ShowCursor(FALSE) > 0) { };
//                if (!g_bMusicPaused) ResumeMidi();
            }
            else
            {
                ShowCursor(TRUE);
//                PauseMidi();
            }
        }
        break;
        
        case WM_CLOSE:
        {                       
//			FreeSamples();
//			DxlShutdownSound();
			StopCDTrack();
			CloseCDDevice();
            DestroyWindow(g_hWnd);
        }
        break;

        case WM_DESTROY:
        {                       
            // Stop midi music
//            StopMidi();
//			FreeSamples();
//			DxlShutdownSound();
			StopCDTrack();
			CloseCDDevice();

            // Destroy scene
            TermScene();
            
            // Terminate all the DirectX objects, surfaces devices etc
            TermDirectX();

            // Show the mouse
            ShowCursor(TRUE);

            // If an error occured, show it
            if (g_bErrorOccured)
            {
                MessageBeep(0);
                MessageBox(NULL, g_sError, "Error!", MB_OK);
            }
                    
            // Time to leave this mortal coil
            PostQuitMessage(0);
        }
        break;
        
        case MCI_NOTIFY:
        {
            if (wParam == MCI_NOTIFY_SUCCESSFUL)
            {
//                ReplayMidi();
            }
        }
        break;
    }
    
    return DefWindowProc(hWnd, message, wParam, lParam);
}

//------------------------------------------------------------------
// 
// Function     : WinMain()
//
// Purpose      : Entry point to application
//
//------------------------------------------------------------------

int FAR PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;
	HRESULT	rval;
    // Set global handle
    g_hInst = hInst;    

    // Initialise window class
    if (!InitClass(hInst)) return 1;

    // Initialise window
    if (!InitWindow(hInst, nCmdShow)) return 1;

    // Initialise DirectX objects (Termination is handled in WM_DESTROY)
    if (!InitDirectX())
    {
            DestroyWindow(g_hWnd);
            return 1;
    }

    // Show the splash screen
    DoSplashScreen("titlescreen\\titlescreen.bmp");

    // Load the scene
    TRY_D3DRM(g_lpD3DRM->CreateFrame(NULL, &g_lpWorld));
    TRY_D3DRM(g_lpD3DRM->CreateFrame(g_lpWorld, &g_lpBackground));
    TRY_D3DRM(g_lpD3DRM->CreateFrame(g_lpWorld, &g_lpScene));

	ResetCamera();
	InitActor(&camera);

    if ((rval = g_lpD3DRM->CreateFrame(g_lpScene,&(camera.frame))) != D3DRM_OK) 
	{
        Msg("Failed to create the camera's frame.\n%s", D3DAppErrorToString(rval));
        return FALSE;
    }
    if (!BuildScene(g_lpD3DRMDevice,g_lpD3DRMViewport,g_lpScene,camera.frame)) 
    {
            DestroyWindow(g_hWnd);
            return 1;
    }

    // Load accelerators
    HACCEL hAccel = LoadAccelerators(hInst, "APPACCEL");

    // Release the splash screen
	InitSound();
    ReleaseSplashScreen();
	InitJoystick();
	InitTimerList();
    // Set DirectDraw exclusive mode here so that the splash could stay
    // up during initialisation if we are using a different DirectDraw device
    // that could not support 640x480x8 for hardware rendering.
    if (!SetDirectDrawExclusiveMode())
    {
            RegError("Could not set exclusive mode!");
            return FALSE;
    }

    // Hide the mouse
    ShowCursor(FALSE);

    // Enter video mode set in g_dwCurMode
    if (!EnterVideoMode(g_dwCurrMode))
    {
            DestroyWindow(g_hWnd);
            return 1;
    }

    // Start the music!
//    PlayMidi("RockEm3D.mid");

    // Pump messages via a PeekMessage loop
	if(detailFlags & DF_CDAUDIO)
	{
		OpenCDDevice(g_hWnd);
		playCDTrack(g_hWnd,2);
		musicPlaying = YES;
	}

	
	dprintf("start\n");
    while (TRUE)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {                                                           
                PostQuitMessage(msg.wParam);
                return 1;
            }

            if (hAccel && (msg.hwnd == g_hWnd))
            {
                TranslateAccelerator(g_hWnd, hAccel, &msg);
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Perform any neccessary updating during idle time
        if (g_bActive)
        {
            // Update everything
  //          CheckInputAndUpdate();

			MainLoop();
            
            // Render the current scene
			StartTiming(TIMER__RENDER_LOOP);
            if (!RenderScene())
            {
                DestroyWindow(g_hWnd);
                return 1;
            }
			EndTiming(TIMER__RENDER_LOOP);
        }
    }

    // Exit WinMain and terminate the app....
    return msg.wParam;
}

