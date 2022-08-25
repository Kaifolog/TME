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

using namespace std;

bool App::check_arguments(char *search, int argc, char **argv)
{
    bool res = 0;
    for (int i = 0; i < argc; i++)
        res = res + (string(argv[i]) == string(search));
    return res;
}

void App::context_free_analysis_and_parsing()
{
    Parser parser;
    LOG(INFO) << "Parser started... ";
    try
    {
        parser.parse(path);
    }
    catch (const char *message)
    {
        LOG(ERROR) << message;
        return;
    }
    catch (string message)
    {
        LOG(ERROR) << message;
        return;
    }

    LOG(INFO) << "Parsing ended.";
};

void App::emulator_executing_procedure()
{
    TuringMachine tm;

    if (!debug_statement)
    {
        LOG(INFO) << "Starting emulator...";
        try
        {

            tm.execute(path, lambda);
        }
        catch (const char *message)
        {
            LOG(ERROR) << message;
            return;
        }

        LOG(INFO) << "Run complete";
        return;
    }

    LOG(INFO) << "Starting debugger...";
    MachineState result;
    try
    {
        string debug_line;
        tm.lazyStart(path, lambda);

        do
        {
            result = tm.lazyDebug();
            cout << result.current_strip << endl;
            cout << "current_state: " << result.current_state << ", current_word: " << result.current_word << ", current line: " << result.line << endl;
            getline(cin, debug_line);
        } while (result.current_state != "end");

        tm.lazyFinalize();
        LOG(INFO) << "Run complete";
    }
    catch (const char *message)
    {
        LOG(ERROR) << message;
    }
    catch (string message)
    {
        LOG(ERROR) << message;
    }
};

void App::semantic_analysis()
{
    Parser parser;
    LOG(INFO) << "Starting analyser...";
    try
    {
        parser.analyse(path);
    }
    catch (const char *message)
    {
        LOG(ERROR) << message;
        LOG(INFO) << "Ooh. Looks bad.";
        return;
    }
    catch (string message)
    {
        LOG(ERROR) << message;
        LOG(INFO) << "Ooh. Looks bad.";
        return;
    }
    LOG(INFO) << "Analysis complete. All set for execution!";
};
