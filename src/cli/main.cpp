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

#include "App.hpp"
#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    Tools::ProjectName pname;
    if (argc > 1 && string(argv[1]) != string("-v"))
    {
        pname.setOriginal(string(argv[1]));

        // logger configuring
        el::Configurations defaultConf;
        defaultConf.setToDefault();
        defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime :: %level %msg");
        defaultConf.setGlobally(el::ConfigurationType::ToFile, "true");
        defaultConf.setGlobally(el::ConfigurationType::Filename, pname.getLogFile());
        defaultConf.setGlobally(el::ConfigurationType::MaxLogFileSize, "104857600"); // 100mb
        el::Loggers::reconfigureLogger("default", defaultConf);

        // clearing log file
        std::ofstream cleanLogs;
        cleanLogs.open(pname.getLogFile(), std::ofstream::out | std::ofstream::trunc);
        cleanLogs.close();
    }

    cli::App a(pname, argc, argv);

    return a.execute();
}