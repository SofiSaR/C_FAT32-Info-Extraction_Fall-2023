#include "dir.h"
#include "bpb_reader.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdbool.h>

// void dbg_print_dentry(dentry_t *dentry) {
//     if (dentry == NULL) {
//         return;
//     }

    // printf("%s   ", dentry->DIR_Name);
    // printf("DIR_Attr: 0x%x\n", dentry->DIR_Attr);
    // printf("DIR_FstClusHI: 0x%x\n", dentry->DIR_FstClusHI);
    // printf("DIR_FstClusLO: 0x%x\n", dentry->DIR_FstClusLO);
    // printf("DIR_FileSize: %u\n", dentry->DIR_FileSize);
// }

dentry_t *encode_dir_entry(FILE *fat32_fd, uint32_t offset) {
    dentry_t *dentry = (dentry_t*)malloc(sizeof(dentry_t));
    fseek(fat32_fd, offset, SEEK_SET);
    if(fread(dentry, sizeof(dentry_t), 1, fat32_fd) != 1){
        printf("Unsuccessful entry grab\n");
        free(dentry);
        return NULL;
    }

    // printf("Successful entry grab\n");
    return dentry;
}

void dir_main(currDirect currDentry, bpb_t* bpb) {
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

    uint32_t offset = currDentry.address;

    if (offset == RootDirFirstByteOffset - 32) {
    // Read the root directory entries
        for (uint32_t i = 0; i < (bpb->BPB_SecPerClus * bpb->BPB_BytsPerSec / sizeof(dentry_t)); i++) {
            uint32_t offset = currDentry.address + (i * (sizeof(dentry_t)));
            dentry_t *dentry = encode_dir_entry(fd, offset);
            if (dentry) {
                if (offset != RootDirFirstByteOffset - 32) {
                    char* name = dentry->DIR_Name;
                    char *p = strchr(name, ' ');
                    if (p != NULL)
                        *p = '\0';
                    // dbg_print_dentry(dentry);
                    if (dentry->DIR_Attr == 1 || dentry->DIR_Attr == 2 || dentry->DIR_Attr == 4 || dentry->DIR_Attr == 16 || dentry->DIR_Attr == 32)
                        printf("%s   ", name);
                    //printf("\nDIR_Attr: 0x%x\n", dentry->DIR_Attr);
                }
                free(dentry);
            } else {
                // If the directory entry was not read, break the loop
                break;
            }
        }
        printf("\n");
    }
    else {
        dentry_t *dentry = encode_dir_entry(fd, offset);
        uint32_t firstCluster = (dentry->DIR_FstClusHI << 16) | dentry->DIR_FstClusLO;
        offset = (RootDirFirstByteOffset - 32) + ((firstCluster - 2) * (bpb->BPB_SecPerClus * bpb->BPB_BytsPerSec));
        for (uint32_t i = 0; i < (bpb->BPB_SecPerClus * bpb->BPB_BytsPerSec / sizeof(dentry_t)); i++) {
            offset += 32;
            dentry = encode_dir_entry(fd, offset);
            if (dentry->DIR_Attr == 1 || dentry->DIR_Attr == 2 || dentry->DIR_Attr == 4 || dentry->DIR_Attr == 8 || dentry->DIR_Attr == 16 || dentry->DIR_Attr == 32) {
                char* name = dentry->DIR_Name;
                char *p = strchr(name, ' ');
                if (p != NULL)
                    *p = '\0';
                printf("%s  ", name);
            }
        }
        printf("\n");
        free(dentry);
        fclose(fd);
    }
}

uint32_t getFileAddress(currDirect currDentry, char* filename, bpb_t* bpb) {
    FILE* fd = fopen("fat32.img", "r");
    if (fd == NULL) {
        perror("open fat32.img failed\n");
        return NULL;
    }

    uint32_t offset = currDentry.address;

    uint32_t FirstDataSector = bpb->BPB_RsvdSecCnt + (bpb->BPB_NumFATs * bpb->BPB_FATSz32);
    uint32_t RootDirFirstSector = FirstDataSector + (bpb->BPB_SecPerClus * (bpb->BPB_RootClus - 2));
    uint32_t RootDirFirstByteOffset = RootDirFirstSector * bpb->BPB_BytsPerSec;

    if (offset == RootDirFirstByteOffset - 32) {
    // Read the root directory entries
        for (uint32_t i = 0; i < (bpb->BPB_SecPerClus * bpb->BPB_BytsPerSec / sizeof(dentry_t)); i++) {
            uint32_t offset = currDentry.address + (i * (sizeof(dentry_t)));
            dentry_t *dentry = encode_dir_entry(fd, offset);
            if (dentry) {
                if (offset != RootDirFirstByteOffset - 32) {
                    // dbg_print_dentry(dentry);
                    if (dentry->DIR_Attr == 32 && strstr(dentry->DIR_Name, filename) != NULL) {
                        free(dentry);
                        return offset;
                    }
                    //printf("\nDIR_Attr: 0x%x\n", dentry->DIR_Attr);
                }
                free(dentry);
            } else {
                // If the directory entry was not read, break the loop
                break;
            }
        }
    }
    else {
        dentry_t *dentry = encode_dir_entry(fd, offset);
        uint32_t firstCluster = (dentry->DIR_FstClusHI << 16) | dentry->DIR_FstClusLO;
        offset = (RootDirFirstByteOffset - 32) + ((firstCluster - 2) * (bpb->BPB_SecPerClus * bpb->BPB_BytsPerSec));
        for (uint32_t i = 0; i < (bpb->BPB_SecPerClus * bpb->BPB_BytsPerSec / sizeof(dentry_t)); i++) {
            offset += 32;
            dentry = encode_dir_entry(fd, offset);
            if (dentry->DIR_Attr == 32 && strstr(filename, dentry->DIR_Name) != NULL) {
                free(dentry);
                return offset;
            }
        }
        free(dentry);
        fclose(fd);
    }
    return NULL;
}
