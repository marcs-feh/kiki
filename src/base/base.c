#include "strings.c"
#include "memory.c"
#include "arena.c"

#if defined(TARGET_OS_LINUX)
	#include "os_linux.c"
	#include "virtual_memory_linux.c"
#elif defined(TARGET_OS_WINDOWS)
	#include "os_windows.c"
	#include "virtual_memory_windows.c"
#endif

