@echo off

rem Set your CPU name here
if "%cpu%"=="" set "cpu=f429"

rem Set to 1 if you need FreeRTOS
set "freertos=1"

for %%I in (.) do set "d=%%~nxI"
cd ..
call compile_%cpu%.bat %d%
cd %d%
