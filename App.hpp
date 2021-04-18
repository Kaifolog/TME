extern "C"
{
#include <stdio.h>
#include "lib/logging.h"
#include "lib/svwm.h"
#include "vendor/sqlite3.h"
}
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <algorithm>
#include "TuringMachine.hpp"
#include "text_functions.hpp"

using namespace std;

class App
{
    void command_to_sqlite3(sqlite3 *db, struct Command *current_command, sqlite3_stmt *ppStmt);
    bool check_str_in_vec(vector<string> v, string s);

public:
    void semantic_analysis(int argc, char *argv[]);
    void context_free_analysis_and_parsing(int argc, char *argv[]);
    void emulator_executing_procedure(int argc, char *argv[], bool debug_statement);
    bool check_arguments(int argc, char *argv[], char *search);
};