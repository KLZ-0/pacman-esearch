#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <regex.h>

#include "error.h"
#include "def.h"
#include "util.h"
#include "database.h"

// colors
char *COLOR_BOLD = "\033[0;1m";
char *COLOR_BOLDRED = "\033[1;31m";
char *COLOR_BOLDGREEN = "\033[1;32m";
char *COLOR_LIGHTGREEN = "\033[0;32m";
char *COLOR_INFO = "\033[1;33m";
char *COLOR_WARN = "\033[1;35m";
char *COLOR_ERROR = "\033[1;31m";
char *COLOR_RESET = "\033[0;0m";

int esearch(int argc, char *argv[], char **db_filename, FILE **db, regex_t *regexp) {
	int ret = EXIT_SUCCESS;

	// argument parsing
	uint8_t arg_opts = 0;
	char pattern[PATTERN_LEN_MAX] = {0};
	ret = parseArgs(argc, argv, &arg_opts, pattern);
	if (ret != INT_MAX) {
		return EXIT_FAILURE;
	}

	// query database path
	*db_filename = getHomePath(DATABASE);
	if (*db_filename == NULL) {
		error("Home directory not found, is your $HOME set?\n");
		return EXIT_FAILURE;
	}

	// open database
	*db = fopen(*db_filename, "r");
	if (*db == NULL) {
		error("Failed to open database, did you run eupdatedb?\n");
		ret = EXIT_FAILURE;
	}

	// compile regex
	ret = regcomp(regexp, pattern, REG_ICASE);
	if (ret != EXIT_SUCCESS) {
		error("Could not compile regex\n");
		return EXIT_FAILURE;
	}

	// traverse database
	printf("[ Results for search key : %s%s%s ]\n\n", COLOR_BOLD, pattern, COLOR_RESET);
	ret = traverseDB(*db, arg_opts, regexp);
	dbAgeCheck(*db_filename, arg_opts);

	return ret;
}

int main(int argc, char *argv[]) {
	int ret = EXIT_SUCCESS;
	char *db_filename = NULL;
	FILE *db = NULL;
	regex_t regexp = {0};

	ret = esearch(argc, argv, &db_filename, &db, &regexp);

	// cleanup
	free(db_filename);
	if (db != NULL) { fclose(db); }
	regfree(&regexp);

	return ret;
}
