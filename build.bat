@echo off
setlocal enableextensions enabledelayedexpansion

set PROJECT_NAME=oddballs
set VERSION=970311
set TYPE=Win32

set RSC=rc.exe
set MTL=mktyplib.exe
set CPP=cl.exe
set BSC32=bscmake.exe
set LINK32=link.exe
set OUT_DIR=./build/!VERSION!

if "!VERSION!"=="960912" (
	set SRC_FILES=camera collision d3dapp d3dcalls ddcalls Debug input lclib main misc rmfull rmstats scene texture
) else (
	set SRC_FILES=actors anim bullets cam collectables collision Debug directx enemies error files graphics hand input main maths memory nmebehaviour personal platforms puzzles rm rmstats scene sound speccase spikes timegates wind winmain
)

if !TYPE!=="Release" (
    set OUT_DIR=!OUT_DIR!/Release
	set CPP_FLAGS=/nologo /ML /W3 /GX /O2
	set DEBUG_TYPE=NDEBUG
	if !VERSION!=="970311" (
		set SRC_FILES=!SRC_FILES! rmfull
	)
	set LINK_RELEASETYPE_ARGS=/incremental:no
) else if !TYPE!=="Debug" (
	set OUT_DIR=!OUT_DIR!/Debug
	set CPP_FLAGS=/nologo /MLd /Gm /GX /Zi /Od
	set DEBUG_TYPE=_DEBUG
	set LINK_RELEASETYPE_ARGS=/incremental:yes /debug
) else (
:: "Remote Debug" by default
    set OUT_DIR=!OUT_DIR!/DebugRemote
	set CPP_FLAGS=/nologo /MLd /Gm /GX /Zi /Od /Ob1
	set DEBUG_TYPE=_DEBUG
	set LINK_RELEASETYPE_ARGS=/incremental:yes /debug
)

if not exist "%OUT_DIR%/obj/" mkdir "%OUT_DIR%/obj"
if not exist "%OUT_DIR%/sbr/" mkdir "%OUT_DIR%/sbr"

for %%f in (%SRC_FILES%) do (
	set "OBJ_FILES=!OBJ_FILES! %OUT_DIR%/obj/%%f.obj"
	set "SBR_FILES=!SBR_FILES! %OUT_DIR%/sbr/%%f.sbr"
	if exist ./source!VERSION!/%%f.cpp (
		set SRC_EXTENSION=.cpp
	) else (
		set SRC_EXTENSION=.c
	)
	%CPP% %CPP_FLAGS% /I "./source!VERSION!/" /I "./dxsdk/sdk/inc" /D "WIN32" /D "_FIXES" /D "%DEBUG_TYPE%" /D "_WINDOWS" /FR"%OUT_DIR%/sbr/" /Fp"%OUT_DIR%/%PROJECT_NAME%.pch" /YX /Fo"%OUT_DIR%/obj/" /Fd"%OUT_DIR%/obj/" /c ./source!VERSION!/%%f!SRC_EXTENSION!
)

echo Compile resource file...
%RSC% /l 0x809 /fo"%OUT_DIR%/rmfull.res" /d "%DEBUG_TYPE%" "./source!VERSION!/rmfull.rc"
set "OBJ_FILES=!OBJ_FILES! %OUT_DIR%/rmfull.res"

echo Linking object files...
%LINK32% ./dxsdk/sdk/lib/*.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib /nologo /subsystem:windows /pdb:"%OUT_DIR%/%PROJECT_NAME%.pdb" %LINK_RELEASETYPE_ARGS% /machine:I386 /out:"%OUT_DIR%/%PROJECT_NAME%.exe" %OBJ_FILES%
echo Creating bsc...
%BSC32% /nologo /o"%OUT_DIR%/%PROJECT_NAME%.bsc" %SBR_FILES%
echo Done!