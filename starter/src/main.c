#include "lexer.h"
#include "bpb_reader.h"
#include "stdio.h"
#include "stdbool.h"
#include <string.h>
#include "dir.h"
#include "open.h"

void prompt(char*);

int main(int argc, char* argv[]){
    char* cmd;
    tokenlist *tokens;
    bpb_t boot = bpbReader();

    uint32_t FirstDataSector = boot.BPB_RsvdSecCnt + (boot.BPB_NumFATs * boot.BPB_FATSz32);
    uint32_t RootDirFirstSector = FirstDataSector + (boot.BPB_SecPerClus * (boot.BPB_RootClus - 2));
    uint32_t RootDirFirstByteOffset = RootDirFirstSector * boot.BPB_BytsPerSec;

    currDirect currDentry;
    currDentry.address = RootDirFirstByteOffset - 32;
    currDentry.path = argv[1];
    currDentry.pastAddresses = (uint32_t*)malloc(0 * sizeof(uint32_t));
    currDentry.lastAddressIndex = 0;

    while(true){
        //In here, we will call the prompt and pass the image argument into it
        //For now, prompt path is hardcoded.
        prompt(argv[1]);
        cmd = get_input();

        tokens = get_tokens(cmd);


        //Either print out bpb data for info or exit function
        if(strcmp(tokens->items[0], "info") == 0){
            printf("The position of the root cluster: %u\n", boot.BPB_RootClus);
            printf("The number of bytes per sector: %u\n", boot.BPB_BytsPerSec);
            printf("The number of sectors per cluster: %u\n", boot.BPB_SecPerClus);
            printf("The total number of clusters in the data region: %u\n",
                    boot.BPB_TotSec32/boot.BPB_SecPerClus);
            printf("The number of entries in a FAT: %u\n", 
                    (boot.BPB_FATSz32*boot.BPB_BytsPerSec)/4);
            printf("The image size: %u\n", boot.BPB_TotSec32*boot.BPB_BytsPerSec);
            printf("\n");
        }
        else if (strcmp(tokens->items[0], "cd") == 0)
            cd(tokens->items[1], &currDentry, &boot);
        else if (strcmp(tokens->items[0], "ls") == 0)
            dir_main(currDentry, &boot);
        else if (strcmp(tokens->items[0], "open") == 0) {
            if (getFileAddress(currDentry, tokens->items[1], &boot) != NULL)
                openFile(tokens->items[1], tokens->items[2], currDentry.path);
            else
                printf("error: file does not exist\n");
        }
        else if (strcmp(tokens->items[0], "close") == 0)
            closeFile(tokens->items[1]);
        else if (strcmp(tokens->items[0], "lsof") == 0)
            lsof();
        else if(strcmp(tokens->items[0], "exit") == 0)
            break;
    }

    return 0;
}
