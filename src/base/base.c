// Core translation unit

#include "strings.c"
#include "memory.c"
#include "arena.c"

#if defined(TARGET_OS_LINUX)
	#include "virtual_memory_linux.c"
	#include "virtual_memory.c"
#elif defined(TARGET_OS_WINDOWS)
	#include "virtual_memory_windows.c"
	#include "virtual_memory.c"

#else
	#error "Target OS not defined."
#endif

