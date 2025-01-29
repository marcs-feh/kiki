#ifndef _base_h_include_
#define _base_h_include_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdalign.h>
#include <stdnoreturn.h>
#include <stdatomic.h>
#include <limits.h>

#define nil NULL

typedef int8_t  I8;
typedef int16_t I16;
typedef int32_t I32;
typedef int64_t I64;

typedef uint8_t  U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef ptrdiff_t Size; // Signed size
typedef uintptr_t Uintptr;

typedef U32 Rune;

typedef float  F32;
typedef double F64;

// This is to avoid conflict with stdlib's "abs()"
#define abs_val(X) (( (X) < 0ll) ? -(X) : (X))

#define min(A, B)  (((A) < (B)) ? (A) : (B))
#define max(A, B)  (((A) > (B)) ? (A) : (B))

#define clamp(Lo, X, Hi) min(max(Lo, X), Hi)

#define container_of(Ptr, Type, Member) \
	((Type *)(((void *)(Ptr)) - offsetof(Type, Member)))

#ifndef __cplusplus
#undef bool
typedef _Bool bool;
#define static_assert(Pred, Msg) _Static_assert(Pred, Msg)
#endif

static_assert(sizeof(F32) == 4 && sizeof(F64) == 8, "Bad float size");
static_assert(sizeof(void(*)(void)) == sizeof(void*), "Function pointers and data pointers must be of the same width");
static_assert(sizeof(void(*)(void)) == sizeof(Uintptr), "Mismatched pointer types");
static_assert(sizeof(Size) == sizeof(size_t), "Mismatched size");
static_assert(CHAR_BIT == 8, "Invalid char size");

static inline noreturn
void panic() {
	__builtin_trap();
}

static inline
void ensure(bool pred, char const * msg){
	if(!(pred)){
		panic();
	}
}

typedef struct String String;

#define StrLit(cslit) (String){ .data = (byte const*)(cslit), .len = (sizeof(cslit) - 1) }

struct String {
	U8 const * v;
	Size len;
};

#endif /* Include guard */
