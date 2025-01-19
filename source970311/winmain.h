/*==========================================================================
 *
 *  Copyright (C) 1995, 1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File: winmain.h
 *
 ***************************************************************************/

#ifndef __WINMAIN_H
#define __WINMAIN_H

extern SIZE szClient;
extern HWND		g_hWnd;
extern HRESULT	dxerror;

void            RegError(char *sErr);

BOOL            InitClass(HINSTANCE hInst);
BOOL            InitWindow(HINSTANCE hInst, int nCmdShow);
long FAR PASCAL WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int FAR PASCAL  WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow);   

#endif

