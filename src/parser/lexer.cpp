#include "lexer.hpp"

using namespace std;

void Command::define_fsm(string line_v)
{
    int i;
    if (strstr(line_v.c_str(), "#define ") == NULL)
    {
        error("parse error at line #", parser->line_counter);
        return;
    }
    else
    {
        i = 8;
    }

    //стоим на пробеле и точно знаем что у нас сзади 6 знаков нужного идентифиатора
    string *macros_name = new string;
    string *macros_value = new string;
    while (line_v[i] != ' ')
    {
        macros_name->push_back(line_v[i++]);
    }
    macros_name->push_back('\0');
    i++;
    while (line_v[i] != '\0')
    {
        macros_value->push_back(line_v[i++]);
    }
    //macros_value->push_back('\0');
    vector<string *> *macros = new vector<string *>;
    macros->push_back(macros_name);
    macros->push_back(macros_value);

    parser->macros_table.push_back(macros);
}

void Command::command_fsm(string line_v)
{
    if (line_v.size() == 0)
    {
        return;
    }

    for (int i = 0; i < line_v.size(); i++) //удалим все пробелы
        if (line_v[i] == ' ')
        {
            line_v.erase(i, 1);
            i--;
        }
    //теперь нужно придумать как передать строки обратно, желательно, в структурированном виде, именно структуру мы и напишем
    //макроподстановки

    for (int k = 0; k != parser->macros_table.size(); k++)
    {
        vector<string *> macros = parser->macros_table[k][0];
        int m_len = 0, st = -1;
        for (int p = 0; p < line_v.size() - 1; p++)
        {
            if (line_v[p] == macros[0][0][m_len])
            {
                m_len++;
                if (st == -1)
                    st = p;
                if (m_len == macros[0][0].size() - 1)
                    break;
            }
            else
            {
                st = -1;
                m_len = 0;
            }
        }
        if (m_len == macros[0][0].size() - 1)
        {

            for (int m = 0; m < m_len; m++)
                line_v.erase(st, 1);
            for (int m = 0; m < macros[1][0].size() - 1; m++)
                line_v = line_v.insert(st++, &macros[1][0][m]);
            k = -1;
        }
    }

    int i = 0;

    for (; i < line_v.size() - 1 && line_v[i] != ','; i++) //st1
    {
        if (isalnum(line_v[i]) || line_v[i] == '_')
            this->initial_state.push_back(line_v[i]);
        else
        {
            log_message((char *)line_v.c_str());
            error("parse error at line #", parser->line_counter);
            return;
        }
    }
    this->initial_state.push_back('\0');
    i++;

    for (; i < line_v.size() - 1 && !(line_v[i] == '-' && line_v[i + 1] == '>'); i++) //w1
    {
        if (isalnum(line_v[i]) || line_v[i] == '+' || line_v[i] == '-' || line_v[i] == '*' || line_v[i] == '=' || line_v[i] == '/' || line_v[i] == ':' || line_v[i] == '^' || line_v[i] == '#' || line_v[i] == '!' || line_v[i] == '?' || line_v[i] == '&')
            this->initial_word.push_back(line_v[i]);
        else
        {
            error("parse error at line #", parser->line_counter);
            return;
        }
    }
    this->initial_word.push_back('\0');

    i++;
    i++;
    for (; i < line_v.size() - 1 && line_v[i] != ','; i++) //st2
    {
        if (isalnum(line_v[i]) || line_v[i] == '_')
            this->final_state.push_back(line_v[i]);
        else
        {
            error("parse error at line #", parser->line_counter);
            return;
        }
    }
    this->final_state.push_back('\0');
    i++;
    for (; i < line_v.size() - 1 && line_v[i] != ','; i++) //w2
    {
        if (isalnum(line_v[i]) || line_v[i] == '+' || line_v[i] == '-' || line_v[i] == '*' || line_v[i] == '=' || line_v[i] == '/' || line_v[i] == ':' || line_v[i] == '^' || line_v[i] == '#' || line_v[i] == '!' || line_v[i] == '?' || line_v[i] == '&')
            this->final_word.push_back(line_v[i]);
        else
        {
            error("parse error at line #", parser->line_counter);
            return;
        }
    }
    this->final_word.push_back('\0');
    i++;
    if (isalnum(line_v[i]))
        this->direction = line_v[i];
    else
    {
        error("parse error at line #", parser->line_counter);
        return;
    }
}

void Command::sector_fsm(string line_v)
{
    if (!strcmp(line_v.c_str(), "section .data"))
    {
        parser->section = 'd';
        return;
    }
    else if (!strcmp(line_v.c_str(), "section .text"))
    {
        parser->section = 't';
        return;
    }
    command_fsm(line_v);
}

int Command::datasection_fsm(string line_v)
{
    FILE *datasection;
    if (datasection = fopen("datasection.tmp", "w"))
    {
        fputs(line_v.c_str(), datasection);
        parser->section = "";
        fclose(datasection);
        return '\0';
    }
    error("Temple file error;", 0);
}

string Command::preprocessor(string line_v)
{
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

string Command::data_preprocessor(string line_v)
{
    for (int i = 0; i < line_v.size(); i++)
    {
        if (line_v[i] == ';') //comments
        {
            for (int j = i; line_v[j + 1] != '\0';)
                line_v.erase(j + 1, 1);
            line_v.erase(i, 1);
        }
        if (line_v[i] == ' ' && line_v[i + 1] == '\0') //spacet at end of the line
        {
            line_v.erase(i, 1);
            i--;
        }
        if (!isalnum(line_v[i]) && !(line_v[i] == '_') && !(line_v[i] == '|') && !(line_v[i] == ':') && !(line_v[i] == ' ') && !(line_v[i] == '+') && !(line_v[i] == '-') && !(line_v[i] == '*') && !(line_v[i] == '=') && !(line_v[i] == '/') && !(line_v[i] == '\0') && !(line_v[i] == '#') && !(line_v[i] == '^') && !(line_v[i] == '!') && !(line_v[i] == '&') && !(line_v[i] == '?'))
            error("parse warning at data section (you have to use only allowed symbols) at char #", i + 1);
    }
    return line_v;
}

string Command::define_preprocessor(string line_v)
{
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

Command::Command(string command, Parser *parser_)
{
    parser = parser_;
    parser->line_counter++;
    string line_v = command; //перепишем нашу строку из стека в выделенный для этого svwm-вектор
    if (line_v.size() > 1)
    {
        switch (line_v[0]) //тк наш язык довольно прост, у нас есть всего 3 варианта событий. (1)описание макропеременной, (2)обозначение секции, (3)логических выражений,
                           //все они имеют малую вариативность, откуда нам потребуется 3 конечных автомата.
        {
        case '#':
            line_v = define_preprocessor(line_v);
            define_fsm(line_v);
            line_v = "";
            break;
        case ';':
            break;
        case 's':
            if (parser->section == "")
            {
                line_v = preprocessor(line_v);
                sector_fsm(line_v);
                break;
            }
        default:
            if (parser->section == "d")
            {
                line_v = data_preprocessor(line_v);
                datasection_fsm(line_v);
                break;
            }
            if (parser->section == "t" || parser->section == "")
            {
                line_v = preprocessor(line_v);
                command_fsm(line_v);
            }
        };
    }
}