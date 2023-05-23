#ifndef GPIO_H
#define GPIO_H

#define PORTA				0
#define PORTB				1
#define PORTC				2
#define PORTD				3
#define PORTE				4
#define PORTF				5
#define PORTG				6
#define PORTH				7
#define PORTI				8

#define PIN_MODE_INPUT			0x00
#define PIN_MODE_OUTPUT			0x01
#define PIN_MODE_AF			0x02
#define PIN_MODE_ANALOG			0x03

#define PIN_TYPE_PUSHPULL		0x00
#define PIN_TYPE_OPENDRAIN		0x01

#define PIN_SPEED_LOW			0x00
#define PIN_SPEED_MEDIUM		0x01
#define PIN_SPEED_HIGH			0x02
#define PIN_SPEED_VERYHIGH		0x03

#define PIN_PULL_FLOAT			0x00
#define PIN_PULL_UP			0x01
#define PIN_PULL_DOWN			0x02

#ifdef __cplusplus
extern "C" {
#endif

// port: 0 - A, 1 - B, ...
// Pin configuration functions
void pin_conf(int port, int pin, int func, int mode, int type, int pull, int speed);
void pin_conf_output(int port, int pin);
void pin_conf_opendrain(int port, int pin);
void pin_conf_input_pullup(int port, int pin);
void pin_conf_input_float(int port, int pin);
void pin_conf_input_pulldown(int port, int pin);
void pin_conf_analog(int port, int pin);

// Set mode of pre-configured pin
void pin_dir_output(int port, int pin);
void pin_dir_input(int port, int pin);
void pin_select_alternate_fn(int port, int pin, int func);

// Get / set pin's value
void pin_high(int port, int pin);
void pin_low(int port, int pin);
void pin_set(int port, int pin, int value);
int  pin_get(int port, int pin);

#ifdef __cplusplus
}
#endif

#endif
