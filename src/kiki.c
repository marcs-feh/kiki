#if defined(TARGET_OS_LINUX)
	#ifndef _XOPEN_SOURCE
	#define _XOPEN_SOURCE 800
	#endif
#elif defined(TARGET_OS_WINDOWS)
	#define WIN32_MEAN_AND_LEAN
#else
	#error "Target OS not specified."
#endif

#include "kiki.h"

#include "lexer.c"
#include "parser.c"
#include "checker.c"

