@echo off
REM Syntax check for Space Shooter Game
REM This script only checks for assembly syntax errors without linking

echo Checking assembly syntax...

REM Only assemble to check syntax
ml /c /coff /Fl SpaceShooter.asm
if errorlevel 1 goto error

echo Syntax check passed!
echo Assembly file compiles successfully
goto end

:error
echo Syntax errors found!
echo Check the assembly code for issues

:end
del SpaceShooter.obj 2>nul
pause