//
// Created by klz on 12/28/19.
//

#ifndef ESEARCH_MISC_H
#define ESEARCH_MISC_H

#include <stdbool.h>

enum linetype{
    LINE_HEADER,
    LINE_NORMAL,
    LINE_SOFTBROKEN,
    LINE_BLOCKTERM,
};

enum searchflag {
    FLAG_INST,
    FLAG_NOINST,
    FLAG_NOCOLOR,
    FLAG_NOWARNDB,
    FLAG_EXACT
};

enum errors {
    ERR_NOERROR,
    ERR_VERSION,
    ERR_HELP,
    ERR_BADOPTION,
    ERR_NOPATTERN,
    ERR_ALLOC,
};

void setBit(unsigned *var, unsigned bit);

bool isBit(unsigned var, unsigned bit);

char *help();

char *strCpy(char *src);

int strAppend(char *pre, char **str, char *post);

/**
 * Parses the strings in argv and sets the appropriate flags
 * @param argc Argument count
 * @param argv Argument vector
 * @param flags Pointer to uint to store bit flags
 * @param pattern The last argument without '-' will be set as pattern
 * @return ERR_NOERROR on success or a valid error code on fail
 * @pre flags and pattern must be initialized
 */
int parseArguments(int argc, char **argv, unsigned *flags, char **pattern);

#endif //ESEARCH_MISC_H
