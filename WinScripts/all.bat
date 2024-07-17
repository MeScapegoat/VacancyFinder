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
call %scriptsFolder%\generate.bat
if %ERRORLEVEL% NEQ 0 (
    exit /b %errorCode%
)
call %scriptsFolder%\buildAndRun.bat