
rmdir /s /q trse
set dir=C:\Users\leuat\Documents\GitHub\TRSE\Release
mkdir trse
xcopy /s %dir%\release\*.exe trse\
xcopy /s %dir%\release\iconengines trse\
xcopy /s %dir%\release\platforms trse\
xcopy /s %dir%\release\styles trse\
xcopy /s %dir%\release\*.dll trse\
xcopy /s ..\source\* trse
xcopy /s ..\..\..\OK64\Release\release trse\OK64 /y /i
del trse\OK64\*.obj

xcopy 7z.exe trse\
REM move trse\fluff64.exe trse\trse.exe
REM move trse bin
REM mkdir trse
REM move bin trse
mkdir trse\tutorials\
mkdir trse\project_templates\
mkdir trse\units\

xcopy /e  ..\tutorials\* trse\tutorials\
xcopy /e  ..\project_templates\* trse\project_templates\
xcopy /e  ..\..\units\* trse\units\
del /s trse\tutorials\clear.* 
xcopy /e dll\* trse\
REM xcopy start.cmd trse\
