#include <iostream>
#include "Query.h"

using namespace std;

Query::Query() {

}

void Query::execute() {
    main_db.loadDB(pattern, searchexplicit, searchdesc, searchexact);
}

void Query::print() {
    string important_color = "\033[1;31m";
    string header_color = "\033[0;1m";
    string green_asterix_color = "\033[38;5;46m";
    string normal_color = "\033[0m";
    string slight_color = "\033[38;5;34m";
    if (!color) important_color = header_color = normal_color = slight_color = "";

    cout << endl;
    for (string line : main_db.getDB()) {
        if (line[0] == 'N') {
            line = line.substr(line.find(':')+2);
            cout << green_asterix_color << "*  " << header_color << line.substr(0, line.size()-1);
            string version = main_db.nameToVersion(line);
            if (version != "") {
                cout << important_color << " [ installed ]\n" << slight_color << "      Current Version : " << header_color << version << " \n" << normal_color;
            } else cout << endl;
        }
        else if (line[0] == ' ') cout << "      " << header_color << line << normal_color;
        else cout << slight_color << "      " << line.substr(0, line.find(':')+1) << header_color << line.substr(line.find(':')+1) << normal_color;
    }
}

bool Query::isPatternValid() {
    return (pattern == 0) ? false : true;
}

void Query::setPattern(char ptrn[]) {
    pattern = ptrn;
}
void Query::setSearchExplicitly(unsigned char srcexp) {
    searchexplicit = srcexp;
}
void Query::setSearchDescription(bool ds) {
    searchdesc = ds;
}
void Query::setExactMatch(bool es) {
    searchexact = es;
}
void Query::setColoredOutput(bool clr) {
    color = clr;
}
