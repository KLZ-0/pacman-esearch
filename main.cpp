#include <iostream>
#include <vector>
#include "Database.h"

using namespace std;

#ifndef VERSION
#define VERSION 1.01
#endif

void help() {
    cout << "\nesearch <pkgname> [options]\n\
    --instonly, -I\tFind only packages which are installed\n\
    --notinst, -N\tFind only packages which are NOT installed\n\
    --nocolor, -n\tDon't use ANSI codes for colored output\n\
    --version, -v\tShow version\n\
    --help, -h\tShow this message\n\
    " << endl;
}

int main(int argc, char* argv[]) {
    char* pattern = 0;
    unsigned char srcexp = 0;
    bool colored = true;

    if (argc < 2) { help(); return 0; }
    for (int i = 1; i < argc ; i++) {
        string option = argv[i];
        if (option == "-n" || option == "--nocolor") colored = false;
        else if (option == "-I" || option == "--instonly") srcexp = 1;
        else if (option == "-N" || option == "--notinst") srcexp = 2;
        else if (option == "-v" || option == "--version") { cout << VERSION << endl; return 0; }
        else if (option == "-h" || option == "--help") { help(); return 0; }
        else pattern = argv[i];
    }
    if (!pattern) {cout << "Pattern not found, check arguments.." << endl; return 1;}

    Database main_db(pattern, srcexp);
    main_db.printOut(colored);

    return 0;
}
