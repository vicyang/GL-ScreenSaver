@echo off

set path=%path%;../DLL
set NAME=CodeRain
set SRC=CodeRain.c outline.c loadtext.c
set INCDIR=-ID:\Lib\freeglut-MinGW-3.0.0-1.mp\include -ID:/lib/freetype-2.7/include -I./
set LIBDIR=-LD:\Lib\freeglut-MinGW-3.0.0-1.mp\lib\x64 -LD:/lib/freetype-2.7/objs/.libs
set LIB=-lfreeglut -lopengl32 -lglu32 -lfreetype -lpng -lz

:Compile
gcc -std=c11 %SRC% -o "%NAME%" %INCDIR% %LIBDIR% %LIB%

:Run
if %errorlevel% == 0 (
	CodeRain.exe
) else (
	echo Compile Error
	rem pause
)

::2016-11-07 add -lpng -lz