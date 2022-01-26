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
    string path;
    bool lambda;
    bool debug_statement;
    void command_to_sqlite3(sqlite3 *db, struct Command *current_command, sqlite3_stmt *ppStmt);
    bool check_str_in_vec(vector<string> v, string s);
    App(){};

public:
    App(string path_)
    {
        path = path_;
        lambda = 0;
        debug_statement = 0;
    };
    void setLambda(bool lambda_)
    {
        lambda = lambda_;
    };
    void setDebug(bool debug_)
    {
        debug_statement = debug_;
    };
    void semantic_analysis();
    void context_free_analysis_and_parsing();
    void emulator_executing_procedure();
    bool check_arguments(char *search, int argc, char **argv);
};