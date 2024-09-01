@echo off
set projectFolder=LinkedInParser
set currentFolder=%cd%
for %%I in ("%cd%") do set currentFolder=%%~nI
if not %currentFolder% == %projectFolder% (
    echo Error: couldn't find project folder
    echo Hint: call from %projectFolder%
    exit /b 1
)

cmake --build build --config Debug
if %ERRORLEVEL% NEQ 0 (
    echo Error: Build failed.
    exit /b %ERRORLEVEL%
)
echo Build successfully compeleted!