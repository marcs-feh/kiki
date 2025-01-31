#ifndef _virtual_memory_c_include_
#define _virtual_memory_c_include_

#if defined(TARGET_OS_LINUX) || defined(TARGET_OS_WINDOWS)
#include "memory.h"
#include "virtual_memory.h"

// Reserve a block of `cap` bytes, aligned to a memory page
VirtualMemBlock virtual_block_create(Size cap){
    cap = align_forward_size(cap, VIRTUAL_PAGE_SIZE);
    void* ptr = virtual_reserve(cap);
    VirtualMemBlock block = {
        .reserved = cap,
        .commited = 0,
        .ptr = ptr,
    };
    return block;
}

void virtual_block_destroy(VirtualMemBlock* block){
	ensure(((Uintptr)block->ptr & (VIRTUAL_PAGE_SIZE - 1)) == 0, "Pointer is not aligned to page boundary");
    virtual_free(block->ptr, block->reserved);
}

// Commit +n bytes of memory (page aligned) in block and return pointer to start of that memory.
void* virtual_block_push(VirtualMemBlock* block, Size len){
    void* base = (U8*)block->ptr + block->commited;
    len = align_forward_size(len, VIRTUAL_PAGE_SIZE);
    if((block->commited + len) > block->reserved){ /* Out of reserved space*/
        return NULL;
    }
    void* ptr = virtual_commit(base, len);
    if(ptr != NULL){
        block->commited += len;
    }
    
    return ptr;
}

// Decommit memory that is after `keep` bytes (page aligned) from the start of the block
void virtual_block_decommit(VirtualMemBlock* block, Size keep){
	keep = align_forward_size(keep, VIRTUAL_PAGE_SIZE);
	if(keep >= block->commited){ return; }

	if(keep <= 0){
		virtual_decommit((U8*)block->ptr, block->commited);
		block->commited = 0;
	}
	else {
		Size to_decommit = block->commited - keep;
		virtual_decommit((U8*)block->ptr + keep, to_decommit);
		block->commited -= to_decommit;
	}
}

#endif
#endif /* Include guard */
