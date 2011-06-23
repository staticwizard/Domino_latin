/**
* header describing a player and its functions
* \file player.h 
* \author W.k.Tse
* \version 0.1
* \date 29-04-10
*/

#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include <list>
#include "Block.h"
#include "../defines.h"

class Player {
public:
	enum AnalyzeMode { addBlock, removeBlock };

	Player();
	/**
	* print a player info
	* @param pPlayer the player pointer pointing to a player structure
	* @pre pPlayer is not null
	* @post this functions has printed the player info
	*/
	string toString();

	/**
	 * Add a block to a players hand
	 * @param pBlock the block to add to the players hand
	 * @pre	pBlock is not null
	 * @post pBLock is added to the players hand
	 */
	void addBlock(Block *pBlock);

	/**
	 * Remove a block from a players hand
	 * @param n The position at which the block is located
	 * @pre n < inHand of pPlayer
	 * @post the block located at position n is removed from player hand
	 */
	void removeBlock (int n);

	/**
	 * Clears a players hand
	 */
	void clearHand();

	/**
	 * Get data from the nth item without removing it from the list
	 * @param n the nth element that we want to peek at
	 * @pre n >= 0 and n < pPlayer->inHand
	 */
	Block* getBlock (int n);

	/**
	 * Count the total number of pips in a players hand
	 *@param pPlayer the player to count the pips
	 *@pre pPlayer is pointing to a valid player
	 *@pre pPlayer has 0 or more in his/her hand and less than the maximum
	 *@post 0 or more is returned
	 */
	int countInHand(Player *pPlayer);

	/**
	 * Get the number of blocks in a players hand
	 * @return The number of blocks in a players hand
	 */
	int getBlocksInHand();

	/**
	 * Returns true if the player has a move to play depending on the open ends
	 *@param openFirst the number of the first open endif
	 *@param openSecond the number of the second open end
	 *@pre openFirst is a valid number between 0 and MAX_PIP
	 *@pre openSecond is a valid number between 0 and MAX_PIP
	 *@post return true iff the player has a playable block
	 */
	bool canPlay (int openFirst, int openSecond);

	string getName();
	void setName (string *pName);

private:
    string name;		//name of the player
    list<Block>   hand; 		//the hand of the player (filled with blocks)
    bool   playableHand[MAX_PIP+1]; //the playable hand in the players hand
    int    frequencyHand[MAX_PIP+1];	//the frequency of the players hand

	/**
	 * Analyzes the hand of a player and updates it accordingly
	 *@param pPlayer the player to analyze the hand of
	 *@pre pPlayer is not null
	 *@pre pPlayer has a hand
	 *@post pPlayers hand is analyzed
	 */
	void playerAnalyzeHand (Block *pBlock, AnalyzeMode pMode);

};



#endif
