/*! Hello world program */

#include <stdio.h>
#include <api/prog_info.h>
#include <kernel/memory.h>

int hello_world()
{
	printf("Example program: [%s:%s]\n%s\n\n", __FILE__, __FUNCTION__,
		 hello_world_PROG_HELP);

	// printf("Hello World!\n");

	// LAB3 testni primjer
printf("[+] testni primjer pripreme za lab3\n\n");


 size_t sizes[8] = {10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000};
 void* ptrs[8];

 for (int i=0; i<8; i++) { ptrs[i] = kmalloc(sizes[i]); }
 for (int i=0; i<8; i++) { kfree(ptrs[i]); }

 // size_t ostatak_mem = 6930084;
 // kmalloc(6920084);

 // printf("[+] sad trazim blok od 100 000\n");
 // printf("		--> treba se dogoditi merge!\n");

 kmalloc(100000);

	printf("\n\n------------------------k-r-a-j----------------------------\n");

#if 0	/* test escape sequence */
	printf("\x1b[20;40H" "Hello World at 40, 20!\n");
#endif

	return 0;
}
