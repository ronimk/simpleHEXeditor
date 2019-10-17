#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "file_handler.h"
#include "parser.h"

void close_file(file_handler *fh)
{
	free(fh->filename);
	free(fh->patchfile_name);
	free(fh->file_data);
	fh->modified = 0;
	fh->filesize = 0;
	fh->max_filesize = 0;

}

int open_file(file_handler *fh, char *filename, char *patchfile_name)
{
	const int base_filesize = 128;
	const int filesize_increments = 64;

	FILE *fp = fopen(patchfile_name, "a");
	if (!fp)
	{
		printf("Error opening the patchfile [%s].\n", patchfile_name);
		return -1;
	}
	fclose(fp);

	fp = fopen(filename, "rb");
	if (!fp)
	{
		printf("Error opening the file [%s]. Please make sure it exists.\n", filename);
		return -1;
	}
	else
	{
		fh->file_data = (unsigned char *)malloc(base_filesize);

		if (!fh->file_data)
		{
			printf("Memory allocation error while reading in the file data.\n");
			free(patchfile_name);
			free(filename);
			fclose(fp);
			return -1;
		}

		int max_size = base_filesize;
		int i=0;
		unsigned char ch;
		while (fread(&ch, 1, 1, fp))
		{
			if (i >= max_size-1)
			{
				max_size += filesize_increments;
				unsigned char *b = (unsigned char *)realloc(fh->file_data, max_size);

				if (!b)
				{
					printf("Memory allocation error while reading in the file data.\n");
					free(fh->file_data);
					free(patchfile_name);
					free(filename);
					fclose(fp);
					return -1;
				}
				else
					fh->file_data = b;
			}

			fh->file_data[i] = ch;
			i++;
		}

		if (ferror(fp))
		{
			printf("Unexpected problem while reading in the file data. Closing the file...\n");
			free(fh->file_data);
			free(patchfile_name);
			free(filename);
			fclose(fp);
			return -1;
		}
		else
		{
			fh->patchfile_name = patchfile_name;
			fh->filename = filename;
			fh->max_filesize = max_size;
			fh->filesize = i+1;
			fh->modified = 0;
			fh->record_patch = 1;
			return 0;
		}
	}
}

int save_file(file_handler *fh)
{
	FILE *fp = fopen(fh->filename, "wb");

	if (!fp)
	{
		printf("Error opening the file [%s]. Please make sure it exists.\n", fh->filename);
		return -1;
	}
	else
	{
		if (fwrite(fh->file_data, 1, fh->filesize, fp) != fh->filesize)
		{
			printf("Error writing into the file. The file may be corrupted...\n");
			fclose(fp);
			return -1;
		}

		fclose(fp);
		fh->modified = 0;
		return 0;
	}
}

int delete_from_file(file_handler *fh, unsigned int start, unsigned int end)
{
	// First, update the patch file, if necessary:
	if (fh->record_patch)
    {
        FILE *fp = fopen(fh->patchfile_name, "a");
        if (!fp)
        {
            printf("Error opening the patch file for updating. Command aborted\n");
            return -1;
        }

        fprintf(fp, "del %u %u ", start, end);

        for (unsigned int i=start; i<=end; i++)
            fprintf(fp, "%02X", fh->file_data[i]);

        fprintf(fp, "\n");

        fclose(fp);
    }

	// next, delete the bytes from the file_handler:
	for (unsigned int i=start, j=end+1; j < fh->filesize; i++, j++)
		fh->file_data[i] = fh->file_data[j];

	fh->filesize -= 1 + end - start;
	fh->modified = 1;
	return 0;
}

int add_to_file(file_handler *fh, unsigned int start, const char *hex_buf)
{
    unsigned int hex_len;

	// First, update the patch file, if necessary:
	if (fh->record_patch)
    {
        FILE *fp = fopen(fh->patchfile_name, "a");
        if (!fp)
        {
            printf("Error opening the patch file for updating. Command aborted\n");
            return -1;
        }

        hex_len = strlen(hex_buf);

        fprintf(fp, "add %u ", start);

        for (unsigned int i=0; i<hex_len; i++)
            fprintf(fp, "%c", toupper(hex_buf[i]));

        fprintf(fp, "\n");

        fclose(fp);
    }

	// next, add the bytes to the file_handler:
	// 1) if there is not enough room, extend the file_data buffer:
	hex_len /= 2;

	if (fh->max_filesize < fh->filesize+hex_len)
	{
		unsigned char *b = (unsigned char *)realloc(fh->file_data, fh->filesize+hex_len);

		if (!b)
		{
			printf("Memory allocation error.\n");
			return -1;
		}

		fh->file_data = b;
		fh->max_filesize = fh->filesize + hex_len;
	}

	// 2) shift all the bytes past the starting point by hex_len to the "right":
	for (unsigned int i=fh->filesize-1; i>=start; i--)
		fh->file_data[i+hex_len] = fh->file_data[i];

	// 3) add the new data into file_data buffer:
	for (unsigned int i=start, j=0; i<start+hex_len; i++, j+=2)
		next_hex_into_byte(hex_buf+j, &fh->file_data[i]);

	fh->filesize += hex_len;
	fh->modified = 1;
	return 0;
}
