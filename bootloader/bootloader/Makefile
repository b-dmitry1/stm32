MCU?=103

ifeq ($(MCU), 103)
  MCUFAMILY=stm32f10x
  MCUMODEL=stm32f103
  PREPROC=-DSTM32F1XX -DSTM32F10X_MD
endif

ifeq ($(MCU), 105)
  MCUFAMILY=stm32f10x
  MCUMODEL=stm32f105
  PREPROC=-DSTM32F1XX -DSTM32F10X_CL
endif

ifeq ($(MCU), 401)
  MCUFAMILY=stm32f4xx
  MCUMODEL=stm32f401
  PREPROC=-DSTM32F4XX -DSTM32F401xx
endif

ifeq ($(MCU), 407)
  MCUFAMILY=stm32f4xx
  MCUMODEL=stm32f407
  PREPROC=-DSTM32F4XX -DSTM32F40_41xxx
endif

ifeq ($(MCU), 429)
  MCUFAMILY=stm32f4xx
  MCUMODEL=stm32f429
  PREPROC=-DSTM32F4XX -DSTM32F429_439xx
endif

CFLAGS+=-Wall -fmax-errors=3 -x none -mcpu=cortex-m3 -mthumb -mfloat-abi=soft \
	-fsigned-char -Os -fno-exceptions -mlittle-endian \
	-I. -I../../inc -I../../inc/$(MCUFAMILY) $(PREPROC) -DNO_GOBOOT
LDFLAGS+=-nostartfiles
LIBS=
LDLIBS=

CC=arm-none-eabi-gcc

OBJ=$(wildcard *.c) $(wildcard ../../lib$(MCUFAMILY)/*.c)

all:	bin/a.out

clean:
	rm -f *.o
	rm -f bin/a.out

bin/a.out: $(OBJ)
	@$(CC) $(CFLAGS) $(LDFLAGS) -T../../lib$(MCUFAMILY)/$(MCUMODEL).ld -o $@ $^ $(LDLIBS)
	@arm-none-eabi-objcopy -O binary bin/a.out bin/a.bin
	@rm -f bin/a.out
	@echo "$(MCUMODEL) / $(shell basename $(CURDIR))"
