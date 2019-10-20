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
        printf("[start] and [end] are either decimal or hexadecimal (prepended with \"0X\") values denoting the starting\n");
        printf("and ending positions of a modifiable block of bytes. They both must be within the accepted limits of the\n");
        printf("file: from 0 to one less than the size of the file.\n");
        printf("\n");
        printf("Examples:\n");
        printf("\t\"show 32 63\": outputs all the bytes of opened file from the 32nd byte of the file up to the 63rd byte.\n");
        printf("\t\"show 0x0 0xB\": shows the first 13 bytes in the file.\n");
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
        printf("[start] and [end] are either decimal or hexadecimal (prepended with \"0X\") values denoting the starting\n");
        printf("and ending positions of the byte-block to be deleted. They both must be within the accepted limits of the\n");
        printf("file: from 0 to one less than the size of the file.\n");
        printf("\n");
        printf("Examples:\n");
        printf("\t\"del 32 62\": deletes all the bytes of the opened file from the 33rd byte of the file up to the 63rd byte.\n");
        printf("\t\"del 0x0 0x11\": deletes the first 18 bytes in the file.\n");
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
        printf("[start] is  either a decimal or a hexadecimal (prepended with \"0X\") value denoting the position where the\n");
        printf("new bytes should be added. It must be within a range from 0 to the size of the file. When [start] equals\n");
        printf("filesize, the new bytes will be added as to the end of the file as the last bytes in the file.\n");
        printf("\n");
        printf("[hex+] is a non-empty stream of hexadecimal data.\n");
        printf("\n");
        printf("Examples:\n");
        printf("\t\"add 0x4A 0FA1CC\": adds the bytes 0FA1CC into the 75th byte position, moving all upper\n");
        printf("\t                   bytes three spaces to make room for the new bytes.\n");
        printf("\t\"add 0 C356ABD451FF00\"\n");
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
        printf("[start] and [end] are either decimal or hexadecimal (prepended with \"0X\") values denoting the starting\n");
        printf("and ending positions of a modifiable block of bytes. They both must be within the accepted limits of the file:\n");
        printf("from 0 to one less than the size of the file.\n");
        printf("\n");
        printf("[hex+] is a non-empty stream of hexadecimal data. The change is performed to the entire blocks of bytes.\n");
        printf("Thus the size of [hex+] block does not have to match [end]-[start]+1.\n");
        printf("\n");
        printf("Examples:\n");
        printf("\t\"mod 0x4A 0x8C 0FA1CC\": first deletes the block starting from byte position 4A and ending into\n");
        printf("\t                        byte position 8C, then adds the bytes 0FA1CC into the byte position 4A.\n");
        printf("\t\"mod 0 2 C356ABFF00\":   deletes the first three bytes and injects the bytes C356ABFF00 to\n");
        printf("\t                        the beginning of the file.");
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
    printf("apply [patch] (optional: reverse): applies a change-patch to the current file.\n");
    if (m == detailed)
    {
        printf("\n");
        printf("[patch]: the name of the patch file.\n");
        printf("If \"reverse\" is given as a second argument to apply, the patch is applied so that the file will be reverted\n");
        printf("back to an earlier, unmodified version. Otherwise the commands in the patch are applied directly, as if\n");
        printf("they were typed one by one into the console.\n");
        printf("\n");
        printf("The file should not contain inconsistent data relative to the patch or the file will become corrupted.\n");
        printf("It is left as the user's responsibility to ensure the validity of the patchfile to be applied.\n");
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
