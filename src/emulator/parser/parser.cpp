#include "parser.hpp"

void command_to_sqlite3(sqlite3 *db, Command *current_command, sqlite3_stmt *ppStmt)
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
};

void Parser::init(string &path)
{
        this->needsToFinalize = true;

        this->dir = path;
        fin.open(dir);
        if (!fin.is_open())
        {
                throw "Can't open the file.";
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

        a = dir;
        a.append(".db");
        if (sqlite3_open(a.c_str(), &db))
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
        a.clear();
        sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &err);
        sprintf(insert_bind, "INSERT INTO commands VALUES (?,?,?,?,?,?,?)");
        sqlite3_prepare_v2(db, insert_bind, 256, &ppStmt, NULL);
};

void Parser::parse(string &path)
{
        init(path);
        Command *current_command;
        string buffer;
        while (getline(fin, buffer))
        {
                try
                {
                        current_command = new Command(buffer, this);
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
                if (!current_command->is_empty())
                {
                        command_to_sqlite3(db, current_command, ppStmt);
                }
                delete current_command;
        }
        finalize();
};

void Parser::finalize()
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