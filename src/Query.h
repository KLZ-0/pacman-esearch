#include <vector>
#include "Database.h"

class Query {
    private:
        Database main_db;

        char *pattern = 0;
        bool searchdesc = false;
        bool searchexact = false;
        unsigned char searchexplicit = 0;
        bool color = true;
        
    public:
        Query();
        void execute();
        void print();

        bool isPatternValid();

        void setPattern(char ptrn[]);
        void setSearchExplicitly(unsigned char srcexp);
        void setSearchDescription(bool ds);
        void setExactMatch(bool es);
        void setColoredOutput(bool clr);
};
