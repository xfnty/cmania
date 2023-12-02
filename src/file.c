#include "file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "error.h"


err_t file_read(file_t* file, const char* const path) {
    ASSERT(file != NULL);
    ASSERT(path != NULL);

    FILE* fhandle = fopen(path, "r");
    ASSERT_RETURN_VALUE(fhandle, ERROR_FILE_NOT_FOUND);

    fseek(fhandle, 0, SEEK_END);
    file->size = ftell(fhandle) + 1;
    fseek(fhandle, 0, SEEK_SET);

    file->data = malloc(file->size + 1);
    ASSERT(file->data);

    size_t bytes_read = fread(file->data, 1, file->size, fhandle);
    ASSERT_GOTO(bytes_read + 1 == file->size, FAILURE);

    strncpy(file->name, path, LENGTH(file->name) - 1);
    file->name[LENGTH(file->name) - 1] = '\0';

    return ERROR_SUCCESS;

    FAILURE:
    free(file->data);
    fclose(fhandle);
    return ERROR_FILE_IO;
}

void file_free(file_t* file) {
    ASSERT(file != NULL);

    if (file->size == 0 || file->data == NULL)
        return;

    free(file->data);
    file->data = NULL;
    file->size = 0;
    memset(file->name, '\0', LENGTH(file->name));
}
