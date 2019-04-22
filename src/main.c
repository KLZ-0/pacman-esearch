#include <string.h>
#include <stdio.h>
#include <regex.h>

#ifndef VERSION
#define VERSION "2.1.0"
#endif

#define COLOR_IMPORTANT "\033[1;31m"
#define COLOR_HEADER "\033[0;1m"
#define COLOR_GREEN_ASTERIX "\033[38;5;46m"
#define COLOR_NORMAL "\033[0m"
#define COLOR_LIGHT "\033[38;5;34m"

typedef enum {
    LINE_HEADER,
    LINE_NORMAL,
    LINE_SOFTBROKEN,
    LINE_BLOCKTERM
} LineType;

void help() {
    printf("\
pacman-esearch (%s) - Replacement for both pacman -Ss and pacman -Si\n\n\
esearch <pkgname> [options]\n\
    --instonly, -I\tFind only packages which are installed\n\
    --notinst, -N\tFind only packages which are NOT installed\n\
    --nocolor, -n\tDon't use ANSI codes for colored output\n\
    --searchdesc, -S\tSearch also in package description\n\
    --exact-match, -e\tShow only exact match\n\
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
            strcpy(line, &line[lastIndexOf(line, ' ')+1]);
            printf("%s*  %s%s", COLOR_GREEN_ASTERIX, COLOR_HEADER, line);
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

int searchFile(const regex_t *ex) {

    FILE *fstream;
    char line[4096];
    fstream = fopen("/home/klz/.cache/esearch-database" , "r");

    if(fstream == NULL) {
        fprintf(stderr, "Error opening file");
        return 1;
    }

    char repoline[4096];
    int flag = 0;
    while (fgets(line, 4096, fstream)) {
        if (flag) {
            if (strlen(line) == 1) {
                formatLine(line, LINE_BLOCKTERM);
                flag = 0;
                continue;
            }

            if (line[0] != ' ') {
                formatLine(line, LINE_NORMAL);
            }
            else {
                formatLine(line, LINE_SOFTBROKEN);
            }
            
            continue;
        }
                
        if (line[0] == 'N' && !regexec(ex, line, 0, NULL, 0)) {
            flag = 1;
            formatLine(line, LINE_HEADER);
            formatLine(repoline, LINE_NORMAL);
            continue;
        }

        if (startsWith(line, "Repo")) {
            strncpy(repoline, line, 4096);
        }
        
    }

    fclose(fstream);
    return 0;

}

int main(int argc, char *argv[]) {

    char *option;
    char *pattern = NULL;
    regex_t ex;

    if (argc < 2) { help(); return 0; }
    for (int i = 1; i < argc ; i++) {
        option = argv[i];
        if (option[0] == '-' && option[1] == '-') {
            // if (option == "--instonly") query.setSearchExplicitly(1);
            // else if (option == "--notinst") query.setSearchExplicitly(2);
            // else if (option == "--nocolor") query.setColoredOutput(false);
            // else if (option == "--searchdesc") query.setSearchDescription(true);
            // else if (option == "--exact-match") query.setExactMatch(true);
            if (strcmp(option, "--version") == 0) { printf("%s\n", VERSION); return 0; }
            else if (strcmp(option, "--help") == 0) { help(); return 0; }
            else { printf("unknown option! see --help for all options\n"); return 1; }
        }
        else if (option[0] == '-') {
            for(size_t i = 1; i < strlen(option); i++) {
                switch (option[i])
                {
                    // case 'I': query.setSearchExplicitly(1); break;
                    // case 'N': query.setSearchExplicitly(2); break;
                    // case 'n': query.setColoredOutput(false); break;
                    // case 'S': query.setSearchDescription(true); break;
                    // case 'e': query.setExactMatch(true); break;
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
            pattern = argv[i];
        }
    }
    
    if (pattern == NULL) {
        fprintf(stderr, "Pattern not found\n");
        return 1;
    }
    
    if (regcomp(&ex, pattern, 0)) {
        fprintf(stderr, "Could not compile regex\n");
        return 1;
    }

    if (searchFile(&ex)) {
        return -127;
    }

    // if (!query.isPatternValid()) {cout << "Pattern not found, check arguments.." << endl; return 1;}

    // query.execute();
    // query.print();

    return 0;

}
