#ifndef _memory_h_include_
#define _memory_h_include_

#include "base.h"

#define mem_new(Type, Num, Alloc) mem_alloc((Alloc), sizeof(Type) * (Num), alignof(Type));

#define KiB (1024ll)
#define MiB (1024ll * 1024ll)
#define GiB (1024ll * 1024ll * 1024ll)

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

static inline
bool mem_valid_alignment(Size align){
	return (align & (align - 1)) == 0 && (align != 0);
}

#endif /* Include guard */
