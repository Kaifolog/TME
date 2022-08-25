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

#include "parser/parser.hpp"

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

    LOG(INFO) << "Database closed";
    LOG(INFO) << "Parsing ended.";
}

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
}

bool App::check_str_in_vec(vector<string> v, string s)
{
    return find(v.begin(), v.end(), s) == v.end();
}

void App::semantic_analysis()
{
    sqlite3 *db = 0;
    sqlite3_stmt *ppStmt;

    LOG(INFO) << "Starting analyser...";
    TuringMachine tm;
    string dir;
    dir = path;
    if (dir.substr(dir.find_last_of(".") + 1) == "tme" || dir.substr(dir.find_last_of(".") + 1) == "txt")
    {
        dir.pop_back();
        dir.pop_back();
        dir.pop_back();
        dir.pop_back();
    }

    /**************/
    string a = dir;
    a.append(".db");
    LOG(INFO) << "Trying to open/create database";
    if (sqlite3_open(a.c_str(), &db))
    {
        LOG(ERROR) << "UNDEFINED ERROR : DATABASE UNAVAILABLE";
        throw std::exception();
    }
    // sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &err); //SO FUCKING RISKY
    string select_command;
    select_command = "SELECT *FROM commands";
    sqlite3_prepare_v2(db, select_command.c_str(), 256, &ppStmt, NULL);
    vector<string> alphabet;

    while (sqlite3_step(ppStmt) != SQLITE_DONE)
    {
        if (check_str_in_vec(alphabet, string((char *)sqlite3_column_text(ppStmt, 1))))
            alphabet.push_back(string((char *)sqlite3_column_text(ppStmt, 1)));
        if (check_str_in_vec(alphabet, string((char *)sqlite3_column_text(ppStmt, 3))))
            alphabet.push_back(string((char *)sqlite3_column_text(ppStmt, 3)));
    }
    sqlite3_finalize(ppStmt);

    select_command = "SELECT *FROM commands";
    sqlite3_prepare_v2(db, select_command.c_str(), 256, &ppStmt, NULL);

    vector<string> statements;
    while (sqlite3_step(ppStmt) != SQLITE_DONE)
    {
        if (check_str_in_vec(statements, string((char *)sqlite3_column_text(ppStmt, 2))))
        {
            statements.push_back(string((char *)sqlite3_column_text(ppStmt, 2)));
        }
        if (check_str_in_vec(statements, string((char *)sqlite3_column_text(ppStmt, 0))))
        {
            if (string((char *)sqlite3_column_text(ppStmt, 0)) != "end")
            {
                statements.push_back(string((char *)sqlite3_column_text(ppStmt, 0)));
            }
            else
            {
                LOG(ERROR) << "ANALYSER FATAL ERROR! There are \"end\" at the left side of command.";
                sqlite3_close(db);
                throw std::exception();
            }
        }
    }
    sqlite3_finalize(ppStmt);

    if (check_str_in_vec(statements, "start"))
    {
        LOG(ERROR) << "ANALYSER FATAL ERROR! There are no \"start\" command.";
        sqlite3_close(db);
        throw std::exception();
    }

    if (check_str_in_vec(statements, "end"))
    {
        LOG(ERROR) << "ANALYSER FATAL ERROR! There are no \"end\" command.";
        sqlite3_close(db);
        throw std::exception();
    }

    for (int i = 0; i < statements.size(); i++)
        for (int j = 0; j < alphabet.size(); j++)
        {
            string select_command = "SELECT *FROM commands WHERE initial_state=\"" + statements[i] + "\" AND initial_word=\"" + alphabet[j] + "\"";
            sqlite3_prepare_v2(db, select_command.c_str(), 256, &ppStmt, NULL);
            if (sqlite3_step(ppStmt) != SQLITE_DONE)
            {
                if (sqlite3_step(ppStmt) != SQLITE_DONE)
                {
                    string a = "ANALYSER FATAL ERROR!!! There are two or more commands with same left side. " + string((char *)sqlite3_column_text(ppStmt, 0)) + " " + string((char *)sqlite3_column_text(ppStmt, 1));
                    LOG(ERROR) << a;
                    sqlite3_close(db);
                    throw std::exception();
                }
            }
            else
            {
                string a = "You might have to create command with state =  " + statements[i] + ", \tword = " + alphabet[j];
                LOG(WARNING) << a;
            }
        }

    sqlite3_close(db);
    LOG(INFO) << "Analysis complete";
}
