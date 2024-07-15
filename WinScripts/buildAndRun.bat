@echo off
set projectFolder=LinkedInParser
set currentFolder=%cd%
for %%I in ("%cd%") do set currentFolder=%%~nI
if not %currentFolder% == %projectFolder% (
    echo Error: couldn't find project folder
    echo Hint: call from %projectFolder%
    exit /b 1
)

set scriptsFolder=WinScripts
call %scriptsFolder%\build.bat
set errorCode=%errorlevel%
if not %errorCode%==0 (
    exit /b %errorCode%
)
build\LinkedInParser.exe