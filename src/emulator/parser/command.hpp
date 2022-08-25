#include "../vendor/easyloggingpp/easylogging++.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <vector>
#include <string>

using namespace std;

#ifndef _COMMAND
#define _COMMAND

class Parser;

class Command
{
    Parser *parser;

    void define_fsm(string line_v);
    void command_fsm(string line_v);
    void sector_fsm(string line_v);
    int datasection_fsm(string line_v);
    string command_preprocessor(string line_v);
    string sector_preprocessor(string line_v);
    string data_preprocessor(string line_v);
    string define_preprocessor(string line_v);

public:
    string initial_state;
    string initial_word;
    string final_state;
    string final_word;
    string direction;
    string debug;
    string lineNumber;

    Command(string command, Parser *parser_);
    bool is_empty() // 0 if empty
    {
        return initial_state.empty() && initial_word.empty() && final_state.empty() && final_word.empty() && direction.empty();
    }
};

#endif