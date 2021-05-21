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
extern "C"
{
#include "parser/lexer.h"
}

using namespace std;

void App::command_to_sqlite3(sqlite3 *db, struct Command *current_command, sqlite3_stmt *ppStmt)
{
    sqlite3_bind_text(ppStmt, 1, current_command->initial_state, -1, SQLITE_STATIC);
    sqlite3_bind_text(ppStmt, 2, current_command->initial_word, -1, SQLITE_STATIC);
    sqlite3_bind_text(ppStmt, 3, current_command->final_state, -1, SQLITE_STATIC);
    sqlite3_bind_text(ppStmt, 4, current_command->final_word, -1, SQLITE_STATIC);
    string sLastName;
    sLastName.insert(sLastName.size(), 1, current_command->direction);
    sqlite3_bind_text(ppStmt, 5, sLastName.c_str(), -1, SQLITE_STATIC);
    sqlite3_step(ppStmt);
    sqlite3_clear_bindings(ppStmt);
    sqlite3_reset(ppStmt);
}

bool App::check_arguments(int argc, char *argv[], char *search)
{
    bool res = 0;
    for (int i = 0; i < argc; i++)
        res = res + (string(argv[i]) == string(search));
    return res;
}

void App::context_free_analysis_and_parsing(int argc, char *argv[])
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

    /***************************/

    sqlite3 *db = 0;
    char *err = 0;
    a = dir;
    a.append(".db");
    if (sqlite3_open(a.c_str(), &db))
        error("undefined error sqlite3 db.", 0);
    //sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &err); //SO FUCKING RISKY
    const char *drop_table_query = "DROP TABLE IF EXISTS commands";
    if (sqlite3_exec(db, drop_table_query, 0, 0, &err))
    {
        error(err, 0);
        sqlite3_free(err);
    }
    const char *create_table_query =
        "CREATE TABLE IF NOT EXISTS commands(initial_state,initial_word,final_state, final_word,  direction)";
    if (sqlite3_exec(db, create_table_query, 0, 0, &err))
    {
        error(err, 0);
        sqlite3_free(err);
    }
    /***************************/
    a.clear();
    macros_table = (char ***)svwm_vector(0, sizeof(char **));
    struct Command *current_command;

    log_message("Starting Parsing");
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &err);
    char insert_bind[256];
    sqlite3_stmt *ppStmt;
    sprintf(insert_bind, "INSERT INTO commands VALUES (?,?,?,?,?)");
    sqlite3_prepare_v2(db, insert_bind, 256, &ppStmt, NULL);
    while (!fin.eof())
    {
        string buffer;
        getline(fin, buffer);
        if (current_command = lexer(buffer.c_str(), buffer.size()))
        {
            command_to_sqlite3(db, current_command, ppStmt);
            command_destructor(current_command);
        }
    }
    sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &err);
    sqlite3_close(db);
    fin.close();
    macros_table_destructor();
    log_message("Parsing ended.");
}

void App::emulator_executing_procedure(int argc, char *argv[], bool debug_statement)
{

    sqlite3 *db = 0;
    char *err = 0;
    sqlite3_stmt *ppStmt;

    log_message("Starting emulator...");
    TuringMachine tm;

    if (tm.load_strip("datasection.tmp"))
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

        /**************/
        string a = dir;
        a.append(".db");

        sqlite3_open(a.c_str(), &db);
        sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &err); //SO FUCKING RISKY
        sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &err);
        string select_command;
        while (!tm.is_end(dir))
        {
            select_command = "SELECT *FROM commands WHERE initial_state=\"" + tm.get_current_state() + "\" AND initial_word=\"" + tm.get_current_word() + "\"";
            sqlite3_prepare_v2(db, select_command.c_str(), 256, &ppStmt, NULL);
            if (debug_statement)
            {
                cout << tm.get_strip() << endl;
                cout << "Current data: " << tm.get_current_state() << " " << tm.get_current_word() << endl;
                getchar();
            }

            if (sqlite3_step(ppStmt) != SQLITE_DONE)
            {
                tm.set_current_state(string((char *)sqlite3_column_text(ppStmt, 2)));
                tm.set_current_word(string((char *)sqlite3_column_text(ppStmt, 3)));
                tm.get_step(((char *)sqlite3_column_text(ppStmt, 4))[0]);
            }
            else
            {
                error("emulation error : cant find next command", 0);
            }
            sqlite3_finalize(ppStmt);
        }
        sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &err);
        cout << sqlite3_close(db);
        fin.close();
        log_message("Run completed");
    }
    else
    {
        error("there are some troubles with .data file", 0);
    }
}
bool App::check_str_in_vec(vector<string> v, string s)
{
    return find(v.begin(), v.end(), s) == v.end();
}
void App::semantic_analysis(int argc, char *argv[])
{
    sqlite3 *db = 0;
    char *err = 0;
    sqlite3_stmt *ppStmt;

    log_message("Starting analiser...");
    TuringMachine tm;
    string dir;
    dir = argv[1];
    if (dir.substr(dir.find_last_of(".") + 1) == "tme")
    {
        dir.pop_back();
        dir.pop_back();
        dir.pop_back();
        dir.pop_back();
    }

    /**************/
    string a = dir;
    a.append(".db");

    sqlite3_open(a.c_str(), &db);
    sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &err); //SO FUCKING RISKY
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
        if (check_str_in_vec(statements, string((char *)sqlite3_column_text(ppStmt, 0))))
        {
            if (string((char *)sqlite3_column_text(ppStmt, 0)) != "end")
                statements.push_back(string((char *)sqlite3_column_text(ppStmt, 0)));
            else
                error("analiser fatal error!!! There are \"end\" at the left side of command.", 0);
        }
        if (check_str_in_vec(statements, string((char *)sqlite3_column_text(ppStmt, 2))))
            statements.push_back(string((char *)sqlite3_column_text(ppStmt, 2)));
    }
    sqlite3_finalize(ppStmt);

    if (check_str_in_vec(statements, "start"))
        error("analiser fatal error!!! There are no \"start\" command.", 0);
    if (check_str_in_vec(statements, "end"))
        log_message("analiser warning! There are no \"end\" command.");

    for (int i = 0; i < statements.size(); i++)
        for (int j = 0; j < alphabet.size(); j++)
            cout << statements[i] << " " << alphabet[j] << endl;

    for (int i = 0; i < statements.size(); i++)
        for (int j = 0; j < alphabet.size(); j++)
        {
            string select_command = "SELECT *FROM commands WHERE initial_state=\"" + statements[i] + "\" AND initial_word=\"" + alphabet[j] + "\"";
            sqlite3_prepare_v2(db, select_command.c_str(), 256, &ppStmt, NULL);
            if (sqlite3_step(ppStmt) != SQLITE_DONE)
            {
                if (sqlite3_step(ppStmt) != SQLITE_DONE)
                {
                    string a = "analiser fatal error!!! There are two or more commands with same left side. " + string((char *)sqlite3_column_text(ppStmt, 0)) + " " + string((char *)sqlite3_column_text(ppStmt, 1));
                    error((char *)a.c_str(), 0);
                }
            }
            else
            {
                string a = "analiser warning! You might have to create command with state =  " + statements[i] + " word = " + alphabet[j];
                log_message((char *)a.c_str());
            }
        }

    sqlite3_close(db);
    log_message("Analisys completed");
}