#include <regex>
#include <vector>

struct DatabaseLocation {
    std::string all;
    std::string installed;
};

struct InstalledPKG {
    std::string name;
    std::string version;
};

class Database {
    private:
        DatabaseLocation dbloc;
        bool exactsearch;

        std::vector<std::string> db;
        std::vector<InstalledPKG> installed;

        void loadInstalled();
        void checkTime();
        bool searchLine(std::string substr);

        std::regex ex;
        
    public:
        Database();
        ~Database();
        void loadDB(char pattern[], unsigned char srcexp, bool ds, bool es);

        bool isInstalled(std::string search);
        std::string nameToVersion(std::string name);

        DatabaseLocation getDBloc();
        std::vector<std::string> getDB();
};
