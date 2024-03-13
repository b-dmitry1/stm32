@echo off

if "%1"=="" echo Please do not use this script directly & goto :end

rem Set device family here
set "cpufamily=stm32f4xx"

rem Set device linker script here
set "ld=..\..\lib%cpufamily%\stm32f407.ld"

rem Set device type here
set "defines=-DSTM32F40_41xxx -DSTM32F4XX -DCPU_FREQ=168000000"
set "cpuconfig=-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16"


set "cflags=-fmax-errors=3 -x none -fsigned-char -O2 -ffunction-sections -fdata-sections -fno-exceptions -mlittle-endian -g -lm -nostartfiles -Wall"

rem FreeRTOS port.c and portmacro.h path
set "freertos_port=..\..\FreeRTOS\portable\GCC\ARM_CM3"

call do_compile.bat %1

:end
