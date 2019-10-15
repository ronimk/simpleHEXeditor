#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "hex_editor.h"
#include "parser.h"

static int terminal_char_p(char ch);

extern command commands[NUM_CMDS];

command *extract_command(const char *cmd_str)
{
    int i=0;
    char buf[MAX_CMD_NAME_LEN];

    while (i < MAX_CMD_NAME_LEN-1 && !terminal_char_p(cmd_str[i]) && !isspace(cmd_str[i]))
    {
        buf[i] = cmd_str[i];
        i++;
    }

    buf[i] = '\0';

    for (int i=0; i<NUM_CMDS; i++)
    {
        if (!strcmp(buf, commands[i].name))
            return &commands[i];
    }

    return NULL;
}

const char *extract_arguments(const char *cmd_str)
{
    while (!isspace(*cmd_str) && !terminal_char_p(*cmd_str))
        cmd_str++;

    if (isspace(*cmd_str))
        cmd_str++;

    return cmd_str;
}

char *get_first_argument(const char *arg_str)
{
	int len = 1;
	const char *end_p = arg_str;
	while (!isspace(*end_p) && !terminal_char_p(*end_p))
	{
        end_p++;
        len++;
	}

	char *buf = (char *)malloc(len);
	if (!buf)
		return NULL;

	for (int i=0; i<len-1; i++)
		buf[i] = arg_str[i];

	buf[len-1] = '\0';

	return buf;
}

int argument_arity(const char *arg_str)
{
    int arity = 0;

    int i=0;

    while (!terminal_char_p(arg_str[i]))
    {
        if (isspace(arg_str[i]))
            while (isspace(arg_str[i++])) ;

        if (!isspace(arg_str[i]))
        {
            while (!isspace(arg_str[i++]) && !terminal_char_p(arg_str[i])) ;
            arity++;
        }
    }

    return arity;
}

/*
 * read_nonnegative_int works similarly to the library function atoi
 * with the following exceptions:
 *
 * 1) it returns 0 when the conversion was successful and -1 otherwise.
 *
 * 2) It only works on nonnegative integers, thus converting "-123" fails.
 *
 * 3) Conversion of strings like "1123jf ..." fails. Only conversion of
 *    strings like "1123" and "1123 ..." works.
 *
 * 4) The result is stored in the given pointer argument (res), which is
 *    of type unsigned int.
 *
 * Also note that if an integer too large for type unsigned int is input,
 * the result will be garbage.
 */
int parse_nonnegative_int(const char *str, unsigned int *res)
{
    if (!isdigit(str[0]))
        return -1;

    int val=0;
    int i=0;
    while (isdigit(str[i]))
    {
        val *= 10;
        val += str[i] - '0';
        i++;
    }

    if (isspace(str[i]) || terminal_char_p(str[i]))
    {
        *res = val;
        return 0;
    }
    else return -1;
}

/*
 * valid_hexadecimal_p checks if the given string contains a valid hexadecimal.
 * 1 is returned if it does and 0 is returned if the string does not contain
 * a valid hexadecimal.
 *
 * This function should be used before read_next_byte, to check the integrity of
 * the given string.
*/
int valid_hexadecimal_p(char *str)
{
    return 0;
}

char printable_byte(char byte)
{
	return isprint(byte)? byte: '.';
}

static int terminal_char_p(char ch)
{
    return (ch == '\n' || ch == '\0');
}
