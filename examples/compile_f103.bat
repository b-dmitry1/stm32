@echo off

if "%1"=="" echo Please do not use this script directly & goto :end

rem Set device family here
set "cpufamily=stm32f10x"

rem Set device linker script here
set "ld=..\..\lib%cpufamily%\stm32f103.ld"

rem Set device type here
set "defines=-DSTM32F10X_MD -DSTM32F1XX -DCPU_FREQ=72000000"
set "cpuconfig=-mcpu=cortex-m3 -mthumb -mfloat-abi=soft"


set "cflags=-fmax-errors=3 -x none -fsigned-char -O2 -ffunction-sections -fdata-sections -fno-exceptions -mlittle-endian -g -lm -nostartfiles -Wall"

rem FreeRTOS port.c and portmacro.h path
set "freertos_port=..\..\FreeRTOS\portable\GCC\ARM_CM3"

call do_compile.bat %1

:end
