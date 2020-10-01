#include <stdio.h>
#include <stdarg.h>

#include "error.h"

extern char *COLOR_WARN;
extern char *COLOR_ERROR;
extern char *COLOR_RESET;

void warn(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);

	fprintf(stderr, "%swarning:%s ", COLOR_WARN, COLOR_RESET);
	vfprintf(stderr, fmt, args);
}

void error(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);

	fprintf(stderr, "%serror:%s ", COLOR_ERROR, COLOR_RESET);
	vfprintf(stderr, fmt, args);
}
