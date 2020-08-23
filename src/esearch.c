#include "error.h"
#include "esearch.h"
#include "util.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	char* fullpath = getHomePath(INDEX);
	printf("Index: %s\n", fullpath);
	free(fullpath);

	char* installedpath = getHomePath(INDEX_INSTALLED);
	printf("Index: %s\n", installedpath);
	free(installedpath);

	return 0;
}
