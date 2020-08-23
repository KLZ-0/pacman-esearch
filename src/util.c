#include "util.h"

#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>

/**
 * @brief Appends the given relpath to the home directory path
 * @param relpath relative path "~/{relpath}"
 * @return Allocated char* with the full path, it needs to be freed afterwards
 */
char* getHomePath(const char* relpath) {
	struct passwd *pw = getpwuid(getuid());
	const char *homedir = pw->pw_dir;

	char* fullpath = malloc(strlen(homedir) + strlen(relpath) + 1);
	strcpy(fullpath, homedir);
	strcat(fullpath, relpath);

	return fullpath;
}
