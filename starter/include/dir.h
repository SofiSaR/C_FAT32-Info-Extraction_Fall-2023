#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <bpb_reader.h>

typedef struct __attribute__((packed)) currDentry {
    uint32_t address;
    char* path;
    uint32_t* pastAddresses;
    int lastAddressIndex;
} currDirect;

typedef struct __attribute__((packed)) directory_entry {
    char DIR_Name[11];
    uint8_t DIR_Attr;
    char padding_1[8]; // DIR_NTRes, DIR_CrtTimeTenth, DIR_CrtTime, DIR_CrtDate, 
                       // DIR_LstAccDate. Since these fields are not used in
                       // Project 3, just define as a placeholder.
    uint16_t DIR_FstClusHI;
    char padding_2[4]; // DIR_WrtTime, DIR_WrtDate
    uint16_t DIR_FstClusLO;
    uint32_t DIR_FileSize;
} dentry_t;

void dbg_print_dentry(dentry_t *dentry);

dentry_t *encode_dir_entry(FILE * fat32_fd, uint32_t offset);

void dir_main(currDirect, bpb_t*);

uint32_t getFileAddress(currDirect, char*, bpb_t*);

void mov(char*, currDirect*, bpb_t*);

void cd(char*, currDirect*, bpb_t*);

