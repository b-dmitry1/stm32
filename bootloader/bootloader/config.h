#ifndef CONFIG_H
#define CONFIG_H

#define NO_GOBOOT

/* UART config */
#define BOOT_UART	1
#define BOOT_SPEED	115200
#define BOOT_PARITY	NOPARITY
#define BOOT_STOPBITS	ONESTOPBIT
#define BOOT_RS485PIN	NO485
#define BOOT_RXD	PORTA, 10
#define BOOT_TXD	PORTA, 9

/* User area offset */
#define USER_START	0x4000


#define STACK_SIZE	4096
#define HEAP_SIZE	4096

#define UART1_RX_SIZE	32
#define UART1_TX_SIZE	32
#define UART2_RX_SIZE	32
#define UART2_TX_SIZE	32
#define UART3_RX_SIZE	32
#define UART3_TX_SIZE	32
#define UART4_RX_SIZE	32
#define UART4_TX_SIZE	32
#define UART5_RX_SIZE	32
#define UART5_TX_SIZE	32
#define UART6_RX_SIZE	32
#define UART6_TX_SIZE	32

#endif
