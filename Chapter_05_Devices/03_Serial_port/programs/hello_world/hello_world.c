/*! Hello world program */

#include <stdio.h>
#include <api/prog_info.h>

#include <arch/interrupt.h>
#include <arch/processor.h>


static void test1(uint irqn){
	printf(">>prekid manjeg prioriteta\n");
	raise_interrupt(42);
	printf(">>kraj prvog prekida(manji prioritet)\n");
}

static void test2(uint irqn){
	printf("[+] PREKID S VECIM PRIORITETOM \n", irqn);
}

int hello_world()
{
	printf("Example program: [%s:%s]\n%s\n\n", __FILE__, __FUNCTION__,
		 hello_world_PROG_HELP);
	printf("Hello World!\n");

	arch_register_interrupt_handler(40, test1, NULL, 8);
	arch_register_interrupt_handler(42, test2, NULL, 3);
	raise_interrupt(40);

#if 0	/* test escape sequence */
	printf("\x1b[20;40H" "Hello World at 40, 20!\n");
#endif

	return 0;
}
