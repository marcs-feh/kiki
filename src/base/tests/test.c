#include "../base.h"
#include "test.h"
#include "../memory.h"
#include "../virtual_memory.h"
#include "../arena.h"

static inline
void arena_buf_test(){
    TEST_BEGIN("Arena (Buffer)");
    static U8 memory[256];
    Arena arena = {0};
    arena_init_buffer(&arena, memory, sizeof(memory));

    U32* n = arena_alloc(&arena, sizeof(U32), alignof(U32));
    Test(arena.offset == 4);
    Test(n != NULL);

    while(arena_alloc(&arena, 1, 1)){}
    Test(arena.offset == arena.capacity);
    Test(!arena_alloc(&arena, 1, 1));

    arena_free_all(&arena);
    n = arena_alloc(&arena, 20, 4);
    Test(n != NULL);
    n = arena_resize(&arena, n, 64);
    Test(n != NULL);
    arena_alloc(&arena, 1, 1);
    n = arena_resize(&arena, n, 64);
    Test(n == NULL);
    TEST_END;
}

static inline
void arena_virt_test(){
    TEST_BEGIN("Arena (Virtual)");
    TEST_END;
}
#include <stdlib.h>
int main(){
    arena_buf_test();
    arena_virt_test();
}