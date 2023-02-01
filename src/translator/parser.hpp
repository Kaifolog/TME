

#ifndef _PARSER
#define _PARSER

#include <ctype.h>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

#include "command.hpp"

namespace translator
{

class Parser
{
    std::string _section;
    int _line_counter = 0;
    std::vector<std::vector<std::string>> _macros_table;

    void defineFsm(std::string line_v);
    Command commandFsm(std::string line_v);
    void sectorFsm(std::string line_v);
    void datasectionFsm(std::string line_v);
    std::string commandPreprocessor(std::string line_v);
    std::string sectorPreprocessor(std::string line_v);
    std::string dataPreprocessor(std::string line_v);
    std::string definePreprocessor(std::string line_v);

  public:
    Command parse(std::string command);
};

} // namespace translator

#endif