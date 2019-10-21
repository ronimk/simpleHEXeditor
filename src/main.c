#if 1 // LPCXpresso 1549 version:

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>
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

int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif

    init_commands();

    printf("Simple HEX editor.\nFor help, type \'?\'\n\n");

    char *input;
    file_handler fh = {NULL, NULL, NULL, 0, 1, 0, 0};

    // The "REPL":
    while (!quit_flag)
    {
        printf("\n>> ");
        input = readline(stdin);
        eval(input, &fh);
        free(input);
    }

    return 0;
}

#endif

#if 0 // The generic version:

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
    file_handler fh = {NULL, NULL, NULL, 0, 1, 0, 0};

    // The "REPL":
    while (!quit_flag)
    {
        printf("\n>> ");
        input = readline(stdin);
        eval(input, &fh);
        free(input);
    }

    return 0;
}

#endif
