#include <iostream>
#include <regex>
#include <fstream>
#include "Database.h"

using namespace std;

string home = getenv("HOME");

Database::Database(char pattern[], unsigned char srcexp, bool searchdesc) {
    loadInstalled();
    loadDB(pattern, srcexp, searchdesc);
}

void Database::loadInstalled() {
    installed.clear();
    string line;

    ifstream fp(home + "/.cache/esearch-database-installed");
    if (!fp.is_open()) {cout << "Failed to open database!" << endl; exit(1);}

    while (getline(fp, line)) {
        installed.push_back(line + '\n');
    }
    fp.close();
}

void Database::loadDB(char pattern[], unsigned char srcexp, bool searchdesc) {
    db.clear();
    string line, subline;

    ifstream fp(home + "/.cache/esearch-database");
    if (!fp.is_open()) {cout << "Failed to open database!" << endl; exit(1);}

    vector<string> backupline;
    bool flag = false;
    regex ex(pattern, regex_constants::icase);
    while (getline(fp, line)) {
        line += "\n";
        if (flag) {
            if (line[0] != '\n') {db.push_back(line); continue;}
            flag = false;
            db.push_back(line);
            continue;
        }
        subline = line.substr(line.find(':')+2);
        if (line[0] == 'N' && regex_search(subline.substr(0, subline.size()-1),ex)) {
            if (srcexp == 1 && find(installed.begin(), installed.end(), subline) == installed.end()) continue; // if explicitly installed and not find in installed then skip
            if (srcexp == 2 && find(installed.begin(), installed.end(), subline) != installed.end()) continue; // if explicitly NOTinstalled and find in installed then skip
            db.push_back(line);
            flag = true;
        }
        else if (searchdesc && ((line[0] == 'D' && line[2] == 's') && regex_search(subline.substr(0, subline.size()-1),ex))) {
            subline = backupline[0].substr(backupline[0].find(':')+2);
            if (srcexp == 1 && find(installed.begin(), installed.end(), subline) == installed.end()) continue; // if explicitly installed and not find in installed then skip
            if (srcexp == 2 && find(installed.begin(), installed.end(), subline) != installed.end()) continue; // if explicitly NOTinstalled and find in installed then skip
            db.push_back(backupline[0]);
            db.push_back(backupline[1]);
            db.push_back(line);
            flag = true;
        }
        else if (searchdesc && (line[0] == 'N' || line[0] == 'V')) {
            backupline.push_back(line);
        }
        else {
            if (backupline.size() > 0) {
                backupline.clear();
            }
        }
    }
    fp.close();
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
