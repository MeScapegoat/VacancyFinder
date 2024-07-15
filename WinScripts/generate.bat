@echo off
set projectFolder=LinkedInParser
set currentFolder=%cd%
for %%I in ("%cd%") do set currentFolder=%%~nI
if not %currentFolder% == %projectFolder% (
    echo Error: couldn't find project folder
    echo Hint: call from %projectFolder%
    exit /b 1
)

rmdir /S /Q build
cmake -G Ninja -S . -B build
if %ERRORLEVEL% NEQ 0 (
    echo Error: Generating files failed.
    exit /b %ERRORLEVEL%
)
echo Files generated successfully
