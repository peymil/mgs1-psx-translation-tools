
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
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


int str_length(char str[]) {
    // initializing count variable (stores the length of the string)
    int count;

    // incrementing the count till the end of the string
    for (count = 0; str[count] != '\0'; ++count);

    // returning the character count of the string
    return count;
}

int main(int argc, char *argv[]) {
    unsigned char *path = argv[1];
    uint16_t freq = 1;
    int peek = 1;

    FILE *radioF = fopen("RADIO.DAT", "r+");
    if (radioF == NULL) {
        fprintf(stderr, "Can't find RADIO.DAT. Please specify it in first param\n");
        return 1;
    }

    FILE *stageF = fopen("STAGE.DIR", "r+");
    if (stageF == NULL) {
        fprintf(stderr, "Can't find STAGE.DIR. Please specify it in first param\n");
        return 1;
    }

    FILE *translatedF = fopen("texts.txt", "r+");
    if (translatedF == NULL) {
        fprintf(stderr, "Can't find RADIO.DAT. Please specify it in first param\n");
        return 1;
    }

    FILE *mapF = fopen("", "r+");
    if (mapF == NULL) {
        fprintf(stderr, "Can't find STAGE.DIR. Please specify it in first param\n");
        return 1;
    }
    int globalOffsetDifference = 0;

    char *containerId = malloc(50);
    int containerOffset;
    int textOpcodeOffset;
    int textScriptSize;
    char *speaker = malloc(50);
    char *referenceText = malloc(1024);
    char *translatedText = malloc(1024);

    char *peekByte = malloc(27);

    containerRead:
    if (fscanf(translatedF, "%s %d \n\n", containerId) == EOF) {
        return 1;
    }

    fscanf(translatedF, "%d %d %s\n%s \n%s \n ", textOpcodeOffset, textScriptSize, speaker, referenceText,
           translatedText);


    fseek(radioF, containerOffset + globalOffsetDifference, SEEK_SET);
    int refTranslatedSizeDiffer = str_length(translatedText) - textScriptSize;

    globalOffsetDifference += refTranslatedSizeDiffer;

    fread(peekByte, 1, 27, translatedF);
    if (strcmp(peekByte, "--------------------------") == 0) {
        goto containerRead;
    }
}
