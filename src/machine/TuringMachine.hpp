#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <algorithm>

#include "MachineState.hpp"
extern "C"
{
#include "../emulator/vendor/sqlite3/sqlite3.h"
}

#include "../tools/ProjectName.hpp"

#ifndef _TURINGMACHINE
#define _TURINGMACHINE

using namespace std;

class TuringMachine
{
    vector<string> strip;
    string statement = "start";
    int cursor;

    string dir;
    string output;
    bool lambda;
    sqlite3 *db = 0;
    sqlite3_stmt *ppStmt;
    char *err = 0;
    ifstream fin;

    inline bool
    test_for_primary_cursor(string cell)
    {
        return cell[0] == '|' && cell[cell.size() - 1] == '|';
    }

    bool is_end(string dir, bool lambda);
    bool load_strip(string dir);
    string get_current_state();
    string get_current_word();
    string get_strip();
    string get_strip(bool lambda);
    void set_current_state(string a);
    void set_current_word(string a);
    bool get_step(char a);

public:
    int execute(ProjectName &pname, bool lambda);
    void lazyStart(ProjectName &pname, bool lambda);
    MachineState lazyDebug(bool step_by_step = 0);
    void lazyFinalize();
};

#endif