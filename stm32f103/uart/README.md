# UART example

In this example UART2 is set to 115200 bps.

When you run this program, the UART console will prompt you to enter a simple mathematical expression of 2 numbers and a mathematical operation sign between them.
The sign should be "+", "-", "*", or "/". Expression example: 1 + 2

If the expression is entered correctly, it will be evaluated.

The UART should work with different crystal frequences. In this example I used 16 MHz external crystal oscillator (HSE).
If you wish to test it with an internal 8 MHz RC oscillator (HSI), comment out the following line:

```
	rcc_high_performance(XTAL_16MHZ);
```

Please note that the "scanf" function may cause an I/O error on incorrect input and is used here for STDIN example only.

Also note that the "printf" function comsumes a lot of program memory and a stack space. It can be used only on 64K+ EEPROM devices.

### RS485 mode

To connect RS485 instead of RS232 just pass DE line's GPIO to "uart_open" function:

```
	#define RS485_DE	PORTA, 11

	uart_open(2, 115200, 8, NOPARITY, ONESTOPBIT, RS485_DE);
```

The driver will switch RS485 line direction automatically.
