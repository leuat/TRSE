
rmdir /s /q trse
set dir=C:\Users\leuat\OneDrive\Documents\GitHub\TRSE\build\
mkdir trse
xcopy /s %dir%\release\*.exe trse\
xcopy /s %dir%\release\iconengines trse\
xcopy /s %dir%\release\platforms trse\
xcopy /s %dir%\release\*.dll trse\
xcopy /s ..\source\* trse
xcopy 7z.exe trse\
REM move trse\fluff64.exe trse\trse.exe
REM move trse bin
REM mkdir trse
REM move bin trse

xcopy /e  ..\tutorials\RasLib trse\RasLib\
xcopy /e  ..\tutorials\C64\* trse\C64\
xcopy /e  ..\tutorials\C128\* trse\C128\
xcopy /e  ..\tutorials\Vic20\* trse\Vic20\
xcopy /e dll\* trse\
REM xcopy start.cmd trse\
