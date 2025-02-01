#ifndef _virtual_memory_h_include_
#define _virtual_memory_h_include_

#include "base.h"
#include "memory.h"

typedef struct MemoryBlock MemoryBlock;
enum MemoryProtection {
    MemoryProtection_Read  = (1 << 0),
    MemoryProtection_Write = (1 << 1),
    MemoryProtection_Exec  = (1 << 2),
};

// Region of virtual memory, can also be used as a generic buffer if virtual memory is not supported
struct MemoryBlock {
    void* ptr;
    Size commited;
    Size reserved;
};

#define DEFAULT_VIRTUAL_PAGE_SIZE (4 * KiB)

#if defined(TARGET_OS_LINUX) || defined(TARGET_OS_WINDOWS)

U32 virtual_page_size();

MemoryBlock virtual_block_create(Size reserve);

void* virtual_block_push_pages(MemoryBlock* block, Size len);

void virtual_block_pop_pages(MemoryBlock* block, Size len);

bool virtual_protect(void* ptr, Size len, U8 prot);

void* virtual_reserve(Size len);

void virtual_free(void* ptr, Size len);

void virtual_decommit(void* ptr, Size len);

void* virtual_commit(void* ptr, Size len);


#endif
#endif /* Include guard */
