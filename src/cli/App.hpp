#ifndef _APP
#define _APP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

extern "C"
{
#include "../external/sqlite3/sqlite3.h"
}
#include "../external/easyloggingpp/easylogging++.h"

#include "../machine/MachineState.hpp"
#include "../machine/TuringMachine.hpp"
#include "../tools/ProjectName.hpp"
#include "../translator/translator.hpp"
#include "libs/text_functions.hpp"

namespace cli
{

class App
{
    int _argc;
    char **_argv;

    ProjectName _pname;

    bool _no_lambda = false;
    bool _parsing = false;
    bool _semantic = false;
    bool _emulating = false;
    bool _debug = false;

    App(){};

  public:
    App(ProjectName pname, int argc, char *argv[])
    {
        _pname = std::move(pname);
        _argc = argc;
        _argv = argv;
        handleArguments();
    };

    void parse();
    void semanticAnalysis();
    void emulate();
    void handleArguments();
    int execute();
};

} // namespace cli

#endif