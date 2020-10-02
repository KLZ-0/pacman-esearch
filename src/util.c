#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <limits.h>

#include "util.h"
#include "def.h"
#include "error.h"
#include "colors.h"

/**
 * @brief Appends the given relpath to the home directory path
 * @param relpath relative path "~/{relpath}"
 * @return Allocated char* with the full path or NULL on error, it needs to be freed afterwards
 */
char *append_home_path(const char *relpath) {
	struct passwd *pw = getpwuid(getuid());
	if (pw == NULL)
		return NULL;

	const char *homedir = pw->pw_dir;
	if (homedir == NULL)
		return NULL;

	char *fullpath = malloc(strlen(homedir) + strlen(relpath) + 1);
	if (fullpath == NULL)
		return NULL;
	strcpy(fullpath, homedir);
	strcat(fullpath, relpath);

	return fullpath;
}

void help() {
	printf("pacman-esearch (%s) - Replacement for both pacman -Ss and pacman -Si\n\n"
		"Usage: esearch <regexp> [options]\n"
		"Options:\n"
		"    --verbose, -v\tPrint even more information about packages\n"
		"    --instonly, -I\tFind only packages which are installed\n"
		"    --notinst, -N\tFind only packages which are NOT installed\n"
		"    --nocolor, -n\tDon't use ANSI codes for colored output\n"
		"    --exact-match, -e\tShow only exact match\n"
		"    --nowarndb, -w\tDo not complain about database age\n"
		"    --version, -V\tShow version\n"
		"    --help, -h\t\tShow this message\n\n", VERSION);
}

int parse_args(int argc, char **argv, uint8_t *arg_opts, char *pattern) {
	char *tmp_pattern = NULL;

	if (argc < 2) {
		help();
		return EXIT_SUCCESS;
	}
	for (int opti = 1; opti < argc; opti++) {
		char *option = argv[opti];
		if (option[0] == '-' && option[1] == '-') {
			if (strcmp(option, "--verbose") == 0) set_flag(*arg_opts, FLAG_VERBOSE);
			else if (strcmp(option, "--instonly") == 0) set_flag(*arg_opts, FLAG_INST);
			else if (strcmp(option, "--notinst") == 0) set_flag(*arg_opts, FLAG_NOINST);
			else if (strcmp(option, "--nocolor") == 0) set_flag(*arg_opts, FLAG_NOCOLOR);
			else if (strcmp(option, "--nowarndb") == 0) set_flag(*arg_opts, FLAG_NOWARNDB);
			else if (strcmp(option, "--exact-match") == 0) set_flag(*arg_opts, FLAG_EXACT);
			else if (strcmp(option, "--version") == 0) {
				printf("%s\n", VERSION);
				return EXIT_SUCCESS;
			}
			else if (strcmp(option, "--help") == 0) {
				help();
				return EXIT_SUCCESS;
			}
			else {
				error("unknown option! see --help for all options\n");
				return EXIT_FAILURE;
			}
		} else if (option[0] == '-') {
			for (size_t optc = 1; optc < strlen(option); optc++) {
				switch (option[optc]) {
					case 'v':
						set_flag(*arg_opts, FLAG_VERBOSE);
						break;
					case 'I':
						set_flag(*arg_opts, FLAG_INST);
						break;
					case 'N':
						set_flag(*arg_opts, FLAG_NOINST);
						break;
					case 'n':
						set_flag(*arg_opts, FLAG_NOCOLOR);
						break;
					case 'w':
						set_flag(*arg_opts, FLAG_NOWARNDB);
						break;
					case 'e':
						set_flag(*arg_opts, FLAG_EXACT);
						break;
					case 'V':
						printf("%s\n", VERSION);
						return EXIT_SUCCESS;
					case 'h':
						help();
						return EXIT_SUCCESS;
					case '\0':
						break;

					default:
						error("unknown option! see --help for all options\n");
						return EXIT_FAILURE;
				}
			}
		} else {
			tmp_pattern = argv[opti];
		}
	}

	// NOTE: This doesn't trigger when the pattern is "" -> will print every package (this behavior is intended)
	if (tmp_pattern == NULL) {
		error("Pattern not found, check arguments..\n");
		return EXIT_FAILURE;
	}

	if (is_flag(*arg_opts, FLAG_EXACT)) {
		char *lastchar = memccpy(pattern + 1, tmp_pattern, '\0', PATTERN_LEN_MAX - 3);
		if (lastchar == NULL) {
			error("Pattern too long, set PATTERN_LEN_MAX in ('include/def.h') to an appropriate value and recompile esearch\n");
			return EXIT_FAILURE;
		}
		pattern[0] = '^';
		*(lastchar - 1) = '$';
	} else {
		memccpy(pattern, tmp_pattern, '\0', PATTERN_LEN_MAX - 1);
	}

	if (is_flag(*arg_opts, FLAG_NOCOLOR)) {
		COLOR_BOLDRED = COLOR_BOLD = COLOR_BOLDGREEN = COLOR_LIGHTGREEN = COLOR_INFO = COLOR_WARN = COLOR_ERROR = COLOR_RESET = "";
	}

	return INT_MAX;
}
