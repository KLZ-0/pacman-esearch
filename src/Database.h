#include <regex>
#include <vector>

struct DatabaseLocation {
    std::string all;
    std::string installed;
};


class Database {
    private:
        DatabaseLocation dbloc;
        bool exactsearch;

        std::vector<std::string> db;
        std::vector<std::string> installed;

        void loadInstalled();
        void checkTime();
        bool searchLine(std::string substr);

        std::regex ex;
        
    public:
        Database();
        ~Database();
        void loadDB(char pattern[], unsigned char srcexp, bool ds, bool es);

        bool searchInstalled(std::string substr);

        DatabaseLocation getDBloc();
        std::vector<std::string> getDB();
};
