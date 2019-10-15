#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hex_editor.h"
#include "parser.h"
#include "reader.h"
#include "file_handler.h"

void print_help(const char *help_cmd);
void eval(const char *input, file_handler *fh);

int quit_flag = 0;
extern command commands[NUM_CMDS];

int main(void) {
    init_commands();

    printf("Simple HEX editor.\nFor help, type \'?\'\n\n");
    commands[0].description(brief);

    char *input;
    file_handler fh = {NULL, NULL, NULL, 0, 0, 0};

    // The "REPL":
    while (!quit_flag)
    {
        printf("\n>> ");
        input = readline();
        eval(input, &fh);
        free(input);
    }

    return 0;
}

void eval(const char *input, file_handler *fh)
{
    command *cmd = extract_command(input);

    if (!cmd)
        printf("ERROR: unknown command.\n");
    else
    {
        cmd->eval_f(extract_arguments(input), fh);
    }
}
