#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "error.h"

void warn(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);

	fprintf(stderr,"warning: ");
	vfprintf(stderr, fmt, args);
}

void error_exit(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);

	fprintf(stderr,"error: ");
	vfprintf(stderr, fmt, args);

	exit(EXIT_FAILURE);
}
