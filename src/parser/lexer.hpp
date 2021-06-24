extern "C"
{
#include "../lib/logging.h"
}

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <vector>
#include <string>

using namespace std;

class Parser
{
    string section;
    int line_counter;
    vector<vector<string *> *> macros_table;
    friend class Command;

public:
    Parser()
    {
        section = "";
        line_counter = 0;
    }
};

class Command
{
    Parser *parser;

    void define_fsm(string line_v);
    void command_fsm(string line_v);
    void sector_fsm(string line_v);
    int datasection_fsm(string line_v);
    string preprocessor(string line_v);
    string data_preprocessor(string line_v);
    string define_preprocessor(string line_v);

public:
    string initial_state;
    string initial_word;
    string final_state;
    string final_word;
    string direction;

    Command(string command, Parser *parser_);
    bool is_empty() // 0 if empty
    {
        return initial_state.empty() && initial_word.empty() && final_state.empty() && final_word.empty() && direction.empty();
    }
};