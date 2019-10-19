#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "hex_editor.h"
#include "parser.h"

static int hex_char_p(char ch);
static int hex_value(char hex_ch);

extern command commands[NUM_CMDS];

command *extract_command(const char *cmd_str)
{
    int i=0;
    char buf[MAX_CMD_NAME_LEN];

    while (i < MAX_CMD_NAME_LEN-1 && cmd_str[i]!='\0' && !isspace(cmd_str[i]))
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
    while (!isspace(*cmd_str) && *cmd_str!='\0')
        cmd_str++;

    if (isspace(*cmd_str))
        cmd_str++;

    return cmd_str;
}

char *get_first_argument(const char *arg_str)
{
	int len = 1;
	const char *end_p = arg_str;
	while (!isspace(*end_p) && *end_p != '\0')
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

    while (arg_str[i] != '\0')
    {
        if (isspace(arg_str[i]))
            while (isspace(arg_str[i++])) ;

        if (!isspace(arg_str[i]))
        {
            while (!isspace(arg_str[i++]) && arg_str[i]!='\0') ;
            arity++;
        }
    }

    return arity;
}

void remove_last_arg(char *arg_str)
{
    int len = strlen(arg_str);
    int i = len-1;

    while (i>0 && arg_str[i] != ' ')
        i--;

    arg_str[i] = '\0';
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
    // Note that nothing currently prevents
	// the user from "overflowing" the number by simply
	// typing enough digits...
	// A better digit reader should be used for a real application
	unsigned int val=0;
	// there are two cases to consider: either the number is a decimal number,
	//  or it is a hexadecimal number, in which case it starts with a "0x" or "0X":
    if (str[0] == '0' && toupper(str[1]) == 'X')
    {
        int i=2;
        char next_hex;
        while (hex_char_p((next_hex = toupper(str[i]))))
        {
            val <<= 4;
            val |= hex_value(next_hex);
            i++;
        }

        if (i>2 && (isspace(next_hex) || next_hex == '\0'))
        {
            *res = val;
            return 0;
        }
        else return -1;
    }
    if (isdigit(str[0]))
    {
        int i=0;
        while (isdigit(str[i]))
        {
            val *= 10;
            val += str[i] - '0';
            i++;
        }

        if (isspace(str[i]) || str[i] == '\0')
        {
            *res = val;
            return 0;
        }
        else return -1;
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
int valid_hexadecimal_p(const char *str)
{
	char parity = 0;
	int i=0;
	while(str[i] != '\0')
	{
		if (!hex_char_p(toupper(str[i])))
			return 0;

		parity = ~parity;
		i++;
	}

	if(parity == 0)
		return 1;
	else
		return 0;
}

void next_hex_into_byte(const char *hex_str, unsigned char *res)
{
    *res = 0;
	*res = hex_value(toupper(hex_str[0]));
	*res <<=4;
	*res += hex_value(toupper(hex_str[1]));
}

void byte_to_hex(char byte, char hex[3])
{
    static char hex_table[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    int h = (byte & 0xF0)>>4;
    hex[0] = hex_table[h];
    h = byte & 0x0F;
    hex[1] = hex_table[h];
}

char printable_byte(char byte)
{
	return isprint(byte)? byte: '.';
}

static int hex_char_p(char ch)
{
	return (isdigit(ch) || (ch >= 'A' && ch <= 'F'));
}

static int hex_value(char hex_ch)
{
    return isdigit(hex_ch)? (hex_ch - '0'): hex_ch - 'A' + 10;
}
