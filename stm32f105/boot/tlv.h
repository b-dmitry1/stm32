#ifndef TLV_H
#define TLV_H

int tlv_add(unsigned char **buffer, unsigned char *limit, unsigned char tag, const void *value, int len);
int tlv_add_str(unsigned char **buffer, unsigned char *limit, unsigned char tag, const char *value);
int tlv_add_int(unsigned char **buffer, unsigned char *limit, unsigned char tag, int value);

int tlv_get_length(const unsigned char *data, const unsigned char *limit, unsigned char tag);
const unsigned char *tlv_get_data(const unsigned char *data, const unsigned char *limit, unsigned char tag);
int tlv_get_int(const unsigned char *data, const unsigned char *limit, unsigned char tag, int def);

#endif
