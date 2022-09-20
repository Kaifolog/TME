extern "C"
{
#include <stdio.h>
}
#include <algorithm>
#include <chrono>
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

#ifndef _APP
#define _APP

using namespace std;

class App
{
    int argc;
    char **argv;

    ProjectName _pname;

    bool noLambda = false;
    bool parsing = false;
    bool semantic = false;
    bool emulating = false;
    bool debug = false;

    App(){};

  public:
    App(ProjectName pname, int _argc, char *_argv[])
    {
        _pname = pname;
        argc = _argc;
        argv = _argv;
        handleArguments();
    };

    void parse();
    void semantic_analysis();
    void emulate();
    void handleArguments();
    int execute();
};

#endif