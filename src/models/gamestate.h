#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "player.h"
#include "Board.h"
#include "domino.h"
#include <stdint.h>

/**
 * A state that describes a game of domino
 * \struct GameState
 */
typedef struct _GameState {
    //global variables
    Player players[NUM_PLAYERS];  //we have a number of players
    data *deck;
    Block PASS_BLOCK;
    Board *board;
    int32_t score[2];  //the score from the teams
    int32_t turn;   //which players turn it is
    int32_t playerToStart;   //the player which will start first at a new set
} GameState;


#endif
