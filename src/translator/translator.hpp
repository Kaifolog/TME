extern "C"
{
#include "../external/sqlite3/sqlite3.h"
}

#include "parser.hpp"
#include "../tools/ProjectName.hpp"

using namespace std;

#ifndef _TRANSLATOR
#define _TRANSLATOR

class Translator
{
        sqlite3 *db = 0;
        sqlite3_stmt *ppStmt = 0;
        char insert_bind[512];
        char *err = 0;
        ifstream fin;
        bool needsToFinalize = false;

        void init(ProjectName &pname);
        void finalize();

public:
        void parse(ProjectName &pname);
        void analyse(ProjectName &pname);
};

#endif