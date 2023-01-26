#include "board103.h"
#include "esp8266custom.h"

#define CON_UART	1

/* TODO: Place your WiFi router AP name here: */
#define WIFI_NAME	""
/* TODO: Place your WiFi password here: */
#define WIFI_PASS	""

/* TODO: Place your API URL here: */
#define URL		""

char response[4096];

void con_puts(const char *s)
{
	uart_send_str(CON_UART, s);
}

void wait(void)
{
	volatile int w;
	for (w = 0; w < 1000000; w++);
}

void upgrade_esp8266(void)
{
	/*

	To upgrade ESP8266's firmware:
	  1. Connect PA0 to ESP's RESET pin.
	  2. Connect PA1 to ESP's GPIO0 pin.
	  3. Connect PA2 to ESP's RxD.
	  4. Connect PA3 to ESP's TxD.
	  5. Call this function.
          6. Use ESP's "flash download tool" to write a new firmware.

	*/

	set_pin_mode(GPIOA, 9, OUTPUT);
	set_pin_mode(GPIOA, 10, INPUT_PULLUP);
	set_pin_mode(GPIOA, 2, OUTPUT);
	set_pin_mode(GPIOA, 3, INPUT_PULLUP);

	// ESP8266 GPIO 0
	set_pin_mode(GPIOA, 1, OUTPUT);	
	clr_pin(GPIOA, 1);

	// ESP8266 RESET
	set_pin_mode(GPIOA, 0, OUTPUT);	
	clr_pin(GPIOA, 0);
	wait();
	set_pin(GPIOA, 0);

	while (1)
	{
		set_pin_to(GPIOA, 2, get_pin(GPIOA, 10));
		set_pin_to(GPIOA, 9, get_pin(GPIOA, 3));
	}
}

int main(void)
{
	int n;

	board_init();

	// Activate to upgrade software in ESP8266
	// upgrade_esp8266();
	
	set_pin_mode(GPIOC, 13, OUTPUT);

	uart_open(CON_UART, 115200, SERIAL_8N1);

	uart_open(ESP_UART, 115200, SERIAL_8N1);

	con_puts("\n\n\nReset\n");

	for (n = 0; n < 3; n++)
	{
		clr_pin(GPIOC, 13);
		wait();
		set_pin(GPIOC, 13);
		wait();
	}

	// con_puts("Setting WiFi name and password\n");

	// wifi_set_name_password(WIFI_NAME, WIFI_PASS);
	
	// wait();

	while (uart_recv_byte(2) >= 0);

	for (;;)
	{
		for (n = 0; n < sizeof(response); n++)
		{
			response[n] = 0;
		}

		n = http_get(URL, response, sizeof(response) - 1);
		if (n > 0)
		{
			uart_send_str(CON_UART, "LED: ");
			response[n] = 0;
			uart_send_str(CON_UART, response);
			if (response[0] == '1') clr_pin(GPIOC, 13);
			if (response[0] == '0') set_pin(GPIOC, 13);
		}
		else
		{
			uart_send_str(CON_UART, "x");
		}
		uart_send_str(CON_UART, "\n");
		
		wait();
	}
}
