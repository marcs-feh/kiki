#ifndef _virtual_memory_h_include_
#define _virtual_memory_h_include_
#include "base.h"
#include "memory.h"

// NOTE: Should ideally be a multiple of the platform's page size
#define VIRTUAL_BLOCK_SIZE (16 * KiB)

void* virtual_reserve(Size len);

void* virtual_commit(void* ptr, Size len);

void virtual_free(void* ptr, Size len);

#endif /* Include guard */
