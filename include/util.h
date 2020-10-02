#ifndef ESEARCH_UTIL_H
#define ESEARCH_UTIL_H

#include <stdint.h>

char *append_home_path(const char *relpath);
void help();
int parse_args(int argc, char **argv, uint8_t *arg_opts, char *pattern);

#endif //ESEARCH_UTIL_H
