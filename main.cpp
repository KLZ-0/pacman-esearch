#include <iostream>
#include <vector>
#include "Database.h"

using namespace std;

#ifndef VERSION
#define VERSION "1.03-r2"
#endif

void help() {
    cout << "\
pacman-esearch (" << VERSION <<") - Replacement for both pacman -Ss and pacman -Si\n\n\
esearch <pkgname> [options]\n\
    --instonly, -I\tFind only packages which are installed\n\
    --notinst, -N\tFind only packages which are NOT installed\n\
    --nocolor, -n\tDon't use ANSI codes for colored output\n\
    --searchdesc, -S\tSearch also in package description\n\
    --version, -v\tShow version\n\
    --help, -h\t\tShow this message\n\
" << endl;
}

int main(int argc, char* argv[]) {
    char* pattern = 0;
    unsigned char srcexp = 0;
    bool colored = true, searchdesc = false;

    if (argc < 2) { help(); return 0; }
    for (int i = 1; i < argc ; i++) {
        string option = argv[i];
        if (option == "-n" || option == "--nocolor") colored = false;
        else if (option == "-S" || option == "--searchdesc") searchdesc = true;
        else if (option == "-I" || option == "--instonly") srcexp = 1;
        else if (option == "-N" || option == "--notinst") srcexp = 2;
        else if (option == "-v" || option == "--version") { cout << VERSION << endl; return 0; }
        else if (option == "-h" || option == "--help") { help(); return 0; }
        else if (option[0] == '-') { cout << "unknown option! see --help for all options" << endl; return 1; }
        else pattern = argv[i];
    }
    if (!pattern) {cout << "Pattern not found, check arguments.." << endl; return 1;}

    Database main_db(pattern, srcexp, searchdesc);
    main_db.printOut(colored);

    return 0;
}
