/**
 * @Author: W.k. Tse
 * @Date: 8-3-'11
 * @Description: Implementing the command line interface functions
 */
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "interface.h"


void printInfo() {
    printf ("Welcome to the command-line domino block game\n");
    printf ("This is still a work-in-progress game\n");
    printf ("Author: W.k.Tse\n\n");
}

char getInputChar() {
    char input;
    scanf ("%c", &input);
    while (getchar() != '\n');    //clear the reset
    return input;
}


int32_t getInputNumber(char *pString) {
    assert (pString != NULL);

    int n;    
    printf ("%s", pString);

    //get the input
    scanf("%d", &n);

    //clear till end of line
    while (getchar() != '\n');

    return n;
}

//the index that selects which info to print
void printInstruction (int32_t pIndex) {
    assert (pIndex >= 0);
    
    if (pIndex & 1) {
        printf ("G) Start a new Game\n");
    }
    
    if (pIndex & 2) {
        printf ("S) See team Score\n");
    }
    
    if (pIndex & 4) {
        printf ("B) View the Board\n");
    }
    
    if (pIndex & 8) {
        printf ("P) Play a block\n");
    }
    
    if (pIndex & 16) {
        printf ("H) See the Hand\n");
    }
    
    if (pIndex & 32) {
        printf ("R) start a new set\n");
    }
    
    if (pIndex & 64) {
        printf ("D) see the game state\n");
    }
    
    if (pIndex && 128) {
        printf ("Q) Quit the game\n");
    }
}

void printReadyForStart() {
    printInstruction (1+128);
}

void printGameStarted() {
    printInstruction (32+128); 
}

void printGameEnded() {
    printInstruction (1+2+128);
}

void printSetStarted() {
    printInstruction(2+4+8+16+64+128);
}

void printSetEnded() {
    printInstruction(2+32+128);
}

void printLine(char* pString) {
    assert (pString != NULL);

    printf (pString);
}
