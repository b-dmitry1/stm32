#include <stdio.h>
#include "board1xx.h"
#include "crc.h"
#include "simple_boot_protocol.h"

/* boot packet */
/* MSB is transmitted first */
typedef struct
{
	unsigned char crc16[2];
	unsigned char dest_addr;
	unsigned char command;
	unsigned char length[2];
	unsigned char data[BOOT_DATA_SIZE];
} PACKED packet_t;

packet_t packet;
int packet_pos = -2;

static void send_response(int length, int port_handle, int (*send)(int port_handle, const void *data, int count))
{
	unsigned char *p = (unsigned char *)&packet;
	int i;
	unsigned short crc = 0xFFFFu;
	int total_len;

	/* Set bit 6 of dest_addr to indicate that this packet is device to host packet */
	packet.dest_addr |= 0x40;
	packet.length[0] = length >> 8;
	packet.length[1] = length & 0xFF;

	total_len = sizeof(packet) - sizeof(packet.data) + packet.length[0] * 256 + packet.length[1];

	for (i = 2; i < total_len; i++)
	{
		crc = crc16_update(p[i], crc);
	}

	packet.crc16[0] = crc >> 8;
	packet.crc16[1] = crc & 0xFF;

	send(port_handle, &packet, total_len);
}

void simple_boot_protocol_char_received(unsigned char ch, int port_handle, int (*send)(int port_handle, const void *data, int count))
{
	unsigned char *p = (unsigned char *)&packet;
	int len;
	int i;
	unsigned short crc;

	/* Wait for start sequence 0x55AA */
	if (packet_pos == -2)
	{
		if (ch == 0x55)
			packet_pos++;
		return;
	}

	if (packet_pos == -1)
	{
		if (ch == 0xAA)
			packet_pos++;
		else
			packet_pos = -2;
		return;
	}

	/* Packet body */
	p[packet_pos++] = ch;
	if (packet_pos < 6)
	{
		return;
	}

	len = packet.length[0] * 256 + packet.length[1];
	if (len > sizeof(packet.data))
	{
		/* Overflow */
		packet_pos = -2;
		return;
	}

	if (packet_pos < sizeof(packet) - sizeof(packet.data) + len)
	{
		/* Need more data */
		return;
	}

	crc = 0xFFFFu;
	for (i = 2; i < packet_pos; i++)
	{
		/* Calc packet's CRC */
		crc = crc16_update(p[i], crc);
	}

	/* Reset packet pointer */
	packet_pos = -2;

	if (crc != packet.crc16[0] * 256 + packet.crc16[1])
	{
		/* CRC error */
		return;
	}

	/* Call user's function */
	len = boot_command(packet.command, packet.data, len, sizeof(packet.data));
	
	if (len <= 0 || len > sizeof(packet.data))
	{
		/* Buffer overflow */
		return;
	}

	/* Send response if length is not zero */
	send_response(len, port_handle, send);
}
