/*
MIT License
Copyright (c) 2021 Kaifolog
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// The official repository: <https://github.com/Kaifolog/TME>.

#include "lexer.h"

//Во избежание множественных проходов опишем функционал препроцессора в лексический анализатор. В литературе не существует одного мнения на этот счет, высказываются
//даже о пост-препроцессорах. Наш язык не так уж сложен и не требует таких ухищрений. Достаточно завести таблицу с подстановками, что грозит +1 сравнением прикаждом проходе,
//однако, это малая кровь. В любой другой ситуации кроме сравнения с шаблоном мы должны были бы делать еще лишний проход.

char section = 0;
int line_counter = 0;

void macros_table_destructor()
{
    for (int i = svwm_size(macros_table) - 1; i != -1; i--)
    {
        char **macros = macros_table[i];
        svwm_free(macros[0]);
        svwm_free(macros[1]);
        svwm_free(macros);
        svwm_erase(macros_table, i);
    }
    svwm_free(macros_table);
}
void command_destructor(struct Command *c)
{
    svwm_free(c->final_state);
    svwm_free(c->final_word);
    svwm_free(c->initial_state);
    svwm_free(c->initial_word);
    free(c);
}

char *define_fsm(char *line_v) //define final-state machine
{
    int statement = 0;
    int i;
    for (i = 0; i < svwm_size(line_v); i++)
    {
        if (statement != 7)
            switch (line_v[i])
            {
            case '#':
                if (statement == 0)
                    statement++;
                else
                    error("parse error at line #", line_counter);
                break;
            case 'd':
                if (statement == 1)
                    statement++;
                else
                    error("parse error at line #", line_counter);
                break;
            case 'e':
                if (statement == 2 || statement == 6)
                    statement++;
                else
                    error("parse error at line #", line_counter);
                break;
            case 'f':
                if (statement == 3)
                    statement++;
                else
                    error("parse error at line #", line_counter);
                break;
            case 'i':
                if (statement == 4)
                    statement++;
                else
                    error("parse error at line #", line_counter);
                break;
            case 'n':
                if (statement == 5)
                    statement++;
                else
                    error("parse error at line #", line_counter);
                break;
            case ' ':
                if (statement == 7)
                    statement++;
                else
                    error("parse error at line #", line_counter);
                break;
            default:
                error("parse error at line #", line_counter);
            }
        else
        {
            i++;
            break;
        }
    }

    //стоим на пробеле и точно знаем что у нас сзади 6 знаков нужного идентифиатора
    char *macros_name = svwm_vector(0, sizeof(char));
    char *macros_value = svwm_vector(0, sizeof(char));
    while (line_v[i] != ' ')
    {
        macros_name = svwm_pb_char(macros_name, line_v[i++]);
    }
    macros_name = svwm_pb_char(macros_name, '\0');
    i++;
    while (line_v[i] != '\0')
    {
        macros_value = svwm_pb_char(macros_value, line_v[i++]);
    }
    macros_value = svwm_pb_char(macros_value, '\0');

    char **macros = svwm_vector(2, sizeof(char *));
    macros[0] = macros_name;
    macros[1] = macros_value;
    macros_table = svwm_pb_charptr3(macros_table, macros);

    svwm_free(line_v);
    return 0;
}

struct Command *command_fsm(char *line_v) //command final-state machine
                                          //комманды всегда имеют регулярный вид, тогда нужно их парсить.
{
    for (int i = 0; i < svwm_size(line_v); i++) //удалим все пробелы
        if (line_v[i] == ' ')
        {
            svwm_erase(line_v, i);
            i--;
        }
    //теперь нужно придумать как передать строки обратно, желательно, в структурированном виде, именно структуру мы и напишем
    //макроподстановки

    for (int k = 0; k != svwm_size(macros_table); k++)
    {
        char **macros = macros_table[k];
        int m_len = 0, st = -1;
        for (int p = 0; p < svwm_size(line_v) - 1; p++)
        {
            //printf("%s\n", macros[1]);
            // if (line_v[p] == ',' || line_v[p] == '-' || line_v[p] == '>')
            //     printf("%c|%c || %d ||#%d\n__\n", line_v[p], macros[0][m_len], m_len, line_counter);
            if (line_v[p] == macros[0][m_len])
            {
                m_len++;
                if (st == -1)
                    st = p;
                if (m_len == svwm_size(macros[0]) - 1)
                    break;
            }
            else
            {
                st = -1;
                m_len = 0;
            }
        }
        if (m_len == svwm_size(macros[0]) - 1)
        {

            for (int m = 0; m < m_len; m++)
                svwm_erase(line_v, st);
            for (int m = 0; m < svwm_size(macros[1]) - 1; m++)
                line_v = svwm_insert(line_v, st++, &macros[1][m]);
            k = -1;
        }
    }

    struct Command *command;
    command = malloc(sizeof(struct Command));
    command->initial_state = svwm_vector(0, sizeof(char));
    int i = 0;
    for (; i < svwm_size(line_v) - 1 && line_v[i] != ','; i++)
    {
        if (isalnum(line_v[i]) || line_v[i] == '_')
            command->initial_state = svwm_pb_char(command->initial_state, line_v[i]);
        else
            error("parse error at line #", line_counter);
    }
    command->initial_state = svwm_pb_char(command->initial_state, '\0');
    i++;
    command->initial_word = svwm_vector(0, sizeof(char));
    for (; i < svwm_size(line_v) - 1 && line_v[i] != '-'; i++)
    {
        if (isalnum(line_v[i]))
            command->initial_word = svwm_pb_char(command->initial_word, line_v[i]);
        else
            error("parse error at line #", line_counter);
    }
    command->initial_word = svwm_pb_char(command->initial_word, '\0');
    i++;
    i++;
    command->final_state = svwm_vector(0, sizeof(char));
    for (; i < svwm_size(line_v) - 1 && line_v[i] != ','; i++)
    {
        if (isalnum(line_v[i]) || line_v[i] == '_')
            command->final_state = svwm_pb_char(command->final_state, line_v[i]);
        else
            error("parse error at line #", line_counter);
    }
    command->final_state = svwm_pb_char(command->final_state, '\0');
    i++;
    command->final_word = svwm_vector(0, sizeof(char));
    for (; i < svwm_size(line_v) - 1 && line_v[i] != ','; i++)
    {
        if (isalnum(line_v[i]))
            command->final_word = svwm_pb_char(command->final_word, line_v[i]);
        else
            error("parse error at line #", line_counter);
    }
    command->final_word = svwm_pb_char(command->final_word, '\0');
    i++;
    command->direction = malloc(1);
    if (isalnum(line_v[i]))
        command->direction = line_v[i];
    else
        error("parse error at line #", line_counter);

    if (svwm_size(command->initial_state) == 1 || svwm_size(command->final_state) == 1 || (command->direction != 's' && command->direction != 'S' && command->direction != 'l' && command->direction != 'L' && command->direction != 'r' && command->direction != 'R'))

        svwm_free(line_v);

    return command;
}

char *sector_fsm(char *line_v) //sector final-state machine
{
    if (!strcmp(line_v, "section .data"))
    {
        section = 'd';
        svwm_free(line_v);
        return '\0';
    }
    else if (!strcmp(line_v, "section .text"))
    {
        section = 't';
        svwm_free(line_v);
        return '\0';
    }
    return command_fsm(line_v);
}

char *datasection_fsm(char *line_v) //datasection final-state machine
{
    FILE *datasection;
    if (datasection = fopen("datasection.tmp", "w"))
    {
        fputs(line_v, datasection);
        section = 0;
        svwm_free(line_v);
        fclose(datasection);
        return '\0';
    }
    error("Temple file error;", 0);
}

char *preprocessor(char *line_v)
{
    for (int i = 0; i < svwm_size(line_v); i++)
    {
        if (line_v[0] == ' ')
        {
            svwm_erase(line_v, 0);
            i--;
        }
        if (line_v[i] == ' ' && line_v[i + 1] == ' ')
        {
            svwm_erase(line_v, i);
            i--;
        }
        if (line_v[i] == ';')
        {
            for (int j = i; line_v[j + 1] != '\0';)
                svwm_erase(line_v, j + 1);
            svwm_erase(line_v, i);
        }
        if (line_v[i] == ' ' && line_v[i + 1] == ',')
        {
            svwm_erase(line_v, i);
            i--;
        }
        if (line_v[i] == ',' && line_v[i + 1] == ' ')
        {
            svwm_erase(line_v, i + 1);
        }
        if (line_v[i] == ' ' && line_v[i + 1] == '\0')
        {
            svwm_erase(line_v, i);
            i--;
        }
    }
    return line_v;
}

char *data_preprocessor(char *line_v)
{
    for (int i = 0; i < svwm_size(line_v); i++)
    {
        if (line_v[i] == ';')
        {
            for (int j = i; line_v[j + 1] != '\0';)
                svwm_erase(line_v, j + 1);
            svwm_erase(line_v, i);
        }
        if (line_v[i] == ' ' && line_v[i + 1] == '\0')
        {
            svwm_erase(line_v, i);
            i--;
        }
        if (!isalnum(line_v[i]) && !(line_v[i] == '_') && !(line_v[i] == '|') && !(line_v[i] == ' ') && !(line_v[i] == '\0'))
            error("parse error at data section (you have to use only numbers, letters and \"_\") at char #", i);
    }
    return line_v;
}
char *define_preprocessor(char *line_v)
{
    for (int i = 0; i < svwm_size(line_v); i++)
    {
        if (line_v[0] == ' ')
        {
            svwm_erase(line_v, 0);
            i--;
        }
        if (line_v[i] == ' ' && line_v[i + 1] == ' ')
        {
            svwm_erase(line_v, i);
            i--;
        }
        if (line_v[i] == ';')
        {
            for (int j = i; line_v[j + 1] != '\0';)
                svwm_erase(line_v, j + 1);
            svwm_erase(line_v, i);
        }
        if (line_v[i] == ' ' && line_v[i + 1] == '\0')
        {
            svwm_erase(line_v, i);
            i--;
        }
    }
    return line_v;
}

struct Command *lexer(const char *line, int linec)
{
    line_counter++;
    if (linec > 0)
    {
        char *line_v = svwm_vector(linec, sizeof(char)); //перепишем нашу строку из стека в выделенный для этого svwm-вектор
        for (int i = 0; i < linec; i++)
            line_v[i] = line[i];

        svwm_pb_char(line_v, '\0'); //не уверен что stdlib будет работать, хуй знает как чел до вызова ее отрезал

        switch (line_v[0]) //тк наш язык довольно прост, у нас есть всего 3 варианта событий. (1)описание макропеременной, (2)обозначение секции, (3)логических выражений,
                           //все они имеют малую вариативность, откуда нам потребуется 3 конечных автомата.
        {
        case '#':
            line_v = define_preprocessor(line_v);
            return define_fsm(line_v);
            break;
        case ';':
            return '\0';
            break;
        case 's':
            if (section == 0)
            {
                line_v = preprocessor(line_v);
                return sector_fsm(line_v);
            }
        default:
            if (section == 'd')
            {
                line_v = data_preprocessor(line_v);
                return datasection_fsm(line_v);
            }
            if (section == 't' || section == '0')
            {
                line_v = preprocessor(line_v);
                return command_fsm(line_v);
            }
        };
    }
    return '\0';
}