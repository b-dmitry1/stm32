@echo off

rem Set your CPU name here
if "%cpu%"=="" set "cpu=f401"


for %%I in (.) do set "d=%%~nxI"
cd ..
call compile_%cpu%.bat %d%
cd %d%
