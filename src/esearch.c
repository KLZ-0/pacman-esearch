#include "error.h"
#include "esearch.h"
#include "util.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/types.h>
#include <regex.h>

// colors
char *COLOR_IMPORTANT = "\033[1;31m";
char *COLOR_HEADER = "\033[0;1m";
char *COLOR_BOLDGREEN = "\033[1;32m";
char *COLOR_RESET = "\033[0;0m";
char *COLOR_LIGHT = "\033[0;32m";

int getDBheader(FILE *db, uint8_t *indent_size) {
	// return code buffer
	size_t ret;

	// read db header indicating the indent character (':') posistion
	ret = fread(indent_size, sizeof(uint8_t), 1, db);
	if (ret != 1) {
		// TODO: Handle in error.h
		fprintf(stderr, "Failed to read from database, did you run eupdatedb?");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int traverseDB(FILE *db) {
	uint8_t indent_size;
	if (getDBheader(db, &indent_size) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	char line_buf[LINE_BUFFER_SIZE];
	bool installed = false;

	while (fgets(line_buf, LINE_BUFFER_SIZE, db) != NULL) {
		// '-' signals a new package
		if (line_buf[0] == '-') {
			installed = line_buf[1] - '0';
			printf("%s*%s %s%s", COLOR_BOLDGREEN, COLOR_HEADER, line_buf+2, COLOR_RESET);
		}
	}

	return EXIT_SUCCESS;
}

int parseArgs(int argc, char *argv[], uint8_t *arg_opts, char *pattern) {
	char *option;

	if (argc < 2) { help(); return EXIT_SUCCESS; }
	for (int opti = 1; opti < argc ; opti++) {
		option = argv[opti];
		if (option[0] == '-' && option[1] == '-') {
			if (strcmp(option, "--instonly") == 0) setFlag(*arg_opts, FLAG_INST);
			else if (strcmp(option, "--notinst") == 0) setFlag(*arg_opts, FLAG_NOINST);
			else if (strcmp(option, "--nocolor") == 0) setFlag(*arg_opts, FLAG_NOCOLOR);
			else if (strcmp(option, "--nowarndb") == 0) setFlag(*arg_opts, FLAG_NOWARNDB);
			else if (strcmp(option, "--exact-match") == 0) setFlag(*arg_opts, FLAG_EXACT);
			else if (strcmp(option, "--version") == 0) { printf("%s\n", VERSION); return EXIT_SUCCESS; }
			else if (strcmp(option, "--help") == 0) { help(); return EXIT_SUCCESS; }
			else { printf("unknown option! see --help for all options\n"); return EXIT_FAILURE; }
		}
		else if (option[0] == '-') {
			for(size_t optc = 1; optc < strlen(option); optc++) {
				switch (option[optc]) {
					case 'I': setFlag(*arg_opts, FLAG_INST); break;
					case 'N': setFlag(*arg_opts, FLAG_NOINST); break;
					case 'n': setFlag(*arg_opts, FLAG_NOCOLOR); break;
					case 'w': setFlag(*arg_opts, FLAG_NOWARNDB); break;
					case 'e': setFlag(*arg_opts, FLAG_EXACT); break;
					case 'v': printf("%s\n", VERSION); return EXIT_SUCCESS;
					case 'h': help(); return EXIT_SUCCESS;
					case '\0': break;

					default:
						printf("unknown option! see --help for all options\n");
						return EXIT_FAILURE;
				}
			}
		}
		else {
			strncpy(pattern, argv[opti], PATTERN_LEN_MAX-1);
		}
	}

	return INT_MAX;
}

int main(int argc, char *argv[]) {
	char* db_filename = getHomePath(INDEX);

	uint8_t arg_opts = 0;
	char pattern[PATTERN_LEN_MAX];

	int ret = parseArgs(argc, argv, &arg_opts, pattern);
	if (ret != INT_MAX) {
		return ret;
	}

	FILE *db = fopen(db_filename, "r");
	if (db == NULL) {
		// TODO: Handle in error.h
		fprintf(stderr, "Failed to open database, did you run eupdatedb?");
		return EXIT_FAILURE;
	}

	ret = traverseDB(db);

	cleanup:
	fclose(db);
	free(db_filename);

	return ret;
}
