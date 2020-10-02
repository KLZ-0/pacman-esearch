#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "database.h"
#include "def.h"
#include "error.h"
#include "colors.h"

void dbAgeCheck(char* db_filename, uint8_t arg_opts) {
	if (isFlag(arg_opts, FLAG_NOWARNDB)) {
		return;
	}

	struct stat buf;
	stat(db_filename, &buf);
	time_t dbcreation = buf.st_ctime;
	time_t now;
	time(&now);

	time_t dbage = now-dbcreation;
	if (dbage > DB_WARN_AGE) { // 7 days
		warn("You should run eupdatedb, the last update was %lu days ago - on %s", dbage/SEC_DAY, ctime(&dbcreation));
		info("to disable this message run esearch with -w\n");
	}
}

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
				printf("%s*%s  %s%s%s%s\n", COLOR_BOLDGREEN, COLOR_BOLD, pkg_name, COLOR_BOLDRED, (installed) ? " [ installed ]" : "", COLOR_RESET);
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
