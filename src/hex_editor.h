#ifndef HEX_EDITOR_H_
#define HEX_EDITOR_H_

#include <stdio.h>
#include "file_handler.h"

#define NUM_CMDS 10
#define MAX_CMD_NAME_LEN 9

typedef enum description_mode {brief, detailed} descr_mode;

typedef struct command {
    char *name;
    void (*eval_f)(const char *arg_str, file_handler *open_file);
    void (*description)(descr_mode m);
} command;

void init_commands(void);
void eval(const char *input, file_handler *fh);

#endif /* HEX_EDITOR_H_ */
