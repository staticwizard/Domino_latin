/**
* Describing the structures and functions that are needed in a domino game
* \file domino.h 
* 
* \author W.k.Tse 
* \version 0.1
* \date 28-04-10
* @todo add a pass move, which will also record the pass on the played move list
*/

#ifndef DOMINO_H
#define DOMINO_H

#include <stdint.h>
//#include "list.h"
//#include "player.h"
//#include "block.h"
//#include "board.h"
#include "../defines.h"

/**
* A block that defines a domino block, we have an upper pip and lower pip
* \struct Block
*/
typedef struct {
    int8_t upper; //<0..6>
    int8_t lower; //<0..6>
} Block;

/**
* A structure defining a player in a domino game
* \struct Player
*/


/**
* Generate a deck of blocks based on the size 
* @return returns a pointer to the created deck
*/
Block* blocksGenerate();

/**
 * Generate the pass blocks for use
 *@return returns a pointer to the created deck of pass moves
 */
Block* blocksPassGenerate();

/**
* Shuffles the deck of blocks with a random generator
* @param pDeck points to the structure with the blocksGenerate
* @pre pDeck point to a structure and is not null
* @post the deck pointed by pDeck is being shuffled
*/
void blocksShuffle(Block *pDeck);

/**
 * A helper function to swap items in the array between A and B
 * @param pArray is a pointed pointing to the array of blocksDestroy
 * @param pA is the position of item A
 * @param pB is the position of item B
 * @pre pArray is not null
 * @pre pA >= 0 and < size
 * @pre pB >= 0 and < size
 * @post the item in position A and B are swappen in the array pointed by pArray
 */
void blockSwap (Block *pArray, int pA, int pB);

/**
* Distrubte the blocks to each player
*/
void blocksDistribute();

/**
 * Destroy the list of blocks generated
 */
void blocksDestroy();

/**
* Prints a deck 
* @param pDeck pDeck is pointing to a deck of blocks with size elements
* @pre pDeck is poiting to a deck of blocks (array) and is not null
* @post the blocks in the array pointed by pDeck is being printed
*/
void blocksPrint(Block *pBlocks);

/**
 * Initialized a game of domino
 * @pre
 * @post A new game of domino will be created
 */
void initialize();

/**
 * Fress all the used resources by the domino game
 * @pre The domino is initialized
 * @post The domino is being freed of all resources
 */
void breakdown();

void testCases();

void testPlayer();

/**
 * Re-initialize the domino board for a new set
 * @pre the board is initialized
 * @post the board will be reinitialized for a new set
 */
void reInitializeForSet();

/**
* Re-initialize the domino board for a new game
* @pre the board is initialized
* @post the board will be reinitialized for a new game
*/
void reInitializeForGame();

/**
 * Get the team score from the board
 * @param pTeam the team to get the score from
 * @pre pTeam is either FIRST_TEAM or SECOND_TEAM
 * @post the score is returned from team pTeam on board pBoard
 */
int32_t getTeamScore (int32_t pTeam);

/**
* Set the team score from the board
* @param pTeam the team to set the score to
* @param pScore the score to set it to
* @pre pTeam is either FIRST_TEAM or SECOND_TEAM
* @pre pScore is >= 0
* @post the score is set for the team
*/
void setTeamScore (int32_t pTeam, int32_t pScore);

#endif
