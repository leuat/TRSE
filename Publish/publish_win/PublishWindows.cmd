
rmdir /s /q trse
set dir=C:\Users\leuat\Documents\GitHub\TRSE\release
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
mkdir trse\tutorials\

xcopy /e  ..\tutorials\RasLib trse\RasLib\
xcopy /e  ..\tutorials\C64\* trse\tutorials\C64\
xcopy /e  ..\tutorials\C128\* trse\tutorials\C128\
xcopy /e  ..\tutorials\Vic20\* trse\tutorials\Vic20\
xcopy /e  ..\tutorials\NES\* trse\tutorials\NES\
xcopy /e  ..\tutorials\Fjong\* trse\tutorials\Fjong\
xcopy /e  ..\tutorials\PET\* trse\tutorials\PET\
xcopy /e  ..\tutorials\Amiga\* trse\tutorials\Amiga\
xcopy /e dll\* trse\
REM xcopy start.cmd trse\
