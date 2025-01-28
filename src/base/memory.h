#ifndef _memory_h_include_
#define _memory_h_include_

#include "base.h"

#define mem_new(Type, Num, Alloc) mem_alloc((Alloc), sizeof(Type) * (Num), alignof(Type));

// Helper to use with printf "%.*s"
#define fmt_str(buf) (int)((buf).len), (buf).v

// Set n U8s of p to value.
void mem_set(void* p, U8 val, Size count);

// Copy n U8s for source to destination, they may overlap.
void mem_copy(void* dest, void const * src, Size count);

// Compare 2 buffers of memory, returns -1, 0, 1 depending on which buffer shows
// a bigger U8 first, 0 meaning equality.
I32 mem_compare(void const * a, void const * b, Size count);

// Copy n U8s for source to destination, they should not overlap, this tends
// to be faster then mem_copy
void mem_copy_no_overlap(void* dest, void const * src, Size count);

// Align p to alignment a, this only works if a is a non-zero power of 2
Uintptr align_forward_ptr(Uintptr p, Uintptr a);

// Align p to alignment a, this works for any positive non-zero alignment
Size align_forward_size(Size p, Size a);

typedef struct Arena Arena;

typedef enum ArenaKind ArenaKind;

enum ArenaKind {
	ArenaKind_Static = 0,
	ArenaKind_Dynamic = 1,
	ArenaKind_Virtual = 2,
};

struct Arena {
	Size offset;
	Size capacity;
	Uintptr last_allocation;
	U32 kind;

	U8* data;
	Arena* next;
};

// Initialize a memory arena with a buffer
void arena_init(Arena* a, U8* data, Size len);

// Deinit the arena
void arena_destroy(Arena *a);

// Resize arena allocation in-place, gives back same pointer on success, null on failure
void* arena_resize(Arena* a, void* ptr, Size new_size);

// Reset arena, marking all its owned pointers as freed
void arena_free_all(Arena* a);

// Allocate `size` bytes aligned to `align`, return null on failure
void *arena_alloc(Arena* a, Size size, Size align);

#endif /* Include guard */
