#include "TuringMachine.hpp"
extern "C"
{
#include "lib/logging.h"
}

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
        fout.open(dir + "_out.txt");

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
        error("direction arg error", 0);
        break;
    }
    // for (int i = 0; i < strip.size(); i++)
    // {
    //     if (i != cursor)
    //     {
    //         cout << strip[i] << " ";
    //     }
    //     else
    //     {
    //         cout << "|" << strip[i] << "|"
    //              << " ";
    //     }
    // }
    // cout << endl;
}