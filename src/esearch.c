#include "error.h"
#include "esearch.h"
#include "util.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define FLAG_INST 0u
#define FLAG_NOINST 1u
#define FLAG_NOCOLOR 2u
#define FLAG_NOWARNDB 3u
#define FLAG_EXACT 4u

#define PARSE_OK (UINT8_MAX - 1)
#define PARSE_ERR (UINT8_MAX)

#define setFlag(flags, flag) (flags |= (1u << (flag)))
#define isFlag(flags, flag) ((flags >> (flag)) & 1u)


uint8_t parseArgs(int argc, char *argv[]) {
	uint8_t arg_opts = 0;

	char *option;
	char pattern[PATTERN_LEN_MAX];

	if (argc < 2) { help(); return PARSE_OK; }
	for (int opti = 1; opti < argc ; opti++) {
		option = argv[opti];
		if (option[0] == '-' && option[1] == '-') {
			if (strcmp(option, "--instonly") == 0) setFlag(arg_opts, FLAG_INST);
			else if (strcmp(option, "--notinst") == 0) setFlag(arg_opts, FLAG_NOINST);
			else if (strcmp(option, "--nocolor") == 0) setFlag(arg_opts, FLAG_NOCOLOR);
			else if (strcmp(option, "--nowarndb") == 0) setFlag(arg_opts, FLAG_NOWARNDB);
			else if (strcmp(option, "--exact-match") == 0) setFlag(arg_opts, FLAG_EXACT);
			else if (strcmp(option, "--version") == 0) { printf("%s\n", VERSION); return PARSE_OK; }
			else if (strcmp(option, "--help") == 0) { help(); return PARSE_OK; }
			else { printf("unknown option! see --help for all options\n"); return PARSE_ERR; }
		}
		else if (option[0] == '-') {
			for(size_t optc = 1; optc < strlen(option); optc++) {
				switch (option[optc]) {
					case 'I': setFlag(arg_opts, FLAG_INST); break;
					case 'N': setFlag(arg_opts, FLAG_NOINST); break;
					case 'n': setFlag(arg_opts, FLAG_NOCOLOR); break;
					case 'w': setFlag(arg_opts, FLAG_NOWARNDB); break;
					case 'e': setFlag(arg_opts, FLAG_EXACT); break;
					case 'v': printf("%s\n", VERSION); return PARSE_OK;
					case 'h': help(); return PARSE_OK;
					case '\0': break;

					default:
						printf("unknown option! see --help for all options\n");
						return PARSE_ERR;
				}
			}
		}
		else {
			strncpy(pattern, argv[opti], PATTERN_LEN_MAX-1);
		}
	}

	return arg_opts;
}

int main(int argc, char *argv[]) {
	char* indexpath = getHomePath(INDEX);

	uint8_t arg_opts = parseArgs(argc, argv);
	if (arg_opts == PARSE_OK) return EXIT_SUCCESS;
	else if (arg_opts == PARSE_ERR) return EXIT_FAILURE;
	printf("Here: %d\n", arg_opts);

	cleanup:
	free(indexpath);

	return 0;
}
