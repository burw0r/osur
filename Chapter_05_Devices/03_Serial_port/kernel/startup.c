/*! Startup function - initialize kernel subsystem */
#define _K_STARTUP_C_

#include "time.h"
#include "device.h"
#include "memory.h"
#include "fs.h"
#include <kernel/errno.h>
#include <kernel/features.h>
#include <arch/interrupt.h>
#include <arch/processor.h>
#include <api/stdio.h>
#include <api/prog_info.h>
#include <lib/string.h>

char system_info[] = 	OS_NAME ": " NAME_MAJOR ":" NAME_MINOR ", "
			"Version: " VERSION " (" ARCH ")";

/* state of kernel features */
uint kernel_features = FEATURE_SUPPORTED; /* initially set all to "on" state */

/*!
 * First kernel function (after boot loader loads it to memory)
 */
void k_startup()
{
	extern void *k_stdout; /* console for kernel messages */

	/* set initial stdout */
	kdevice_set_initial_stdout();

	/* initialize memory subsystem(needed for boot) */
	k_memory_init();

	/*! start with regular initialization */

	/* interrupts */
	arch_init_interrupts();

	/* detect memory faults(qemu do not detect segment violations!) */
	arch_register_interrupt_handler(INT_MEM_FAULT, k_memory_fault, NULL);
	arch_register_interrupt_handler(INT_UNDEF_FAULT, k_memory_fault, NULL);

	/* timer subsystem */
	k_time_init();

	/* devices */
	k_devices_init();

	/* switch to default 'stdout' for kernel */
	k_stdout = k_device_open(K_STDOUT, O_WRONLY);

	kprintf("%s\n", system_info);

	/* enable interrupts */
	enable_interrupts();

	stdio_init(); /* initialize standard input & output devices */

	k_fs_init("DISK", 512, 4096);

	// ----------------------------------------------------------------------
	printf("\n\n[#] testni program za lab4\n");


	// ================ otvaranje ========================
	int fd = open("file:test", O_CREAT | O_WRONLY, 0);
	kprintf("\totvoren file \"test\",  fd=%d\n", fd);

	int fd2 = open("file:test2", O_CREAT | O_WRONLY, 0);
	kprintf("\totvoren file \"test2\", fd=%d\n", fd2);

  // ================ pisanje ========================
	int retval = write(fd, "neki tekst", 11);
	kprintf("\tzapisao \"neki tekst\" u test, retval=%d\n", retval);

	int retval2 = write(fd2, "AAAA BBBBBCCC", 14);
	kprintf("\tzapisao \"AAAA BBBBBCCC\" u test2, retval=%d\n", retval2);

	retval = close(fd);
	// kprintf("\tzatvaram file, retval=%d\n", retval);
	retval2 = close(fd2);
	// kprintf("\tzatvaram file, retval=%d\n", retval2);


	fd = open("file:test", O_RDONLY, 0);
	fd2 = open("file:test2", O_RDONLY, 0);
	char buff[11];
	char buff2[11];
	retval = read(fd, buff, 11);
	kprintf("\tiz \"test\" procitao: %s, retval:%d\n", buff, retval);
	retval2 = read(fd2, buff2, 14);
	kprintf("\tiz \"test2\" procitao: %s, retval:%d\n", buff2, retval2);







	// ----------------------------------------------------------------------

	/* start desired program(s) */
	//hello_world();
	//keyboard();
	//timer();
	/* segm_fault(); */

	kprintf("\nSystem halted!\n");
	halt();
}

/*! Turn kernel feature on/off */
uint sys__feature(uint features, int cmd, int enable)
{
	uint prev_state = kernel_features & features;

	ASSERT(!(features & ~FEATURE_SUPPORTED));

	if (cmd == FEATURE_GET)
		return prev_state;

	/* update state */
	if (enable)
		kernel_features |= features;
	else
		kernel_features &= ~features;

	/* action required? */

	if ((features & FEATURE_INTERRUPTS))
	{
		if (enable)
			enable_interrupts();
		else
			disable_interrupts();
	}

	return prev_state;
}
