#include "TuringMachine.hpp"

namespace Machine
{

inline bool space(char c)
{
    return isspace(c);
}

inline bool notspace(char c)
{
    return !isspace(c);
}

std::list<std::string> split(const std::string &s)
{
    typedef std::string::const_iterator iter;
    std::list<std::string> ret;
    iter i = s.begin();
    while (i != s.end())
    {
        i = find_if(i, s.end(), notspace);
        iter j = find_if(i, s.end(), space);
        if (i != s.end())
        {
            ret.push_back(std::string(i, j));
            i = j;
        }
    }
    return ret;
}

bool TuringMachine::isEnd(std::string dir, bool lambda)
{
    if (_statement == "end")
    {
        std::ofstream fout;
        fout.open(dir);

        std::list<std::string>::iterator it;
        for (it = _strip.begin(); it != _strip.end(); ++it)
        {
            if (*it == "lambda" && lambda)
                *it = " ";
            if (it != _cursor)
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
    return _statement == "end";
}

bool TuringMachine::loadStrip(std::string dir)
{
    std::string buffer;
    std::ifstream fin;
    fin.open(dir);
    if (!fin.eof() && fin.is_open())
    {
        getline(fin, buffer);
        _strip = split(buffer);
        std::list<std::string>::iterator it;
        for (it = _strip.begin(); it != _strip.end(); ++it)
        {
            if (testForPrimaryCursor(*it))
            {
                _cursor = it;
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
std::string TuringMachine::getStrip()
{
    std::string res;
    std::list<std::string>::iterator it;
    for (it = _strip.begin(); it != _strip.end(); ++it)
    {
        if (it != _cursor)
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
std::string TuringMachine::getStrip(bool lambda)
{
    if (!lambda)
        return getStrip();

    std::string res;
    std::list<std::string>::iterator it;
    for (it = _strip.begin(); it != _strip.end(); ++it)
    {
        if (it != _cursor)
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

void TuringMachine::getStep(char a)
{
    a = tolower(a);
    switch (a)
    {
    case 's':
        return;
        break;
    case 'l':
        if (_cursor != _strip.begin())
        {
            _cursor--;
        }
        else
        {
            _strip.push_front("lambda");
            _cursor--;
        }
        break;
    case 'r':
        if (_cursor != (--_strip.end()))
        {
            _cursor++;
        }
        else
        {
            _strip.push_back("lambda");
            _cursor++;
        }
        break;
    default:
        throw std::exception();
        break;
    }
}

void TuringMachine::lazyStart(ProjectName &pname, bool lambda)
{
    this->_lambda = lambda;

    if (this->loadStrip("datasection.tmp"))
    {
        std::string a = pname.getDBFile();
        this->_output = pname.getOutFile();

        if (sqlite3_open(a.c_str(), &_db))
        {
            throw "UNDEFINED ERROR : DATABASE UNAVAILABLE";
        }
        if (sqlite3_exec(_db, "PRAGMA synchronous = OFF", NULL, NULL, &_err))
        {
            sqlite3_free(_err);
            throw _err;
        }
        if (sqlite3_exec(_db, "BEGIN TRANSACTION", NULL, NULL, &_err))
        {
            sqlite3_free(_err);
            sqlite3_close(_db);
            throw _err;
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
    std::string select_command;
    while (!this->isEnd(this->_output, _lambda))
    {
        select_command = "SELECT *FROM commands WHERE initial_state=\"" + this->_statement + "\" AND initial_word=\"" +
                         *(this->_cursor) + "\"";
        sqlite3_prepare_v2(this->_db, select_command.c_str(), 256, &_pp_stmt, NULL);

        if (sqlite3_step(_pp_stmt) != SQLITE_DONE)
        {
            this->_statement = (std::string((char *)sqlite3_column_text(this->_pp_stmt, 2)));
            *this->_cursor = (std::string((char *)sqlite3_column_text(this->_pp_stmt, 3)));
            this->getStep(((char *)sqlite3_column_text(this->_pp_stmt, 4))[0]);
        }
        else
        {
            sqlite3_finalize(_pp_stmt);
            sqlite3_exec(_db, "END TRANSACTION", NULL, NULL, &_err);
            sqlite3_close(_db);

            throw "EMULATING ERROR : CANT FIND NEXT COMMAND";
        }

        sqlite3_finalize(_pp_stmt);
    }
    this->lazyFinalize();
    return 0;
}

MachineState TuringMachine::lazyDebug(bool step_by_step)
{
    std::string select_command;
    while (!this->isEnd(this->_output, _lambda))
    {
        select_command = "SELECT *FROM commands WHERE initial_state=\"" + this->_statement + "\" AND initial_word=\"" +
                         *(this->_cursor) + "\"";
        sqlite3_prepare_v2(this->_db, select_command.c_str(), 256, &_pp_stmt, NULL);

        if (sqlite3_step(_pp_stmt) != SQLITE_DONE)
        {
            this->_statement = (std::string((char *)sqlite3_column_text(_pp_stmt, 2)));
            *this->_cursor = (std::string((char *)sqlite3_column_text(_pp_stmt, 3)));
            this->getStep(((char *)sqlite3_column_text(_pp_stmt, 4))[0]);
        }
        else
        {
            sqlite3_finalize(_pp_stmt);
            sqlite3_exec(_db, "END TRANSACTION", NULL, NULL, &_err);
            sqlite3_close(_db);
            throw "EMULATING ERROR : CANT FIND NEXT COMMAND\nWith last state " + this->_statement + " and word " +
                *(this->_cursor);
        }

        if (step_by_step || (std::string((char *)sqlite3_column_text(_pp_stmt, 5)) == "1" || this->_statement == "end"))
        {
            MachineState step;
            step.current_strip = this->getStrip(_lambda);
            step.current_state = this->_statement;
            step.current_word = *(this->_cursor);
            step.line = std::string((char *)sqlite3_column_text(_pp_stmt, 6));
            this->_last_line = step.line;
            sqlite3_finalize(_pp_stmt);
            return step;
        }
        sqlite3_finalize(_pp_stmt);
    }
    MachineState step;
    step.current_strip = this->getStrip(_lambda);
    step.current_state = this->_statement;
    step.current_word = *(this->_cursor);
    step.line = this->_last_line;
    return step;
}

void TuringMachine::lazyFinalize()
{
    sqlite3_exec(_db, "END TRANSACTION", NULL, NULL, &_err);
    sqlite3_close(_db);
    _fin.close();
    _strip.clear();
    _statement = "start";
    _dir.clear();
    _output.clear();
    _pp_stmt = 0;
    _db = 0;
    _err = 0;
}

} // namespace Machine