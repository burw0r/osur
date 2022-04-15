/*! linker script for memory layout */

/* Its parsed as C before used in linking! */

ROM_addr = 0x100000;
RAM_addr = 0x200000;
/*STOG=0x600000*/

ENTRY(arch_startup)

SECTIONS {

	.code ROM_addr : AT(ROM_addr)
 {
	 /* kernel_code_addr = .; */

	 /* instructions */
	 *?/boot/startup.asm.o ( .text* )
		*?/boot/premjesti.o ( .text* )
	}
	size_c = SIZEOF(.code);


	.instrukcije ROM_addr + size_c : AT(ROM_addr + size_c)  /*ostaje u ROMu, ne premjestati*/
	{
	 *( .text* )
	 *( .rodata* )   /* LAB2 --> instrukcije i konstante ne premjestati*/
  }

	size_i = SIZEOF( .instrukcije );
	xxxx = 0x10000;


	.data RAM_addr : AT(ROM_addr + size_c + size_i)
 {
	 kernel_data_addr = .;
	 * ( .data* )
 }
	size_data = SIZEOF(.data);


	.bss RAM_addr + size_data : AT(ROM_addr + size_c + size_i + size_data)
 {
	 *( .bss* COMMON* )

	 . = ALIGN (4096);
 }
 size_bss = SIZEOF(.bss);

	.stog_u_slici 0x600000 : AT(0x600000)
	{
		*( .stog )
		. = ALIGN (4096);
	}
	size_stog = SIZEOF(.stog_u_slici);

	/*
	 * what with other sections generated with gcc (various versions)?
	 * if optimizing for size discard them with: /DISCARD/ : { *(*) }
	 * if debugging with qemu/gdb they must be included
	 * - they can be included implicitly, without declaring them here
	 * - but they will be put behind "kernel_end_addr" so its best to use
	 *   multiboot loader (QEMU is that)
	 */

/DISCARD/ : { *(.eh_frame) }

#ifndef DEBUG
	/DISCARD/ : { *(*) }
#endif
	kernel_end_addr = .;
}
