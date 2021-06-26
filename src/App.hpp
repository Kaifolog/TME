extern "C"
{
#include <stdio.h>
#include "vendor/sqlite3/sqlite3.h"
}
#include "vendor/easyloggingpp/easylogging++.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <algorithm>
#include "TuringMachine.hpp"
#include "libs/text_functions.hpp"

using namespace std;

class App
{
    int argc;
    char **argv;
    void command_to_sqlite3(sqlite3 *db, struct Command *current_command, sqlite3_stmt *ppStmt);
    bool check_str_in_vec(vector<string> v, string s);

public:
    void semantic_analysis();
    void context_free_analysis_and_parsing();
    void emulator_executing_procedure(bool debug_statement);
    bool check_arguments(char *search);
    App(int argc_, char **argv_)
    {
        argc = argc_;
        argv = argv_;
    }
};