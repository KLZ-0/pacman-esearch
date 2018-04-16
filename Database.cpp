#include <iostream>
#include <vector>
#include <regex>
#include <fstream>
#include "Database.h"

using namespace std;

string home = getenv("HOME");

Database::Database(char pattern[], unsigned char srcexp) {
    loadInstalled();
    loadDB(pattern, srcexp);
}

void Database::loadInstalled() {
    installed.clear();
    string path;

    ifstream fp(home + "/.cache/esearch-database-installed");
    if (!fp.is_open()) {cout << "Failed to open database!" << endl; exit(1);}

    while (getline(fp, path)) {
        installed.push_back(path + '\n');
    }
    fp.close();
}

void Database::loadDB(char pattern[], unsigned char srcexp) {
    db.clear();
    string path, subpath;

    ifstream fp(home + "/.cache/esearch-database");
    if (!fp.is_open()) {cout << "Failed to open database!" << endl; exit(1);}

    bool flag = false;
    regex ex(pattern);
    while (getline(fp, path)) {
        path += "\n";
        if (flag) {
            if (path[0] != '\n') {db.push_back(path); continue;}
            flag = false;
            db.push_back(path);
            continue;
        }
        subpath = path.substr(path.find(':')+2);
        if (regex_search(subpath.substr(0, subpath.size()-1),ex) && path[0] == 'N') {
            if (srcexp == 1 && std::find(installed.begin(), installed.end(), subpath) == installed.end()) continue; // if explicitly installed and not find in installed then skip
            if (srcexp == 2 && std::find(installed.begin(), installed.end(), subpath) != installed.end()) continue; // if explicitly NOTinstalled and find in installed then skip
            db.push_back(path);
            flag = true;
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
    for (string x : db) {
        if (x[0] == 'N') {
            x = x.substr(x.find(':')+2);
            cout << header_color << "*  " << x.substr(0, x.size()-1);
            if (std::find(installed.begin(), installed.end(), x) != installed.end()) {
                cout << important_color << " [ installed ] \n" << normal_color;
            } else cout << endl;
        }
        else if (x[0] == ' ') cout << "      " << header_color << x << normal_color;
        else cout << slight_color << "      " << x.substr(0, x.find(':')+1) << header_color << x.substr(x.find(':')+1) << normal_color;
    }
}
