#ifndef CONFIG_H
#define CONFIG_H

#define STACK_SIZE		8192

#define HEAP_SIZE		4096


/* UART config */
#define BOOT_UART	3
#define BOOT_SPEED	9600
#define BOOT_PARITY	NOPARITY
#define BOOT_STOPBITS	ONESTOPBIT
#define BOOT_RS485PIN	PORTB, 2


#endif
