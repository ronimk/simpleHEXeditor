#include <stdlib.h>
#include <string.h>
#include "hex_editor.h"
#include "descriptions.h"
#include "parser.h"
#include "reader.h"
#include "file_handler.h"

command commands[NUM_CMDS];

static void eval_more(const char *arg_str, file_handler *fh);
static void eval_show(const char *arg_str, file_handler *fh);
static void eval_del(const char *arg_str, file_handler *fh);
static void eval_add(const char *arg_str, file_handler *fh);
static void eval_mod(const char *arg_str, file_handler *fh);
static void eval_filesize(const char *arg_str, file_handler *fh);
static void eval_save(const char *arg_str, file_handler *fh);
static void eval_load(const char *arg_str, file_handler *fh);
static void eval_apply(const char *arg_str, file_handler *fh);
static void eval_quit(const char *arg_str, file_handler *fh);

static void print_hextable(file_handler *fh, unsigned int start, unsigned int end);

extern int quit_flag;

void init_commands(void)
{
    commands[0].name = "?";
    commands[0].eval_f = eval_more;
    commands[0].description = print_more_info;

    commands[1].name = "show";
    commands[1].eval_f = eval_show;
    commands[1].description = print_show_info;

    commands[2].name = "del";
    commands[2].eval_f = eval_del;
    commands[2].description = print_del_info;

    commands[3].name = "add";
    commands[3].eval_f = eval_add;
    commands[3].description = print_add_info;

    commands[4].name = "mod";
    commands[4].eval_f = eval_mod;
    commands[4].description = print_mod_info;

    commands[5].name = "filesize";
    commands[5].eval_f = eval_filesize;
    commands[5].description = print_filesize_info;

    commands[6].name = "sav";
    commands[6].eval_f = eval_save;
    commands[6].description = print_save_info;

    commands[7].name = "ld";
    commands[7].eval_f = eval_load;
    commands[7].description = print_load_info;

    commands[8].name = "apply";
    commands[8].eval_f = eval_apply;
    commands[8].description = print_apply_info;

    commands[9].name = "quit";
    commands[9].eval_f = eval_quit;
    commands[9].description = print_quit_info;
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

static void eval_more(const char *arg_str, file_handler *fh)
{
	int arguments = argument_arity(arg_str);

	if (arguments > 1)
    {
        printf("ERROR: ? takes exactly zero or one argument(s).\n");
        printf("For help, type \"? ?\"\n");
        return;
    }
	else if(arguments == 0)
    {
        printf("Available commands:\n\n");
        for (int i=0; i<NUM_CMDS; i++)
            commands[i].description(brief);
    }
    else
    {
        command *cmd = extract_command(arg_str);
        if (!cmd)
        {
            printf("ERROR: unknown command given to ? as an argument.\n");
            return;
        }

        cmd->description(detailed);
    }
}

static void eval_show(const char *arg_str, file_handler *fh)
{
	if (argument_arity(arg_str) != 2)
	{
	    printf("ERROR: show takes exactly two argument(s).\n");
	    printf("For help, type \"? show\"\n");
	    return;
	}

	if (!fh->file_data)
	{
		printf("ERROR: no currently open file exists.\n");
		return;
	}

     unsigned int start, end;

     if (parse_nonnegative_int(arg_str, &start) || start >= fh->filesize)
     {
    	 printf("Invalid [start] value given.\n");
    	 printf("For help, type \"? show\"\n");
    	 return;
     }

     if (parse_nonnegative_int(extract_arguments(arg_str), &end) || end >= fh->filesize)
     {
         printf("Invalid [end] value given.\n");
         printf("For help, type \"? show\"\n");
         return;
     }

     if (start > end)
     {
    	printf("[start] must be smaller than [end].\n");
    	printf("For help, type \"? show\"\n");
    	return;
     }

     print_hextable(fh, start, end);
}

static void eval_del(const char *arg_str, file_handler *fh)
{
	if (argument_arity(arg_str) != 2)
	{
	    printf("ERROR: del takes exactly two argument(s).\n");
	    printf("For help, type \"? del\"\n");
	    return;
	}

	if (!fh->file_data)
	{
		printf("ERROR: no currently open file exists.\n");
		return;
	}

    unsigned int start, end;

    if (parse_nonnegative_int(arg_str, &start) || start >= fh->filesize)
    {
   	 printf("Invalid [start] value given.\n");
   	 printf("For help, type \"? del\"\n");
   	 return;
    }

    if (parse_nonnegative_int(extract_arguments(arg_str), &end) || end >= fh->filesize)
    {
        printf("Invalid [end] value given.\n");
        printf("For help, type \"? del\"\n");
        return;
    }

    if (start > end)
    {
   	printf("[start] must be smaller than [end].\n");
   	printf("For help, type \"? del\"\n");
   	return;
    }

    if(!delete_from_file(fh, start, end))
    	printf("Deleted %u bytes from the file...\n", end-start+1);
}

static void eval_add(const char *arg_str, file_handler *fh)
{
	if (argument_arity(arg_str) != 2)
	{
		printf("ERROR: add takes exactly two argument(s).\n");
	    printf("For help, type \"? add\"\n");
	    return;
	}

	if (!fh->file_data)
	{
		printf("ERROR: no currently open file exists.\n");
		return;
	}

	unsigned int start;
	if (parse_nonnegative_int(arg_str, &start) || start > fh->filesize)
	{
	 	 printf("Invalid [start] value given.\n");
	   	 printf("For help, type \"? add\"\n");
	   	 return;
	}

	const char *hex = extract_arguments(arg_str);
	if (!valid_hexadecimal_p(hex))
	{
		printf("Invalid [hex+] block given.\n");
	    printf("For help, type \"? add\"\n");
	}
	else
	{
		if(!add_to_file(fh, start, hex))
			printf("Adding to file finished...\n");
	}

}

static void eval_mod(const char *arg_str, file_handler *fh)
{
	if (argument_arity(arg_str) != 3)
	{
	    printf("ERROR: mod takes exactly three argument(s).\n");
	    printf("For help, type \"? mod\"n");
	    return;
	}
    if (!fh->file_data)
	{
		printf("ERROR: no currently open file exists.\n");
		return;
	}

    unsigned int start, end;
    const char *next_arg = arg_str;
    if (parse_nonnegative_int(next_arg, &start) || start >= fh->filesize)
    {
   	 printf("Invalid [start] value given.\n");
   	 printf("For help, type \"? mod\"\n");
   	 return;
    }
    next_arg = extract_arguments(next_arg);
    if (parse_nonnegative_int(next_arg, &end) || end >= fh->filesize)
    {
        printf("Invalid [end] value given.\n");
        printf("For help, type \"? mod\"\n");
        return;
    }

    if (start > end)
    {
   	printf("[start] must be smaller than [end].\n");
   	printf("For help, type \"? mod\"\n");
   	return;
    }

    next_arg = extract_arguments(next_arg);
	if (!valid_hexadecimal_p(next_arg))
	{
		printf("Invalid [hex+] block given.\n");
	    printf("For help, type \"? mod\"\n");
	}
	else
    {
        if(delete_from_file(fh, start, end) || add_to_file(fh, start, next_arg))
        {
            printf("Error modifying the file. Data may be corrupted.\nSaving the file not advised.\n");
            return;
        }

        printf("Finished modifying the file...\n");
    }
}

static void eval_filesize(const char *arg_str, file_handler *fh)
{
    if (argument_arity(arg_str) != 0)
    {
        printf("ERROR: filesize does not take any arguments.\n");
    }
    else
    {
        if (!fh->file_data)
        {
            printf("ERROR: no currently opened file exists.\n");
        }
        else
        {
            printf("Current size of the file: %u bytes\n", fh->filesize);
            printf("The file has %sbeen modified since last save.\n", fh->modified? "" : "not ");

        }
    }
}

static void eval_save(const char *arg_str, file_handler *fh)
{
	if (argument_arity(arg_str) != 0)
	{
		printf("ERROR: sav does not take any arguments.\n");
		printf("For more information, type \"? sav\"\n");
		return;
	}

	if (!fh->file_data)
	{
		printf("ERROR: no currently open file exists.\n");
		return;
	}

	if (fh->modified)
	{
		if(!save_file(fh))
			printf("Saving complete...\n");
	}
	else
		printf("No modifications done to the file.\n");
}

static void eval_load(const char *arg_str, file_handler *fh)
{
	if (argument_arity(arg_str) != 2)
	{
	    printf("ERROR: ld takes exactly two arguments: the names of the file and the patchfile.\n");
	    printf("For more information, type \"? ld\"\n");
	    return;
	}

	if (fh->file_data && fh->modified)
    {
		printf("A modified file is currently opened. Do you wish to save the old file? (y/n)");
	    if (prompt_yes_no())
	    	save_file(fh);
	    else
	    	printf("File not saved\n");

	    close_file(fh);
	}

	char *filename = get_first_argument(arg_str);

	if (!filename)
	{
		printf("Memory allocation error.\n");
		return;
	}

	const char *next_arg = extract_arguments(arg_str);
	char *patchfile_name = get_first_argument(next_arg);

	if (!patchfile_name)
	{
		printf("Memory allocation error.\n");
		free(filename);
		return;
	}

	if (!strcmp(filename, patchfile_name))
	{
		printf("Error: [filename] and [patchfile_name] cannot be the same.\n");
		free(filename);
		free(patchfile_name);
		return;
	}

	if (!open_file(fh, filename, patchfile_name))
		printf("File loaded...\n");
}

static void eval_apply(const char *arg_str, file_handler *fh)
{
    int arity = argument_arity(arg_str);
    if (arity < 1 || arity > 2)
    {
        printf("ERROR: apply takes one mandatory argument and one optional argument.\n");
        printf("For more information, type \"? apply\"\n");
        return;
    }

    if (!fh->file_data)
	{
		printf("ERROR: no currently open file exists.\n");
		return;
	}

	const char *next_arg = arg_str;
	char *applypatch_filename = get_first_argument(next_arg);

	if (!applypatch_filename)
	{
		printf("Memory allocation error.\n");
		return;
	}

	fh->record_patch = 0;

	if (arity == 1)
    {
        FILE *fp = fopen(applypatch_filename, "r");
        if (!fp)
        {
            printf("Error opening the patch file\n");
            free(applypatch_filename);
            return;
        }

        char *next_line;
        while ((next_line = readline(fp)))
        {
            printf("~%s~ : ", next_line);
            if (!strcmp("del", extract_command(next_line)->name))
            {
                // Due to the initial decisions to both make sure the argument arity
                // is not violated for any command, and to allow for applying patches
                // in "both directions", we need to make ugly looking
                // extra work here. In a future version, the easiest solution to avoid
                // this extra work would be to make the argument_arity checking a little
                // less strict...
                remove_last_arg(next_line);
            }
            printf("~%s~\n", next_line);
            eval(next_line, fh);
            free(next_line);
        }

        if (ferror(fp))
            printf("Error applying the patch file. The open file may be corrupted.\nSaving not recommended.\n");
    }
    else
    {
        // handle reverse case here
         printf("apply not fully implemented yet.\n");
    }

	fh->record_patch = 1;
}

static void eval_quit(const char *arg_str, file_handler *fh)
{
    if (argument_arity(arg_str) != 0)
    {
        printf("ERROR: quit does not take any arguments.\n");
    }
    else
    {
        if (fh->file_data)
        {
        	if (fh->modified)
        	{
        		printf("Do you wish to save the modified file? (y/n)");
        	    if (prompt_yes_no())
        	    	save_file(fh);
        		else
        			printf("File not saved\n");
        	}

        	close_file(fh);
        }

        quit_flag = 1;
    }
}

static void print_hextable(file_handler *fh, unsigned int start, unsigned int end)
{
	#define BYTES_IN_LINE 16

	unsigned int bytes = 1 + end - start;

	// Print the title line:
	printf(" Offset(h)  ");
	for (unsigned int i=0; i<BYTES_IN_LINE; i++)
		printf("%02X ", i);
	printf("\t----------------");
	printf("\n");

	unsigned int lines = bytes/BYTES_IN_LINE + 1;
	for (unsigned int i=0, pos = start; i<lines; i++)
	{
		unsigned int line_start = pos;
		unsigned int j=0;

		// print each line:
		// 1) print the offset hexadecimal of the first byte in the line:
		printf("%010X  ", line_start);

		//2) print the bytes for the line in hexadecimal format:
		for (; j<BYTES_IN_LINE && pos <= end; j++, pos++)
			printf("%02X ", fh->file_data[pos]);

        // 2.a) For the last line, fill in the necessary empty spaces
        //      between the hex and text representations:
		if (i == lines-1)
			printf("%*c", (BYTES_IN_LINE-j)*3, ' ');

		printf("\t");

		// 2.b) Print the text representation for the line
		//      on the right of the hex representation:
		for (int j=0; j<BYTES_IN_LINE && line_start<pos; j++, line_start++)
			printf("%c", printable_byte(fh->file_data[line_start]));

		printf("\n");
	}
}
