# Bootloader for stm32

## Supported devices:
* STM32F103 with 64-512K EEPROM
* STM32F105/107 with 128-1024K EEPROM
* STM32F401 with 256K EEPROM
* STM32F405/407 with 128-1024K EEPROM
* STM32F429/439 with 128-1024K EEPROM

### Compiling on a Windows host:
* Open "_compile_all.bat" in a text editor.
* Select you CPU family in the following line: 'set "cpu=f407"'. Supported CPU families are: f103, f105, f401, f407, f429.
* Run "_compile_all.bat" script.

To compile a single project use a "_compile.bat" script from a project's directory.

Please change the path to your STM programmer util in the "_write.bat" file.

### Compiling on a Linux host
Run "make MCU=(model)"

where (model) is 103, 105, 401, 407, or 429.

### To use it in your system
* Set RS232/RS485 parameters in bootloader/config.h file.
* Compile and flash bootloader application. It will occupy first 16 KB of MCU's EEPROM.
* Use boottest as a template for your application.

Please note that the "printf" function comsumes a lot of program memory and stack space.
It will not be a problem on 128K+ devices, but on 64K (like "Bluepill" stm32f103c8 board) it is preferrable to use an alternative implementation of "printf".
