#include <stdio.h>
#include "descriptions.h"

void print_more_info(descr_mode m)
{
    printf("?: prints the help menu.\n");
    printf("? [command_name]: gives more details about a specific command.\n");
    if (m == detailed)
    {
        printf("\n");
        printf("Example:\n");
        printf("\t\"? mod\": gives specific details about the mod command.\n");
    }
}

void print_show_info(descr_mode m)
{
    printf("show [start] [end]: outputs all bytes from [start] to [end].\n");
    if (m == detailed)
    {
        printf("\n");
        printf("[start] and [end] are integer values denoting the starting and ending positions of a modifiable block of bytes.\n");
        printf("They both must be within the accepted limits of the file: from 0 to the size of the file minus one.\n");
        printf("\n");
        printf("Examples:\n");
        printf("\t\"show 32 63\": outputs all the bytes of opened file from the 32nd byte of the file up to the 63rd byte.\n");
        printf("\t\"show 0 11\": shows the first 12 bytes in the file.\n");
        printf("\n");
        printf("NOTE: There must exist a currently open file, or the command produces and error.\n");
    }
}

void print_del_info(descr_mode m)
{
    printf("del [start] [end]: deletes all bytes from [start] to [end].\n");
    if (m == detailed)
    {
        printf("\n");
        printf("[start] and [end] are integer values denoting the starting and ending positions of the byte-block to be deleted.\n");
        printf("They both must be within the accepted limits of the file: from 0 to the size of the file minues one.\n");
        printf("\n");
        printf("Examples:\n");
        printf("\t\"del 32 62\": deletes all the bytes of the opened file from the 32nd byte of the file up to the 63rd byte.\n");
        printf("\t\"del 0 11\": deletes the first 12 bytes in the file.\n");
        printf("\n");
        printf("NOTE: There must exist a currently open file, or the command produces and error.\n");
    }
}

void print_add_info(descr_mode m)
{
    printf("add [start] [hex+]: adds [hex+] input into offset [start].\n");
    if (m == detailed)
    {
        printf("\n");
        printf("[start] is an integer value denoting the position where the new bytes should be added.\n");
        printf("It must be within the accepted limits of the file: from 0 to the size of the file minus one.\n");
        printf("\n");
        printf("[hex+] is a non-empty stream of hexadecimal data.\n\tEx. 1: A9B65FF7\n\tEx. 2: 000ff95b\n");
        printf("\n");
        printf("NOTE: There must exist a currently open file, or the command produces and error.\n");
    }
}

void print_mod_info(descr_mode m)
{
    printf("mod [start] [end] [hex+]: changes the desired block of bytes into [hex+] block.\n");
    if (m == detailed)
    {
        printf("\n");
        printf("[start] and [end] are integer values denoting the starting and ending positions of a modifiable block of bytes.\n");
        printf("They both must be within the accepted limits of the file: from 0 to the size of the file minus one.\n");
        printf("\n");
        printf("[hex+] is a non-empty stream of hexadecimal data.\n\tEx. 1: A9B65FF7\n\tEx. 2: 000ff95b\n\n");
        printf("The change is performed to the entire blocks of bytes. Thus the size of [hex+] block\n");
        printf("does not have to match [end]-[start]+1.\n");
        printf("\n");
        printf("NOTE: There must exist a currently open file, or the command produces and error.\n");
    }
}

void print_filesize_info(descr_mode m)
{
    printf("filesize: outputs the current size of the opened file.\n");
    if (m == detailed)
    {
        printf("\n");
        printf("NOTE: There must exist a currently opened file, or the command produces and error.\n");
    }
}

void print_save_info(descr_mode m)
{
    printf("sav: saves the currently open file.\n");
    if (m == detailed)
    {
        printf("\n");
        printf("NOTE: There must exist a currently opened file, or the command produces and error.\n");
    }
}

void print_load_info(descr_mode m)
{
    printf("ld [filename] [patchfilename]: loads a new file.\n");
    if (m == detailed)
    {
        printf("\n");
        printf("[filename]: the name of the file. The file must already exist!\n");
        printf("[patchfilename]: the name of the patchfile where all the changes are logged.\n");
        printf("\n");
        printf("If another file is already opened, the load command prompts user to save the currently opened file\n");
        printf("before opening a new file.\n");
    }
}

void print_apply_info(descr_mode m)
{
    printf("apply [patch]: applies a change-patch to the current file.\n");
    if (m == detailed)
    {
        printf("\n");
        printf("[patch]: the name of the patch file.\n");
        printf("The file must not contain inconsistent data relative to the patch.\n");
        printf("\n");
        printf("NOTE: There must exist a currently opened file, or the command produces and error.\n");
    }
}

void print_quit_info(descr_mode m)
{
    printf("quit: quits the program.\n");
    if (m == detailed)
    {
        printf("\n");
        printf("If a file is opened, the quit command prompts user to save the currently opened file\n");
        printf("before quitting the program.\n");
    }
}
