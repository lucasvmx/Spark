@echo off

rem cmake and git must be in %PATH% 
rem You should run this script from Qt Terminal environment

set configuration=%1
if "%configuration%" == "" set configuration=release

echo building configuration: %configuration%
rem download autorevision script
set START_FOLDER=%cd%
del /S /Q BUILD\spark\%configuration%
if not exist autorevision.sh (
	curl "https://raw.githubusercontent.com/Autorevision/autorevision/master/autorevision.sh" -o autorevision.sh
) else (
	echo autorevision script already exists
)

sh -c "./autorevision.sh -t c > 3rdparty/libhack/autorevision.h"

rem generate makefiles with CMAKE

cmake -G "MinGW Makefiles" -S "3rdparty/libhack" -B BUILD/libhack
mkdir "BUILD\spark"

rem build libhack
mingw32-make -C BUILD/libhack

rem Move binary files to another path
move /y %cd%\BUILD\libhack\libhack.dll.a %cd%\BUILD\libhack.a
move /y %cd%\BUILD\libhack\libhack.dll %cd%\BUILD\libhack.dll

rem generate build files for Qt
qmake "config+=%configuration%" -o BUILD\spark qt\Spark.pro

rem build spark
mingw32-make -C BUILD\spark

rem cleanup object files
del /q BUILD\spark\%configuration%\*.o
del /q BUILD\spark\%configuration%\*.cpp BUILD\spark\%configuration%\*.h

