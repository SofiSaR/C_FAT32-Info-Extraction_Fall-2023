#include "open.h"
#include <stdio.h>
#include <string.h>

File openFiles[512];
int FilesCount = 0;

void openFile(char* filename, char* mode, char* path) 
{
    for (int i = 0; i < FilesCount; i++) {
        if (strcmp(openFiles[i].filename, filename) == 0) {
            printf("error: file is already open\n");
            return;
        }
    }
    openFiles[FilesCount].filename = filename;
    openFiles[FilesCount].mode = mode;
    openFiles[FilesCount].offset = 0;
    openFiles[FilesCount].path = path;

    printf("opened %s\n", openFiles[FilesCount].filename);

    FilesCount++;
}

void closeFile(char* filename)
{
    for(int i = 0; i < FilesCount; i++){
        if(strcmp(openFiles[i].filename, filename) == 0){
            for(int k = i+1; k < FilesCount; k++)
            {
                openFiles[k-1].filename = openFiles[k].filename;
                openFiles[k-1].mode = openFiles[k].mode;
                openFiles[k-1].offset = openFiles[k].offset;
                openFiles[k-1].path = openFiles[k].path;
            }
            openFiles[FilesCount - 1].filename = "";
            FilesCount--;
            printf("closed %s\n", filename);
            return;
        }
    }
    printf("error: no file of that name has been opened\n");
}

void lsof() {
    if (FilesCount == 0) {
        printf("no files are opened\n");
        return;
    }
    printf("%-7s", "INDEX");
    printf("%-11s", "NAME");
    printf("%-11s", "MODE");
    printf("%-11s", "OFFSET");
    printf("%-11s\n", "PATH");
    for (int i = 0; i < FilesCount; i++) {
        printf("%-7d%-11s%-11s%-11d%-11s\n", i, openFiles[i].filename, openFiles[i].mode, openFiles[i].offset, openFiles[i].path);
    }
}