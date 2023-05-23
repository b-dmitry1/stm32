#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "board.h"
#include "crc.h"
#include "tlv.h"
#include "simple_boot_protocol.h"

const char *bootloader_ver = "1.01";

void (*char_received)(unsigned char ch, int port_handle,
	int (*send)(int port_handle, const void *data, int count)) =
	simple_boot_protocol_char_received;

unsigned int find_user_end(void)
{
	const unsigned int *user = (const unsigned int *)(FLASH_START + USER_START);
	unsigned int p, res = 0;

	/* Last non-FF value is an user's firmware end */
	for (p = 0; p < USER_SIZE / 4; p++)
	{
		if (user[p] != 0xFFFFFFFFu)
		{
			res = p * 4 + 4;
		}
	}

	return res;
}

const char *find_user_ver(void)
{
	const char *user = (const char *)(FLASH_START + USER_START);
	unsigned int p, i, j;
	const char *ver_id = "$AppInfo$";
	int found;

	for (p = 0; p < USER_SIZE - 138; p++)
	{
		if (user[p] == ver_id[0])
		{
			found = 1;
			for (i = 0; ver_id[i]; i++)
			{
				if (user[p + i] != ver_id[i])
				{
					found = 0;
				}
			}
			
			if (found)
			{
				for (j = 0; j < 128; j++)
				{
					if (user[p + i + j] == 0)
					{
						/* Should be 0-terminated string up to 127 chars */
						return &user[p + i];
					}
				}

				break;
			}
		}
	}

	return "";
}

static int send_response(int port_handle, const void *data, int count)
{
	uart_send(port_handle, data, count);
	return count;
}

void execute_app(void)
{
	unsigned int *app = (unsigned int *)(FLASH_START + USER_START);
	unsigned int stack = app[0];
	void (*start)() = (void (*)())(app[1]);

	/* Disable bootloader's interrupts and power-off devices */
	uart_close(BOOT_UART);
	timer_disable(5);

	/* Set new vectors */
	SCB->VTOR = FLASH_START + USER_START;

	/* New stack */	
	__set_MSP(stack);

	/* Start app */
	start();
}

/*
  Boot command callback
  cmd - command code
  arg - optional int32 value
  buffer - I/O buffer
  in_bytes - data bytes in request
  buffer_size - max bytes in response
  Returns number of bytes in response or 0 if no response
*/
int boot_command(int cmd, unsigned char *buffer, int in_bytes, int buffer_size)
{
        unsigned int user_size;
	const unsigned char *user = (const unsigned char *)(FLASH_START + USER_START);
	const char *ver;
	unsigned char *p = buffer;
	unsigned char *e = buffer + buffer_size;
	int addr, len;
	
	switch (cmd)
	{
		case 'i':
			/* Firmware info in TLV request */
			user_size = find_user_end();
			ver = find_user_ver();
			tlv_add_int(&p, e, 's', user_size);
			tlv_add_str(&p, e, 'v', ver);
			tlv_add_str(&p, e, 'b', bootloader_ver);
			break;
		case 'r':
			/* Read user area */
			addr = tlv_get_int(p, e, 'a', 0);
			if (addr >= 0 && addr < USER_SIZE - 256)
			{
				tlv_add_int(&p, e, 'a', addr);
				tlv_add(&p, e, 'd', &user[addr], 256);
			}
			break;
		case 'e':
			/* Erase user area */
			flash_erase_user();
			tlv_add_int(&p, e, 'r', 1);
			break;
		case 'w':
			/* Write user area */
			addr = tlv_get_int(p, e, 'a', 0);
			len = tlv_get_int(p, e, 's', 0);
			if (len == 256 && addr >= 0 && addr < USER_SIZE - len)
			{
				tlv_add_int(&p, e, 'r', flash_write(addr + USER_START, tlv_get_data(p, e, 'd'), len));
				tlv_add_int(&p, e, 'a', addr);
				tlv_add_int(&p, e, 's', len);
			}
			break;
	}

	return p - buffer;
}

void bootloader(void)
{
	int timeout;

	/* 10 seconds of inactivity to exit bootloader */
	for (timeout = 0; timeout < 10000; timeout++)
	{
		while (uart_rx_count(BOOT_UART) > 0)
		{
			timeout = 0;
			char_received(uart_recv_byte(BOOT_UART), BOOT_UART, send_response);
		}

		msleep(1);
	}
}

int main(void)
{
	int ch;
	int goboot = 0;

	uart_open(BOOT_UART, BOOT_SPEED, 8, BOOT_PARITY, BOOT_STOPBITS, BOOT_RS485PIN);

	/* "GoBoot" message in the first second will start the bootloader */
	while (get_tick_count() < 1000)
	{
		if (uart_rx_count(BOOT_UART) > 0)
		{
			ch = uart_blocking_recv_byte(BOOT_UART);
			switch (ch)
			{
				case 'G': goboot = 1; break;
				case 'o': if ((goboot == 1) || (goboot == 3) || (goboot == 4)) goboot++; else goboot = 0; break;
				case 'B': if (goboot == 2) goboot++; else goboot = 0; break;
				case 't': if (goboot == 5) goboot++; break;
				default: goboot = 0; break;
			}
			if (goboot >= 6)
			{
				break;
			}
		}
	}

	if (goboot >= 6)
	{
		bootloader();
	}

	execute_app();
}
