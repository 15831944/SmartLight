#ifndef _DEBUG_DEG_H
#define _DEBUG_DEG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

extern int g_debug_level;

#ifndef _NO_DEBUG_INFO
	int debug_printf(int debug_level, const char *format, ...);
#else
#	define debug_printf(void)
#endif

#define dfl_nonsense		5
#define dfl_lowclass		4
#define dfl_sensive			3
#define dfl_caution			2
#define dfl_emergency		1
#define dfl_hurt_me_plenty	0

#ifdef __cplusplus
}
#endif

#endif
