#include "../lib/svwm.h"
#include "../lib/logging.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

struct Command
{
    char *initial_state;
    char *initial_word;
    char *final_state;
    char *final_word;
    char direction;
};

void macros_table_destructor();

void command_destructor(struct Command *c);

char ***macros_table;

struct Command *lexer(const char *line, int linec);