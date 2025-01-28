#include "base.h"

//// UTF-8 /////////////////////////////////////////////////////////////////////
typedef struct UTF8Encode UTF8Encode;
typedef struct UTF8Decode UTF8Decode;
typedef struct UTF8Iterator UTF8Iterator;

// UTF-8 encoding result, a len = 0 means an error.
struct UTF8Encode {
	U8 bytes[4];
	i8 len;
};

// UTF-8 encoding result, a len = 0 means an error.
struct UTF8Decode {
	rune codepoint;
	i8 len;
};

// The error rune
#define UTF8_ERROR ((rune)(0xfffd))

// The error rune, U8 encoded
static const UTF8Encode UTF8_ERROR_ENCODED = {
	.bytes = {0xef, 0xbf, 0xbd},
	.len = 0,
};

// Encode a unicode rune
UTF8Encode utf8_encode(rune c);

// Decode a rune from a UTF8 buffer of bytes
UTF8Decode utf8_decode(U8 const* data, Size len);

// Allows to iterate a stream of bytes as a sequence of runes
struct UTF8Iterator {
	U8 const* data;
	Size data_length;
	Size current;
};

// Steps iterator forward and puts rune and Length advanced into pointers,
// returns false when finished.
bool utf8_iter_next(UTF8Iterator* iter, rune* r, i8* len);

// Steps iterator backward and puts rune and its length into pointers,
// returns false when finished.
bool utf8_iter_prev(UTF8Iterator* iter, rune* r, i8* len);

//// Strings ///////////////////////////////////////////////////////////////////
typedef struct String String;

#define str_lit(CstrLit) (String){ .data = (U8 const*)(CstrLit), .len = (sizeof(CstrLit) - 1) }

struct String {
	U8 const * data;
	Size _length;
};

static inline
Size cstring_len(char const* cstr){
	static const Size CSTR_MAX_LENGTH = (~(u32)0) >> 1;
	Size size = 0;
	for(Size i = 0; i < CSTR_MAX_LENGTH && cstr[i] != 0; i += 1){
		size += 1;
	}
	return size;
}

// Create substring from a cstring
String str_from(char const* data);

// Create substring from a piece of a cstring
String str_from_range(char const* data, Size start, Size length);

// Create substring from a raw slice of bytes
String str_from_bytes(U8 const* data, Size length);

// Get a sub string, starting at `start` with `length`
String str_sub(String s, Size start, Size length);

// Get how many codeponits are in a string
Size str_codepoint_count(String s);

// Get the U8 offset of the n-th codepoint
Size str_codepoint_offset(String s, Size n);

// Clone a string
// String str_clone(String s, Mem_Allocator allocator);

// Destroy a string
// void str_destroy(String s, Mem_Allocator allocator);

// Concatenate 2 strings
// String str_concat(String a, String b, Mem_Allocator allocator);

// Check if 2 strings are equal
bool str_eq(String a, String b);

// Trim leading codepoints that belong to the cutset
String str_trim_leading(String s, String cutset);

// Trim trailing codepoints that belong to the cutset
String str_trim_trailing(String s, String cutset);

// Trim leading and trailing codepoints
String str_trim(String s, String cutset);

// Check if string starts with a prefix
bool str_starts_with(String s, String prefix);

// Check if string ends with a suffix
bool str_ends_with(String s, String suffix);

// Get an utf8 iterator from string
UTF8Iterator str_iterator(String s);

// Get an utf8 iterator from string, already at the end, to be used for reverse iteration
UTF8Iterator str_iterator_reversed(String s);

// Is string empty?
bool str_empty(String s);

