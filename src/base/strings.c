#include "base.h"
#include "strings.h"

bool str_empty(String s){
	return s.len == 0 || s.v == NULL;
}

String str_from(char const * data){
	String s = {
		.v = (U8 const *)data,
		.len = cstring_len(data),
	};
	return s;
}

String str_from_bytes(U8 const* data, Size length){
	String s = {
		.v = (U8 const *)data,
		.len = length,
	};
	return s;
}

String str_from_range(char const * data, Size start, Size length){
	String s = {
		.v = (U8 const *)&data[start],
		.len = length,
	};
	return s;
}

Size str_codepoint_count(String s){
	UTF8Iterator it = str_iterator(s);

	Size count = 0;
	Rune c; I8 len;
	while(utf8_iter_next(&it, &c, &len)){
		count += 1;
	}
	return count;
}

bool str_starts_with(String s, String prefix){
	if(prefix.len > s.len){ return false; }

	s = str_sub(s, 0, prefix.len);

	I32 res = mem_compare(prefix.v, s.v, prefix.len);
	return res == 0;
}

bool str_ends_with(String s, String suffix){
	if(suffix.len > s.len){ return false; }

	s = str_sub(s, s.len - suffix.len, suffix.len);

	I32 res = mem_compare(suffix.v, s.v, suffix.len);
	return res == 0;
}

Size str_codepoint_offset(String s, Size n){
	UTF8Iterator it = str_iterator(s);

	Size acc = 0;

	Rune c; I8 len;
	do {
		if(acc == n){ break; }
		acc += 1;
	} while(utf8_iter_next(&it, &c, &len));

	return it.current;
}

// TODO: Handle length in codepoint count
String str_sub(String s, Size start, Size byte_count){
	if(start < 0 || byte_count < 0 || (start + byte_count) > s.len){ return (String){0}; }

	String sub = {
		.v = &s.v[start],
		.len = byte_count,
	};

	return sub;
}

bool str_eq(String a, String b){
	if(a.len != b.len){ return false; }

	for(Size i = 0; i < a.len; i += 1){
		if(a.v[i] != b.v[i]){ return false; }
	}

	return true;
}

UTF8Iterator str_iterator(String s){
	return (UTF8Iterator){
		.current = 0,
		._length = s.len,
		.v = s.v,
	};
}

UTF8Iterator str_iterator_reversed(String s){
	return (UTF8Iterator){
		.current = s.len,
		.v_length = s.len,
		.v = s.v,
	};
}

#define MAX_CUTSET_LEN 64

String str_trim(String s, String cutset){
	String st = str_trim_leading(str_trim_trailing(s, cutset), cutset);
	return st;
}

String str_trim_leading(String s, String cutset){
	ensure(cutset.len <= MAX_CUTSET_LEN, "Cutset string exceeds MAX_CUTSET_LEN");

	Rune set[MAX_CUTSET_LEN] = {0};
	Size set_len = 0;
	Size cut_after = 0;

	decode_cutset: {
		Rune c; I8 n;
		UTF8Iterator iter = str_iterator(cutset);

		Size i = 0;
		while(utf8_iter_next(&iter, &c, &n) && i < MAX_CUTSET_LEN){
			set[i] = c;
			i += 1;
		}
		set_len = i;
	}

	strip_cutset: {
		Rune c; I8 n;
		UTF8Iterator iter = str_iterator(s);

		while(utf8_iter_next(&iter, &c, &n)){
			bool to_be_cut = false;
			for(Size i = 0; i < set_len; i += 1){
				if(set[i] == c){
					to_be_cut = true;
					break;
				}
			}

			if(to_be_cut){
				cut_after += n;
			}
			else {
				break; // Reached first Rune that isn't in cutset
			}

		}
	}

	return str_sub(s, cut_after, s.len - cut_after);
}

String str_trim_trailing(String s, String cutset){
	ensure(cutset.len <= MAX_CUTSET_LEN, "Cutset string exceeds MAX_CUTSET_LEN");

	Rune set[MAX_CUTSET_LEN] = {0};
	Size set_len = 0;
	Size cut_until = s.len;

	decode_cutset: {
		UTF8Decode dec = {0};
		UTF8Iterator iter = str_iterator(cutset);

		Size i = 0;
		while(utf8_iter_next(&iter, &dec) && i < MAX_CUTSET_LEN){
			set[i] = dec.codepoint;
			i += 1;
		}
		set_len = i;
	}

	strip_cutset: {
		UTF8Decode dec = {0};
		UTF8Iterator iter = str_iterator_reversed(s);

		while(utf8_iter_prev(&iter, &dec)){
			bool to_be_cut = false;
			for(Size i = 0; i < set_len; i += 1){
				if(set[i] == dec.codepoint){
					to_be_cut = true;
					break;
				}
			}

			if(to_be_cut){
				cut_until -= dec.len;
			}
			else {
				break; // Reached first Rune that isn't in cutset
			}

		}
	}

	return str_sub(s, 0, cut_until);
}
