#ifndef ESEARCH_DATABASE_H
#define ESEARCH_DATABASE_H

#include <stdio.h>
#include <stdint.h>
#include <regex.h>

void dbAgeCheck(char* db_filename, uint8_t arg_opts);
int getDBheader(FILE *db, uint8_t *indent_size);
int traverseDB(FILE *db, uint8_t arg_opts, const regex_t *regexp);

#endif //ESEARCH_DATABASE_H
