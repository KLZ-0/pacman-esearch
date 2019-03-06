#include <string.h>

#include "stdio.h"
#include "misc.h"
#include "formatter.h"

#ifndef VERSION
#define VERSION "2.0.0"
#endif

char *option;

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

int main(int argc, char *argv[]) {
    // Query query;

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
            for(int i = 1; i < strlen(option); i++) {
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
        // else query.setPattern(argv[i]);
    }
    // if (!query.isPatternValid()) {cout << "Pattern not found, check arguments.." << endl; return 1;}

    // query.execute();
    // query.print();
    return 0;
}
