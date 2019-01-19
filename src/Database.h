class Database {
    private:
        std::vector<std::string> db;
        std::vector<std::string> installed;
    public:
        Database(char pattern[], unsigned char srcexp, bool searchdesc);
        void loadInstalled();
        void loadDB(char pattern[], unsigned char srcexp, bool searchdesc);
        void printOut(bool colored);
};
