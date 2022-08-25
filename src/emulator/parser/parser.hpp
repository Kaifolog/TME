#include "./command.hpp"
extern "C"
{
#include "../vendor/sqlite3/sqlite3.h"
}

using namespace std;

#ifndef _PARSER
#define _PARSER

class Parser
{
        string section;
        int line_counter;
        vector<vector<string *> *> macros_table;
        friend class Command;

        string dir;
        sqlite3 *db = 0;
        sqlite3_stmt *ppStmt;
        char insert_bind[256];
        char *err = 0;
        ifstream fin;
        bool needsToFinalize = false;

        void init(string &path);
        void finalize();

public:
        Parser()
        {
                section = "";
                line_counter = 0;
        };
        void parse(string &path);
};

#endif