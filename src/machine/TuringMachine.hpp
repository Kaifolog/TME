#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <string>

#include "MachineState.hpp"
extern "C"
{
#include "../external/sqlite3/sqlite3.h"
}

#include "../tools/ProjectName.hpp"

#ifndef _TURINGMACHINE
#define _TURINGMACHINE

using namespace std;

class TuringMachine
{
    list<string> strip;
    list<string>::iterator cursor;
    string statement = "start";
    string lastLine;

    string dir;
    string output;
    bool lambda;
    sqlite3 *db = 0;
    sqlite3_stmt *ppStmt;
    char *err = 0;
    ifstream fin;

    inline bool test_for_primary_cursor(string cell)
    {
        return cell[0] == '|' && cell[cell.size() - 1] == '|';
    }

    bool is_end(string dir, bool lambda);
    bool load_strip(string dir);
    string get_strip();
    string get_strip(bool lambda);
    void get_step(char a);

  public:
    int execute(ProjectName &pname, bool lambda);
    void lazyStart(ProjectName &pname, bool lambda);
    void setLambda(bool lambda)
    {
        this->lambda = lambda;
    };
    MachineState lazyDebug(bool step_by_step = 0);
    void lazyFinalize();
};

#endif