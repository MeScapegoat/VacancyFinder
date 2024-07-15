@echo off
cmake --build build
if %ERRORLEVEL% NEQ 0 (
    echo Error: Build failed.
    exit /b %ERRORLEVEL%
)
echo Build successfully compeleted!