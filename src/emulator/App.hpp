extern "C"
{
#include <stdio.h>
}
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <algorithm>

extern "C"
{
#include "vendor/sqlite3/sqlite3.h"
}
#include "vendor/easyloggingpp/easylogging++.h"

#include "../machine/TuringMachine.hpp"
#include "libs/text_functions.hpp"
#include "../machine/MachineState.hpp"
#include "../analyser/parser.hpp"
#include "../tools/ProjectName.hpp"

using namespace std;

class App
{
    int argc;
    char **argv;

    ProjectName _pname;

    bool noLambda = false;
    bool parsing = false;
    bool semantic = false;
    bool emulating = false;
    bool debug = false;

    void command_to_sqlite3(sqlite3 *db, struct Command *current_command, sqlite3_stmt *ppStmt);
    bool check_str_in_vec(vector<string> v, string s);
    App(){};

public:
    App(ProjectName pname, int _argc, char *_argv[])
    {
        _pname = pname;
        argc = _argc;
        argv = _argv;
        handleArguments();
    };

    void parse();
    void semantic_analysis();
    void emulate();
    void handleArguments();
    int execute();
};