@echo off

SetLocal EnableExtensions EnableDelayedExpansion

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
for %%f in ("..\..\libstm32f10x\*.c") do (
  set c=!c!%%f 
)

for %%f in (!c!) do (
  arm-none-eabi-g++ -fmax-errors=3 -x none -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -fsigned-char -O2 -ffunction-sections -fdata-sections -fno-exceptions -mlittle-endian ^
  -I. -I.. -I..\..\inc -I..\..\inc\stm32f10x -T..\..\libstm32f10x\stm32f105.ld -g -lm -nostartfiles -c ^
  -DSTM32F10X_CL -DSTM32F1XX -Wall ^
  %%f -o bin\%%~nf.o

  if errorlevel 1 goto :end
)

set "obj="
for %%f in ("bin\*.o") do (
  set obj=!obj!%%f 
)

rem -Wl,-Map=a.map
arm-none-eabi-g++ -x none -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -fsigned-char -O2 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -mlittle-endian -fcommon ^
-T..\..\libstm32f10x\stm32f105.ld -g -lm -nostartfiles -Wl,-Map,bin\build.map ^
-DSTM32F10X_CL -DSTM32F1XX -Wall -o bin\a.out ^
!obj!


if errorlevel 1 goto :end

arm-none-eabi-objcopy -O ihex bin\a.out bin\a.hex
arm-none-eabi-objcopy -O binary bin\a.out bin\a.bin

:end
