
rmdir /s /q trse
set dir=C:\Users\leuat\Documents\GitHub\TRSE\release
mkdir trse
xcopy /s %dir%\release\*.exe trse\
xcopy /s %dir%\release\iconengines trse\
xcopy /s %dir%\release\platforms trse\
xcopy /s %dir%\release\*.dll trse\
xcopy /s ..\source\* trse
xcopy /S ..\..\..\OK64\Release\release\ok64 trse\ /y
xcopy /s ..\..\..\OK64\Release\release\*.exe trse\ /y
xcopy /s ..\..\..\OK64\Release\release\iconengines trse\ /y
xcopy /s ..\..\..\OK64\Release\release\platforms trse\ /y
xcopy /s ..\..\..\OK64\Release\release\*.dll trse\ /y


xcopy 7z.exe trse\
REM move trse\fluff64.exe trse\trse.exe
REM move trse bin
REM mkdir trse
REM move bin trse
mkdir trse\tutorials\
mkdir trse\project_templates\

xcopy /e  ..\tutorials\* trse\tutorials\
xcopy /e  ..\project_templates\* trse\project_templates\
del /s trse\tutorials\clear.* 
xcopy /e dll\* trse\
REM xcopy start.cmd trse\
