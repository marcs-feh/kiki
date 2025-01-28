#include "base.h"
#include "memory.h"

#if !defined(__clang__) && !defined(__GNUC__)
#include <string.h>
#define mem_set_impl             memset
#define mem_copy_impl            memmove
#define mem_copy_no_overlap_impl memcpy
#define mem_compare_impl         memcmp
#else
#define mem_set_impl             __builtin_memset
#define mem_copy_impl            __builtin_memmove
#define mem_copy_no_overlap_impl __builtin_memcpy
#define mem_compare_impl         __builtin_memcmp
#endif

static inline
bool mem_valid_alignment(Size align){
	return (align & (align - 1)) == 0 && (align != 0);
}

void mem_set(void* p, U8 val, Size count){
	mem_set_impl(p, val, count);
}

void mem_copy(void* dest, void const * src, Size count){
	mem_copy_impl(dest, src, count);
}

void mem_copy_no_overlap(void* dest, void const * src, Size count){
	mem_copy_no_overlap_impl(dest, src, count);
}

I32 mem_compare(void const * a, void const * b, Size count){
	return mem_compare_impl(a, b, count);
}

Uintptr align_forward_ptr(Uintptr p, Uintptr a){
	ensure(mem_valid_alignment(a), "Invalid memory alignment");
	Uintptr mod = p & (a - 1);
	if(mod > 0){
		p += (a - mod);
	}
	return p;
}

Size align_forward_size(Size p, Size a){
	ensure(a > 0, "Invalid size alignment");
	Size mod = p % a;
	if(mod > 0){
		p += (a - mod);
	}
	return p;
}

Uintptr arena_required_mem(Uintptr cur, Size nU8s, Size align){
	ensure(mem_valid_alignment(align), "Alignment must be a power of 2");
	Uintptr aligned  = align_forward_ptr(cur, align);
	Uintptr padding  = (Uintptr)(aligned - cur);
	Uintptr required = padding + nU8s;
	return required;
}

void *arena_alloc(Arena* a, Size size, Size align){
	Uintptr base = (Uintptr)a->data;
	Uintptr current = (Uintptr)base + (Uintptr)a->offset;

	Uintptr available = (Uintptr)a->capacity - (current - base);
	Uintptr required = arena_required_mem(current, size, align);

	if(required > available){
		return NULL;
	}

	a->offset += required;
	void* allocation = &a->data[a->offset - size];
	a->last_allocation = (Uintptr)allocation;
	return allocation;
}

void arena_free_all(Arena* a){
	a->offset = 0;
}

void* arena_resize(Arena* a, void* ptr, Size new_size){
	if((Uintptr)ptr == a->last_allocation){
		Uintptr base = (Uintptr)a->data;
		Uintptr current = base + (Uintptr)a->offset;
		Uintptr limit = base + (Uintptr)a->capacity;
		Size last_allocation_size = current - a->last_allocation;

		if((current - last_allocation_size + new_size) > limit){
			return NULL; /* No space left*/
		}

		a->offset += new_size - last_allocation_size;
		return ptr;
	}

	return NULL;
}

void arena_init(Arena* a, U8* data, Size len){
	a->capacity = len;
	a->data = data;
	a->offset = 0;
}

void arena_destroy(Arena* a){
	arena_free_all(a);
	a->capacity = 0;
	a->data = NULL;
}

#undef mem_set_impl
#undef mem_copy_impl
#undef mem_copy_no_overlap_impl
#undef mem_compare_impl
