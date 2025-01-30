#ifndef _arena_h_include_
#define _arena_h_include_

#include "base.h"
#include "memory.h"

typedef struct Arena Arena;

typedef enum ArenaKind ArenaKind;

enum ArenaKind {
	ArenaKind_Buffer = 0,  // Uses single fixed length buffer, it's the most basic type of arena.
	ArenaKind_Dynamic = 1, // Can aquire more memory as needed
	// ArenaKind_Virtual = 2, // Uses a single buffer with a big reserved address space, committing pages as necessary
};

#define ARENA_VIRTUAL_BLOCK_SIZE (16 * KiB)

typedef void* (*ArenaDynamicAllocFunc)(Size count, Size align);

struct Arena {
	U8* data;
	Size offset;
	Size capacity;
	U32 kind;
	Uintptr last_allocation;
};

// Initialize a memory arena from a buffer
bool arena_init_buffer(Arena* a, U8* data, Size len);

// Deinit the arena
void arena_destroy(Arena *a);

// Resize arena allocation in-place, gives back same pointer on success, null on failure
void* arena_resize(Arena* a, void* ptr, Size new_size);

// Reset arena, marking all its owned pointers as freed
void arena_free_all(Arena* a);

// Allocate `size` bytes aligned to `align`, return null on failure
void *arena_alloc(Arena* a, Size size, Size align);


#endif /* Include guard */
