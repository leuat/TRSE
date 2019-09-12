
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

xcopy /e  ..\tutorials\* trse\tutorials\
del /s trse\tutorials\clear.* 
xcopy /e dll\* trse\
REM xcopy start.cmd trse\
