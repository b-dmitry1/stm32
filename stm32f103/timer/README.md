# Timer example

In this example the timer callback function is defined and TIMER 2 is set to 2 Hz frequency (500 ms period).

```
	void timer2_func(void)
	{
		led = !led;

		pin_set(LED, led);
	}

	/* in the "main" function */
	timer_init_periodic_ns(2, TIMER_PERIOD_MS(500), timer2_func);
```

The "timer2_func" callback function will be called 2 times per second so the led will turn on and off each time the function is called producing 1 blink per second.

The example is designed to work with an external 16 MHz crystal oscillator but should work correctly with any 3 MHz - 25 MHz oscillator (HSE), and with an internal 8 MHz RC oscillator (HSI).
The blink interval should be the same in all cases.

Currently, only timers 2, 3, 4, 5 could be used.
