# Blinky example

This example simply blinks a LED connected to PC13.

Try to comment out the following line:

```
	rcc_high_performance(XTAL_16MHZ);
```

This will disable external crystal oscillator and PLL so blinking will become much slower.
