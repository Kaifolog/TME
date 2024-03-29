#ifndef _TURINGMACHINE
#define _TURINGMACHINE

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <string>

#include "MachineState.hpp"
extern "C"
{
#include "../external/sqlite3/sqlite3.h"
}

#include "../tools/ProjectName.hpp"

namespace machine
{

class TuringMachine
{
    std::list<std::string> _strip;
    std::list<std::string>::iterator _cursor;
    std::string _statement;
    std::string _last_line;
    long long int _max_steps;
    long long int _steps_counter;

    // connection parameters
    std::string _dir;
    std::string _output;
    bool _lambda;
    sqlite3 *_db;
    sqlite3_stmt *_pp_stmt;
    char *_err;
    std::ifstream _fin;

    inline bool testForPrimaryCursor(std::string cell)
    {
        return cell[0] == '|' && cell[cell.size() - 1] == '|';
    }

    bool isEnd(std::string dir, bool lambda);
    bool loadStrip(std::string dir);
    std::string getStrip();
    std::string getStrip(bool lambda);
    void getStep(char a);

  public:
    TuringMachine()
    {
        _statement = "start";
        _db = nullptr;
        _err = nullptr;
        _lambda = false;
        _max_steps = 0;
        _steps_counter = 0;
    };

    TuringMachine(long long int max_steps)
    {
        _statement = "start";
        _db = nullptr;
        _err = nullptr;
        _lambda = false;
        _max_steps = max_steps;
        _steps_counter = 0;
    };

    int execute(tools::ProjectName &pname, bool lambda);
    void lazyStart(tools::ProjectName &pname, bool lambda);
    void setLambda(bool lambda)
    {
        this->_lambda = lambda;
    };
    void setMaxStep(long long int max_steps)
    {
        this->_max_steps = max_steps;
    };
    MachineState lazyDebug(bool step_by_step = false);
    void lazyFinalize();
};

} // namespace machine

#endif