#include "command.hpp"

using namespace std;

#ifndef _PARSER
#define _PARSER

class Parser
{
    string section;
    int line_counter = 0;
    vector<vector<string>> macros_table;

    void define_fsm(string line_v);
    Command command_fsm(string line_v);
    void sector_fsm(string line_v);
    int datasection_fsm(string line_v);
    string command_preprocessor(string line_v);
    string sector_preprocessor(string line_v);
    string data_preprocessor(string line_v);
    string define_preprocessor(string line_v);

  public:
    Command parse(string command);
};

#endif