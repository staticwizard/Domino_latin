/**
 * A header describing a board, this is basically where the played blocks are gathered
 * \file board.h
 * \author W.k.Tse
 * \Date 29-04-10
 * \version 0.1
 */

#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <stdbool.h>
#include "queue.h"
/**
 * A struct to describe a normal domino board of 4 player and 2 ends
 *\struct Board
 */
typedef struct _Board {
    List *unused;           //the list with a number of unused list for filling in the other lists
    
    //some book keeping variables
    
    //Variables for the state of the game, depicting the actual board
    List *headState;    //the head of the domino board {open "left" side of the domino} 
    List *tailState;    //the tail of the domino board {open "right" side of the domino}
    
    //variables to keep the order of the play intact
    //playedInOrder to start at the 1st block played, or we can reverse the lasInOrder
    List *playedInOrder;    //The first block played
    List *lastInOrder;      //the last block played
   
    int8_t items;              //the number of items on the board {not including passes}
    int8_t numPlayed;          //the number of blocks played {these includes passes}
    int8_t openFirst;          //the open ends
    int8_t openSecond;         //the open ends
    
    //board info of the played blocks
    uint8_t onBoard[MAX_PIP+1]; //statistics about the blocks played on board.

    //status for the to be played block
    bool canPlayBothEnds;   //boolean if the current to be played block can be played on boths
                            //ends
    int8_t endToPlay;       //the end to play the current block when canPlayBothEnds is set to
                            //true

    Queue LRChoices;        //the choice the player made, when player wants to play a block that
                            //can be played on both ends
} Board;


/**
 * Initialized a board
 *@param pBoard the board to initialize
 *@pre pBoard the board is not pointing to null
 */
Board* boardInitialize(Board *pBoard);

/**
 * Print the board, the last state is was in and the moves playedInOrder
 * @param pBoard the board that we are going to print
 * @pre pBoard the board is not pointing to null
 */
void boardPrint (Board *pBoard);

/**
 * Returns the the first open end of the game
 *@param pBoard the board we want to get the first end
 *@param indicating which end to get
 *@pre pBoard the board is not pointing to null
 *@post the indicated end is returned
 */
int32_t boardGetEnd (Board *pBoard, int32_t pEnd);


/**
* Add a block to the game
*@param pBoard The board we want to play the block on
*@param pBlock the block we are going to play
*@pre pBoard the board is not pointing to null
*@pre pBlock the block is not null
*@pre pBlock can be played on the board (there is enough free spots left in unused)
*@pre pEnd Either the first end or the second end we want to play on
*/
void boardAddBlock(Board *pBoard, Block *pBlock);

/**
 * Update the onBoard statistics with a block to be added to the playing board
 *@param pBoard the block where the block info is to be added to the onBoard statistics
 *@pre pBoard is not null
 *@post the onBoard info is updated with pBoard info
 */
void updateOnBoard (Board *pBoard, Block *pBlock);

/**
 * Get the blocks stats on the board (how many blocks are played with a certain pip)
 * @param pBoard the board to get the block stats info from
 * @param pPos get the information for block with pip pPos
 * @pre pBoard is not null
 * @pre pPos >= 0 && <= MAX_PIP
 * @post return the block stats with pPos pip on board pBoard
 */
int32_t boardGetBlocksStats(Board* pBoard, int32_t pPos);

/**
 * See if we can play a block on the board
 *@param pBoard the board pointing to the board
 *@param pBlock the block to be played on the board
 *@pre pBoard is not pointing to a null board 
 *@pre pBlock the block to be played is not null
 *@post returns if the move is playable or not
 */
bool boardCanPlayBlock (Board *pBoard, Block *pBlock);

/**
 * See if we can play on the given end
 *@param pBoard the board we want to check the block with
 *@param pBlock the block to check
 *@param pEnd the end we want to check on the board
 *@pre pBoard != null
 *@pre pBlock != null
 *@pre pEnd == FIRST_END or SECOND_END
 *@post returns true iff block can be played on pEnd given pBoard
 */
bool boardCanPlayOnEnd (Board *pBoard, Block *pBlock, int32_t pEnd);


/**
 * Destroy a board, releasing all the resources used on the board
 *@param pBoard the board to destroy
 *@pre pBoard is not null
 */
void boardDestroy(Board *pBoard);

/**
 * reinitializes a created board, all the resources and cleard and put to a brand new state
 *@param pBoard the board to reInitialize
 *@pre pBoard is not null
 *@pre pBoard has already been created
 *@post pBoard is reinitialized
 */
void boardReInitialize (Board *pBoard);

/**
 * Find the open end of the playing block with the closed end given
 * @pre open is not null
 * @pre open is a valid blocks
 * @pre closed is a valid number
 * @post the open end is given, otherwise -1 if no match is found
 */
int32_t boardFindOpenEnd (Block *open, int32_t closed);

 /**
  * Getter for canPlayBothEnds
  * @pre pBoard is not null
  * @return the value  canPlayBothEnds
  */
 bool boardGetCanPlayBothEnds(Board* pBoard);
 
 /**
 * The setter for canPlayBothEnds
 * @pre pBoard is not null
 * @pre pValue is set
 * @post the value of canPlayBothEnds is set to pValue
 */
 void boardSetCanPlayBothEnds(Board* pBoard, bool pValue);
 
 /**
  * getter for end to play
  * @pre pBoard != NULL
  */
 int32_t boardGetEndToPlay(Board* pBoard);
 
 /**
  * setter for endToPlay
  * @pre pBoard is not null
  * @pre pEnd is a valid value
  * @post endToPlay is set to pEnd
  */
 void boardSetEndToPlay (Board* pBoard, int32_t pEnd);
 

 
#endif
