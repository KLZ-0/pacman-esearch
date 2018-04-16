#include <iostream>
#include <vector>
#include "Database.h"

using namespace std;

int main(int argc, char* argv[]) {
    char* pattern = 0;
    unsigned char srcexp = 0;
    bool colored = true;

    if (argc < 2) { cout << "\nesearch <pkgname> [-I|-N]\n" << '\n'; return 0; }
    for (int i = 1; i < argc ; i++) {
        string option = argv[i];
        if (option == "-n" || option == "--nocolor") colored = false;
        else if (option == "-I" || option == "--instonly") srcexp = 1;
        else if (option == "-N" || option == "--notinst") srcexp = 2;
        else pattern = argv[i];
    }
    if (!pattern) {cout << "Pattern not found, check arguments.." << endl; return 1;}

    Database main_db(pattern, srcexp);
    main_db.printOut(colored);

    return 0;
}
