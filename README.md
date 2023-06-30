# STM32 small library and examples
* Small library for STM32F103, STM32F105(107), STM32F401, STM32F405(407), STM32F429(439).
* Usage examples.
* Bootloader.

Status:

| Demo       | f103 | f105 | f401cc | f407 | f429 |
| ---------- | ---- | ---- | ------ | ---- | ---- |
| Bootloader |      | ok   |        | ok   | ok   |
| Blinky     | ok   | ok   | ok     | ok   | ok   |
| UART       | ok   | ok   | ok     | ok   | ok   |
| Timer      | ok   | ok   | ok     | ok   | ok   |
| fm25 (SPI) |      | ok   |        | ok   | ok   |
| VGA (LTDC) | n/a  | n/a  | n/a    | n/a  | ok   |

Bootloader PC app: in progress.

## Setting up the environment

### Debian-like systems

    sudo apt install git gcc-arm-none-eabi openocd
    
If you see "not found" message please add "extended" repository to /etc/apt/sources.list.

### Windows

Prebuilt "arm-gnu-toolchain" Windows package could be found on an official "ARM developer" repository:
https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads

**-or-** you can build it from sources, but it could be a little bit tricky.

If you wish to use "STM32CubeProgrammer" to write your programs to a microcontroller's flash memory - download and install
"STM32CubeProgrammer" from official STMicroelectronics site: https://www.st.com/en/development-tools/stm32cubeprog.html

**-or-**

If you wish to use "openocd" debugger - it is here: https://openocd.org/pages/getting-openocd.html

## Compiling

On Linux, cd to a project's directory (where "Makefile" located) and type "make". The script will put you firmware file a.bin to the "bin" directory.

On Windows please use "_compile.bat" script in a project's directory or type the commands manually like shown in the script.

## Flashing

After a successful program compilation connect your device to a ST-Link programmer to write it to a microcontroller's memory.

On Linux, cd to a project's directory and use _write.sh script or an "openocd" command from it.
To make the script file executable please type

    chmod +x _write.sh
    
On Windows please change the path to a "STM32_Programmer_CLI.exe" file in a "_write.bat" script. Usually this file could be found
in the "Program Files" directory on a drive "C:" where "STMCubeProgrammer" software installed.

The script will automatically write your program and start it execution.
