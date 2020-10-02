#include "error.h"
#include "esearch.h"
#include "util.h"
#include "database.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
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

// TODO: Refactor
// TODO: Reformat

int main(int argc, char *argv[]) {
	int ret = EXIT_SUCCESS;
	FILE *db = NULL;
	char *db_filename = NULL;
	regex_t regexp = {0};

	// argument parsing
	uint8_t arg_opts = 0;
	char pattern[PATTERN_LEN_MAX] = {0};
	ret = parseArgs(argc, argv, &arg_opts, pattern);
	if (ret != INT_MAX) {
		goto cleanup;
	}

	// query database path
	db_filename = getHomePath(INDEX);
	if (db_filename == NULL) {
		error("Home directory not found, is your $HOME set?\n");
		ret = EXIT_FAILURE;
		goto cleanup;
	}

	// open database
	db = fopen(db_filename, "r");
	if (db == NULL) {
		error("Failed to open database, did you run eupdatedb?\n");
		ret = EXIT_FAILURE;
		goto cleanup;
	}

	// compile regex
	ret = regcomp(&regexp, pattern, REG_ICASE);
	if (ret != EXIT_SUCCESS) {
		error("Could not compile regex\n");
		goto cleanup;
	}

	// traverse database
	printf("[ Results for search key : %s%s%s ]\n\n", COLOR_BOLD, pattern, COLOR_RESET);
	ret = traverseDB(db, arg_opts, &regexp);
	dbAgeCheck(db_filename, arg_opts);

	// cleanup
	cleanup:
	free(db_filename);
	if (db != NULL) { fclose(db); }
	regfree(&regexp);

	return ret;
}
