# Examples

## Supported devices:
* STM32F103 with 64-512K EEPROM
* STM32F105/107 with 128-1024K EEPROM
* STM32F405/407 with 128-1024K EEPROM
* STM32F429/439 with 128-1024K EEPROM

### Compiling on a Windows host:
* Open "_compile_all.bat" in a text editor.
* Select you CPU family in the following line: 'set "cpu=f407"'. Supported CPU families are: f103, f105, f407, f429.
* Run "_compile_all.bat" script.

To compile a single project use a "_compile.bat" script from a project's directory.

### Compiling on a Linux host
- Under development -

Please change the path to your STM programmer util in the "_write.bat" file if you are working in a Windows environment.
