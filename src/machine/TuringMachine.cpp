#include "TuringMachine.hpp"

using namespace std;

inline bool space(char c)
{
    return isspace(c);
}

inline bool notspace(char c)
{
    return !isspace(c);
}

list<string> split(const string &s)
{
    typedef string::const_iterator iter;
    list<string> ret;
    iter i = s.begin();
    while (i != s.end())
    {
        i = find_if(i, s.end(), notspace);
        iter j = find_if(i, s.end(), space);
        if (i != s.end())
        {
            ret.push_back(string(i, j));
            i = j;
        }
    }
    return ret;
}

bool TuringMachine::is_end(string dir, bool lambda)
{
    if (statement == "end")
    {
        ofstream fout;
        fout.open(dir);

        std::list<string>::iterator it;
        for (it = strip.begin(); it != strip.end(); ++it)
        {
            if (*it == "lambda" && lambda)
                *it = " ";
            if (it != cursor)
            {
                fout << *it << " ";
            }
            else
            {
                fout << "|" << *it << "|"
                     << " ";
            }
        }
    }
    return statement == "end";
}

bool TuringMachine::load_strip(string dir)
{
    string buffer;
    ifstream fin;
    fin.open(dir);
    if (!fin.eof() && fin.is_open())
    {
        getline(fin, buffer);
        strip = split(buffer);
        std::list<string>::iterator it;
        for (it = strip.begin(); it != strip.end(); ++it)
        {
            if (test_for_primary_cursor(*it))
            {
                cursor = it;
                it->pop_back();
                it->erase(0, 1);
            }
        }
        return 1;
    }
    else
    {
        return 0;
    }
}
string TuringMachine::get_strip()
{
    string res;
    std::list<string>::iterator it;
    for (it = strip.begin(); it != strip.end(); ++it)
    {
        if (it != cursor)
        {
            res = res + *it + " ";
        }
        else
        {
            res = res + "|" + *it + "|" + " ";
        }
    }
    return res;
}
string TuringMachine::get_strip(bool lambda)
{
    if (!lambda)
        return get_strip();

    string res;
    std::list<string>::iterator it;
    for (it = strip.begin(); it != strip.end(); ++it)
    {
        if (it != cursor)
        {
            if (*it == "lambda")
                res = res + " " + " ";
            else
                res = res + *it + " ";
        }
        else
        {
            if (*it == "lambda")
                res = res + "|" + " " + "|" + " ";
            else
                res = res + "|" + *it + "|" + " ";
        }
    }
    return res;
}

void TuringMachine::get_step(char a)
{
    a = tolower(a);
    switch (a)
    {
    case 's':
        return;
        break;
    case 'l':
        if (cursor != strip.begin())
        {
            cursor--;
        }
        else
        {
            strip.push_front("lambda");
            cursor--;
        }
        break;
    case 'r':
        if (cursor != (--strip.end()))
        {
            cursor++;
        }
        else
        {
            strip.push_back("lambda");
            cursor++;
        }
        break;
    default:
        throw std::exception();
        break;
    }
}

void TuringMachine::lazyStart(ProjectName &pname, bool lambda)
{
    this->lambda = lambda;

    if (this->load_strip("datasection.tmp"))
    {
        string a = pname.getDBFile();
        this->output = pname.getOutFile();

        if (sqlite3_open(a.c_str(), &db))
        {
            throw "UNDEFINED ERROR : DATABASE UNAVAILABLE";
        }
        if (sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &err))
        {
            sqlite3_free(err);
            throw err;
        }
        if (sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &err))
        {
            sqlite3_free(err);
            sqlite3_close(db);
            throw err;
        }
    }
    else
    {
        throw "there are some troubles with .data file";
    }
}

int TuringMachine::execute(ProjectName &pname, bool lambda)
{
    this->lazyStart(pname, lambda);
    string select_command;
    while (!this->is_end(this->output, lambda))
    {
        select_command = "SELECT *FROM commands WHERE initial_state=\"" + this->statement + "\" AND initial_word=\"" +
                         *(this->cursor) + "\"";
        sqlite3_prepare_v2(this->db, select_command.c_str(), 256, &ppStmt, NULL);

        if (sqlite3_step(ppStmt) != SQLITE_DONE)
        {
            this->statement = (string((char *)sqlite3_column_text(this->ppStmt, 2)));
            *this->cursor = (string((char *)sqlite3_column_text(this->ppStmt, 3)));
            this->get_step(((char *)sqlite3_column_text(this->ppStmt, 4))[0]);
        }
        else
        {
            sqlite3_finalize(ppStmt);
            sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &err);
            sqlite3_close(db);
            throw "EMULATING ERROR : CANT FIND NEXT COMMAND";
        }

        sqlite3_finalize(ppStmt);
    }
    this->lazyFinalize();
    return 0;
}

MachineState TuringMachine::lazyDebug(bool step_by_step)
{
    string select_command;
    while (!this->is_end(this->output, lambda))
    {
        select_command = "SELECT *FROM commands WHERE initial_state=\"" + this->statement + "\" AND initial_word=\"" +
                         *(this->cursor) + "\"";
        sqlite3_prepare_v2(this->db, select_command.c_str(), 256, &ppStmt, NULL);

        if (sqlite3_step(ppStmt) != SQLITE_DONE)
        {
            this->statement = (string((char *)sqlite3_column_text(ppStmt, 2)));
            *this->cursor = (string((char *)sqlite3_column_text(ppStmt, 3)));
            this->get_step(((char *)sqlite3_column_text(ppStmt, 4))[0]);
        }
        else
        {
            sqlite3_finalize(ppStmt);
            sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &err);
            sqlite3_close(db);
            throw "EMULATING ERROR : CANT FIND NEXT COMMAND\nWith last state " + this->statement + " and word " +
                *(this->cursor);
        }

        if (step_by_step || (string((char *)sqlite3_column_text(ppStmt, 5)) == "1" || this->statement == "end"))
        {
            MachineState step;
            step.current_strip = this->get_strip(lambda);
            step.current_state = this->statement;
            step.current_word = *(this->cursor);
            step.line = string((char *)sqlite3_column_text(ppStmt, 6));
            this->lastLine = step.line;
            sqlite3_finalize(ppStmt);
            return step;
        }
        sqlite3_finalize(ppStmt);
    }
    MachineState step;
    step.current_strip = this->get_strip(lambda);
    step.current_state = this->statement;
    step.current_word = *(this->cursor);
    step.line = this->lastLine;
    return step;
}

void TuringMachine::lazyFinalize()
{
    sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &err);
    sqlite3_close(db);
    fin.close();
    strip.clear();
    statement = "start";
    dir.clear();
    output.clear();
    ppStmt = 0;
    db = 0;
    err = 0;
}