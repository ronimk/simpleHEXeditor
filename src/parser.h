#ifndef PARSER_H_
#define PARSER_H_

#include "hex_editor.h"

command *extract_command(const char *cmd_str);
const char *extract_arguments(const char *cmd_str);
char *get_first_argument(const char *arg_str);
int argument_arity(const char *arg_str);
int parse_nonnegative_int(const char *str, unsigned int *res);
int valid_hexadecimal_p(const char *str);
void next_hex_into_byte(const char *hex_str, unsigned char *res);
char printable_byte(char byte);

#endif /* PARSER_H_ */