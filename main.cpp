#include <iostream>
#include <vector>
#include <stdio.h>
#include <regex>

using namespace std;

void updateDB(vector<string> &data, char pattern[]) {
    data.clear();
    char path[1035];
    FILE *fp;

    fp = fopen("database", "r");
    if (fp == NULL) {
        cout << "Failed to open database!" << endl;
        exit(1);
    }

    /* Read the output a line at a time - output it. */
    bool flag = false;
    regex ex(pattern);
    while (fgets(path, sizeof(path)-1, fp) != NULL) {
        string str(path);
        if (flag) {
            if (path[0] != '\n') {data.push_back(path); continue;}
            flag = false;
            data.push_back(path);
            continue;
        }
        if (regex_search(path,ex) && path[0] == 'N') {
            data.push_back(path);
            flag = true;
        }
    }
    pclose(fp);
}

vector<string> getInstalled(vector<string> data) { // only for full db
    vector<string> installed;
    regex pattern("\\[installed\\]");
    for (string x : data) {
        if (regex_search(x,pattern) && x[0] != ' ') {
            installed.push_back(x);
        }
    }
    return installed;
}

void printOut(vector<string> data) {
    string header_color = "\033[38;5;46m";
    string normal_color = "\033[0m";
    string slight_color = "\033[38;5;34m";

    cout << endl;
    for (string x : data) {
        if (x[0] == 'N') cout << header_color << "*  " << x.substr(x.find(':')+2) << normal_color;
        else if (x[0] == ' ') cout << "      " << header_color << x << normal_color;
        else cout << slight_color << "      " << x.substr(0, x.find(':')+1) << header_color << x.substr(x.find(':')+1) << normal_color;
    }
}

int main(int argc, char* argv[]) {
    vector<string> db;
    if (argc < 2) {
        cout << "\nesearch <pkgname>\n" << '\n';
        return 0;
    }
    char* pattern = argv[1];
    // for (int i = 2; i < argc ; i++) {
    //     string option = argv[i];
    //     if (option == "-i") db = getInstalled(db);
    // }

    updateDB(db, pattern);
    printOut(db);
    cout << db.size() << '\n';

    return 0;
}
