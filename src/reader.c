#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 *  readline reads a new line from the user. It reserves a new string buffer
 *  for the user input form the heap and returns a pointer to that string buffer.
 *
 *  The calling program should take care of freeing any unneeded string buffers
 *  created by readline.
*/
char *readline(FILE *stream)
{
    const int realloc_size = 32;
    int line_len = 32;

    char *input = (char *)malloc(line_len);

    if (!input)
    {
        fprintf(stderr, "Memory allocation error");
        exit(-1);
    }

    char *endp;

    if (fgets(input, line_len, stream) == NULL)
    {
        free(input);
        return NULL;
    }

    while (input[strlen(input)-1] != '\n')
    {
        line_len += realloc_size;
        char *b = (char *)realloc(input, line_len);
        if (!b)
        {
            fprintf(stderr, "Memory allocation error");
            free(input);
            exit(-1);
        }
        input = b;

        endp = input + strlen(input);
        fgets(endp, realloc_size, stream);
    }

    strtok(input, "\r\n");
    return input;
}

int prompt_yes_no(void)
{
	const int ans_len = 4;
	char input[4];

	while (1)
	{
	    fgets(input, ans_len, stdin);
	    strtok(input, "\r\n");

	    if (!(strcmp(input, "yes") && strcmp(input, "y")))
		    return 1;
	    else if (!(strcmp(input, "no") && strcmp(input, "n")))
		    return 0;
	    else
	    	printf("Invalid input, please try again.\n");
	}


}
