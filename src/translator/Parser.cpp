#include "Parser.hpp"

namespace translator
{

void Parser::defineFsm(std::string line_v)
{
    int i;
    if (strstr(line_v.c_str(), "#define ") != nullptr)
    {
        i = 8;
    }
    else
    {
        throw "Parse error at line #" + std::to_string(this->_line_counter);
    }

    // the current symbol is a space after '#define'
    std::string macros_name;
    std::string macros_value;
    while (line_v[i] != ' ')
    {
        macros_name.push_back(line_v[i++]);
    }
    i++;
    while (line_v[i] != '\0')
    {
        macros_value.push_back(line_v[i++]);
    }
    std::vector<std::string> macros;
    macros.push_back(macros_name);
    macros.push_back(macros_value);

    this->_macros_table.push_back(macros);
}

Command Parser::commandFsm(std::string line_v)
{
    Command result;

    if (line_v.empty())
    {
        return Command();
    }

    int i = 0;

    for (; i < line_v.size() - 1 && line_v[i] != ','; i++) // st1
    {
        if (isalnum(line_v[i]) != 0 || line_v[i] == '_')
        {
            result.initial_state.push_back(line_v[i]);
        }
        else
        {
            throw "Parse error at line #" + std::to_string(this->_line_counter);
        }
    }
    result.initial_state.push_back('\0');
    i++;

    for (; i < line_v.size() - 1 && !(line_v[i] == '-' && line_v[i + 1] == '>'); i++) // w1
    {
        if (isalnum(line_v[i]) != 0 || line_v[i] == '+' || line_v[i] == '-' || line_v[i] == '*' || line_v[i] == '=' ||
            line_v[i] == '/' || line_v[i] == ':' || line_v[i] == '^' || line_v[i] == '#' || line_v[i] == '!' ||
            line_v[i] == '?' || line_v[i] == '&' || line_v[i] == '>' || line_v[i] == '<' || line_v[i] == '%' ||
            line_v[i] == '(' || line_v[i] == ')' || line_v[i] == '_')
        {
            result.initial_word.push_back(line_v[i]);
        }
        else
        {
            throw "Parse error at line #" + std::to_string(this->_line_counter);
        }
    }
    result.initial_word.push_back('\0');

    i++;
    i++;
    for (; i < line_v.size() - 1 && line_v[i] != ','; i++) // st2
    {
        if (isalnum(line_v[i]) != 0 || line_v[i] == '_')
        {
            result.final_state.push_back(line_v[i]);
        }
        else
        {
            throw "Parse error at line #" + std::to_string(this->_line_counter);
        }
    }
    result.final_state.push_back('\0');
    i++;
    for (; i < line_v.size() - 1 && line_v[i] != ','; i++) // w2
    {
        if (isalnum(line_v[i]) != 0 || line_v[i] == '+' || line_v[i] == '-' || line_v[i] == '*' || line_v[i] == '=' ||
            line_v[i] == '/' || line_v[i] == ':' || line_v[i] == '^' || line_v[i] == '#' || line_v[i] == '!' ||
            line_v[i] == '?' || line_v[i] == '&' || line_v[i] == '>' || line_v[i] == '<' || line_v[i] == '%' ||
            line_v[i] == '(' || line_v[i] == ')' || line_v[i] == '_')
        {
            result.final_word.push_back(line_v[i]);
        }
        else
        {
            throw "Parse error at line #" + std::to_string(this->_line_counter);
        }
    }
    result.final_word.push_back('\0');
    i++;
    if (line_v[i] == 'r' || line_v[i] == 'R' || line_v[i] == 's' || line_v[i] == 'S' || line_v[i] == 'l' ||
        line_v[i] == 'L')
    {
        result.direction = line_v[i];
    }
    else
    {
        throw "Parse error at line #" + std::to_string(this->_line_counter);
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
    result.lineNumber = std::to_string(this->_line_counter);
    return result;
}

void Parser::sectorFsm(std::string line_v)
{
    if (strcmp(line_v.c_str(), "section .data") == 0)
    {
        this->_section = 'd';
        return;
    }
    else if (strcmp(line_v.c_str(), "section .text") == 0)
    {
        this->_section = 't';
        return;
    }

    throw "Section parse error at line #" + std::to_string(this->_line_counter);
}

void Parser::datasectionFsm(std::string line_v)
{
    std::ofstream datasection;
    datasection.open("datasection.tmp", std::ios::out);
    if (!datasection.is_open())
    {
        throw "Can't open datasection.tmp file";
    }
    datasection << line_v;
    this->_section = "";
    datasection.close();
}

std::string Parser::commandPreprocessor(std::string line_v)
{
    // for windows CRLF encoding support
    if (line_v.back() == '\r')
    {
        line_v.pop_back();
    }

    // macroses

    for (int k = 0; k != this->_macros_table.size(); k++)
    {
        std::vector<std::string> macros = this->_macros_table[k];
        int m_len = 0;
        int st = -1;
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

    if (line_v.empty())
    {
        return line_v;
    }

    return line_v;
}

std::string Parser::sectorPreprocessor(std::string line_v)
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
            {
                line_v.erase(j + 1, 1);
            }
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

std::string Parser::dataPreprocessor(std::string line_v)
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
            {
                line_v.erase(j + 1, 1);
            }
            line_v.erase(i, 1);
        }
        if (line_v[i] == ' ' && line_v[i + 1] == '\0') // space at end of the line
        {
            line_v.erase(i, 1);
            i--;
        }
        if (isalnum(line_v[i]) == 0 && !(line_v[i] == '_') && !(line_v[i] == '|') && !(line_v[i] == ':') &&
            !(line_v[i] == ' ') && !(line_v[i] == '+') && !(line_v[i] == '-') && !(line_v[i] == '*') &&
            !(line_v[i] == '=') && !(line_v[i] == '/') && !(line_v[i] == '\0') && !(line_v[i] == '#') &&
            !(line_v[i] == '^') && !(line_v[i] == '!') && !(line_v[i] == '&') && !(line_v[i] == '?') &&
            !(line_v[i] == '>') && !(line_v[i] == '<') && !(line_v[i] == '%') && !(line_v[i] == '(') &&
            !(line_v[i] == ')'))
        {
            throw "at data section (you have to use only allowed symbols) at char #" + std::to_string(i + 1);
        }
        if (line_v[i] == '|')
        {
            if (i < line_v.size() && i > 0) // " | lambda | " error
            {
                if ((line_v[i + 1] == ' ' && line_v[i - 1] == ' ') || (line_v[i - 1] == ' ' && line_v[i + 1] == '\0'))
                {
                    throw "Parse error (check space splits) at line #" + std::to_string(this->_line_counter);
                }
            }
            if (i == 0 && line_v[i + 1] == ' ')
            {
                throw "Parse error (check space splits) at line #" + std::to_string(this->_line_counter);
            }
        }
    }
    for (char i : line_v)
    {
        if (i == '|')
        {
            cursor_char++;
        }
    }
    if (cursor_char != 2)
    {
        throw "Parse error at (data section) line #" + std::to_string(this->_line_counter);
    }
    return line_v;
}

std::string Parser::definePreprocessor(std::string line_v)
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
            {
                line_v.erase(j + 1, 1);
            }
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

Command Parser::parse(std::string command)
{
    this->_line_counter++;
    std::string line_v = command;
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
            line_v = definePreprocessor(line_v);
            defineFsm(line_v);
            line_v = "";
            break;
        case ';':
            break;
        case 's':
            if (this->_section.empty())
            {
                line_v = sectorPreprocessor(line_v);
                sectorFsm(line_v);
                break;
            }
        default:
            if (this->_section == "d")
            {
                line_v = dataPreprocessor(line_v);
                datasectionFsm(line_v);
                break;
            }
            if (this->_section == "t" || this->_section.empty())
            {
                line_v = commandPreprocessor(line_v);
                return commandFsm(line_v);
            }
        };
    }
    return Command();
}

} // namespace translator