/**
 * @file
 * @author Adrián Kálazi (KLZ-0)
 * @brief Replacement for pacman -Ss and pacman -Si
 * @details A little program to prettify the output from the search features of pacman
 */

#include "misc.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <time.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdbool.h>

#ifndef VERSION
#define VERSION "3.0.1"
#endif

#define DB ".cache/esearch-database"

char *COLOR_IMPORTANT = "\033[1;31m";
char *COLOR_HEADER = "\033[0;1m";
char *COLOR_GREEN_ASTERIX = "\033[38;5;46m";
char *COLOR_NORMAL = "\033[0m";
char *COLOR_LIGHT = "\033[38;5;34m";

char db_main[256];
char db_installed[256];
char *installedBuffer = 0;

// TODO: Make option to print only found package names/only first



int startsWith(char *str, char* substr) {
    for(size_t i = 0; i < strlen(substr); i++) {
        if (str[i] != substr[i]) {
            return 0;
        }
    }
    return 1;
}

size_t lastIndexOf(char *str, char search) {
    for(int i = strlen(str)-1; i > 0; i--) {
        if (str[i] == search) {
            return i;
        }
    }
    return -1;
}

void formatLine(char* line, int type) {

    char *tempToken;
    switch (type) {
        case LINE_HEADER:
            printf("%s*  %s%s\n", COLOR_GREEN_ASTERIX, COLOR_HEADER, line);
            break;

        case LINE_NORMAL:
        tempToken = strtok(line, ":");
            printf("%s      %s%s:%s%s", COLOR_LIGHT, tempToken, COLOR_HEADER, strtok(NULL, ""), COLOR_NORMAL);
            break;

        case LINE_SOFTBROKEN:
            printf("      %s%s%s", COLOR_HEADER, line, COLOR_NORMAL);
            break;

        case LINE_BLOCKTERM:
            printf(line);
            break;

        default:
            break;
    }

}

void getWordUntilDelimiter(char *newchar, char *point, char delimiter) {

    // detection of the first space to get a whole word
    char *nextWordPos = 0;
    for(size_t i = 1; i < 30; i++) {
        if (point[i] == ' ') {            
            nextWordPos = &point[i]+1;
            break;
        }
    }  

    for(size_t i = 0; i < 30; i++) {
        newchar[i] = *(nextWordPos+i);
        if (*(nextWordPos+i+1) == delimiter) {
            break;
        }
    }

}

int searchFile(const regex_t *ex, char* installed, unsigned flags) {

    FILE *fstream = fopen(db_main, "r");
    char line[4096];

    if(fstream == NULL) {
        fprintf(stderr, "Error opening file");
        return 1;
    }

    char repoline[4096];
    int inblock = 0;
    char header[4000];
    char *installedIndex;
    char pkgVer[30];
    while (fgets(line, 4096, fstream)) {
        if (inblock) {
            // Flag "inblock" set -> just print the line as is
            if (strlen(line) == 1) {
                // Final line -> print "Repository" line and end block
                formatLine(repoline, LINE_NORMAL);
                formatLine(line, LINE_BLOCKTERM);
                inblock = 0;
                continue;
            }

            if (line[0] != ' ') {
                // Normal line
                formatLine(line, LINE_NORMAL);
            }
            else {
                // Partly broken line (e.g. "Depends On" or "Optional Deps")
                formatLine(line, LINE_SOFTBROKEN);
            }

            continue;
        }

        if (line[0] == 'N') {
            // Search "Name" line for matches and set inblock for block start
            strcpy(header, &line[lastIndexOf(line, ' ')+1]);
            strtok(header, "\n");

            if (!regexec(ex, header, 0, NULL, 0)) {
                installedIndex = strstr(installed, header);
                if (installedIndex) {
                    if (isBit(flags, FLAG_NOINST)) {
                        continue;
                    }
                    getWordUntilDelimiter(pkgVer, installedIndex, '\n');
                    sprintf(line, "%s %s[ installed ]\n      %sCurrent Version : %s%s", header, COLOR_IMPORTANT, COLOR_LIGHT, COLOR_HEADER, pkgVer);
                }
                else {
                    if (isBit(flags, FLAG_INST)) {
                        continue;
                    }
                    sprintf(line, "%s", header);
                }

                inblock = 1;
                formatLine(line, LINE_HEADER);
                continue;
            }
        }

        if (startsWith(line, "Repo")) {
            // Store "Repository" line in a buffer to be printed at the end of a block
            strncpy(repoline, line, 4096);
        }

    }

    fclose(fstream);
    return 0;

}

void dbAgeCheck(char* db) {
    struct stat buf;
    stat(db, &buf);
    time_t dbcreation = buf.st_ctime;
    time_t now;
    time(&now);

    time_t dbage = now-dbcreation;
    if (dbage > 604800) { // 7 days
        printf("\033[93;1m *** \033[0mYou should run eupdatedb, the last update was %lu days ago - on %s", dbage/86400, ctime(&dbcreation));
    }
}

char *strCpy(char *str) {
    size_t len = strlen(str) + 1;
    char *nstr = malloc(sizeof(str) * (len));
    strncpy(nstr, str, len);
    return nstr;
}

/**
 * Prints an error text after execution depending on the error code
 * @param errorcode Valid error code (from errors enum)
 */
void printError(int errorcode) {
    switch (errorcode) {
        case ERR_NOERROR:
            break;

        case ERR_HELP:
            printf(help(), VERSION);
            break;

        case ERR_VERSION:
            printf("%s\n", VERSION);
            break;

        case ERR_BADOPTION:
            fprintf(stderr, "unknown option! see --help for all options\n");
            break;

        case ERR_NOPATTERN:
            fprintf(stderr, "Pattern not found, check arguments..\n");
            break;

        default:
            fprintf(stderr, "Unknown error code: %d\n", errorcode);
            break;
    }
}

int main(int argc, char *argv[]) {

    int state = 0;
    unsigned flags = 0;
    char *pattern = NULL;
    regex_t ex;

    if (argc < 2) {
        state = ERR_HELP;
    }

    if (state == ERR_NOERROR) {
        state = parseArguments(argc, argv, &flags, &pattern);
    }

    if (state == ERR_NOERROR) {
        printf("Pattern: %s\n", pattern);
    }

    printError(state);
    return (state < ERR_BADOPTION) ? 0 : state;

    ////// Instantly applyable search flags

    if (isBit(flags, FLAG_NOCOLOR)) {
        COLOR_GREEN_ASTERIX = COLOR_HEADER = COLOR_IMPORTANT = COLOR_LIGHT = COLOR_NORMAL = "";
    }

    if (isBit(flags, FLAG_EXACT)) {
        char tmp[250];
        strcpy(tmp, pattern);
        sprintf(pattern, "^%s$", tmp);
    }


    ////// Additional checks

    if (pattern == NULL) {
        fprintf(stderr, "Pattern not found, check arguments..\n");
        return 1;
    }

    if (regcomp(&ex, pattern, REG_ICASE)) {
        fprintf(stderr, "Could not compile regex\n");
        return 1;
    }

    ////// Load installed

    sprintf(db_main, "%s/%s", getenv("HOME"), DB);
    sprintf(db_installed, "%s/%s-installed", getenv("HOME"), DB);
    // TODO: Add option to disable database time check

    long length;
    FILE *installedFile = fopen (db_installed, "r");
    if(installedFile == NULL) {
        fprintf(stderr, "Error opening file");
        return 1;
    }

    if (installedFile) {
        fseek (installedFile, 0, SEEK_END);
        length = ftell (installedFile);
        fseek (installedFile, 0, SEEK_SET);
        installedBuffer = malloc (length);
        if (installedBuffer) {
            fread (installedBuffer, 1, length, installedFile);
        }
        fclose (installedFile);
    }

    ////// Actual search

    printf("\n");
    if (searchFile(&ex, installedBuffer, flags)) {
        free(installedBuffer);
        return -127;
    }
    free(installedBuffer);
    regfree(&ex);

    ////// Check database age
    if (!isBit(flags, FLAG_NOWARNDB)) {
        dbAgeCheck(db_main);
    }

    return 0;

}
