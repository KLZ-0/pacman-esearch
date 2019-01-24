#include <regex>
#include <vector>

class Database {
    private:
        bool descsearch;
        bool exactsearch;
        unsigned char explicitsearch;

        std::vector<std::string> db;
        std::vector<std::string> installed;
        std::regex ex;

        void loadInstalled();
        void loadDB(char pattern[]);
        bool searchLine(std::string substr);
        bool searchInstalled(std::string substr);
        
    public:
        Database(char pattern[], unsigned char srcexp, bool ds, bool es);
        void printOut(bool colored);
};
