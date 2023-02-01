#include "translator.hpp"

void command_to_sqlite3(sqlite3 *db, translator::Command current_command, sqlite3_stmt *ppStmt)
{
    sqlite3_bind_text(ppStmt, 1, current_command.initial_state.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(ppStmt, 2, current_command.initial_word.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(ppStmt, 3, current_command.final_state.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(ppStmt, 4, current_command.final_word.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(ppStmt, 5, current_command.direction.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(ppStmt, 6, current_command.debug.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(ppStmt, 7, current_command.lineNumber.c_str(), -1, SQLITE_STATIC);
    sqlite3_step(ppStmt);
    sqlite3_clear_bindings(ppStmt);
    sqlite3_reset(ppStmt);
};

bool check_str_in_vec(vector<string> v, string s)
{
    return find(v.begin(), v.end(), s) == v.end();
}

void Translator::init(Tools::ProjectName &pname)
{
    this->needsToFinalize = true;

    fin.open(pname.getOriginal());
    if (!fin.is_open())
    {
        throw "Can't open the file.";
    }

    if (sqlite3_open(pname.getDBFile().c_str(), &db))
    {
        throw "UNDEFINED ERROR : DATABASE UNAVAILABLE";
    }

    const char *drop_table_query = "DROP TABLE IF EXISTS commands";
    if (sqlite3_exec(db, drop_table_query, 0, 0, &err))
    {
        sqlite3_free(err);
        sqlite3_close(db);
        throw err;
    }
    const char *create_table_query =
        "CREATE TABLE IF NOT EXISTS commands(initial_state,initial_word,final_state, final_word,direction,debug,line)";
    if (sqlite3_exec(db, create_table_query, 0, 0, &err))
    {
        sqlite3_free(err);
        sqlite3_close(db);
        throw err;
    }
    /***************************/
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &err);
    sprintf(insert_bind, "INSERT INTO commands VALUES (?,?,?,?,?,?,?)");
    sqlite3_prepare_v2(db, insert_bind, 256, &ppStmt, NULL);
};

void Translator::parse(Tools::ProjectName &pname)
{
    init(pname);
    translator::Parser parser;
    translator::Command current_command;
    string buffer;
    while (getline(fin, buffer))
    {
        try
        {
            current_command = parser.parse(buffer);
        }
        catch (const char *message)
        {
            finalize();
            throw message;
        }
        catch (string message)
        {
            finalize();
            throw message;
        }
        if (!current_command.lineNumber.empty())
        {
            command_to_sqlite3(db, current_command, ppStmt);
        }
    }
    finalize();
};

void Translator::finalize()
{
    if (this->needsToFinalize)
    {
        if (sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &err))
        {
            sqlite3_free(err);
            sqlite3_close(db);
            throw err;
        }
        sqlite3_finalize(ppStmt);
        sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &err);
        sqlite3_free(err);
        sqlite3_close(db);
        fin.close();
    }
};

void Translator::analyse(Tools::ProjectName &pname)
{
    string a = pname.getDBFile();
    if (sqlite3_open(a.c_str(), &db))
    {
        throw "UNDEFINED ERROR : DATABASE UNAVAILABLE";
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
                sqlite3_close(db);
                throw "ANALYSER FATAL ERROR! There are \"end\" at the left side of command.";
            }
        }
    }
    sqlite3_finalize(ppStmt);

    if (check_str_in_vec(statements, "start"))
    {
        sqlite3_close(db);
        throw "ANALYSER FATAL ERROR! There are no \"start\" command.";
    }

    if (check_str_in_vec(statements, "end"))
    {
        sqlite3_close(db);
        throw "ANALYSER FATAL ERROR! There are no \"end\" command.";
    }

    for (int i = 0; i < statements.size(); i++)
        for (int j = 0; j < alphabet.size(); j++)
        {
            string select_command = "SELECT *FROM commands WHERE initial_state=\"" + statements[i] +
                                    "\" AND initial_word=\"" + alphabet[j] + "\"";
            sqlite3_prepare_v2(db, select_command.c_str(), 256, &ppStmt, NULL);
            if (sqlite3_step(ppStmt) != SQLITE_DONE)
            {
                if (sqlite3_step(ppStmt) != SQLITE_DONE)
                {
                    string a = "ANALYSER FATAL ERROR!!! There are two or more commands with same left side. " +
                               string((char *)sqlite3_column_text(ppStmt, 0)) + " " +
                               string((char *)sqlite3_column_text(ppStmt, 1));
                    sqlite3_close(db);
                    throw a;
                }
            }
            else
            {
                string a =
                    "You might have to create command with state =  " + statements[i] + ", \tword = " + alphabet[j];
            }
        }

    sqlite3_close(db);
};