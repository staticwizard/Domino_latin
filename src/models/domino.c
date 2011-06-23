/**
* A game of domino, with players, a board and domino blocks
* \file domino.c
* \author W.k.Tse 
* \version 0.1
* \date 28-04-10
*/

#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "domino.h"
#include "gamestate.h"
#include "../defines.h"
#include "../utilities/utilities.h"

const uint16_t size = (0.5*(MAX_PIP+1)) * (MAX_PIP + 2);    //the max size of the deck


//global variables
GameState gameState;    //the game state

/**
 * Initialize the game and its needed structures
 */
void initialize() {
    
    //initialize the game state
    gameState.score[0] = 0;
    gameState.score[1] = 0;
    gameState.turn = 0;
    gameState.playerToStart = 0;
    
    //create the players 
    int32_t i, j;
    for (i = 0; i < NUM_PLAYERS ;i++) {
        playerCreate (&gameState.players[i] );
    }

    //generate the blocks
    gameState.deck = blocksGenerate();
    printf ("Generated blocks\n");
    
    //initialize the passblock
    gameState.PASS_BLOCK.upper = PASS_MOVE;
    gameState.PASS_BLOCK.lower = PASS_MOVE;
        
    //initialize the board
    gameState.board = malloc (sizeof (Board));  
    boardInitialize (gameState.board);
}

void reInitializeForSet() {
    //1. clear the board
    //2. clear the players hand
    int32_t i;
    
    //clear the board
    boardReInitialize (gameState.board);
    
    //clear the players
    for (i = 0; i < NUM_PLAYERS; i++) {
        playerClearHand (&gameState.players[i]);
    }
}

void reInitializeForGame() {
    //1. reinitialize for a new set
    //2. reset the score
    //reInitializeForSet();
    gameState.score[0] = 0;
    gameState.score[1] = 0;
    
    //increment the next starting player
    gameState.playerToStart += 1;
    gameState.playerToStart %= 4;
}


/**
 * frees up all the allocated memories
 */
void breakdown() {
    free (gameState.deck);
    gameState.deck = NULL;
    
    //free the players hand in the used list
    int32_t i;
    for (i = 0; i< NUM_PLAYERS; i++) {
        playerFreeResource (&gameState.players[i]);
    }
    //free the memory allocated by lists in player 
    
    //free the board
    boardDestroy (gameState.board);
    gameState.board = NULL;
}

Block* blocksGenerate() {
    //we need to generate size blocks
    Block *deck = malloc (sizeof (Block) * size);
    int32_t upper,lower, count;
    count = 0;
    for (upper = 0; upper <= MAX_PIP; upper++) {
        for (lower = upper; lower <= MAX_PIP; lower++) {
            deck[count].upper = upper;
            deck[count++].lower = lower;
        }
    }
    return deck;
}

void blocksPrint(Block *pDeck) {
    assert (pDeck != NULL);
    int32_t i;
    for (i = 0; i < size; i++) {
        printf ("Upper: %d\t lower: %d\n", pDeck[i].upper, pDeck[i].lower);
    }
}

void blocksShuffle(Block *pDeck) {
    assert (pDeck != NULL);
    
    //do a reseed of the random generator();
    reseed();
    
    //shuffle the blocks in the deck based on a random generator
    int32_t i = size-1;   //the size of blocks in the deck
    for (;i > 0; i--) {
        blockSwap (pDeck, i, getRand(i));
    }
}

void blockSwap (Block *pArray, int32_t pA, int32_t pB) {
    assert (pArray != NULL);
    assert (pA >= 0 && pA < size);
    assert (pB >= 0 && pB < size);
    
    Block temp = pArray[pB];
    pArray[pB] = pArray[pA];
    pArray[pA] = temp;
}

void blocksDistribute() {
    assert (gameState.deck != NULL);
    assert (gameState.players != NULL);
    
    int32_t j,i, k;
    j = 0;
    for (i = 0; i < NUM_PLAYERS; i++) {
        for (k = 0; k < BLOCK_PER_PLAYER; k++) {
            playerAddBlock(&gameState.players[i], &gameState.deck[j++]);
        }
    }
}

int32_t getTeamScore (int32_t pTeam) {
    //assert (pBoard != NULL);
    assert (pTeam == FIRST_TEAM || pTeam == SECOND_TEAM);
    
    int32_t score = 0;
    
    if (pTeam == FIRST_TEAM) {
        score = gameState.score[0];
    }
    else if (pTeam == SECOND_TEAM) {
        score = gameState.score[1];
    }
    return score;
}

void setTeamScore (int32_t pTeam, int32_t pScore) {
    //assert (pBoard != NULL);
    assert (pTeam == FIRST_TEAM || pTeam == SECOND_TEAM);
    assert (pScore >= 0);
    
    if (pTeam == FIRST_TEAM) {
        gameState.score[0] = pScore;
    }
    else if (pTeam == SECOND_TEAM) {
        gameState.score[1] = pScore;
    }      
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


