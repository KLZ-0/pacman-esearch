#ifndef ESEARCH_UTIL_H
#define ESEARCH_UTIL_H

#include <stdint.h>

char *getHomePath(const char *relpath);
void help();
int parseArgs(int argc, char *argv[], uint8_t *arg_opts, char *pattern);

#endif //ESEARCH_UTIL_H
