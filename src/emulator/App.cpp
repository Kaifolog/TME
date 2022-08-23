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

#include "parser/lexer.hpp"

using namespace std;

void App::command_to_sqlite3(sqlite3 *db, Command *current_command, sqlite3_stmt *ppStmt)
{
    sqlite3_bind_text(ppStmt, 1, current_command->initial_state.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(ppStmt, 2, current_command->initial_word.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(ppStmt, 3, current_command->final_state.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(ppStmt, 4, current_command->final_word.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(ppStmt, 5, current_command->direction.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(ppStmt, 6, current_command->debug.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(ppStmt, 7, current_command->lineNumber.c_str(), -1, SQLITE_STATIC);
    sqlite3_step(ppStmt);
    sqlite3_clear_bindings(ppStmt);
    sqlite3_reset(ppStmt);
}

bool App::check_arguments(char *search, int argc, char **argv)
{
    bool res = 0;
    for (int i = 0; i < argc; i++)
        res = res + (string(argv[i]) == string(search));
    return res;
}

void App::context_free_analysis_and_parsing()
{
    string dir;
    dir = path;
    ifstream fin;
    fin.open(dir);
    if (!fin.is_open())
    {
        std::cerr << "gg" << std::endl;
    }
    if (dir.substr(dir.find_last_of(".") + 1) == "tme" || dir.substr(dir.find_last_of(".") + 1) == "txt")
    {
        dir.pop_back();
        dir.pop_back();
        dir.pop_back();
        dir.pop_back();
    }
    string a = dir;

    /***************************/

    sqlite3 *db = 0;
    char *err = 0;
    a = dir;
    a.append(".db");
    LOG(INFO) << "Trying to open/create database";
    if (sqlite3_open(a.c_str(), &db))
    {
        LOG(ERROR) << "UNDEFINED ERROR : DATABASE UNAVAILABLE";
        throw std::exception();
    }

    const char *drop_table_query = "DROP TABLE IF EXISTS commands";
    if (sqlite3_exec(db, drop_table_query, 0, 0, &err))
    {
        LOG(ERROR) << err;
        sqlite3_free(err);
        sqlite3_close(db);
        throw std::exception();
    }
    const char *create_table_query =
        "CREATE TABLE IF NOT EXISTS commands(initial_state,initial_word,final_state, final_word,direction,debug,line)";
    if (sqlite3_exec(db, create_table_query, 0, 0, &err))
    {
        LOG(ERROR) << err;
        sqlite3_free(err);
        sqlite3_close(db);
        throw std::exception();
    }
    /***************************/
    a.clear();

    Parser *p = new Parser;
    Command *current_command;

    LOG(INFO) << "Parser started... ";
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &err);
    char insert_bind[256];
    sqlite3_stmt *ppStmt;
    sprintf(insert_bind, "INSERT INTO commands VALUES (?,?,?,?,?,?,?)");
    sqlite3_prepare_v2(db, insert_bind, 256, &ppStmt, NULL);

    string buffer;
    while (getline(fin, buffer))
    {
        try
        {
            current_command = new Command(buffer, p);
        }
        catch (...)
        {
            sqlite3_finalize(ppStmt);
            sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &err);
            sqlite3_free(err);
            sqlite3_close(db);
            fin.close();
            delete p;
            throw;
        }
        if (!current_command->is_empty())
        {
            command_to_sqlite3(db, current_command, ppStmt);
        }
        delete current_command;
    }
    if (sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &err))
    {
        LOG(ERROR) << err;
        sqlite3_free(err);
        sqlite3_close(db);
        throw std::exception();
    }
    sqlite3_finalize(ppStmt);
    sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &err);
    sqlite3_free(err);
    sqlite3_close(db);
    LOG(INFO) << "Database closed";
    fin.close();
    delete p;
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
