#include "TuringMachine.hpp"

using namespace std;

inline bool
space(char c)
{
    return isspace(c);
}

inline bool notspace(char c)
{
    return !isspace(c);
}

vector<string> split(const string &s)
{
    typedef string::const_iterator iter;
    vector<string> ret;
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

        for (int i = 0; i < strip.size(); i++)
        {
            if (strip[i] == "lambda" && lambda)
                strip[i] = " ";
            if (i != cursor)
            {
                fout << strip[i] << " ";
            }
            else
            {
                fout << "|" << strip[i] << "|"
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
        for (int i = 0; i < strip.size(); i++)
            if (test_for_primary_cursor(strip[i]))
            {
                cursor = i;
                strip[i].pop_back();
                strip[i].erase(0, 1);
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
    for (int i = 0; i < strip.size(); i++)
    {
        if (i != cursor)
        {
            res = res + strip[i] + " ";
        }
        else
        {
            res = res + "|" + strip[i] + "|" + " ";
        }
    }
    return res;
}
string TuringMachine::get_strip(bool lambda)
{
    if (!lambda)
        return get_strip();

    string res;
    for (int i = 0; i < strip.size(); i++)
    {
        if (i != cursor)
        {
            if (strip[i] == "lambda")
                res = res + " " + " ";
            else
                res = res + strip[i] + " ";
        }
        else
        {
            if (strip[i] == "lambda")
                res = res + "|" + " " + "|" + " ";
            else
                res = res + "|" + strip[i] + "|" + " ";
        }
    }
    return res;
}
string TuringMachine::get_current_state()
{
    return statement;
}
string TuringMachine::get_current_word()
{
    return strip[cursor];
}
void TuringMachine::set_current_state(string a)
{
    statement = a;
}
void TuringMachine::set_current_word(string a)
{
    strip[cursor] = a;
}
bool TuringMachine::get_step(char a)
{
    a = tolower(a);
    switch (a)
    {
    case 's':
        return 0;
        break;
    case 'l':
        if (cursor)
        {
            cursor--;
        }
        else
        {
            strip.insert(strip.begin(), "lambda");
        }
        break;
    case 'r':
        if (cursor != strip.size() - 1)
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
        select_command = "SELECT *FROM commands WHERE initial_state=\"" + this->get_current_state() + "\" AND initial_word=\"" + this->get_current_word() + "\"";
        sqlite3_prepare_v2(this->db, select_command.c_str(), 256, &ppStmt, NULL);

        if (sqlite3_step(ppStmt) != SQLITE_DONE)
        {
            this->set_current_state(string((char *)sqlite3_column_text(this->ppStmt, 2)));
            this->set_current_word(string((char *)sqlite3_column_text(this->ppStmt, 3)));
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

MachineState TuringMachine::lazyDebug()
{

    string select_command;
    while (!this->is_end(this->output, lambda))
    {
        select_command = "SELECT *FROM commands WHERE initial_state=\"" + this->get_current_state() + "\" AND initial_word=\"" + this->get_current_word() + "\"";
        sqlite3_prepare_v2(this->db, select_command.c_str(), 256, &ppStmt, NULL);

        if (sqlite3_step(ppStmt) != SQLITE_DONE)
        {
            this->set_current_state(string((char *)sqlite3_column_text(ppStmt, 2)));
            this->set_current_word(string((char *)sqlite3_column_text(ppStmt, 3)));
            this->get_step(((char *)sqlite3_column_text(ppStmt, 4))[0]);
        }
        else
        {
            sqlite3_finalize(ppStmt);
            sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &err);
            sqlite3_close(db);
            throw "EMULATING ERROR : CANT FIND NEXT COMMAND\nWith last state " + this->get_current_state() + " and word " + this->get_current_word();
        }

        if (string((char *)sqlite3_column_text(ppStmt, 5)) == "1" || this->get_current_state() == "end")
        {
            MachineState step;
            step.current_strip = this->get_strip();
            step.current_state = this->get_current_state();
            step.current_word = this->get_current_word();
            step.line = string((char *)sqlite3_column_text(ppStmt, 6));
            sqlite3_finalize(ppStmt);
            return step;
        }
        sqlite3_finalize(ppStmt);
    }
}

void TuringMachine::lazyFinalize()
{
    sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &err);
    sqlite3_close(db);
    fin.close();
}