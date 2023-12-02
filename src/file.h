#ifndef FILE_H_
#define FILE_H_

#include <kvec.h>

#include "error.h"
#include "types.h"


typedef struct {
    char name[256];
    char* data;
    size_t size;
} file_t;


#ifdef __cplusplus
extern "C" {
#endif

err_t file_read(file_t* file, const char* const path);
void file_free(file_t* file);

#ifdef __cplusplus
}
#endif


#endif