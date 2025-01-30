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
	ensure(mem_valid_alignment(a), "Invalid size alignment");
	Size mod = p & (a - 1);
	if(mod > 0){
		p += (a - mod);
	}
	return p;
}


#undef mem_set_impl
#undef mem_copy_impl
#undef mem_copy_no_overlap_impl
#undef mem_compare_impl
