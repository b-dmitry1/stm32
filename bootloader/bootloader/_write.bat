@echo off

i:\pf\stm32prog\bin\STM32_Programmer_CLI.exe -c port=SWD -w bin\a.hex -rst -run

rem e:\st_toolset\stvp\stvp_cmdline -BoardName=ST-LINK -Device=STM32F429xE -progMode=SWD -FileProg=a.hex -no_progData -no_progOption -no_loop -no_blank -no_verif -erase -no_log
