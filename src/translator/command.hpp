#ifndef _COMMAND
#define _COMMAND

#include "../external/easyloggingpp/easylogging++.h"
#include <string>

namespace translator
{

class Translator;

struct Command
{
    std::string initial_state;
    std::string initial_word;
    std::string final_state;
    std::string final_word;
    std::string direction;
    std::string debug;
    std::string lineNumber;
};

} // namespace translator

#endif