#include <stdio.h>
#include "board1xx.h"
#include "crc.h"

/* Size of data buffer */
#define BOOT_DATA_SIZE		300


/*
  Boot command callback will be called from char_received function
  cmd - command code
  arg - optional int32 value
  buffer - I/O buffer
  in_bytes - data bytes in request
  buffer_size - max bytes in response
  Returns number of bytes in response or 0 if no response
*/
int boot_command(int cmd, unsigned char *buffer, int in_bytes, int buffer_size);

/*
  Char received function should be called after receiving the next byte
  ch - byte
  port_handle - argument of a "send" function
  send - pointer to function that sends response to a host
*/
void simple_boot_protocol_char_received(unsigned char ch, int port_handle, int (*send)(int port_handle, const void *data, int count));
