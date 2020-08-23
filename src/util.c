#include "util.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

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
