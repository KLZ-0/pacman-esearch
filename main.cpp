#include <iostream>
#include <vector>
#include <stdio.h>
#include <regex>

using namespace std;

void updateDB(vector<string> &data, auto database) {
    data.clear();
    char path[1035];
    FILE *fp;

    fp = fopen(database, "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
    }

    /* Read the output a line at a time - output it. */
    while (fgets(path, sizeof(path)-1, fp) != NULL) {
        string str(path);
        data.push_back(path);
    }
    pclose(fp);
}

vector<string> getInstalled(vector<string> data) { // only for full db
    vector<string> installed;
    bool flag = false;
    regex pattern("\\[installed\\]");
    for (string x : data) {
        if (flag) {installed.push_back(x); flag=!flag; continue;}
        if (regex_search(x,pattern) && x[0] != ' ') {
            installed.push_back(x);
            flag = !flag;
        }
    }
    return installed;
}

void printOut(vector<string> data, char pattern[]) {
    string header_color = "\033[38;5;40m";
    string normal_color = "\033[0m";
    string slight_color = "\033[38;5;28m";

    bool flag = false;
    regex ex(pattern);
    for (string x : data) {
        if (flag) {cout << slight_color << "      Description: " << header_color << x.substr(4) << normal_color << endl; flag=!flag; continue;}
        if (regex_search(x,ex) && x[0] != ' ') {
            cout << header_color << "*  " << x << normal_color;
            flag = !flag;
        }
    }
}

int main(int argc, char* argv[]) {
    vector<string> data;
    if (argc < 2) {
        cout << "\nesearch <pkgname>\n" << '\n';
        return 0;
    }
    else if (argc == 2) updateDB(data, "database");
    else updateDB(data, "database-installed");

    // printOut(getInstalled(data), argv[1]);
    printOut(data, argv[1]);

    return 0;
}
