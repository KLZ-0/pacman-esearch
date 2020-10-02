#include "error.h"
#include "esearch.h"
#include "util.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <regex.h>

// colors
char *COLOR_IMPORTANT = "\033[1;31m";
char *COLOR_BOLD = "\033[0;1m";
char *COLOR_BOLDGREEN = "\033[1;32m";
char *COLOR_LIGHTGREEN = "\033[0;32m";
char *COLOR_INFO = "\033[1;33m";
char *COLOR_WARN = "\033[1;35m";
char *COLOR_ERROR = "\033[1;31m";
char *COLOR_RESET = "\033[0;0m";

int getDBheader(FILE *db, uint8_t *indent_size) {
	// return code buffer
	size_t ret;

	// read db header indicating the indent character (':') posistion
	ret = fread(indent_size, sizeof(uint8_t), 1, db);
	if (ret != 1) {
		error("Failed to read from database, did you run eupdatedb?\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int traverseDB(FILE *db, uint8_t arg_opts, const regex_t *regexp) {
	uint8_t indent_size;
	if (getDBheader(db, &indent_size) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	char line_buf[LINE_BUFFER_SIZE];
	bool pkg_print = false;

	while (fgets(line_buf, LINE_BUFFER_SIZE, db) != NULL) {
		// '-' signals a new package
		if (line_buf[0] == '-') {
			bool installed = line_buf[1] - '0';
			char *pkg_name = strtok(line_buf+2, "\n");
			pkg_print = regexec(regexp, pkg_name, 0, NULL, 0) == EXIT_SUCCESS;
			if ((isFlag(arg_opts, FLAG_INST) && !installed) || (isFlag(arg_opts, FLAG_NOINST) && installed)) {
				pkg_print = false;
			}

			if (pkg_print) {
				printf("%s*%s  %s%s%s%s\n", COLOR_BOLDGREEN, COLOR_BOLD, pkg_name, COLOR_IMPORTANT, (installed) ? " [ installed ]" : "", COLOR_RESET);
			}
		} else if (pkg_print) {
			// test if last line
			if (line_buf[0] == '\n') {
				printf("\n");
			} else {
				printf("      %s%.*s%s%s%s", COLOR_LIGHTGREEN, indent_size, line_buf, COLOR_BOLD, line_buf + indent_size, COLOR_RESET);
			}
		}
	}

	return EXIT_SUCCESS;
}

int parseArgs(int argc, char *argv[], uint8_t *arg_opts, char *pattern) {
	char *tmp_pattern = NULL;

	if (argc < 2) { help(); return EXIT_SUCCESS; }
	for (int opti = 1; opti < argc ; opti++) {
		char *option = argv[opti];
		if (option[0] == '-' && option[1] == '-') {
			if (strcmp(option, "--instonly") == 0) setFlag(*arg_opts, FLAG_INST);
			else if (strcmp(option, "--notinst") == 0) setFlag(*arg_opts, FLAG_NOINST);
			else if (strcmp(option, "--nocolor") == 0) setFlag(*arg_opts, FLAG_NOCOLOR);
			else if (strcmp(option, "--nowarndb") == 0) setFlag(*arg_opts, FLAG_NOWARNDB);
			else if (strcmp(option, "--exact-match") == 0) setFlag(*arg_opts, FLAG_EXACT);
			else if (strcmp(option, "--version") == 0) { printf("%s\n", VERSION); return EXIT_SUCCESS; }
			else if (strcmp(option, "--help") == 0) { help(); return EXIT_SUCCESS; }
			else { error("unknown option! see --help for all options\n"); return EXIT_FAILURE; }
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
						error("unknown option! see --help for all options\n");
						return EXIT_FAILURE;
				}
			}
		}
		else {
			tmp_pattern = argv[opti];
		}
	}

	// NOTE: This doesn't trigger when the pattern is "" -> will print every package (this behavior is intended)
	if (tmp_pattern == NULL) {
		error("Pattern not found, check arguments..\n");
		return EXIT_FAILURE;
	}

	if (isFlag(*arg_opts, FLAG_EXACT)) {
		char *lastchar = memccpy(pattern+1, tmp_pattern, '\0', PATTERN_LEN_MAX-3);
		if (lastchar == NULL) {
			error("Pattern too long, set PATTERN_LEN_MAX to an appropriate value and recompile esearch");
			return EXIT_FAILURE;
		}
		pattern[0] = '^';
		*(lastchar-1) = '$';
	} else {
		memccpy(pattern, tmp_pattern, '\0', PATTERN_LEN_MAX-1);
	}

	if (isFlag(*arg_opts, FLAG_NOCOLOR)) {
		COLOR_IMPORTANT = COLOR_BOLD = COLOR_BOLDGREEN = COLOR_LIGHTGREEN = COLOR_INFO = COLOR_WARN = COLOR_ERROR = COLOR_RESET = "";
	}

	return INT_MAX;
}

int main(int argc, char *argv[]) {
	int ret = EXIT_SUCCESS;
	FILE *db = NULL;
	char *db_filename = NULL;
	regex_t regexp = {0};

	db_filename = getHomePath(INDEX);

	uint8_t arg_opts = 0;
	char pattern[PATTERN_LEN_MAX] = {0};

	ret = parseArgs(argc, argv, &arg_opts, pattern);
	if (ret != INT_MAX) {
		goto cleanup;
	}

	db = fopen(db_filename, "r");
	if (db == NULL) {
		error("Failed to open database, did you run eupdatedb?\n");
		ret = EXIT_FAILURE;
		goto cleanup;
	}

	ret = regcomp(&regexp, pattern, REG_ICASE);
	if (ret != EXIT_SUCCESS) {
		error("Could not compile regex\n");
		goto cleanup;
	}

	printf("[ Results for search key : %s%s%s ]\n\n", COLOR_BOLD, pattern, COLOR_RESET);
	ret = traverseDB(db, arg_opts, &regexp);
	dbAgeCheck(db_filename, arg_opts);

	cleanup:
	free(db_filename);
	if (db != NULL) { fclose(db); }
	regfree(&regexp);

	return ret;
}
