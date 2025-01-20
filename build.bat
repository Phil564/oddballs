@echo off
setlocal enableextensions enabledelayedexpansion

:: set arguments
set PROJECT_NAME=oddballs
if "%1"=="" (
	set /p VERSION="[%PROJECT_NAME%] Version? (960912 or 970311): "
) else (
	set VERSION=%1
)
if "%2"=="" (
	set /p BUILD_TYPE="[%PROJECT_NAME%] Build Type? (Release, Debug or RemoteDebug): "
) else (
	set BUILD_TYPE=%2
)
if "%3"=="" (
	set /p FIXES_ENABLED="[%PROJECT_NAME%] Enable Fixes? (On or Off): "
) else (
	set FIXES_ENABLED=%3
)
set OUT_DIR=./build/!VERSION!/!BUILD_TYPE!
set "ERROR=Done!"

:: define executable names
set RSC=rc.exe
set MTL=mktyplib.exe
set CPP=cl.exe
set BSC32=bscmake.exe
set LINK32=link.exe

:: process Version argument
if "!VERSION!"=="960912" (
	set SRC_FILES=camera collision d3dapp d3dcalls ddcalls Debug input lclib main misc rmfull rmstats scene texture
) else if "!VERSION!"=="970311" (
	set SRC_FILES=actors anim bullets cam collectables collision Debug directx enemies error files graphics hand input main maths memory nmebehaviour personal platforms puzzles rm rmstats scene sound speccase spikes timegates wind winmain
) else (
	set "ERROR=Version (first argument) must be set to either 960912 or 970311"
	goto end
)

:: process Build Type argument
if "!BUILD_TYPE!"=="Release" (
	set CPP_FLAGS=/nologo /ML /W3 /GX /O2
	set DEBUG_TYPE=NDEBUG
::	if "!VERSION!"=="970311" (
::		set SRC_FILES=!SRC_FILES! rmfull
::	)
	set LINK_RELEASETYPE_ARGS=/incremental:no
) else if "!BUILD_TYPE!"=="Debug" (
	set CPP_FLAGS=/nologo /MLd /Gm /GX /Zi /Od
	set DEBUG_TYPE=_DEBUG
	set LINK_RELEASETYPE_ARGS=/incremental:yes /debug
) else if "!BUILD_TYPE!"=="RemoteDebug" (
	set CPP_FLAGS=/nologo /MLd /Gm /GX /Zi /Od /Ob1
	set DEBUG_TYPE=_DEBUG
	set LINK_RELEASETYPE_ARGS=/incremental:yes /debug
) else (
	set "ERROR=Build Type (second argument) must be set to Release, Debug or RemoteDebug"
	goto end
)

:: process fix boolean argument
if "!FIXES_ENABLED!"=="On" (
	set FIX_DEFINE=/D "_FIXES"
) else if not "!FIXES_ENABLED!"=="Off" (
	set "ERROR=Fixes (third argument) must be set to On or Off"
	goto end
)

:: create directories
if not exist "%OUT_DIR%/obj/" mkdir "%OUT_DIR%/obj"
if not exist "%OUT_DIR%/sbr/" mkdir "%OUT_DIR%/sbr"

echo ------- %PROJECT_NAME% -------
echo Version:     !VERSION!
echo Build Type:  !BUILD_TYPE!
echo Fixes:       !FIXES_ENABLED!
echo ------------------------

echo [%PROJECT_NAME%] Compiling CPP and C files...
for %%f in (%SRC_FILES%) do (
	set "OBJ_FILES=!OBJ_FILES! %OUT_DIR%/obj/%%f.obj"
	set "SBR_FILES=!SBR_FILES! %OUT_DIR%/sbr/%%f.sbr"
	if exist ./source!VERSION!/%%f.cpp (
		set SRC_EXTENSION=.cpp
	) else (
		set SRC_EXTENSION=.c
	)
	%CPP% %CPP_FLAGS% /I "./source!VERSION!/" /I "./dxsdk/sdk/inc" /D "WIN32" !FIX_DEFINE! /D "%DEBUG_TYPE%" /D "_WINDOWS" /FR"%OUT_DIR%/sbr/" /Fp"%OUT_DIR%/%PROJECT_NAME%.pch" /YX /Fo"%OUT_DIR%/obj/" /Fd"%OUT_DIR%/obj/" /c ./source!VERSION!/%%f!SRC_EXTENSION!
	if not exist "%OUT_DIR%/obj/%%f.obj" (
		set "ERROR=An error occured while compiling %%f!SRC_EXTENSION!"
		goto end
	)
)

echo [%PROJECT_NAME%] Compiling resource file...
%RSC% /l 0x809 /fo"%OUT_DIR%/rmfull.res" /d "%DEBUG_TYPE%" !FIX_DEFINE! "./source!VERSION!/rmfull.rc"
if not exist "%OUT_DIR%/rmfull.res" (
	set "ERROR=An error occured while compiling rmfull.res"
	goto end
)
set "OBJ_FILES=!OBJ_FILES! %OUT_DIR%/rmfull.res"

echo [%PROJECT_NAME%] Linking object files to create executable (%PROJECT_NAME%.exe)...
%LINK32% ./dxsdk/sdk/lib/*.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib /nologo /subsystem:windows /pdb:"%OUT_DIR%/%PROJECT_NAME%.pdb" %LINK_RELEASETYPE_ARGS% /machine:I386 /out:"%OUT_DIR%/%PROJECT_NAME%.exe" %OBJ_FILES%
if not exist "%OUT_DIR%/%PROJECT_NAME%.exe" (
	set "ERROR=An error occured while linking %PROJECT_NAME%.exe"
	goto end
)
echo [%PROJECT_NAME%] Generating Browse Information File (%PROJECT_NAME%.bsc)...
%BSC32% /nologo /o"%OUT_DIR%/%PROJECT_NAME%.bsc" %SBR_FILES%
if not exist "%OUT_DIR%/%PROJECT_NAME%.exe" (
	set "ERROR=An error occured while generating %PROJECT_NAME%.bsc"
	goto end
)
:end
echo !ERROR!
pause