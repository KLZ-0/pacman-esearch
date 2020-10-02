#ifndef ESEARCH_DEF_H
#define ESEARCH_DEF_H

/*
 * config
 */

// version
#define VERSION "4.3.5"

// esearch database
// must match "index_location" in eupdatedb
#define DATABASE "/.cache/esearch/index"

// max length of the regexp search pattern
#define PATTERN_LEN_MAX 256

// size of line buffer used when traversing the database
#define LINE_BUFFER_SIZE 2048

// warn the user if the database age exceeds this value
// the value is in seconds
#define DB_WARN_AGE 604800 // 7 days

/*
 * defines
 */

// seconds in a day
#define SEC_DAY 86400

// parser related stuff
#define FLAG_INST 0u
#define FLAG_NOINST 1u
#define FLAG_NOCOLOR 2u
#define FLAG_NOWARNDB 3u
#define FLAG_EXACT 4u

/*
 * macros
 */

#define set_flag(flags, flag) (flags |= (1u << (flag)))
#define is_flag(flags, flag) ((flags >> (flag)) & 1u)

#endif //ESEARCH_DEF_H
