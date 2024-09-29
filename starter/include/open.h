#include <stdint.h>
#ifndef IMAGEOPS_H
#define IMAGEOPS_H

typedef struct
{
    char* filename;
    char* mode;
    uint32_t offset;
    char* path;

} File;

void openFile(char* filename, char* mode, char* path);
void closeFile(char* filename);
void lsof();

#endif
