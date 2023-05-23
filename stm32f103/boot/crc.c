unsigned short crc16_update(unsigned char data, unsigned short crc)
{
	unsigned short q;
	q = (crc ^ data) & 0x0f;
	crc = (crc >> 4) ^ (q * 0x1081l);
	q = (crc ^ (data >> 4)) & 0x0f;
	crc = (crc >> 4) ^ (q * 0x1081l);
	return crc;
}
