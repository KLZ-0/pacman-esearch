#include <iostream>
#include <fstream>
#include "Database.h"

using namespace std;

string home = getenv("HOME");

Database::Database(char pattern[], unsigned char srcexp, bool ds, bool es) {
    descsearch = ds;
    exactsearch = es;
    explicitsearch = srcexp;
    
    loadInstalled();
    loadDB(pattern);
}

void Database::loadInstalled() {
    installed.clear();
    string line;

    ifstream fp(home + "/.cache/esearch-database-installed");
    if (!fp.is_open()) {cout << "Failed to open database! - Please run 'eupdatedb' first." << endl; exit(1);}

    while (getline(fp, line)) {
        installed.push_back(line + '\n');
    }
    fp.close();
}

void Database::loadDB(char pattern[]) {
    db.clear();
    string line, subline;

    ifstream fp(home + "/.cache/esearch-database");
    if (!fp.is_open()) {cout << "Failed to open database! - Please run 'eupdatedb' first." << endl; exit(1);}

    string backupline[2];
    bool flag = false;
    ex.assign(pattern, regex_constants::icase);
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
            if (explicitsearch == 1 && searchInstalled(subline)) continue; // if explicitly installed and not found in installed then skip
            if (explicitsearch == 2 && !searchInstalled(subline)) continue; // if explicitly NOTinstalled and found in installed then skip
            db.push_back(line);
            flag = true;
        }
        else if (descsearch && ((line[0] == 'D' && line[2] == 's') && searchLine(subline))) { // 
            subline = backupline[0].substr(backupline[0].find(':')+2); // get the name line
            if (explicitsearch == 1 && searchInstalled(subline)) continue; // if explicitly installed and not found in installed then skip
            if (explicitsearch == 2 && !searchInstalled(subline)) continue; // if explicitly NOTinstalled and found in installed then skip
            db.push_back(backupline[0]);
            db.push_back(backupline[1]);
            db.push_back(line);
            flag = true;
        }
        else if (descsearch && line[0] == 'N') {
            backupline[0] = line; // store the lines in a temp variable in case Description would match
        }
        else if (descsearch && line[0] == 'V') {
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

void Database::printOut(bool colored) {
    string important_color = "\033[1;31m";
    string header_color = "\033[38;5;46m";
    string normal_color = "\033[0m";
    string slight_color = "\033[38;5;34m";
    if (!colored) important_color = header_color = normal_color = slight_color = "";

    cout << endl;
    for (string line : db) {
        if (line[0] == 'N') {
            line = line.substr(line.find(':')+2);
            cout << header_color << "*  " << line.substr(0, line.size()-1);
            if (find(installed.begin(), installed.end(), line) != installed.end()) {
                cout << important_color << " [ installed ] \n" << normal_color;
            } else cout << endl;
        }
        else if (line[0] == ' ') cout << "      " << header_color << line << normal_color;
        else cout << slight_color << "      " << line.substr(0, line.find(':')+1) << header_color << line.substr(line.find(':')+1) << normal_color;
    }
}
