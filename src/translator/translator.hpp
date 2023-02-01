extern "C"
{
#include "../external/sqlite3/sqlite3.h"
}

#include "../tools/ProjectName.hpp"
#include "parser.hpp"

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

    void init(Tools::ProjectName &pname);
    void finalize();

  public:
    void parse(Tools::ProjectName &pname);
    void analyse(Tools::ProjectName &pname);
};

#endif