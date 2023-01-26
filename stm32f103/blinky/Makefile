CFLAGS+=-Wall -fmax-errors=3 -x none -mthumb -mtune=cortex-m3 -march=armv7e-m -fsigned-char -fno-exceptions -mlittle-endian -I. -I../inc -DSTM32F1xx -DSTM32F10X_MD
LDFLAGS+=-nostartfiles
LIBS=
LDLIBS=

CC=arm-none-eabi-gcc

OBJ=main.o ../common/board103.o ../common/vectors.o ../common/syscalls.o ../common/system_stm32f10x.o ../common/startup.o

all:	bin/a.out

clean:
	rm -f *.o

bin/a.out: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -T../memory.ld -o $@ $^ $(LDLIBS)
	arm-none-eabi-objcopy -O binary bin/a.out bin/a.bin
