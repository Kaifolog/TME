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
#include <exception>
#include "App.hpp"

extern "C"
{
#include <stdio.h>
}

using namespace std;

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    App app(argc, argv);
    if (argc > 1)
    {
        if (!strcmp(argv[1], "-v")) //говорим про версию
        {
            text_v_func();
        }
        else //понимаем что следующее это адрес
        {
            string dir = argv[1];
            string logs_path = argv[1];

            if (logs_path.substr(logs_path.find_last_of(".") + 1) == "tme" || logs_path.substr(logs_path.find_last_of(".") + 1) == "txt")
            {
                logs_path.pop_back();
                logs_path.pop_back();
                logs_path.pop_back();
                logs_path.pop_back();
            }

            logs_path.append("_log.txt");

            //logger configuring
            el::Configurations defaultConf;
            defaultConf.setToDefault();
            defaultConf.setGlobally(
                el::ConfigurationType::Format, "%datetime :: %level %msg");
            //      defaultConf.setGlobally(
            //          el::ConfigurationType::ToStandardOutput, "false");
            defaultConf.setGlobally(
                el::ConfigurationType::ToFile, "true");
            defaultConf.setGlobally(
                el::ConfigurationType::Filename, logs_path);

            //logs file clearing
            defaultConf.setGlobally(
                el::ConfigurationType::MaxLogFileSize, "1");
            el::Loggers::reconfigureLogger("default", defaultConf);
            LOG(INFO) << "Log file cleared";
            defaultConf.setGlobally(
                el::ConfigurationType::MaxLogFileSize, "104857600");
            el::Loggers::reconfigureLogger("default", defaultConf);

            ifstream fin(dir);
            if (!fin.is_open())
            {
                LOG(ERROR) << "File opening exception";
                cin.get();
                return -1;
            }
            fin.close();
            LOG(INFO) << "File opened";

            try
            {

                if (app.check_arguments("-d"))
                {
                    app.context_free_analysis_and_parsing();
                    app.semantic_analysis();
                    app.emulator_executing_procedure(1);
                }

                if (app.check_arguments("-g"))
                    app.context_free_analysis_and_parsing();
                if (app.check_arguments("-a"))
                    app.semantic_analysis();
                if (app.check_arguments("-e"))
                    app.emulator_executing_procedure(0);
                if (argc == 2 || (argc == 3 && string(argv[2]) == "-l"))
                {
                    app.context_free_analysis_and_parsing();
                    app.semantic_analysis();
                    app.emulator_executing_procedure(0);
                }
            }
            catch (...)
            {
                cin.get();
                return 1;
            }
        }
    }
    else
        text_start_func();
    getchar();

    return 0;
}