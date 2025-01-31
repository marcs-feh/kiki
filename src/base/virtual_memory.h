#ifndef _virtual_memory_h_include_
#define _virtual_memory_h_include_

#if defined(TARGET_OS_LINUX) || defined(TARGET_OS_WINDOWS)

#include "base.h"
#include "memory.h"

#define VIRTUAL_PAGE_SIZE (4 * KiB)

typedef struct VirtualMemBlock  VirtualMemBlock;

enum MemoryProtection {
    MemoryProtection_Read  = (1 << 0),
    MemoryProtection_Write = (1 << 1),
    MemoryProtection_Exec  = (1 << 2),
};

// Region of virtual memory
struct VirtualMemBlock {
    void* ptr;
    Size commited;
    Size reserved;
};

VirtualMemBlock virtual_block_create(Size cap);

void virtual_block_destroy(VirtualMemBlock* block);

void* virtual_block_push(VirtualMemBlock* block, Size len);

// void virtual_block_decommit(VirtualMemBlock* block);

bool virtual_protect(void* ptr, Size len, U8 prot);

void* virtual_reserve(Size len);

void virtual_free(void* ptr, Size len);

void virtual_decommit(void* ptr, Size len);

void* virtual_commit(void* ptr, Size len);


#endif
#endif /* Include guard */
