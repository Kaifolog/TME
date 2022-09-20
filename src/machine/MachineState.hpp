#include <string>
#include <vector>

#ifndef _MACHINESTATE
#define _MACHINESTATE

struct MachineState
{
    std::string current_strip = "";
    std::string current_word = "";
    std::string current_state = "";
    std::string line = "1";
};

#endif