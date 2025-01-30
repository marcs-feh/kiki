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
    virtual_free(block->ptr, block->reserved);
}

// Commit n bytes of memory (page aligned) in block and return pointer to start of that memory.
void* virtual_block_push(VirtualMemBlock* block, Size len){
    void* base = &block->ptr[block->commited];
    len = align_forward_size(len, VIRTUAL_PAGE_SIZE);
    if((block->commited + len) > block->reserved){ /* Out of reserved space*/
        printf("OOM\n");
        return NULL;
    }
    void* ptr = virtual_commit(base, len);
    if(ptr != NULL){
        block->commited += len;
    }
    
    return ptr;
}

// Decommit all pages of memory block
void virtual_block_decommit(VirtualMemBlock* block){
    virtual_decommit(block->ptr, block->commited);
    block->commited = 0;
}

#endif
#endif /* Include guard */
