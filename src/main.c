#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <time.h>
#include <sys/stat.h>

#ifndef VERSION
#define VERSION "2.1.5"
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

int flags = 0;

typedef enum {
    LINE_HEADER,
    LINE_NORMAL,
    LINE_SOFTBROKEN,
    LINE_BLOCKTERM
} LineType;

typedef enum {
    FLAG_INST,
    FLAG_NOINST,
    FLAG_NOCOLOR,
    FLAG_NOWARNDB,
    FLAG_EXACT
} SearchFlag;

#define setSearchFlag(_f) (flags |= (1 << (_f)))
#define isSearchFlag(_f) ((flags >> (_f)) & 1)

void help() {
    printf("\
pacman-esearch (%s) - Replacement for both pacman -Ss and pacman -Si\n\n\
esearch <pkgname> [options]\n\
    --instonly, -I\tFind only packages which are installed\n\
    --notinst, -N\tFind only packages which are NOT installed\n\
    --nocolor, -n\tDon't use ANSI codes for colored output\n\
    --exact-match, -e\tShow only exact match\n\
    --nowarndb, -w\tDo not complain about database age\n\
    --version, -v\tShow version\n\
    --help, -h\t\tShow this message\n\n\
", VERSION);
}

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

void formatLine(char* line, LineType type) {

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

int searchFile(const regex_t *ex, char* installed) {

    FILE *fstream = fopen(db_main, "r");
    char line[4096];

    if(fstream == NULL) {
        fprintf(stderr, "Error opening file");
        return 1;
    }

    char repoline[4096];
    int flag = 0;
    char header[4000];
    char *installedIndex;
    char pkgVer[30];
    while (fgets(line, 4096, fstream)) {
        if (flag) {
            // Flag set -> just print the line as is
            if (strlen(line) == 1) {
                // Final line -> print "Repository" line and end block
                formatLine(repoline, LINE_NORMAL);
                formatLine(line, LINE_BLOCKTERM);
                flag = 0;
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
            // Search "Name" line for matches and set flag for block start
            strcpy(header, &line[lastIndexOf(line, ' ')+1]);
            strtok(header, "\n");

            if (!regexec(ex, header, 0, NULL, 0)) {
                installedIndex = strstr(installed, header);
                if (installedIndex) {
                    if (isSearchFlag(FLAG_NOINST)) {
                        continue;
                    }
                    getWordUntilDelimiter(pkgVer, installedIndex, '\n');
                    sprintf(line, "%s %s[ installed ]\n      %sCurrent Version : %s%s", header, COLOR_IMPORTANT, COLOR_LIGHT, COLOR_HEADER, pkgVer);
                }
                else {
                    if (isSearchFlag(FLAG_INST)) {
                        continue;
                    }
                    sprintf(line, "%s", header);
                }
                
                
                flag = 1;
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

int main(int argc, char *argv[]) {

    char *option;
    char pattern[256];
    regex_t ex;

    if (argc < 2) { help(); return 0; }
    for (int i = 1; i < argc ; i++) {
        option = argv[i];
        if (option[0] == '-' && option[1] == '-') {
            if (strcmp(option, "--instonly") == 0) setSearchFlag(FLAG_INST);
            else if (strcmp(option, "--notinst") == 0) setSearchFlag(FLAG_NOINST);
            else if (strcmp(option, "--nocolor") == 0) setSearchFlag(FLAG_NOCOLOR);
            else if (strcmp(option, "--nowarndb") == 0) setSearchFlag(FLAG_NOWARNDB);
            else if (strcmp(option, "--exact-match") == 0) setSearchFlag(FLAG_EXACT);
            else if (strcmp(option, "--version") == 0) { printf("%s\n", VERSION); return 0; }
            else if (strcmp(option, "--help") == 0) { help(); return 0; }
            else { printf("unknown option! see --help for all options\n"); return 1; }
        }
        else if (option[0] == '-') {
            for(size_t i = 1; i < strlen(option); i++) {
                switch (option[i])
                {
                    case 'I': setSearchFlag(FLAG_INST); break;
                    case 'N': setSearchFlag(FLAG_NOINST); break;
                    case 'n': setSearchFlag(FLAG_NOCOLOR); break;
                    case 'w': setSearchFlag(FLAG_NOWARNDB); break;
                    case 'e': setSearchFlag(FLAG_EXACT); break;
                    case 'v': printf("%s\n", VERSION); return 0;
                    case 'h': help(); return 0;
                    case '\0': break;
                
                    default:
                        printf("unknown option! see --help for all options\n");
                        return 1;
                }
            }
        }
        else {
            strcpy(pattern, argv[i]);
        }
    }
    
    ////// Instantly applyable search flags

    if (isSearchFlag(FLAG_NOCOLOR)) {
        COLOR_GREEN_ASTERIX = COLOR_HEADER = COLOR_IMPORTANT = COLOR_LIGHT = COLOR_NORMAL = "";
    }

    if (isSearchFlag(FLAG_EXACT)) {
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
    if (searchFile(&ex, installedBuffer)) {
        return -127;
    }
    regfree(&ex);

    ////// Check database age
    if (!isSearchFlag(FLAG_NOWARNDB)) {
        dbAgeCheck(db_main);
    }

    return 0;

}
