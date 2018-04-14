// TODO: where to put DB?

#include <iostream>
#include <vector>
#include <stdio.h>
#include <regex>
#include <fstream>

using namespace std;

vector<string> getInstalled() { // only for full db
    vector<string> installed;
    char path[1035];
    FILE *fp;

    fp = fopen("database-installed", "r");
    if (fp == NULL) {cout << "Failed to open database!" << endl; exit(1);}

    while (fgets(path, sizeof(path)-1, fp) != NULL) {
        string str(path);
        installed.push_back(path);
    }
    return installed;
}

void updateDB(vector<string> &data, char pattern[], unsigned char srcexp) { // eedit name and remove clear function
    data.clear();
    string path;

    ifstream fp("database");
    if (!fp.is_open()) {cout << "Failed to open database!" << endl; exit(1);}

    /* Read the output a line at a time - output it. */
    vector<string> installed = getInstalled();
    bool flag = false;
    regex ex(pattern);
    while (getline(fp, path)) {
        path += "\n";
        if (flag) {
            if (path[0] != '\n') {data.push_back(path); continue;}
            flag = false;
            data.push_back(path);
            continue;
        }
        if (regex_search(path.substr(path.find(':')+2),ex) && path[0] == 'N') {
            if (srcexp == 1 && std::find(installed.begin(), installed.end(), path.substr(path.find(':')+2)) == installed.end()) continue; // if explicitly installed and not find in installed then skip
            if (srcexp == 2 && std::find(installed.begin(), installed.end(), path.substr(path.find(':')+2)) != installed.end()) continue; // if explicitly NOTinstalled and find in installed then skip
            data.push_back(path);
            flag = true;
        }
    }
    fp.close();
}

void printOut(vector<string> data) {
    string important_color = "\033[1;31m";
    string header_color = "\033[38;5;46m";
    string normal_color = "\033[0m";
    string slight_color = "\033[38;5;34m";
    string step_up      = "\033[A";
    vector<string> installed = getInstalled();

    cout << endl;
    for (string x : data) {
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

int main(int argc, char* argv[]) {
    vector<string> db;
    unsigned char srcexp = 0;
    if (argc < 2) {
        cout << "\nesearch <pkgname>\n" << '\n';
        return 0;
    }
    char* pattern = argv[1];
    for (int i = 2; i < argc ; i++) {
        string option = argv[i];
        if (option == "-I") srcexp = 1;
        if (option == "-N") srcexp = 2;
    }

    updateDB(db, pattern, srcexp);
    printOut(db);

    return 0;
}
