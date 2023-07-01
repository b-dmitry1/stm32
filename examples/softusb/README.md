# SoftUSB library demo

Pins:

* PA9 - Uart1 TXD
* PA10 - Uart1 RXD
* PB0 - USB1 D-
* PB1 - USB1 D+
* PB14 - USB2 D-
* PB15 - USB2 D+

USB D-/D+ pins should be pulled down to GND via 10..15K resistors.

Tested on stm32f401ccu6, stm32f407vet6, stm32f429zet6.

Keyboard: Logitech K120. Mouse: Logitech 810-002182.

Only low-speed devices are supported.

For more information please check my "SoftUSB" repo.
