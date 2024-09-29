#include "bpb_reader.h"

bpb_t bpbReader() {
    uint16_t BPB_BytsPerSec = 0;
    FILE * fp = fopen("fat32.img", "r");

    // read by structure
    bpb_t bpb;
    fseek(fp, 0, SEEK_SET);
    fread(&bpb, sizeof(bpb_t), 1, fp);
    printf("BPB_BytsPerSec: %u\n", bpb.BPB_BytsPerSec);
    fclose(fp);

    return bpb;
}
