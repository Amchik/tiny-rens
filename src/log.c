#include <stdio.h>
#include <stdarg.h>

#include "include/log.h"

static const char *LOGF_FMT[] = {
	"", /* prim */
	"\033[0;1m[\033[1;31mERROR\033[1;0m] \033[0m", /* err */
	"\033[0;1m[\033[1;33mWARNING\033[1;0m] \033[0m", /* warn */
	"\033[0;1m[\033[1;34mINFO\033[1;0m] \033[0m", /* info */
	"\033[0;1m[\033[1;35mDEBUG\033[1;0m] \033[0m", /* debug */
};

void logf(int log_level, const char *fmt, ...) {
	va_list va;

	assert_debug(log_level >= 0, "log_level should be non-negative");
	assert_debug(log_level < sizeof(LOGF_FMT), "Invalid log_level");

	if (log_level > LOG_LEVEL)
		return;

	fputs(LOGF_FMT[log_level], stderr);

	va_start(va, fmt);
	vfprintf(stderr, fmt, va);
	va_end(va);

	fputc('\n', stderr);
}

