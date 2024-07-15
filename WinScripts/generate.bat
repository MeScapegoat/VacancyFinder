@echo off
rmdir /S /Q build
cmake -G Ninja -S . -B build
if %ERRORLEVEL% NEQ 0 (
    echo Error: Generating files failed.
    exit /b %ERRORLEVEL%
)
echo Files generated successfully
