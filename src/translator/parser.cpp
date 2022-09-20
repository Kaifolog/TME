#include "parser.hpp"

#include <exception>
#include <iostream>

using namespace std;

void Parser::define_fsm(string line_v)
{
    int i;
    if (strstr(line_v.c_str(), "#define ") == NULL)
    {
        throw "parse error at line #" + to_string(this->line_counter);
    }
    else
    {
        i = 8;
    }

    // the current symbol is a space after '#define'
    string macros_name;
    string macros_value;
    while (line_v[i] != ' ')
    {
        macros_name.push_back(line_v[i++]);
    }
    i++;
    while (line_v[i] != '\0')
    {
        macros_value.push_back(line_v[i++]);
    }
    vector<string> macros;
    macros.push_back(macros_name);
    macros.push_back(macros_value);

    this->macros_table.push_back(macros);
}

Command Parser::command_fsm(string line_v)
{
    Command result;

    if (line_v.size() == 0)
    {
        return Command();
    }

    int i = 0;

    for (; i < line_v.size() - 1 && line_v[i] != ','; i++) // st1
    {
        if (isalnum(line_v[i]) || line_v[i] == '_')
            result.initial_state.push_back(line_v[i]);
        else
        {
            throw "parse error at line #" + to_string(this->line_counter);
        }
    }
    result.initial_state.push_back('\0');
    i++;

    for (; i < line_v.size() - 1 && !(line_v[i] == '-' && line_v[i + 1] == '>'); i++) // w1
    {
        if (isalnum(line_v[i]) || line_v[i] == '+' || line_v[i] == '-' || line_v[i] == '*' || line_v[i] == '=' ||
            line_v[i] == '/' || line_v[i] == ':' || line_v[i] == '^' || line_v[i] == '#' || line_v[i] == '!' ||
            line_v[i] == '?' || line_v[i] == '&' || line_v[i] == '>' || line_v[i] == '<' || line_v[i] == '%' ||
            line_v[i] == '(' || line_v[i] == ')')
            result.initial_word.push_back(line_v[i]);
        else
        {
            throw "parse error at line #" + to_string(this->line_counter);
        }
    }
    result.initial_word.push_back('\0');

    i++;
    i++;
    for (; i < line_v.size() - 1 && line_v[i] != ','; i++) // st2
    {
        if (isalnum(line_v[i]) || line_v[i] == '_')
            result.final_state.push_back(line_v[i]);
        else
        {
            throw "parse error at line #" + to_string(this->line_counter);
        }
    }
    result.final_state.push_back('\0');
    i++;
    for (; i < line_v.size() - 1 && line_v[i] != ','; i++) // w2
    {
        if (isalnum(line_v[i]) || line_v[i] == '+' || line_v[i] == '-' || line_v[i] == '*' || line_v[i] == '=' ||
            line_v[i] == '/' || line_v[i] == ':' || line_v[i] == '^' || line_v[i] == '#' || line_v[i] == '!' ||
            line_v[i] == '?' || line_v[i] == '&' || line_v[i] == '>' || line_v[i] == '<' || line_v[i] == '%' ||
            line_v[i] == '(' || line_v[i] == ')')
            result.final_word.push_back(line_v[i]);
        else
        {
            throw "parse error at line #" + to_string(this->line_counter);
        }
    }
    result.final_word.push_back('\0');
    i++;
    if (line_v[i] == 'r' || line_v[i] == 'R' || line_v[i] == 's' || line_v[i] == 'S' || line_v[i] == 'l' ||
        line_v[i] == 'L')
        result.direction = line_v[i];
    else
    {
        throw "parse error at line #" + to_string(this->line_counter);
    }
    i++;
    if (line_v[i] == '\0')
    {
        result.debug = "0";
    }
    else
    {
        result.debug = "1";
    }
    result.lineNumber = to_string(this->line_counter);
    return result;
}

void Parser::sector_fsm(string line_v)
{
    if (!strcmp(line_v.c_str(), "section .data"))
    {
        this->section = 'd';
        return;
    }
    else if (!strcmp(line_v.c_str(), "section .text"))
    {
        this->section = 't';
        return;
    }

    throw "SECTION parse error at line #" + to_string(this->line_counter);
}

int Parser::datasection_fsm(string line_v)
{
    FILE *datasection;
    if (datasection = fopen("datasection.tmp", "w"))
    {
        fputs(line_v.c_str(), datasection);
        this->section = "";
        fclose(datasection);
        return '\0';
    }
    throw "Temple file error";
}

string Parser::command_preprocessor(string line_v)
{
    // for windows CRLF encoding support
    if (line_v.back() == '\r')
    {
        line_v.pop_back();
    }

    // macroses

    for (int k = 0; k != this->macros_table.size(); k++)
    {
        vector<string> macros = this->macros_table[k];
        int m_len = 0, st = -1;
        for (int p = 0; p < line_v.size() - 1; p++)
        {
            if (line_v[p] == macros[0][m_len])
            {
                m_len++;
                if (st == -1)
                {
                    st = p;
                }
                if (m_len == macros[0].size())
                {
                    st;
                    break;
                }
            }
            else
            {
                st = -1;
                m_len = 0;
            }
        }

        if (m_len == macros[0].size())
        {
            line_v.erase(st, m_len);
            line_v = line_v.insert(st, macros[1]);
            k = k - 1;
        }
    }

    // processing of comments

    for (int i = 0; i < line_v.size(); i++)
    {
        if (line_v[i] == ';')
        {
            if (line_v.size() >= i + 3 && line_v[i + 1] == '#' && line_v[i + 2] == 'd' && line_v[i + 3] == '\0')
            {
                i += 3;
                while (line_v[i] != '\0')
                {
                    line_v.erase(i, 1);
                }
            }
            else
            {
                while (line_v[i] != '\0')
                {
                    line_v.erase(i, 1);
                }
            }
        }
        if (line_v[i] == ' ' || line_v[i] == '	')
        {
            line_v.erase(i, 1);
            i--;
        }
    }

    if (line_v == ";#d")
    {
        line_v = "";
    }

    if (line_v.size() == 0)
    {
        return line_v;
    }

    return line_v;
}

string Parser::sector_preprocessor(string line_v)
{
    // for windows CRLF encoding support
    if (line_v.back() == '\r')
    {
        line_v.pop_back();
    }

    for (int i = 0; i < line_v.size(); i++)
    {
        if (line_v[0] == ' ')
        {
            line_v.erase(0, 1);
            i = 0;
        }
        if (line_v[i] == ' ' && line_v[i + 1] == ' ')
        {
            line_v.erase(i, 1);
            i--;
        }
        if (line_v[i] == ';')
        {
            for (int j = i; line_v[j + 1] != '\0';)
                line_v.erase(j + 1, 1);
            line_v.erase(i, 1);
        }
        if (line_v[i] == ' ' && line_v[i + 1] == ',')
        {
            line_v.erase(i, 1);
            i--;
        }
        if (line_v[i] == ',' && line_v[i + 1] == ' ')
        {
            line_v.erase(i + 1, 1);
        }
        if (line_v[i] == ' ' && line_v[i + 1] == '\0')
        {
            line_v.erase(i, 1);
            i--;
        }
    }
    return line_v;
}

string Parser::data_preprocessor(string line_v)
{
    // for windows CRLF encoding support
    if (line_v.back() == '\r')
    {
        line_v.pop_back();
    }

    int cursor_char = 0;
    for (int i = 0; i < line_v.size(); i++)
    {
        if (line_v[i] == ';') // comments
        {
            for (int j = i; line_v[j + 1] != '\0';)
                line_v.erase(j + 1, 1);
            line_v.erase(i, 1);
        }
        if (line_v[i] == ' ' && line_v[i + 1] == '\0') // space at end of the line
        {
            line_v.erase(i, 1);
            i--;
        }
        if (!isalnum(line_v[i]) && !(line_v[i] == '_') && !(line_v[i] == '|') && !(line_v[i] == ':') &&
            !(line_v[i] == ' ') && !(line_v[i] == '+') && !(line_v[i] == '-') && !(line_v[i] == '*') &&
            !(line_v[i] == '=') && !(line_v[i] == '/') && !(line_v[i] == '\0') && !(line_v[i] == '#') &&
            !(line_v[i] == '^') && !(line_v[i] == '!') && !(line_v[i] == '&') && !(line_v[i] == '?') &&
            !(line_v[i] == '>') && !(line_v[i] == '<') && !(line_v[i] == '%') && !(line_v[i] == '(') &&
            !(line_v[i] == ')'))
        {
            throw "at data section (you have to use only allowed symbols) at char #" + to_string(i + 1);
        }
        if (line_v[i] == '|')
        {
            if (i < line_v.size() && i > 0) // " | lambda | " error
            {
                if ((line_v[i + 1] == ' ' && line_v[i - 1] == ' ') || (line_v[i - 1] == ' ' && line_v[i + 1] == '\0'))
                {
                    throw "parse error (check space splits) at line #" + to_string(this->line_counter);
                }
            }
            if (i == 0 && line_v[i + 1] == ' ')
            {
                throw "parse error (check space splits) at line #" + to_string(this->line_counter);
            }
        }
    }
    for (int i = 0; i < line_v.length(); i++)
        if (line_v[i] == '|')
            cursor_char++;
    if (cursor_char != 2)
    {
        throw "parse error at (data section) line #" + to_string(this->line_counter);
    }
    return line_v;
}

string Parser::define_preprocessor(string line_v)
{
    // for windows CRLF encoding support
    if (line_v.back() == '\r')
    {
        line_v.pop_back();
    }

    for (int i = 0; i < line_v.size(); i++)
    {
        if (line_v[0] == ' ')
        {
            line_v.erase(0, 1);
            i--;
        }
        if (line_v[i] == ' ' && line_v[i + 1] == ' ')
        {
            line_v.erase(i, 1);
            i--;
        }
        if (line_v[i] == ';')
        {
            for (int j = i; line_v[j + 1] != '\0';)
                line_v.erase(j + 1, 1);
            line_v.erase(i, 1);
        }
        if (line_v[i] == ' ' && line_v[i + 1] == '\0')
        {
            line_v.erase(i, 1);
            i--;
        }
    }
    return line_v;
}

Command Parser::parse(string command)
{
    this->line_counter++;
    string line_v = command;
    if (line_v.size() > 1)
    {
        // There are only 3 kinds of rows that translator can understand:
        // (1) a definition of macros
        // (2) a section definition
        // (3) a command definition
        // So, to parse them there are three final state machines.

        switch (line_v[0])
        {
        case '#':
            line_v = define_preprocessor(line_v);
            define_fsm(line_v);
            line_v = "";
            break;
        case ';':
            break;
        case 's':
            if (this->section == "")
            {
                line_v = sector_preprocessor(line_v);
                sector_fsm(line_v);
                break;
            }
        default:
            if (this->section == "d")
            {
                line_v = data_preprocessor(line_v);
                datasection_fsm(line_v);
                break;
            }
            if (this->section == "t" || this->section == "")
            {
                line_v = command_preprocessor(line_v);
                return command_fsm(line_v);
            }
        };
    }
    return Command();
}