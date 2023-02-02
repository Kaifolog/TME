#ifndef _TRANSLATOR
#define _TRANSLATOR

#include <fstream>
#include <string>
#include <vector>

extern "C"
{
#include "../external/sqlite3/sqlite3.h"
}

#include "../tools/ProjectName.hpp"
#include "Parser.hpp"

namespace translator
{

const int BIND_BUFFER_LENGTH = 512;

class Translator
{
    sqlite3 *_db = nullptr;
    sqlite3_stmt *_pp_stmt = nullptr;
    char _insert_bind[BIND_BUFFER_LENGTH];
    char *_err = nullptr;
    std::ifstream _fin;
    bool _needs_to_finalize = false;

    void init(tools::ProjectName &pname);
    void finalize();

  public:
    void parse(tools::ProjectName &pname);
    void analyse(tools::ProjectName &pname);
};

} // namespace translator

#endif