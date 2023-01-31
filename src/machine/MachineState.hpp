#ifndef _MACHINESTATE
#define _MACHINESTATE

#include <string>
#include <vector>

namespace Machine
{

struct MachineState
{
    std::string current_strip;
    std::string current_word;
    std::string current_state;
    std::string line;

    MachineState()
    {
        line = "1";
    }
};

} // namespace Machine

#endif