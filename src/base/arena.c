#include "arena.h"
#include "memory.h"
#include "virtual_memory.h"

bool arena_init_buffer(Arena* a, U8* data, Size len){
	if(a == NULL || len <= 0){ return false; }

	mem_set(a, 0, sizeof(*a));
	a->data     = data;
	a->capacity = len;
	a->kind     = ArenaKind_Static;
	return false;
}

// void arena_init_dynamic(Arena* a, ArenaDynamicAllocFunc fn){
// 	mem_set(a, 0, sizeof(*a));
// 	a->capacity = 0;
// 	a->data = NULL;
// 	a->offset = 0;
// 	a->kind = ArenaKind_Static;
// 	a->commited_blocks = 0;
// 	a->next = NULL;
// }

bool arena_init_virtual(Arena* a, Size max_size){
	if(a == NULL || max_size <= 0){ return false; }
	mem_set(a, 0, sizeof(*a));

	a->data = virtual_reserve(max_size);
	a->capacity = max_size;
	a->kind = ArenaKind_Virtual;
	virtual_commit(a->data, VIRTUAL_BLOCK_SIZE);
	a->commited_blocks += 1;

	return true;
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

void arena_destroy(Arena* a){
	arena_free_all(a);
	a->capacity = 0;
	a->data = NULL;
}
