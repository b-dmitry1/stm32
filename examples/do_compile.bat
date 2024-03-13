@echo off

SetLocal EnableExtensions EnableDelayedExpansion

cd %1

if "%1"=="" echo Please do not use this script directly & goto :end

if "%cpufamily%"=="" echo Please do not use this script directly & goto :end

rem Use special ld script if we need bootloader
if "%bootloader_app%"=="1" set "ld=%ld%_app"

if exist bin\*.o del bin\*.o

if exist a.hex del a.hex
if exist a.bin del a.bin

set "c="
for %%f in ("*.c") do (
  set c=!c!%%f 
)
for %%f in ("*.cpp") do (
  set c=!c!%%f 
)
for %%f in ("..\..\lib%cpufamily%\*.c") do (
  set c=!c!%%f 
)

if "%freertos%" == "1" (
  for %%f in ("..\..\FreeRTOS\*.c") do (
    set c=!c!%%f 
  )
  for %%f in ("%freertos_port%\*.c") do (
    set c=!c!%%f 
  )
  for %%f in ("..\..\FreeRTOS\portable\MemMang\heap_4.c") do (
    rem set c=!c!%%f 
  )
)

for %%f in (!c!) do (
  arm-none-eabi-g++ %cpuconfig% %cflags% ^
  -I. -I..\..\inc -I..\..\inc\%cpufamily% -I..\..\FreeRTOS\include -I%freertos_port% -T%ld% -c ^
  %defines%  ^
  %%f -o bin\%%~nf.o

  if errorlevel 1 goto :end
)

set "obj="
for %%f in ("bin\*.o") do (
  set obj=!obj!%%f 
)

rem -Wl,-Map=a.map
arm-none-eabi-g++ %cpuconfig% %cflags% ^
-T%ld% -Wl,-Map,bin\build.map ^
%defines% -o bin\a.out ^
!obj!


if errorlevel 1 goto :end

arm-none-eabi-objcopy -O ihex bin\a.out bin\a.hex
arm-none-eabi-objcopy -O binary bin\a.out bin\a.bin

:end
