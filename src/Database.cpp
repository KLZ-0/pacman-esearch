#include <iostream>
#include <fstream>
#include "Database.h"

using namespace std;

Database::Database() {
    dbloc.all = string(getenv("HOME")) + "/.cache/esearch-database";
    dbloc.installed = string(getenv("HOME")) + "/.cache/esearch-database-installed";
}

void Database::loadInstalled() {
    installed.clear();
    string line;

    ifstream fp(dbloc.installed);
    if (!fp.is_open()) {cout << "Failed to open database! - Please run 'eupdatedb' first." << endl; exit(1);}

    while (getline(fp, line)) {
        installed.push_back(line + '\n');
    }
    fp.close();
}

void Database::loadDB(char pattern[], unsigned char srcexp, bool ds, bool es) {
    exactsearch = es;
    loadInstalled();

    db.clear();
    ex.assign(pattern, regex_constants::icase);    
    string line, subline;

    ifstream fp(dbloc.all);
    if (!fp.is_open()) {cout << "Failed to open database! - Please run 'eupdatedb' first." << endl; exit(1);}

    string backupline[2];
    bool flag = false;
    while (getline(fp, line)) {
        line += "\n";
        if (flag) { // after the flag is set, append every line until an empty line (sign of the end of a block) is reached, then unset the flag
            if (line[0] != '\n') {db.push_back(line); continue;}
            db.push_back(line);
            flag = false;
            continue;
        }
        subline = line.substr(line.find(':')+2); // creates a searchable string which is examined in the following blocks
        if (line[0] == 'N' && searchLine(subline)) { // if the name matches and survives the filters then append it to the db and set the flag to load all the other lines
            if (srcexp == 1 && searchInstalled(subline)) continue; // if explicitly installed and not found in installed then skip
            if (srcexp == 2 && !searchInstalled(subline)) continue; // if explicitly NOTinstalled and found in installed then skip
            db.push_back(line);
            flag = true;
        }
        else if (ds && ((line[0] == 'D' && line[2] == 's') && searchLine(subline))) { // 
            subline = backupline[0].substr(backupline[0].find(':')+2); // get the name line
            if (srcexp == 1 && searchInstalled(subline)) continue; // if explicitly installed and not found in installed then skip
            if (srcexp == 2 && !searchInstalled(subline)) continue; // if explicitly NOTinstalled and found in installed then skip
            db.push_back(backupline[0]);
            db.push_back(backupline[1]);
            db.push_back(line);
            flag = true;
        }
        else if (ds && line[0] == 'N') {
            backupline[0] = line; // store the lines in a temp variable in case Description would match
        }
        else if (ds && line[0] == 'V') {
            backupline[1] = line; // store the lines in a temp variable in case Description would match
        }
    }
    fp.close();
}

bool Database::searchLine(string search) {
    if (exactsearch) {
        return regex_match(search.substr(0, search.size()-1),ex);
    }
    
    return regex_search(search.substr(0, search.size()-1),ex);
}

bool Database::searchInstalled(string search) {
    return (find(installed.begin(), installed.end(), search) == installed.end());
}

DatabaseLocation Database::getDBloc() {
    return dbloc;
}

std::vector<std::string> Database::getDB() {
    return db;
}
