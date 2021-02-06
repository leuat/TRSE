echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64
set dir=C:\Users\leuat\Documents\GitHub\TRSE_AT_NIGHT
set qt=\Qt\6.0.1\msvc2019_64\bin\
set uploadScript=C:\Users\leuat\Documents\GitHub\uploadScript\upload_win_night.cmd

set PATH=%PATH%;C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.16.27023\bin\HostX64\x86
REM rmdir /s /q %dir%
cd %dir%
REM git clone https://github.com/leuat/TRSE.git %dir%
mkdir Release
cd Release
echo "Qmake.."
REM %qt%\qmake.exe %dir%\trse.pro %dir%\Release
echo "Compiling.."
REM \Qt\Tools\QtCreator\bin\jom.exe -f %dir%\Release\Makefile.Release
cd release
%qt%\windeployqt.exe .

cd %dir%\Publish\publish_win
call PublishWindows.cmd
call %uploadScript%

pause
