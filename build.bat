@echo off

echo Select a build option:
echo 1. Windows/Linux
echo 2. Android 32-bit
echo 3. Android 64-bit
echo 4. Quit

set /p choice=Enter choice (1-4): 

if "%choice%"=="1" (
echo Building for Windows/Linux...
geode build
) else if "%choice%"=="2" (
echo Building for Android 32-bit...
geode build -p android32
) else if "%choice%"=="3" (
echo Building for Android 64-bit...
geode build -p android64
) else if "%choice%"=="4" (
exit
) else (
echo Invalid option
)