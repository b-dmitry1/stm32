#ifndef SDRAM_H
#define SDRAM_H


#ifdef __cplusplus
extern "C" {
#endif

/*
  cas_latency: 2 or 3
  read_burst: 0 - disable, 1 - enable
  data_width: 8 or 16 or 32
  num_banks: 2 or 4
  rows: 11 to 13
  cols: 8 to 11
*/
void sdram_init(int cas_latency, int read_burst, int data_width, int num_banks, int rows, int cols);

#ifdef __cplusplus
}
#endif

#endif
