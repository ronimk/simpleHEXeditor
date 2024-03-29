#ifndef FILE_HANDLER_H_
#define FILE_HANDLER_H_

typedef struct file_handler {
    char *filename;
    char *patchfile_name;
    unsigned char *file_data;
    int modified;
    int record_patch;
    unsigned int filesize;
    unsigned int max_filesize;
} file_handler;

void close_file(file_handler *fh);
int open_file(file_handler *fh, char *filename, char *changefile_name);
int save_file(file_handler *fh);
int delete_from_file(file_handler *fh, unsigned int start, unsigned int end);
int add_to_file(file_handler *fh, unsigned int start, const char *hex_buf);

#endif /* FILE_HANDLER_H_ */
