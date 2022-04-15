#include <types/basic.h>

void premjesti()
{


	extern size_t size_c, size_i, size_data, size_bss, RAM_addr, ROM_addr;
	// extern size_t size_stog;
	size_t copy_size;
	copy_size = (size_t) &size_i + (size_t) &size_data + (size_t) &size_bss;

	char* copy_from =  (char*) ((size_t) &ROM_addr + (size_t) &size_c + (size_t) &size_i);
	char* copy_to = (char *) &RAM_addr;

	for(size_t i=0; i<copy_size; i++)
		*copy_to++ = *copy_from++;


	//copy stack
	// char* copy_from_stog = (char*) (ROM_addr + size_c + size_i + size_data + size_bss);
	// char* copy_to_stog = (char*)  0x600000;
	// for (size_t i=0; i<size_stog; i++){
	// 	*copy_to_stog++ = *copy_from_stog++;
	// }

}
