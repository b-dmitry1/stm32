#include <stdio.h>

int tlv_add(unsigned char **buffer, unsigned char *limit, unsigned char tag, const void *value, int len)
{
	const unsigned char *v = (const unsigned char *)value;
	int i;
	unsigned char *buf = *buffer;

	if (value == NULL)
		len = 0;

	*buf++ = tag;
	*buf++ = len >> 8;
	*buf++ = len;

	if (value != NULL)	
		for (i = 0; i < len; i++)
			*buf++ = v[i];

	*buffer = buf;

	return 1;
}

int tlv_add_str(unsigned char **buffer, unsigned char *limit, unsigned char tag, const char *value)
{
	int len = 0;
	unsigned char *buf = *buffer;

	if (value != NULL)
		for (; value[len]; len++);

	*buf++ = tag;
	*buf++ = len >> 8;
	*buf++ = len;

	if (value != NULL)	
		for (len = 0; value[len]; len++)
			*buf++ = value[len];

	*buffer = buf;

	return 1;
}

int tlv_add_int(unsigned char **buffer, unsigned char *limit, unsigned char tag, int value)
{
	unsigned char *buf = *buffer;

	*buf++ = tag;
	*buf++ = 0;
	*buf++ = 4;
	*buf++ = value >> 24u;
	*buf++ = value >> 16;
	*buf++ = value >> 8;
	*buf++ = value >> 0;

	*buffer = buf;

	return 1;
}

int tlv_get_length(const unsigned char *data, const unsigned char *limit, unsigned char tag)
{
	int len;
	int code;

	while (data < limit)
	{
		code = data[0];
		len = data[1] * 256 + data[2];
		data += 3;
		if (code == tag)
		{
			/* Found */
			return len;
		}

		data += len;
	}

	return 0;
}

const unsigned char *tlv_get_data(const unsigned char *data, const unsigned char *limit, unsigned char tag)
{
	int len;
	int code;

	while (data < limit)
	{
		code = data[0];
		len = data[1] * 256 + data[2];
		data += 3;
		if (code == tag)
		{
			/* Found */
			return data;
		}

		data += len;
	}

	return NULL;
}

int tlv_get_int(const unsigned char *data, const unsigned char *limit, unsigned char tag, int def)
{
	int len;
	const unsigned char *value;

	len = tlv_get_length(data, limit, tag);

	if (len == 0)
		return def;

	value = tlv_get_data(data, limit, tag);
	if (value == NULL)
		return def;

	return value[0] * 16777216u + value[1] * 65536 + value[2] * 256 + value[3];
}
