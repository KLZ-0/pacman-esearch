#ifndef ESEARCH_DATABASE_H
#define ESEARCH_DATABASE_H

#include <stdio.h>
#include <stdint.h>
#include <regex.h>

void db_age_check(char *db_filename, uint8_t arg_opts);
int db_get_header(FILE *db, uint8_t *indent_size);
int db_traverse(FILE *db, uint8_t arg_opts, const regex_t *regexp);

#endif //ESEARCH_DATABASE_H
