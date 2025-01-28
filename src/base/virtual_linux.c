#include "virtual.h"
#include <sys/mman.h>

void* mem_virtual_reserve(Size count){
	void* ptr = mmap(NULL, count, PROT_NONE, MAP_ANONYMOUS, -1, 0);
	return ptr;
}
