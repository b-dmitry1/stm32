# UART example

This is a "printf" via UART demo.

In this example UART1 is set to 115200 bps.

The UART should work with different crystal frequences. In this example I used 16 MHz external crystal oscillator (HSE).
If you wish to test it with an internal (8 or 16) MHz RC oscillator (HSI), comment out the following line:

```
	rcc_high_performance(XTAL_16MHZ);
```

Note that the "printf" function comsumes a lot of program memory and stack space.

### RS485 mode

To connect RS485 instead of RS232 just pass DE line's GPIO to "uart_open" function:

```
	#define RS485_DE	PORTA, 11

	uart_open(1, 115200, 8, NOPARITY, ONESTOPBIT, RXD1, TXD1, RS485_DE);
```

The driver will switch RS485 line direction automatically.
