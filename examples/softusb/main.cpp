#include <stdio.h>
#include "board.h"
#include "softusb.h"

#define RXD1		PORTA, 10
#define TXD1		PORTA, 9

#define NUM_USB_PORTS	2

// Please don't forget to pull down the USB pins to GND via 10..15 K resistors.
// STM32's weak pull-down resistance is not enough for most USB devices.

#define USB1_PORT	PORTB
#define USB1_PIN_DM	0
#define USB1_PIN_DP	1
#define USB2_PORT	PORTB
#define USB2_PIN_DM	14
#define USB2_PIN_DP	15

typedef struct
{
	SoftUsb* device;
	// USB device type: not connected / full-speed / keyboard / mouse / unknown
	int device_type;
	// Last mouse coordinates
	int mouse_x;
	int mouse_y;
	int mouse_b;
	int mouse_wheel;
} usb_port_t;

// Define USB host on PORTB.0 (d-) and PORTB.1 (d+)
SoftUsb usb1(USB1_PORT, USB1_PIN_DM, USB1_PIN_DP);

// Define USB host on PORTB.14 (d-) and PORTB.15 (d+)
SoftUsb usb2(USB2_PORT, USB2_PIN_DM, USB2_PIN_DP);

usb_port_t usb[NUM_USB_PORTS] =
{
	{ &usb1, 0, USB_DEVICE_NOT_CONNECTED },
	{ &usb2, 1, USB_DEVICE_NOT_CONNECTED },
};

// Timer 4 (1 KHz) handler
extern "C" void tick_func(void)
{
	static int port = 0;
	static int powerup = 0;
	int i;

	if (powerup < 1000)
	{
		powerup++;
		return;
	}

	// Only 1 port at a time can do a long work (up to 130 us)
	// Inactive ports go first
	for (i = 0; i < NUM_USB_PORTS; i++)
	{
		if (port == i)
			continue;
		usb[i].device->timer1ms(0);
	}

	// Then active one
	if (port < NUM_USB_PORTS)
	{
		usb[port].device->timer1ms(1);
	}

	port++;
	if (port >= NUM_USB_PORTS)
	{
		port = 0;
	}
}

void print_vid_pid(usb_port_t* port)
{
	printf("VID_%.4X&PID_%.4X", port->device->get_vendor_id(), port->device->get_device_id());
}

void print_device_type(usb_port_t* port)
{
	switch (port->device_type)
	{
		case USB_DEVICE_NOT_CONNECTED:
			printf("not connected");
			break;
		case USB_DEVICE_FULLSPEED:
			printf("full-speed - will not work");
			break;
		case USB_DEVICE_UNKNOWN:
			printf("unknown: ");
			print_vid_pid(port);
			break;
		case USB_DEVICE_KEYBOARD:
			printf("keyboard: ");
			print_vid_pid(port);
			break;
		case USB_DEVICE_MOUSE:
			printf("mouse: ");
			print_vid_pid(port);
			break;
	}
}

void check_ports(void)
{
	usb_port_t* port;
	int i;
	int type;

	for (i = 0; i < NUM_USB_PORTS; i++)
	{
		port = &usb[i];

		type = port->device->get_device_type();

		if (type == port->device_type)
			continue;

		port->device_type = type;

		printf("usb%d: ", i + 1);

		print_device_type(port);

		printf("\n");
	}
}

void print_port_events(usb_port_t* port)
{
	int mx, my, mb, mw;

	if (port->device_type == USB_DEVICE_MOUSE)
	{
		port->device->get_mouse_pos(mx, my, mb, mw);

		if (port->mouse_x != mx || port->mouse_y != my || port->mouse_b != mb || port->mouse_wheel != mw)
		{
			printf("Mouse report: X = %d, Y = %d, buttons = %d, wheel = %d\n",
				mx, my, mb, mw);
			port->mouse_x = mx;
			port->mouse_y = my;
			port->mouse_b = mb;
			port->mouse_wheel = mw;
		}
	}

	if (port->device_type == USB_DEVICE_KEYBOARD)
	{
		if (port->device->kbhit())
		{
			printf("Key: \'%c\'\n", port->device->getch());
		}
	}
}

void print_events(void)
{
	int i;

	for (i = 0; i < NUM_USB_PORTS; i++)
	{
		print_port_events(&usb[i]);
	}
}

int main(void)
{
	rcc_high_performance(XTAL_8MHZ);

	uart_open(1, 115200, 8, NOPARITY, ONESTOPBIT, RXD1, TXD1, NO485);

	// Disable I/O buffering
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	// Print system clock values
	printf("\n\n\nCLK = %d, AHB = %d, APB1 = %d, APB2 = %d\n", sysclk, ahbclk, apb1clk, apb2clk);

	// Setup USB
	pin_conf_input_pulldown(USB1_PORT, USB1_PIN_DM);
	pin_conf_input_pulldown(USB1_PORT, USB1_PIN_DP);
	pin_conf_input_pulldown(USB2_PORT, USB2_PIN_DM);
	pin_conf_input_pulldown(USB2_PORT, USB2_PIN_DP);

	// Set Timer 10 to free-run 1.5 MHz
	RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
	TIM10->PSC = apb2clk * 2 / 3 / 1000000 - 1; // 1.5 MHz
	TIM10->ARR = 0xFFFFFFFFu;
	TIM10->CR1 |= 0x01;
	
	for (;;)
	{
		check_ports();

		print_events();

		msleep(100);
	}
}
