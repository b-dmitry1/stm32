#include <stdint.h>
#include "board.h"

extern unsigned long _sidata;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;

extern void (*__ctors_begin[])(void);
extern void (*__ctors_end[])(void);
extern void (*__preinit_array_start[])(void);
extern void (*__preinit_array_end[])(void);
extern void (*__init_array_start[])(void);
extern void (*__init_array_end[])(void);

extern int main(void);

void Reset_Handler(void)
{
	unsigned long *src, *dst;
	void (**ctor)(void);

	/* Copy the initialized data */
	for (src = &_sidata, dst = &_sdata; dst < &_edata; )
	{
		*(dst++) = *(src++);
	}

	/*
	Zero-fill the BSS using CPU registers
	for (r0 = _sbss, r1 = _ebss, r2 = 0; r0 < r1; r0 += 4)
	{
		*((unsigned long *)r0) = r2;
	}
	*/
	__asm(	"	ldr	r0, =_sbss\n"
		"	ldr	r1, =_ebss\n"
		"	mov	r2, #0\n"
		"loop0:\n"
		"	cmp	r0, r1\n"
		"	it	lt\n"
		"	strlt	r2, [r0], #4\n"
		"	blt	loop0\n"
	);

	/* Complete the initialization, call static constructors */
	for (ctor = __preinit_array_start; ctor < __preinit_array_end; ++ctor)
	{
		(*ctor)();
	}

	for (ctor = __ctors_begin; ctor < __ctors_end; ++ctor)
	{
		(*ctor)();
	}

	for (ctor = __init_array_start; ctor < __init_array_end; ++ctor)
	{
		(*ctor)();
	}

	board_init();

	main();
}
