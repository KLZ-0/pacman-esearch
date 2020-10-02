#ifndef ESEARCH_UTIL_H
#define ESEARCH_UTIL_H

#include <stdint.h>

char* getHomePath(const char* relpath);
void help();
void dbAgeCheck(char* db_filename, uint8_t arg_opts);

#endif //ESEARCH_UTIL_H
