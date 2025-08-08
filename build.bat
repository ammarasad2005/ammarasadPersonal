@echo off
REM Build script for Space Shooter Game
REM Requires MASM32 and Irvine library to be properly installed

echo Building Space Shooter Game...
echo.

if "%1"=="enhanced" goto enhanced
if "%1"=="basic" goto basic

echo Choose version to build:
echo 1. Basic Version (SpaceShooter.asm)
echo 2. Enhanced Version (SpaceShooterEnhanced.asm)
echo.
set /p choice="Enter choice (1 or 2): "

if "%choice%"=="1" goto basic
if "%choice%"=="2" goto enhanced
echo Invalid choice. Building basic version by default.

:basic
echo Building basic version...
ml /c /coff SpaceShooter.asm
if errorlevel 1 goto error
link /subsystem:console SpaceShooter.obj irvine32.lib kernel32.lib user32.lib
if errorlevel 1 goto error
echo Basic version build successful!
echo Run SpaceShooter.exe to play the game
goto end

:enhanced
echo Building enhanced version...
ml /c /coff SpaceShooterEnhanced.asm
if errorlevel 1 goto error
link /subsystem:console SpaceShooterEnhanced.obj irvine32.lib kernel32.lib user32.lib
if errorlevel 1 goto error
echo Enhanced version build successful!
echo Run SpaceShooterEnhanced.exe to play the game
goto end

:error
echo Build failed!
echo Make sure MASM32 and Irvine library are properly installed and in PATH
echo.
echo Common issues:
echo - MASM32 not installed or not in PATH
echo - Irvine32.lib not found
echo - Missing include files
echo.
echo Installation help:
echo 1. Download and install MASM32 SDK from http://www.masm32.com/
echo 2. Download Irvine32 library and copy to MASM32\lib directory
echo 3. Copy Irvine32.inc to MASM32\include directory

:end
echo.
pause