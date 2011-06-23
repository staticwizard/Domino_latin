/**
* The engine that will manage the domino game.
* \file engine.c
* \author W.k.Tse 
* \version 0.1
* \date 1-08-10
*/

#include <stdlib.h>
#include <assert.h>
#include "models/player.h"
#include "models/gamestate.h"
#include "models/domino.h"
#include "models/Board.h"
#include "models/enginestate.h"
#include "models/utilities.h"

//FIXME remember to move out all the printfs
#include <stdio.h>

//see which client we want to use
#define CMD_CLIENT 

//include headers for the cli client
#ifdef CMD_CLIENT
    #include "interface.h"
#endif

//include the headers for the gui client
#ifdef GUI_CLIENT
    #include "interface.h"
#endif

#include <stdbool.h>


//a set, a game
//1 game, will have a few sets. Depending on the score, we will 
//continue with a set until the end of game is reached

//2. Engine will handle the player. For now, all of them are human players numbered 0 - 3

//3. The engine will handle the score. Every team has their score. A domino game has 2 teams

//The engine will handle start of game,
//  a new sets
//  players
//  score
//  create a new game



//global variables to keeping state
EngineState engineState;

bool playerPassed;  //the current player tried to play and passed
bool quitGame = false;

extern GameState gameState;
extern void playerPrint(Player *player);

void initializeEngine() {
    //initialize the engine state
    engineState.gameStarted = false;
    engineState.gameEnded = false;
    engineState.setStarted = false;
    engineState.setEnded = false;
    engineState.setBlocked = false;
    engineState.playing = false;
    
    engineState.currentState = s_readyForStart;
    playerPassed = false;
}

void startGame() {
    assert (engineState.currentState == s_readyForStart);
    
    //do some work
    initialize();
    
    //choose a starting player
    gameState.playerToStart = getRand(4);
    gameState.turn = gameState.playerToStart;
    
    printf ("\nPlayer %d will start the game\n", gameState.playerToStart);
    
    engineState.currentState = s_gameStarted;
    printf ("Game started\n");
}

void restartGame() {
    assert (engineState.currentState == s_gameEnded);
    
    //when to restart a game 
    //do some checks
    
    reInitializeForGame();
    
    printf ("Game restarted\n");
    //update the new state
    engineState.currentState = s_gameStarted;
}

void startSet() {
    assert (engineState.currentState == s_gameStarted);
    
    reInitializeForSet();
    blocksShuffle (gameState.deck);
    blocksDistribute();
    
    printf ("set started\n");
    engineState.currentState = s_setStarted;      
}

void restartSet () {
    assert (engineState.currentState == s_setEnded);
    
    reInitializeForSet();   //clear the board and player hands
    
    //shuffle the blocks again, and distribute to the players
    blocksShuffle (gameState.deck);
    blocksDistribute();
    
    //increment the starting players turn
    gameState.playerToStart += 1;
    gameState.playerToStart %= 4;
   
    printf ("set restarted\n");
    engineState.currentState = s_setStarted; 
}

void setBlocked() {
    assert (engineState.currentState == s_setBlocked);
}

void seeTeamScore() {
        printf ("Team 1: %d points\n", gameState.score[0]);
        printf ("Team 2: %d points\n", gameState.score[1]);
        printf ("\n");
}

void seePlayerHand() {
    //check players turn
    //print players hand
    playerPrint(&gameState.players[ gameState.turn]);
}

void viewBoard() {
    boardPrint (gameState.board);
}

void detectBlockedGame() {
    assert (gameState.board != NULL);
    
    Board *pBoard = gameState.board;
    uint8_t blockFreq1, blockFreq2;
   
    if (boardGetEnd (pBoard, FIRST_END) != START_GAME_BLOCK ) {
        
        blockFreq1 = boardGetBlocksStats (pBoard, boardGetEnd (pBoard, FIRST_END));
        blockFreq2 = boardGetBlocksStats (pBoard, boardGetEnd (pBoard, SECOND_END));
        
        if (blockFreq1 == MAX_POPULATION && blockFreq2 == MAX_POPULATION) {
            engineState.currentState = s_setBlocked;
            printf ("The game is blocked\n");
        }
    }
    /*
    else if (boardGetEnd(pBoard, FIRST_END) == boardGetEnd(pBoard,SECOND_END) ) {
        blockFreq1 = boardGetBlocksStats(pBoard, boardGetEnd(pBoard, FIRST_END));
        
        if (blockFreq1 == MAX_POPULATION) {
            engineState.currentState = s_setBlocked;
        }
    }
    */
}

void preComputeState() {
    //compute the state in which we are in now
    //1. the current player cannot play, play a block move
    //2. the previous player has won the game
    Player *player = &gameState.players[gameState.turn];
    Board *board = gameState.board; 
    bool passed = false;
    bool canPlay = playerCanPlay(player, boardGetEnd(board,FIRST_END),
                                         boardGetEnd(board, SECOND_END));

    //check to see of the player has won
    //if (player->inHand == 1 && canPlay) {
    //    engineState.currentState = s_setEnded;
    //    printf ("Player %d will win the set\n", gameState.turn);
    //}
    if (!canPlay) {
        //the player cannot play, the players is blocked
        //play a pass move
        //increment the turn
        //print a info to tell the user the player has passed
        boardAddBlock (board, &gameState.PASS_BLOCK);
        passed = true;
        
        printf ("Player %d passed\n", gameState.turn);
        
        gameState.turn += 1;
        gameState.turn %= 4;
    }
    playerPassed = passed;
}

void playHand() {
    int n;
    data *block;
    int canPlay; //if the block is playable or not
    bool done = false;
    Player *player = &gameState.players[gameState.turn];
    
    seePlayerHand();
   
    while (!done) {
        printf("Please choose a block to play.\n");
        scanf ("%d", &n);
        //clear the rest of the input
        while (getchar() != '\n');

        if ( n < 0 || n >= player->inHand) {
            printf ("Please enter a number larger or equal to 0 or less than %d\n",          
                    player->inHand);
        }
        else {
            //check to see if the block is playable
            //1. if yes, remove the block from the hand and increment turn modulo 4
            //2. if no, ask for another number
            block = playerGetBlock(player, n);
            canPlay = boardCanPlayBlock (gameState.board, block);
            //canPlay == LEGAL_MOVE
            if (canPlay == true) {
                //the block can be played
                //play the block, remove the block from players hand
                //update the turn in the game state
                
                //detect if we need to choose a side to play the block
                if (boardCanPlayOnEnd(gameState.board, block, FIRST_END) &&
                    boardCanPlayOnEnd(gameState.board, block, SECOND_END)) {
                    
                    int end;//the end where to play the block
                    //we can play on both ends
                    
                    //ask the player where he wants to play the block
                    do {
                        end = getInputNumber ("Please enter where to play (1 or 2): ");
                    }
                    while (end != 1 && end != 2);
                    
                    //set which end to play
                    if (end==1) { boardSetEndToPlay(gameState.board, FIRST_END); }
                    else { boardSetEndToPlay(gameState.board,SECOND_END);}
                    
                    //set to true that we can play on both ends
                    boardSetCanPlayBothEnds (gameState.board, true);                     
                }
                
                boardAddBlock (gameState.board, block);
                playerRemoveBlock (player, n);
                //increment the turn of the player
                gameState.turn += 1;
                gameState.turn %= 4;
                done = true;    //get out of the while loop
                
                printf ("Player played: upper: %d lower: %d\n",block->upper, block->lower);
                
            }
            else {
                printf("The chosen block upper: %d lower: %d cannot be played\n",block->upper,
block->lower);
            }
                
        }
    }
}

/**
* At the end of the set or a blocked game,
* the score will be updated. Depending on the score,
* we will stay in SetEnded or go to gameEnded state.
*/
void updateScore() {
    //* Detect in which state we ended up blocked or setEnded
    //1. figure out which team won
    //2. count the pips of the losing team
    //3. add the score to the losing team
    //4. update the corresponding state
    
    if (engineState.currentState == s_setBlocked) {
        //1.Detect which team has least points
        //1.Detect which team has won
        //*in case of tie, dont update score
        //update the score of the losing team if any
        int32_t team1 = 0;
        int32_t team2 = 0;
        
        team1 += playerCountInHand(&gameState.players[0]);
        team1 += playerCountInHand(&gameState.players[2]);
        
        team2 += playerCountInHand(&gameState.players[1]);
        team2 += playerCountInHand(&gameState.players[3]);
        
        if (team1 == team2) {
            //the game is tied, this set is not counted
            printf ("The set is tied in this blocked game\n");
        }
        else if (team1 < team2) {
            //team 1 won, update the score of team 2
            team2 += getTeamScore (SECOND_TEAM);
            setTeamScore (SECOND_TEAM, team2);
        }
        
        else if (team2 < team1) {
            //team 2 won, update the score of team 1
            team1 += getTeamScore (FIRST_TEAM);
            setTeamScore (FIRST_TEAM, team1);
        }
        
    }
    else if (engineState.currentState == s_setEnded) {
        int32_t previous = gameState.turn-1;
        int32_t points = 0; //the sum of the players who lost the set
        
        //get the correct previous player who played last
        if (previous == -1) { previous = 3; }
        
        if (previous == 0 || previous == 2) {
            //team 1 won
            //get the points of player 1 and 3
            points += playerCountInHand(&gameState.players[1]);
            points += playerCountInHand(&gameState.players[3]);
            
            //update the score for team 2
            points += getTeamScore(SECOND_TEAM);
            setTeamScore(SECOND_TEAM, points);
        }
        else {
            //team 2 won
            //get the points of player 0 and 2
            points += playerCountInHand(&gameState.players[0]);
            points += playerCountInHand(&gameState.players[2]);
            
            //update the score for team 1
            points += getTeamScore(FIRST_TEAM);
            setTeamScore(FIRST_TEAM, points);
        }
        
        
    }
    //detect if we should stay in s_setBlocked, s_setEnded or go to s_gameEnded
    int32_t team1 = getTeamScore(FIRST_TEAM);
    int32_t team2 = getTeamScore(SECOND_TEAM);
    
    if (team1 < END_GAME_SCORE && team2 < END_GAME_SCORE) {
        //the game is not over yet
        engineState.currentState = s_setEnded;
    }
    else if (team1 >= END_GAME_SCORE) {
        printf ("Team 2 won\n");
        engineState.currentState = s_gameEnded;
    }
    else if (team2 >= END_GAME_SCORE) {
        printf ("Team 1 won\n");
        engineState.currentState = s_gameEnded;
    }  
}

/**
 * Doing post computations, will check if the previous played player has won
 * or the game resulted in a blocked state
 */
void postComputeState() {
    
    //precompute the previous player
    int32_t previous = gameState.turn-1;
    if (previous == -1) { previous = 3; }
    
    //Board *pBoard = gameState.board;
    Player *player = &gameState.players[previous];
    
    if (player->inHand == 0) {
        //player has won the game
        engineState.currentState = s_setEnded;
        updateScore();
        printf ("The player %d has won the set\n", previous); 
    }
    else {
        //if the player has not win, we can detect for a blocked game
        detectBlockedGame();
    }
    
}

void showState() {
    seeTeamScore();
    
    if (engineState.setStarted) {
        printf ("The set has started\n");
    }
    else {
        printf ("The set has not started\n");
    }
    
    printf ("Current players is: %d\n", gameState.turn);
}

//=========================================================================
//  Functions related to the state-machine
//  Depending on the state, we have certain functionality
//=========================================================================

void inStateReadyForStart() {
    assert (engineState.currentState == s_readyForStart);
    
    printf ("In state ready to start\n");
    char input;
    do {
        printReadyForStart();
        input = getInputChar();
        switch (input) {
            case 'g': startGame(); break;
            case 'q': quitGame = true; break;
            default : printf ("Please enter a valid option\n"); break;
        }
    } while (input != 'g' && input != 'q');   
}

void inStateGameStarted() {
    assert (engineState.currentState == s_gameStarted);
    
    printf ("In state game started\n");
    char input;
    do {
        printGameStarted();
        input = getInputChar();
        switch (input) {
            case 'r': startSet(); break;
            case 'q': quitGame = true; break;
            default: printf ("Please enter a valid option\n"); break;
        }
    }
    while (input != 'r' && input != 'q');
}

void inStateGameEnded() {
    assert (engineState.currentState == s_gameEnded);
    printf ("In state game ended\n");

    char input;
    do {
        printGameEnded();
        
        //get input and decide what to do
        input = getInputChar();
        switch (input) {
            case 'g': restartGame(); break;
            case 's': seeTeamScore(); break;
            case 'q': quitGame = true; break;
            default: printf ("Please enter a valid option\n"); break;
        }
    }
    while (input != 's' && input != 'r' && input != 'q');
}

void inStateSetStarted() {
    assert (engineState.currentState == s_setStarted);
    
    printf ("In state set started\n");
    
    char input;
    do {
        printf ("\nPlayer %d's turn\n\n",gameState.turn);
        printSetStarted();

        //get an input command from te player
        input = getInputChar();
        
        switch (input) {
            case 's': seeTeamScore(); break;
            case 'b': viewBoard(); break;
            case 'p': {
                preComputeState(); 
                if (!playerPassed) { 
                    playHand();
                    preComputeState();
                }
                break;
            }
            case 'h': seePlayerHand(); break;
            case 'd': showState(); break;
            case 'q': quitGame = true; break;
            default: printf ("Please enter a valid option\n"); break;
        }
    }
    while (input != 's' && input != 'b' && input != 'p' && input != 'h' && 
        input != 'd' && input != 'q');
}

void inStateSetEnded() {
    assert (engineState.currentState == s_setEnded);

    printf ("In state set ended\n");

    char input;
    do {
        printSetEnded();
        
        //get input and decide what to do
        input = getInputChar();
        switch (input) {
            case 's': seeTeamScore(); break;
            case 'r': restartSet(); break;
            case 'q': quitGame = true; break;
            default: printf ("Please enter a valid option\n"); break;
        }
    }
    while (input != 's' && input != 'r' && input != 'q');
}

void inStateSetBlocked() {
    assert (engineState.currentState == s_setBlocked);
    
    printf ("In state set blocked\n");
    updateScore();
}

void engineStart() {

    printf("In the domino game engine\n");
    initializeEngine();

    printInfo();
    while (!quitGame) {
        switch (engineState.currentState) {
                //implement the state-machine described in the documents
            case s_readyForStart: inStateReadyForStart(); break;             
            case s_gameStarted:   inStateGameStarted(); break;
            case s_gameEnded:     inStateGameEnded(); printf ("In state game ended\n"); break;
            case s_setStarted:    inStateSetStarted(); break;             
            case s_setEnded:      inStateSetEnded(); printf ("In state set ended\n"); break;
            case s_setBlocked:    inStateSetBlocked(); break;
            default: break;               
        }
    }
    
    breakdown();
}
