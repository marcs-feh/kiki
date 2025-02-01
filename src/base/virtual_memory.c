#ifndef _virtual_memory_c_include_
#define _virtual_memory_c_include_

#if defined(TARGET_OS_LINUX) || defined(TARGET_OS_WINDOWS)
#include "memory.h"
#include "virtual_memory.h"

MemoryBlock virtual_block_create(Size reserve){
	reserve = align_forward_size(reserve, virtual_page_size());
	MemoryBlock blk = {
		.ptr = virtual_reserve(reserve), 
		.commited = 0,
	};

	blk.reserved = blk.ptr != NULL ? reserve : 0;
	return blk;
}

void* virtual_block_push_pages(MemoryBlock* block, Size len){
	panic("Unimplemented");
}

void virtual_block_pop_pages(MemoryBlock* block, Size len){
	panic("Unimplemented");
}

#endif
#endif /* Include guard */
