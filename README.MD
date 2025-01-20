# oddballs
Late 96' & Early 97' source backups of Glover made easier to compile.

## Changes
- the NMAKE build system has been migrated to a custom build batch file
- a majority of the files have been renamed to lowercase
- a majority of the file names that were cut by DOS were restored
- (_FIXES define) a "debug.log" file is now created next to the executable during gameplay.
- (_FIXES define) VERSIONINFO is added to the executable
- (_FIXES define, 970311 only) set rootPath in personal.cpp to `{"..\\..\\..\\"}`

## Setup
1. Please place the repository close to the root of the drive in the "Pc" folder (example: E:\Pc\oddballs) if you don't plan on building with fixes.
2. For Windows 7 and below, install Microsoft Visual C++ 4.0 (iso can be easily found, same as cd key. i mounted it with poweriso, keys are easy to find for that as well). Later systems must extract the MSDEV from the disc and manually set/add the **user** environment variables at the end of this section to their Windows install.
3. Get the DirectX 3.0 SDK. Place the "SDK" folder in a "dxsdk" folder in the root of the repository (example: E:\Pc\oddballs\dxsdk\SDK)
```
PATH: [msdev folder location]\bin
include: [msdev folder location]\include;[msdev folder location]\mfc\include;%include%
lib: [msdev folder location]\lib;[msdev folder location]\mfc\lib;%lib%
MsDevDir: [msdev folder location]
```

## Building
Run build.bat, the build should be stored at [repository root]/build/[VERSION]/[TYPE]/oddballs.exe

You can also set the building variables like this: `build.bat [version] [build type] [enable fixes]`

## Playing
The build from Glover1and2BackupData.7z/FromOldBackups.zip/SOURCE/RELEASE should also work using the following instructions.
1. If it's not detected, place the [D3DRM dll from dgVoodoo2](https://dege.freeweb.hu/dgVoodoo2/bin/D3DRM.zip) next to the game executable.
2. Connect a controller. Xbox One & PS4 controllers are confirmed to work.
3. (970311 only) Install [dxWnd](https://sourceforge.net/projects/dxwnd/) to run the game in a window. Please read that tool's instructions if you need any help setting it up.
4. Open the built executable/load it in dxWnd.
5. Enjoy the game. (970311 only) It is recommended to change the level path in personal.cpp (level1/dockwoody and dock/dock are confirmed to work).

## file paths in the leak
graphics: Glover1and2BackupData.7z/FromOldBackups.zip/GRAPHICS
sound: Glover1and2BackupData.7z/FromOldBackups.zip/SOUND
source960912: Glover1and2BackupData.7z/FromOldBackups.zip/BACKUP/CODE.ZIP
source970311: Glover1and2BackupData.7z/FromOldBackups.zip/SOURCE