/*
MIT License
Copyright (c) 2021 Kaifolog
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// The official repository: <https://github.com/Kaifolog/TME>.

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <algorithm>
#include "App.hpp"

extern "C"
{
#include <stdio.h>
}

using namespace std;

int main(int argc, char *argv[])
{
    App app;
    if (argc > 1)
    {
        if (!strcmp(argv[1], "-v")) //говорим про версию
        {
            text_v_func();
        }
        else //понимаем что следующее это адрес
        {

            string dir;
            dir = argv[1];
            ifstream fin;
            fin.open(dir);
            if (dir.substr(dir.find_last_of(".") + 1) == "tme")
            {
                dir.pop_back();
                dir.pop_back();
                dir.pop_back();
                dir.pop_back();
            }
            string a = dir;
            a.append("_log.txt");
            log_init(a.c_str());
            if (!fin.is_open())
            {
                error("There are no such file!", 0);
            }

            if (app.check_arguments(argc, argv, "-d"))
            {
                app.context_free_analysis_and_parsing(argc, argv);
                app.semantic_analysis(argc, argv);
                app.emulator_executing_procedure(argc, argv, 1);
            }

            if (app.check_arguments(argc, argv, "-g"))
                app.context_free_analysis_and_parsing(argc, argv);
            if (app.check_arguments(argc, argv, "-a"))
                app.semantic_analysis(argc, argv);
            if (app.check_arguments(argc, argv, "-e"))
                app.emulator_executing_procedure(argc, argv, 0);
            if (argc == 2)
            {
                app.context_free_analysis_and_parsing(argc, argv);
                app.semantic_analysis(argc, argv);
                app.emulator_executing_procedure(argc, argv, 0);
            }
        }
    }
    else
        text_start_func();
    getchar();

    return 0;
}
//cmake ./bin && cmake --build ./bin && start ./bin/Debug/TME.exe