#ifndef _virtual_memory_h_include_
#define _virtual_memory_h_include_
#include "base.h"
#include "memory.h"

#define VIRTUAL_PAGE_SIZE (4 * KiB)

typedef struct VirtualMemBlock  VirtualMemBlock;

enum MemoryProtection {
    MemoryProtection_Read  = (1 << 0),
    MemoryProtection_Write = (1 << 1),
    MemoryProtection_Exec  = (1 << 0),
};

struct VirtualMemBlock {
    void* ptr;
    Size commited;
    Size reserved;
};

bool virtual_protect(void* ptr, Size len, U8 prot);

void* virtual_reserve(Size len);

void* virtual_commit(void* ptr, Size len);

void virtual_free(void* ptr, Size len);

#endif /* Include guard */
