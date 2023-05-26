CFLAGS+=-Wall -fmax-errors=3 -x none -mthumb -mtune=cortex-m3 -march=armv7e-m -fsigned-char -fno-exceptions -mlittle-endian -I. -I../inc -DSTM32F4xx -DSTM32F40_41xxx
LDFLAGS+=-nostartfiles
LIBS=
LDLIBS=

CC=arm-none-eabi-gcc

OBJ=main.o ../common/board4xx.o ../common/board4xx_gpio.o ../common/board4xx_spi.o ../common/board4xx_uart.o ../common/vectors.o ../common/syscalls.o ../common/system_stm32f4xx.o ../common/startup.o

all:	bin/a.out

clean:
	rm -f *.o

bin/a.out: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -T../memory.ld -o $@ $^ $(LDLIBS)
	arm-none-eabi-objcopy -O binary bin/a.out bin/a.bin
