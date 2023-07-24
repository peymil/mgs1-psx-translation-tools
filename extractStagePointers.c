#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t strHexToHex(uint8_t *str, size_t n) {
    for (int i = 0; i < n; ++i) {
        str = (uint8_t*)strtol(str[i], NULL, 16);
    }
}

int main(int argc, char *argv[]) {
    unsigned char *path = argv[1];
    uint8_t frag[7];

    FILE *fp = fopen(path, "r");
    FILE *fRadioPointers = fopen("radio-pointers.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "Can't find STAGER.DIR. Please specify it in first param\n");
        return 1;
    }
    uint8_t bytes[7];
    uint8_t radioPointer[7];
    while (fread(radioPointer, 1, 7, fRadioPointers) != 0) {
        strHexToHex(&radioPointer);
        while (fread(bytes, 1, 7, fp) != 0) {
            if (memcmp(bytes, radioPointer, 7) == 0) {
                printf("%x\n", ftell(fp));
            }
            fseek(fp, -6, SEEK_CUR);
        }
        fseek(radioPointer, 1, SEEK_CUR);
    }
}

