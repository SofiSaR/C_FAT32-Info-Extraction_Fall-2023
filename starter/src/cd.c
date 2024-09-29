#include "lexer.h"
#include "bpb_reader.h"
#include "stdio.h"
#include "stdbool.h"
#include <string.h>
#include "dir.h"

void mov(char* path, currDirect* currDentry, bpb_t* bpb) {
    // bpb_t bpb = bpbReader();
    // Calculate the first sector of the root directory

    // printf("Open sesame!\n");
    FILE* fd = fopen("fat32.img", "r");
    if (fd == NULL) {
        perror("open fat32.img failed");
        return;
    }

    uint32_t FirstDataSector = bpb->BPB_RsvdSecCnt + (bpb->BPB_NumFATs * bpb->BPB_FATSz32);
    uint32_t RootDirFirstSector = FirstDataSector + (bpb->BPB_SecPerClus * (bpb->BPB_RootClus - 2));
    uint32_t RootDirFirstByteOffset = RootDirFirstSector * bpb->BPB_BytsPerSec;


    uint32_t offset = currDentry->address;

    if (offset == RootDirFirstByteOffset - 32) {
        currDentry->pastAddresses = (uint32_t*)realloc(currDentry->pastAddresses, 1 * sizeof(uint32_t));
        currDentry->lastAddressIndex = 0;
        currDentry->pastAddresses[0] = offset;
    // Read the root directory entries
        for (uint32_t i = 0; i < (bpb->BPB_SecPerClus * bpb->BPB_BytsPerSec / sizeof(dentry_t)); i++) {
            offset = currDentry->address + (i * (sizeof(dentry_t)));
            dentry_t *dentry = encode_dir_entry(fd, offset);
            if (dentry) {
                if (strstr(dentry->DIR_Name, path) != NULL) {
                    // currDentry->lastAddressIndex = i;
                    // currDentry->pastAddresses[i] = currDentry->address;
                    currDentry->address = offset;
                    strcat(currDentry->path, "/");
                    strcat(currDentry->path, dentry->DIR_Name);
                    char *p = strchr(currDentry->path, ' ');
                    if (p != NULL)
                        *p = '\0';
                    free(dentry);
                }
            } else {
                // If the directory entry was not read, break the loop
                break;
            }
        }
    }
    else if (strstr(path, "..") != NULL) {
        currDentry->address = currDentry->pastAddresses[currDentry->lastAddressIndex];
        char *last_slash;

        // Find the last occurrence of '/'
        last_slash = strrchr(currDentry->path, '/');

        // If '/' was found, replace it with '\0'
        if (last_slash) {
            *last_slash = '\0';
        }

        currDentry->pastAddresses[currDentry->lastAddressIndex] = 0;
        currDentry->pastAddresses = (uint32_t*)realloc(currDentry->pastAddresses, currDentry->lastAddressIndex * sizeof(uint32_t));
        currDentry->lastAddressIndex--;
    }
    else {
        dentry_t *dentry = encode_dir_entry(fd, offset);
        uint32_t firstCluster = (dentry->DIR_FstClusHI << 16) | dentry->DIR_FstClusLO;
        offset = (RootDirFirstByteOffset - 32) + ((firstCluster - 2) * (bpb->BPB_SecPerClus * bpb->BPB_BytsPerSec));
        for (uint32_t i = 0; i < (bpb->BPB_SecPerClus * bpb->BPB_BytsPerSec / sizeof(dentry_t)); i++) {
            // if (i < 2)
                offset += 32;
            // else
                // offset += 64;
            dentry = encode_dir_entry(fd, offset);
            if (dentry) {
                if (strstr(dentry->DIR_Name, path) != NULL) {
                    currDentry->lastAddressIndex++;
                    currDentry->pastAddresses = (uint32_t*)realloc(currDentry->pastAddresses, (currDentry->lastAddressIndex + 1)* sizeof(uint32_t));
                    currDentry->pastAddresses[currDentry->lastAddressIndex] = currDentry->address;
                    currDentry->address = offset;
                    strcat(currDentry->path, "/");
                    strcat(currDentry->path, dentry->DIR_Name);
                    char *p = strchr(currDentry->path, ' ');
                    if (p != NULL)
                        *p = '\0';
                    break;
                }
            } else {
                // If the directory entry was not read, break the loop
                break;
            }
        }
        free(dentry);
        fclose(fd);
    }
}

    // Read the root directory entries
//     for (uint32_t i = 0; i < (bpb->BPB_SecPerClus * bpb->BPB_BytsPerSec / sizeof(dentry_t)) / 2; i++) {
//         uint32_t offset = currDentry->address + (i * (sizeof(dentry_t) * 2));
//         dentry_t *dentry = encode_dir_entry(fd, offset);
//         if (dentry) {
//             // dbg_print_dentry(dentry);
//             if (strstr(dentry->DIR_Name, path) != NULL) {
//                 currDentry->address = offset;
//                 strcat(currDentry->path, "/");
//                 strcat(currDentry->path, dentry->DIR_Name);
//                 char *p = strchr(currDentry->path, ' ');
//                 if (p != NULL)
//                     *p = '\0';
//                 free(dentry);
//             }
//         } else {
//             // If the directory entry was not read, break the loop
//             break;
//         }
//     }
//     printf("\n");
//     fclose(fd);
// }

void cd (char* pathToGo, currDirect* currDentry, bpb_t* bpb) {
    char* token = strtok(pathToGo, "/");

    while(token != NULL) {
        mov(token, currDentry, bpb);
        token = strtok(NULL, "/");
    }
}