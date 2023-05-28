@echo off

rem Set your CPU name here
if "%cpu%"=="" set "cpu=f407"

rem This app will be started by bootloader
set "bootloader_app=1"

for %%I in (.) do set "d=%%~nxI"
cd ..
call compile_%cpu%.bat %d%
cd %d%

copy bin\a.bin firmware.bin >nul

set "bootloader_app="
