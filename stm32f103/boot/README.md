# Bootloader

A simple STM32 bootloader program with read, erase, and write functions.

To use your app with that bootloader, change the flash start and size values your "ld" script like this:

```
MEMORY
{
	/* rom (rx)  : ORIGIN = 0x08000000, LENGTH = 128K */
	   rom (rx)  : ORIGIN = 0x08004000, LENGTH = 112K
}
```

In this example UART3 is set to 9600 bps and PB2 is used to control RS485 direction (DE).

## The protocol

MSB is transmitted first.

Packet format:
* Start sequence 0x55, 0xAA (2 bytes).
* Packet CRC (2 bytes) - CRC16 of packet body from "Device address" to the end of the Data.
* Device address (1 byte) - not used.
* Command code (1 byte).
* Packet length (2 bytes) - data field length in bytes.
* Data ("packet length" bytes).

Data is a sequence of TLV (tag-length-value) fields:
* Tag (1 byte) - field identifier.
* Length (2 bytes) - field's data length in bytes.
* Value ("length" bytes) - field's data.

TLV types are:
* Int32 - packet 32-bit integer, length = 4 bytes.
* Text - ASCII string (not 0-terminated), length = "length" bytes.
* BLOB - binary data, length = "length" bytes.

Simple TLV library could be found in "tlv.c" and "tlv.h" files.

CRC calculation algorithm:

```
unsigned short crc16_update(unsigned char data, unsigned short crc)
{
	unsigned short q;
	q = (crc ^ data) & 0x0f;
	crc = (crc >> 4) ^ (q * 0x1081l);
	q = (crc ^ (data >> 4)) & 0x0f;
	crc = (crc >> 4) ^ (q * 0x1081l);
	return crc;
}
```

### Firmware Info command
Code: 'i'.
Input TLVs: none.
Output TLVs: 's' (Int32) - firmware size in bytes, 'v' (Text) - firmware version string,
'u' (Text) - bootloader version string.

This command returns installed firmware size and version, and bootloader's version.

### Read Flash command
Code: 'r'.
Input TLVs: 'a' (Int32) - read address.
Output TLVs: 'a' (Int32) - read address, 'd' (BLOB) - EEPROM contents (hardcoded 256 bytes).

This command reads 256 bytes of user-area in EEPROM from given address and transmits it.

### Erase Flash command
Code: 'e'.
Input TLVs: none.
Output TLVs: 'r' (Int32) - result (0 - fail, 1 - OK).

This command erases entire user area. Note: the erase procedure takes some time so the response will be delayed!

### Write Flash command
Code: 'w'.
Input TLVs: 'a' (Int32) - write address, 's' (Int32) - block length in bytes, 'd' (BLOB) - data to be written.
Output TLVs: 'a' (Int32) - write address, 'r' (Int32) - result (0 - fail, 1 - OK).

This command writes "block length" bytes (even number from 2 to 256) to user-area in EEPROM from given address. The EEPROM area should be erased first.
