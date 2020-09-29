#ifndef ESEARCH_ESEARCH_H
#define ESEARCH_ESEARCH_H

/*
 * config
 */

#define VERSION "4.1.0"
#define INDEX "/.cache/esearch/index"
#define PATTERN_LEN_MAX 256

/*
 * defines
 */

#define FLAG_INST 0u
#define FLAG_NOINST 1u
#define FLAG_NOCOLOR 2u
#define FLAG_NOWARNDB 3u
#define FLAG_EXACT 4u

#define PARSE_OK (UINT8_MAX - 1)
#define PARSE_ERR (UINT8_MAX)

/*
 * macros
 */

#define setFlag(flags, flag) (flags |= (1u << (flag)))
#define isFlag(flags, flag) ((flags >> (flag)) & 1u)

#endif //ESEARCH_ESEARCH_H
