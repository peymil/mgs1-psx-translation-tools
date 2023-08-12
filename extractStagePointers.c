#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t strHexToHex(uint8_t *str, size_t n) {
    uint8_t *ptr = str;
    for (int i = 0; i < n; ++i) {
        str = (uint8_t *) strtol(str, NULL, 16);
    }
}

int main(int argc, char *argv[]) {
    unsigned char *path = argv[1];
    unsigned char bytes_to_find[7] = {0x36, 0xbf, 0x0a, 0x00, 0x0, 0x00, 0x00};
    unsigned char buffer[7];
    int bytes_read;
    int offset = 0;
    int found = 0;


    FILE *fStageDir = fopen("./stage.dir", "r");
    FILE *fRadioPointers = fopen("./radio-pointers.txt", "r");

    if (fStageDir == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    if (fRadioPointers == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }
//    while ((read = getline(&line, &len, fp)) != -1) {

    while (!feof(fStageDir)) {
        bytes_read = fread(buffer, 1, 7, fStageDir);

        if (bytes_read < 7) {
            break; // End of file reached
        }

        int i;
        for (i = 0; i < 7; i++) {
            if (buffer[i] == bytes_to_find[i]) {
                found++;
            } else {
                found = 0; // Reset the found counter if the bytes don't match
                break;
            }
        }

        if (found == 7) {
            printf("Found the 6 specific bytes at offset %d in the file.\n", offset);
            fclose(fStageDir);
            return 0;
        }

        offset++;
        fseek(fStageDir, offset, SEEK_SET);
//        }
    }


    printf("The 6 specific bytes were not found in the file.\n");
    fclose(fStageDir);
    return 0;
}

