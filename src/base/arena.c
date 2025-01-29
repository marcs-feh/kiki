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

bool arena_init_dynamic(Arena* a, ArenaDynamicAllocFunc fn){
	panic("Unimplemented");
}

bool arena_init_virtual(Arena* a, Size max_size){
	if(a == NULL || max_size <= 0){ return false; }
	mem_set(a, 0, sizeof(*a));

	max_size = align_forward_size(max_size, ARENA_VIRTUAL_BLOCK_SIZE);

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

static
void *_arena_alloc_static(Arena* a, Size size, Size align){
	Uintptr base = (Uintptr)a->data;
	Uintptr current = (Uintptr)base + (Uintptr)a->offset;

	Size available = a->capacity - (current - base);
	Size required = arena_required_mem(current, size, align);

	if(required > available){
		return NULL;
	}

	a->offset += required;
	void* allocation = &a->data[a->offset - size];
	a->last_allocation = (Uintptr)allocation;
	return allocation;
}

static
void* _arena_alloc_virtual(Arena* a, Size size, Size align){
	Uintptr base = (Uintptr)a->data;
	Uintptr current = (Uintptr)base + (Uintptr)a->offset;

	Size maximum = (Uintptr)a->capacity - (current - base);
	Size required = arena_required_mem(current, size, align);
	Size commited = VIRTUAL_BLOCK_SIZE * a->commited_blocks;

	if(required > maximum){
		return NULL;
	}
	if(required > commited){
		Size aligned_size = align_forward_size(required, VIRTUAL_BLOCK_SIZE);
		virtual_commit(&a->data[commited], aligned_size);
		a->commited_blocks += aligned_size / VIRTUAL_BLOCK_SIZE;
	}

	a->offset += required;
	void* allocation = &a->data[a->offset - size];
	a->last_allocation = (Uintptr)allocation;
	return allocation;
}

void *arena_alloc(Arena* a, Size size, Size align){
	enum ArenaKind kind = a->kind;
	switch(kind){
		case ArenaKind_Static: return _arena_alloc_static(a, size, align);
		case ArenaKind_Virtual: return _arena_alloc_virtual(a, size, align);
		case ArenaKind_Dynamic: panic("Unimplemented");
	}
        return NULL;
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
