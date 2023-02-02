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

namespace cli
{

void App::handleArguments()
{
    // i = 1 because _argv[0] is a program path
    for (int i = 1; i < _argc; i++)
    {
        std::string current = _argv[i];
        if (current == "-l")
        {
            this->_no_lambda = true;
        }
        if (current == "-g")
        {
            this->_parsing = true;
        }
        if (current == "-a")
        {
            this->_semantic = true;
        }
        if (current == "-e")
        {
            this->_emulating = true;
        }
        if (current == "-d")
        {
            this->_debug = true;
        }
    }
}

void App::parse()
{
    translator::Translator translator;
    LOG(INFO) << "Translating started... ";

    translator.parse(_pname);

    LOG(INFO) << "Translating ended.";
};

void App::emulate()
{
    machine::TuringMachine tm;

    if (!_debug)
    {
        LOG(INFO) << "Starting emulator...";

        tm.execute(_pname, _no_lambda);

        LOG(INFO) << "Run complete";
        return;
    }

    LOG(INFO) << "Starting _debugger...";
    machine::MachineState result;

    std::string debug_line;
    tm.lazyStart(_pname, _no_lambda);

    do
    {
        result = tm.lazyDebug();
        std::cout << result.current_strip << std::endl;
        std::cout << "current_state: " << result.current_state << ", current_word: " << result.current_word
                  << ", current line: " << result.line << std::endl;
        getline(std::cin, debug_line);
    } while (result.current_state != "end");

    tm.lazyFinalize();
    LOG(INFO) << "Run complete";
};

void App::semanticAnalysis()
{
    translator::Translator translator;
    LOG(INFO) << "Starting translator...";

    translator.analyse(_pname);

    LOG(INFO) << "Analysis complete. All set for execution!";
};

int App::execute()
{
    if (_argc == 1)
    {
        textStartFunc();
        std::cout << std::endl;
        return 0;
    }
    if (_argc == 2 && std::string(_argv[1]) == std::string("-v"))
    {
        textFuncVersion();
        std::cout << std::endl;
        return 0;
    }

    try
    {
        // if there are only path to origin or path to origin + _no_lambda flag
        if (_argc == 2 || (_argc == 3 && std::string(_argv[2]) == "-l"))
        {
            parse();
            semanticAnalysis();
            emulate();
            std::cout << std::endl;
            return 0;
        }

        std::ifstream fin(_pname.getOriginal());
        if (!fin.is_open())
        {
            LOG(ERROR) << "File opening exception";
            std::cout << std::endl;
            return 1;
        }
        fin.close();
        LOG(INFO) << "File exists.";

        // so, we have 2+ arguments and the _argv[2] is not "-v":
        if (_debug)
        {
            parse();
            semanticAnalysis();
            emulate();
            std::cout << std::endl;
            return 0;
        }

        if (_parsing)
        {
            parse();
        }
        if (_semantic)
        {
            semanticAnalysis();
        }
        if (_emulating)
        {
            emulate();
        }
    }
    catch (const char *message)
    {
        LOG(ERROR) << message;
        std::cout << std::endl;
        return 1;
    }
    catch (std::string message)
    {
        LOG(ERROR) << message;
        std::cout << std::endl;
        return 1;
    }
    catch (...)
    {
        LOG(ERROR) << "Something went wrong." << std::endl << "Please tell about this to the developer." << std::endl;
        std::cout << std::endl;
        return 1;
    }
    std::cout << std::endl;
    return 0;
};
} // namespace cli