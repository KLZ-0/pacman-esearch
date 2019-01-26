#include <iostream>
#include "Query.h"

using namespace std;

#ifndef VERSION
#define VERSION "1.5.0"
#endif

string option;

void help() {
    cout << "\
pacman-esearch (" << VERSION << ") - Replacement for both pacman -Ss and pacman -Si\n\n\
esearch <pkgname> [options]\n\
    --instonly, -I\tFind only packages which are installed\n\
    --notinst, -N\tFind only packages which are NOT installed\n\
    --nocolor, -n\tDon't use ANSI codes for colored output\n\
    --searchdesc, -S\tSearch also in package description\n\
    --exact-match, -e\tShow only exact match\n\
    --version, -v\tShow version\n\
    --help, -h\t\tShow this message\n\
" << endl;
}

int main(int argc, char* argv[]) {
    Query query;

    if (argc < 2) { help(); return 0; }
    for (int i = 1; i < argc ; i++) {
        option = argv[i];
        if (option[0] == '-' && option[1] == '-') {
            if (option == "--instonly") query.setSearchExplicitly(1);
            else if (option == "--notinst") query.setSearchExplicitly(2);
            else if (option == "--nocolor") query.setColoredOutput(false);
            else if (option == "--searchdesc") query.setSearchDescription(true);
            else if (option == "--exact-match") query.setExactMatch(true);
            else if (option == "--version") { cout << VERSION << endl; return 0; }
            else if (option == "--help") { help(); return 0; }
            else { cout << "unknown option! see --help for all options" << endl; return 1; }
        }
        else if (option[0] == '-') {
            for(size_t i = 1; i < option.length(); i++) {
                switch (option[i])
                {
                    case 'I': query.setSearchExplicitly(1); break;
                    case 'N': query.setSearchExplicitly(2); break;
                    case 'n': query.setColoredOutput(false); break;
                    case 'S': query.setSearchDescription(true); break;
                    case 'e': query.setExactMatch(true); break;
                    case 'v': cout << VERSION << endl; return 0;
                    case 'h': help(); return 0;
                
                    default:
                        cout << "unknown option! see --help for all options" << endl;
                        return 1;
                }
            }
        }
        else query.setPattern(argv[i]);
    }
    if (!query.isPatternValid()) {cout << "Pattern not found, check arguments.." << endl; return 1;}

    query.execute();
    query.print();
    return 0;
}
