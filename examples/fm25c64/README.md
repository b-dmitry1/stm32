# FM25CL64 example

fm25cl64 is a 64Kbit Ferroelectric Nonvolatile RAM IC with SPI interface which performs write operations at bus speed.

This example does 2 writes to a FRAM memory connected to SPI2 port (PB12 - PB15 pins) with a dump display after each write.

fm25cl64 connection:

* 1 CS# - connected to PB12.
* 2 MISO - connected to PB14.
* 3 WP# - connected to +3.3V via 10K resistor.
* 4 GND - connected to GND.
* 5 MOSI - connected to PB15.
* 6 SCK - connected to PB13.
* 7 HOLD# - connected to +3.3V via 10K resistor.
* 8 VCC - connected to +3.3V power supply.

In this example UART1 is set to 115200 bps. The CPU is clocked by 16 MHz external crystal oscillator.

Cheaper fm25cl16 IC can be used instead of fm25c64 without any modifications in a source code.

(!) Note the difference between fm25c64 and fm25cL64. The first one requires +5V power supply, and the second one is a +3.3V device.

The output in my case:

```
	CLK = 72000000, AHB = 72000000, APB1 = 36000000, APB2 = 72000000

	Writing "Test1234567890abcdefghij" to FRAM address 5:
	Read:
	0000  E8 03 00 00 00 54 65 73 74 31 32 33 34 35 36 37  Õ....Test1234567
	0010  38 39 30 61 62 63 64 65 66 67 68 69 6A 00 00 00  890abcdefghij...
	0020  00 00 00 00 10 27 00 00 64 00 00 00 00 00 01 00  .....'..d.......
	0030  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................

	Writing "9876543210fedcbaTestTest" to FRAM address 5:
	Read:
	0000  E8 03 00 00 00 39 38 37 36 35 34 33 32 31 30 66  Õ....9876543210f
	0010  65 64 63 62 61 54 65 73 74 54 65 73 74 00 00 00  edcbaTestTest...
	0020  00 00 00 00 10 27 00 00 64 00 00 00 00 00 01 00  .....'..d.......
	0030  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
```
