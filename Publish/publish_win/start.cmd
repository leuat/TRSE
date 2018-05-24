if exist trse_win.zip (
    bin\7z.exe x trse_win.zip  -aoa  -O. 
    xcopy /s /y trse\* .
    del /F /Q trse
    del trse_win.zip
)
cd bin

start trse.exe



