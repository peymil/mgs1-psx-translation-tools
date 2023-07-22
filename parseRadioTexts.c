#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define RDCODE_NULL 0
#define RDCODE_TALK 0x01
#define RDCODE_VOICE 2
#define RDCODE_ANIM 3
#define RDCODE_ADD_CONTACT 4
#define RDCODE_MEMSAVE 5
#define RDCODE_SOUND 6
#define RDCODE_PROMPT 7
#define RDCODE_VARSAVE 8
#define RDCODE_IF 0x10
#define RDCODE_ELSE 0x11
#define RDCODE_ELSEIF 0x12
#define RDCODE_SWITCH 0x20
#define RDCODE_SWITCH_CASE 0x21
#define RDCODE_SWITCH_DEFAULT 0x22
#define RDCODE_RANDSWITCH 0x30
#define RDCODE_RANDSWITCH_CASE 0x31
#define RDCODE_EVAL 0x40
#define RDCODE_SCRIPT 0x80
#define RDCODE_ENDLINE 0xFF


struct ScriptContainer {
    uint16_t radio_freq;
    uint16_t unk1;
    uint16_t face;
    uint16_t unk2;
    uint8_t unk3;
    uint16_t chunksize;
    uint8_t *chunk;
};

void swap_uint16(uint16_t *val) {
    *val = (*val << 8) | (*val >> 8);
}

void replaceMgsNewLine(char* text){
    for (uint i = 0; i < strlen(text) - 3; i++) {
        if (text[i] == 0xffffff80 && text[i + 1] == 0x23 && text[i + 2] == 0xffffff80 && text[i + 3] == 0x4e){
            text[i] = '/';
            text[i + 1] = '/';
            text[i + 2] = '/';
            text[i + 3] = 'n';
        }
    }
}

char *getActor(uint16_t actor) {
    char *actorStr;
    switch (actor) {
        case 0x21ca:
            return "SOLIDSNAKE";
        case 0x33af:
            return "NASTASHARO";
        case 0x3d2c:
            return "DREMMERICH";
        case 0x6588:
            return "COLCAMPBEL";
        case 0x6c22:
            return "MASTMILLER";
        case 0x7982:
            return "CIBORGNINJ";
        case 0x7c90:
            return "NOISYIMAGE";
        case 0x9475:
            return "NAOMIHUNTE";
        case 0x95f2:
            return "MERYLSILVE";
        case 0x962c:
            return "SNIPERWOLF";
        case 0xd78a:
            return "MEILING";
        case 0xfb95:
            return "JIMHOUSEMA";
        default:
            sprintf(actorStr, "%x", actor);
            return actorStr;
    }
}

int parseText(FILE *fp, uint16_t chunkSize) {
    int isEndFound = -1;
    uint16_t offset = 0;
    uint8_t byte;
    while (offset < chunkSize) {
        offset++;
        fread(&byte, 1, 1, fp);
        if (byte == RDCODE_ENDLINE) {
            isEndFound = 1;
        } else if (isEndFound == 1) {
            if (byte == RDCODE_TALK) {
                uint16_t size;
                uint16_t actor;
                fread(&size, 2, 1, fp);
                swap_uint16(&size);
                fread(&actor, 2, 1, fp);
                swap_uint16(&actor);
                fseek(fp, 4, SEEK_CUR);
                uint8_t *text = malloc(size - 8);
                fread(text, 1, size - 8, fp);
                printf("0x%x %s \n", ftell(fp), getActor(actor));
                replaceMgsNewLine(text);
                printf("%s \n%s \n \n", text, text);
                offset = offset + size;
                isEndFound = -1;
                printf("end: %d \n", ftell(fp));
            } else {
                isEndFound = -1;
            }
        }
    }
    fseek(fp, -1, SEEK_CUR);
}

void readContainer(FILE *fp, struct ScriptContainer *container) {
    printf("curr: %d \n", ftell(fp));
    fread(&container->radio_freq, 2, 1, fp);
    swap_uint16(&container->radio_freq);
    fread(&container->unk1, 2, 1, fp);
    fread(&container->face, 2, 1, fp);
    swap_uint16(&container->face);
    fread(&container->face, 2, 1, fp);
    fread(&container->unk3, 1, 1, fp);
    fread(&container->chunksize, 2, 1, fp);
    swap_uint16(&container->chunksize);
    parseText(fp, container->chunksize - 2);
}


int main(int argc, char *argv[]) {
    unsigned char *path = argv[1];
    uint16_t freq = 1;
    int peek = 1;
    uint containerN = 0;

    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        fprintf(stderr, "Can't find RADIO.DAT. Please specify it in first param\n");
        return 1;
    }

//    while (1) {
//        printf("posStart: %ld\n\n", ftell(fp));
//        uint8_t *allocated = malloc(128);
//        fread(allocated, 1, 128, fp);
//        printf("posEnd: %ld\n\n", ftell(fp));
//        sleep(1);
//    }


    while ((peek = fgetc(fp)) != EOF) {
        ungetc(peek, fp);

        fread(&freq, 2, 1, fp);
        fseek(fp, -2, SEEK_CUR);

        swap_uint16(&freq);

        if (freq >= 14000 && freq <= 14300) {
            printf("Container-%d 0x%x \n \n", containerN, ftell(fp));
            struct ScriptContainer *container = malloc(sizeof(struct ScriptContainer));
            readContainer(fp, container);
//            printf("Radio Frequency: %d\n", container->radio_freq);
//            printf("Unknown 1: %d\n", container->unk1);
//            printf("Face: %d\n", container->face);
//            printf("Unknown 2: %d\n", container->unk2);
//            printf("Unknown 3: %d\n", container->unk3);
//            printf("Chunk Size: %d\n", container->chunksize);
//            printf("posEnd: %ld\n\n", ftell(fp));
            printf("--------------------------\n\n");

            containerN++;
        } else {
            uint8_t *bitmap = malloc(36);
            fread(bitmap, 1, 36, fp);
        }


    }

    fclose(fp);
    return 0;
}
