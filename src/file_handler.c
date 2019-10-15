#include <stdio.h>
#include <stdlib.h>
#include "file_handler.h"

void close_file(file_handler *fh)
{
	free(fh->filename);
	free(fh->patchfile_name);
	free(fh->file_data);
	fh->modified = 0;
	fh->filesize = 0;
	fh->max_filesize = 0;

}

void open_file(file_handler *fh, char *filename, char *patchfile_name)
{
	const int base_filesize = 128;
	const int filesize_increments = 64;

	FILE *fp = fopen(patchfile_name, "a");
	if (!fp)
		printf("Error opening the patchfile [%s].\n", patchfile_name);
	fclose(fp);

	fp = fopen(filename, "rb");
	if (!fp)
		printf("Error opening the file [%s]. Please make sure it exists.\n", filename);
	else
	{
		fh->file_data = (char *)malloc(base_filesize);

		if (!fh->file_data)
		{
			printf("Memory allocation error while reading in the file data.\n");
			free(patchfile_name);
			free(filename);
			fclose(fp);
			return;
		}

		int max_size = base_filesize;
		int i=0;
		char ch;
		while (fread(&ch, 1, 1, fp))
		{
			if (i >= max_size)
			{
				max_size += filesize_increments;
				char *b = (char *)realloc(fh->file_data, max_size);

				if (!b)
				{
					printf("Memory allocation error while reading in the file data.\n");
					free(fh->file_data);
					free(patchfile_name);
					free(filename);
					fclose(fp);
					return;
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
		}
		else
		{
			fh->patchfile_name = patchfile_name;
			fh->filename = filename;
			fh->max_filesize = max_size;
			fh->filesize = i;
			fh->modified = 0;
		}
	}
}

void save_file(file_handler *fh)
{
	FILE *fp = fopen(fh->filename, "wb");

	if (!fp)
		printf("Error opening the file [%s]. Please make sure it exists.\n", fh->filename);
	else
	{
		if (fwrite(fh->file_data, 1, fh->filesize, fp) != fh->filesize)
		{
			printf("Error writing into the file. The file may be corrupted...\n");
			fclose(fp);
			return;
		}

		fclose(fp);
		fh->modified = 0;
	}
}

void delete_from_file(file_handler *fh, unsigned int start, unsigned int end)
{
	// First, update the patch file:
	FILE *fp = fopen(fh->patchfile_name, "a");
	if (!fp)
	{
		printf("Error opening the patch file for updating. Command aborted\n");
		return;
	}

	fprintf(fp, "del %d %d ", start, end);

	for (unsigned int i=start; i<=end; i++)
		fprintf(fp, "%02X", fh->file_data[i]);

	fprintf(fp, "\n");

	fclose(fp);

	// next, delete the bytes from the file_handler:
	for (unsigned int i=start, j=end+1; i<=end && j < fh->filesize; i++, j++)
	{
		fh->file_data[i] = fh->file_data[j];
	}

	fh->filesize -= 1 + end - start;
	fh->modified = 1;
}
