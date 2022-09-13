#include "../external/easyloggingpp/easylogging++.h"
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

class Translator;

struct Command
{
    string initial_state;
    string initial_word;
    string final_state;
    string final_word;
    string direction;
    string debug;
    string lineNumber;
};

#endif