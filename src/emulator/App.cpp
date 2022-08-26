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

void App::handleArguments()
{
    // i = 1 because argv[0] is a program path
    for (int i = 1; i < argc; i++)
    {
        string current = argv[i];
        if (current == "-l")
        {
            this->noLambda = true;
        }
        if (current == "-g")
        {
            this->parsing = true;
        }
        if (current == "-a")
        {
            this->semantic = true;
        }
        if (current == "-e")
        {
            this->emulating = true;
        }
        if (current == "-d")
        {
            this->debug = true;
        }
    }
}

void App::parse()
{
    Parser parser;
    LOG(INFO) << "Parser started... ";

    parser.parse(_pname);

    LOG(INFO) << "Parsing ended.";
};

void App::emulate()
{
    TuringMachine tm;

    if (!debug)
    {
        LOG(INFO) << "Starting emulator...";

        tm.execute(_pname, noLambda);

        LOG(INFO) << "Run complete";
        return;
    }

    LOG(INFO) << "Starting debugger...";
    MachineState result;

    string debug_line;
    tm.lazyStart(_pname, noLambda);

    do
    {
        result = tm.lazyDebug();
        cout << result.current_strip << endl;
        cout << "current_state: " << result.current_state << ", current_word: " << result.current_word << ", current line: " << result.line << endl;
        getline(cin, debug_line);
    } while (result.current_state != "end");

    tm.lazyFinalize();
    LOG(INFO) << "Run complete";
};

void App::semantic_analysis()
{
    Parser parser;
    LOG(INFO) << "Starting analyser...";

    parser.analyse(_pname);

    LOG(INFO) << "Analysis complete. All set for execution!";
};

int App::execute()
{
    if (argc == 1)
    {
        text_start_func();
        cout << endl;
        return 0;
    }
    if (argc == 2 && string(argv[1]) == string("-v"))
    {
        text_v_func();
        cout << endl;
        return 0;
    }

    try
    {
        // if there are only path to origin or path to origin + noLambda flag
        if (argc == 2 || (argc == 3 && string(argv[2]) == "-l"))
        {
            parse();
            semantic_analysis();
            emulate();
            cout << endl;
            return 0;
        }

        ifstream fin(_pname.getOriginal());
        if (!fin.is_open())
        {
            LOG(ERROR) << "File opening exception";
            cout << endl;
            return 1;
        }
        fin.close();
        LOG(ERROR) << "File exists.";

        // so, we have 2+ arguments and the argv[2] is not "-v":
        if (debug)
        {
            parse();
            semantic_analysis();
            emulate();
            cout << endl;
            return 0;
        }

        if (parsing)
        {
            parse();
        }
        if (semantic)
        {
            semantic_analysis();
        }
        if (emulating)
        {
            emulate();
        }
    }
    catch (const char *message)
    {
        LOG(ERROR) << message;
        cout << endl;
        return 1;
    }
    catch (string message)
    {
        LOG(ERROR) << message;
        cout << endl;
        return 1;
    }
    catch (...)
    {
        LOG(ERROR) << "Something went wrong." << endl
                   << "Please tell about this to the developer." << endl;
        cout << endl;
        return 1;
    }
    cout << endl;
    return 0;
};