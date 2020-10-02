#include "util.h"
#include "esearch.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>
#include <time.h>

/**
 * @brief Appends the given relpath to the home directory path
 * @param relpath relative path "~/{relpath}"
 * @return Allocated char* with the full path or NULL on error, it needs to be freed afterwards
 */
char* getHomePath(const char* relpath) {
	struct passwd *pw = getpwuid(getuid());
	if (pw == NULL)
		return NULL;

	const char *homedir = pw->pw_dir;
	if (homedir == NULL)
		return NULL;

	char* fullpath = malloc(strlen(homedir) + strlen(relpath) + 1);
	if (fullpath == NULL)
		return NULL;
	strcpy(fullpath, homedir);
	strcat(fullpath, relpath);

	return fullpath;
}

void help() {
	printf("\
pacman-esearch (%s) - Replacement for both pacman -Ss and pacman -Si\n\n\
esearch <regexp> [options]\n\
    --instonly, -I\tFind only packages which are installed\n\
    --notinst, -N\tFind only packages which are NOT installed\n\
    --nocolor, -n\tDon't use ANSI codes for colored output\n\
    --exact-match, -e\tShow only exact match\n\
    --nowarndb, -w\tDo not complain about database age\n\
    --version, -v\tShow version\n\
    --help, -h\t\tShow this message\n\n\
", VERSION);
}

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
