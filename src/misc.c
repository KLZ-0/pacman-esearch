#include "misc.h"
#include <string.h>
#include <stdlib.h>

void setBit(unsigned *var, unsigned bit) {
	*var |= (1u << (bit));
}

bool isBit(unsigned var, unsigned bit) {
	return var & (1u << bit);
}

char *help() {
	return "\
pacman-esearch (%s) - Replacement for pacman -Ss and pacman -Si\n\n\
esearch <pkgname> [options]\n\
	--instonly, -I\tFind only packages which are installed\n\
	--notinst, -N\tFind only packages which are NOT installed\n\
	--nocolor, -n\tDon't use ANSI codes for colored output\n\
	--exact-match, -e\tShow only exact match\n\
	--nowarndb, -w\tDo not complain about database age\n\
	--version, -v\tShow version\n\
	--help, -h\t\tShow this message\n\n\
";
}

char *strCpy(char *src) {
	size_t len = strlen(src) + 1;
	char *new = malloc(sizeof(src) * len);
	if (new == NULL) {
	return NULL;
	}
	strncpy(new, src, len);
	return new;
}

int strAppend(char *pre, char **str, char *post) {
	size_t newlen = strlen(*str) + strlen(pre) + strlen(post) + 1;
	char *new = calloc(newlen, sizeof(*str));
	if (new == NULL) {
	return ERR_ALLOC;
	}
	strcat(new, pre);
	strcat(new, *str);
	strcat(new, post);
	free(*str);
	*str = new;
	return ERR_NOERROR;
}

int applyFlags(char **pattern, unsigned flags) {

	if (isBit(flags, FLAG_EXACT)) {
		int tmp = strAppend("^", pattern, "$");
		if (tmp != ERR_NOERROR) {
			return tmp;
		}
	}
	return ERR_NOERROR;

}

/**
 * Parses command line arguments
 * @param argc
 * @param argv
 * @param flags
 * @param pattern
 * @post @p pattern will be set to an allocated adress pointing to a valid string
 * @return
 */
int parseArguments(int argc, char **argv, unsigned *flags, char **pattern) {
	char *tmp;
	for (int i = 1; i < argc ; i++) {
		tmp = argv[i];
		if (tmp[0] == '-' && tmp[1] == '-') {
			if (strcmp(tmp, "--instonly") == 0) setBit(flags, FLAG_INST);
			else if (strcmp(tmp, "--notinst") == 0) setBit(flags, FLAG_NOINST);
			else if (strcmp(tmp, "--nocolor") == 0) setBit(flags, FLAG_NOCOLOR);
			else if (strcmp(tmp, "--nowarndb") == 0) setBit(flags, FLAG_NOWARNDB);
			else if (strcmp(tmp, "--exact-match") == 0) setBit(flags, FLAG_EXACT);
			else if (strcmp(tmp, "--version") == 0) return ERR_VERSION;
			else if (strcmp(tmp, "--help") == 0) return ERR_HELP;
			else return ERR_BADOPTION;
		}
		else if (tmp[0] == '-') {
			while (*tmp != '\0') {
				switch (*tmp) {
					case '-':
						break;
					case 'I':
						setBit(flags, FLAG_INST);
						break;
					case 'N':
						setBit(flags, FLAG_NOINST);
						break;
					case 'n':
						setBit(flags, FLAG_NOCOLOR);
						break;
					case 'w':
						setBit(flags, FLAG_NOWARNDB);
						break;
					case 'e':
						setBit(flags, FLAG_EXACT);
						break;
					case 'v':
						return ERR_VERSION;
					case 'h':
						return ERR_HELP;
					default:
						return ERR_BADOPTION;
				}
				tmp++;
			}
		}
		else {
			if (*pattern != NULL) {
				free(*pattern);
			}
			if ((*pattern = strCpy(argv[i])) == NULL) {
				return ERR_ALLOC;
			}
		}
	}
	if (*pattern == NULL) {
		return ERR_NOPATTERN;
	}

	int res = applyFlags(pattern, *flags);
	if (res != ERR_NOERROR) {
		return res;
	}

	return ERR_NOERROR;

}
