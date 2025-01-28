
#include "memory.c"
#include "strings.c"

#if defined(TARGET_OS_LINUX)
	#include "os_linux.c"
#elif defined(TARGET_OS_WINDOWS)
	#include "os_windows.c"
#endif
