#include "Translator.hpp"
namespace translator
{

void command_to_sqlite3(sqlite3 *_db, translator::Command current_command, sqlite3_stmt *_pp_stmt)
{
    sqlite3_bind_text(_pp_stmt, 1, current_command.initial_state.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(_pp_stmt, 2, current_command.initial_word.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(_pp_stmt, 3, current_command.final_state.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(_pp_stmt, 4, current_command.final_word.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(_pp_stmt, 5, current_command.direction.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(_pp_stmt, 6, current_command.debug.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(_pp_stmt, 7, current_command.lineNumber.c_str(), -1, SQLITE_STATIC);
    sqlite3_step(_pp_stmt);
    sqlite3_clear_bindings(_pp_stmt);
    sqlite3_reset(_pp_stmt);
};

bool check_str_in_vec(std::vector<std::string> v, std::string s)
{
    return find(v.begin(), v.end(), s) == v.end();
}

void Translator::init(tools::ProjectName &pname)
{
    this->_needs_to_finalize = true;

    _fin.open(pname.getOriginal());
    if (!_fin.is_open())
    {
        throw "Can't open the file.";
    }

    if (sqlite3_open(pname.getDBFile().c_str(), &_db))
    {
        throw "UNDEFINED ERROR : database is unavailable";
    }

    const char *drop_table_query = "DROP TABLE IF EXISTS commands";
    if (sqlite3_exec(_db, drop_table_query, 0, 0, &_err))
    {
        sqlite3_free(_err);
        sqlite3_close(_db);
        throw _err;
    }
    const char *create_table_query =
        "CREATE TABLE IF NOT EXISTS commands(initial_state,initial_word,final_state, final_word,direction,debug,line)";
    if (sqlite3_exec(_db, create_table_query, 0, 0, &_err))
    {
        sqlite3_free(_err);
        sqlite3_close(_db);
        throw _err;
    }
    /***************************/
    sqlite3_exec(_db, "BEGIN TRANSACTION", NULL, NULL, &_err);
    sprintf(_insert_bind, "INSERT INTO commands VALUES (?,?,?,?,?,?,?)");
    sqlite3_prepare_v2(_db, _insert_bind, 256, &_pp_stmt, NULL);
};

void Translator::parse(tools::ProjectName &pname)
{
    init(pname);
    translator::Parser parser;
    translator::Command current_command;
    std::string buffer;
    while (getline(_fin, buffer))
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
        catch (std::string message)
        {
            finalize();
            throw message;
        }
        if (!current_command.lineNumber.empty())
        {
            command_to_sqlite3(_db, current_command, _pp_stmt);
        }
    }
    finalize();
};

void Translator::finalize()
{
    if (this->_needs_to_finalize)
    {
        if (sqlite3_exec(_db, "END TRANSACTION", NULL, NULL, &_err))
        {
            sqlite3_free(_err);
            sqlite3_close(_db);
            throw _err;
        }
        sqlite3_finalize(_pp_stmt);
        sqlite3_exec(_db, "END TRANSACTION", NULL, NULL, &_err);
        sqlite3_free(_err);
        sqlite3_close(_db);
        _fin.close();
    }
};

void Translator::analyse(tools::ProjectName &pname)
{
    std::string a = pname.getDBFile();
    if (sqlite3_open(a.c_str(), &_db))
    {
        throw "UNDEFINED ERROR : database is unavailable";
    }
    // sqlite3_exec(_db, "PRAGMA synchronous = OFF", NULL, NULL, &_err); //SO FUCKING RISKY
    std::string select_command;
    select_command = "SELECT *FROM commands";
    sqlite3_prepare_v2(_db, select_command.c_str(), 256, &_pp_stmt, NULL);
    std::vector<std::string> alphabet;

    while (sqlite3_step(_pp_stmt) != SQLITE_DONE)
    {
        if (check_str_in_vec(alphabet, std::string((char *)sqlite3_column_text(_pp_stmt, 1))))
            alphabet.push_back(std::string((char *)sqlite3_column_text(_pp_stmt, 1)));
        if (check_str_in_vec(alphabet, std::string((char *)sqlite3_column_text(_pp_stmt, 3))))
            alphabet.push_back(std::string((char *)sqlite3_column_text(_pp_stmt, 3)));
    }
    sqlite3_finalize(_pp_stmt);

    select_command = "SELECT *FROM commands";
    sqlite3_prepare_v2(_db, select_command.c_str(), 256, &_pp_stmt, NULL);

    std::vector<std::string> statements;
    while (sqlite3_step(_pp_stmt) != SQLITE_DONE)
    {
        if (check_str_in_vec(statements, std::string((char *)sqlite3_column_text(_pp_stmt, 2))))
        {
            statements.push_back(std::string((char *)sqlite3_column_text(_pp_stmt, 2)));
        }
        if (check_str_in_vec(statements, std::string((char *)sqlite3_column_text(_pp_stmt, 0))))
        {
            if (std::string((char *)sqlite3_column_text(_pp_stmt, 0)) != "end")
            {
                statements.push_back(std::string((char *)sqlite3_column_text(_pp_stmt, 0)));
            }
            else
            {
                sqlite3_finalize(_pp_stmt);
                sqlite3_exec(_db, "END TRANSACTION", NULL, NULL, &_err);
                sqlite3_free(_err);
                sqlite3_close(_db);
                throw "Analyser FATAL ERROR: There are \"end\" at the left side of command.";
            }
        }
    }
    sqlite3_finalize(_pp_stmt);

    if (check_str_in_vec(statements, "start"))
    {
        sqlite3_exec(_db, "END TRANSACTION", NULL, NULL, &_err);
        sqlite3_free(_err);
        sqlite3_close(_db);
        throw "Analyser FATAL ERROR: There are no \"start\" command.";
    }

    if (check_str_in_vec(statements, "end"))
    {
        sqlite3_exec(_db, "END TRANSACTION", NULL, NULL, &_err);
        sqlite3_free(_err);
        sqlite3_close(_db);
        throw "Analyser FATAL ERROR: There are no \"end\" command.";
    }

    for (int i = 0; i < statements.size(); i++)
        for (int j = 0; j < alphabet.size(); j++)
        {
            std::string select_command = "SELECT *FROM commands WHERE initial_state=\"" + statements[i] +
                                         "\" AND initial_word=\"" + alphabet[j] + "\"";
            sqlite3_prepare_v2(_db, select_command.c_str(), 256, &_pp_stmt, NULL);
            if (sqlite3_step(_pp_stmt) != SQLITE_DONE)
            {
                if (sqlite3_step(_pp_stmt) != SQLITE_DONE)
                {
                    std::string a = "Analyser FATAL ERROR! There are two or more commands with same left side. " +
                                    std::string((char *)sqlite3_column_text(_pp_stmt, 0)) + " " +
                                    std::string((char *)sqlite3_column_text(_pp_stmt, 1));
                    sqlite3_finalize(_pp_stmt);
                    sqlite3_exec(_db, "END TRANSACTION", NULL, NULL, &_err);
                    sqlite3_free(_err);
                    sqlite3_close(_db);
                    throw a;
                }
            }
            else
            {
                std::string a =
                    "You might have to create command with state =  " + statements[i] + ", \tword = " + alphabet[j];
            }
        }

    sqlite3_finalize(_pp_stmt);
    sqlite3_exec(_db, "END TRANSACTION", NULL, NULL, &_err);
    sqlite3_free(_err);
    sqlite3_close(_db);
};

} // namespace translator