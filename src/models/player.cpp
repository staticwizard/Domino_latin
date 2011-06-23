/**
* \file player.c
* \author W.k.Tse 
* \version 0.1
* \date 28-04-10
*/

#include <stdio.h>
#include <sstream>
#include <assert.h>

#include <list>
#include "Domino.h"
#include "Player.h"
#include "../defines.h"


Player::Player() {
	name = "null";
	hand.list();	//create a new list
	inHand = 0;
}

//Player functions
string Player::toString() {

	stringstream buff;
	buff << "The player name: " + getName() + "\t in hand: " + getBlocksInHand();
    buff << "The blocks in hand:\n";

    int i = 0;
    for (list<Block>::iterator ite = hand.begin() ; ite != hand.end(); ite++, i++) {
    	buff << i <<". " << ite->toString();
    }
    
    buff << "The hand statistics: \n";
    for (i = 0; i < MAX_PIP+1; i++) {
    	buff << "For frequency [" << i << "]: " << frequencyHand[i] << "\n";
    }

    buff << "The available hand in player: \n";
    for (i = 0; i < MAX_PIP+1; i++) {
        if (playableHand[i]) { buff << "1"; }
        else { buff << "0"; }
    }
    buff <<"\n";
    return buf.str();
}

void playerFreeResource(Player *pPlayer) {
    assert (pPlayer != NULL);
    
    List *finger = pPlayer->hand;
    List *next;
    
    //free the name
    if (pPlayer->name != NULL) {
        free (pPlayer->name);
    }
    
    //free the hand
    if (finger != NULL) {  
        next = finger->next;
        
        while (finger != NULL) {
            free (finger);
            finger = next;
            
            if (finger != NULL) {
                next = finger->next;
            }
        }
    }
    pPlayer->hand = NULL;
    
    //free the unused list
    finger = pPlayer->unused;
    if (finger != NULL) {         
        next = finger->next;
        while (finger != NULL) {
            free (finger);
            finger = next;
            
            if (finger != NULL) {
                next = finger->next;
            }
        }
    }
    pPlayer->unused = NULL;
    pPlayer->name = NULL;
    
    pPlayer->inHand = 0;
    pPlayer->playableHand = 0;
    
    int i;
    
    for (i = 0; i < MAX_PIP+1; i++) {
        pPlayer->frequencyHand[i] = 0;
    }
    
}

void playerCreate (Player *pPlayer) {
    assert (pPlayer != NULL);
    
    pPlayer->name = NULL;
    pPlayer->inHand = 0;
    pPlayer->hand = NULL;
    pPlayer->unused = NULL;
    
    //creat a list of blocks
    int j;
    List *list;
    for (j = 0; j < BLOCK_PER_PLAYER; j++) {
        list = malloc(sizeof (List));
        list->block = NULL;
        //point to the next list in unused list
        list->next = pPlayer->unused;
        
        //assign it to the player's unused list in the head
        pPlayer->unused = list;
    }
    //clear the hand analysis
    pPlayer->playableHand = 0;
    
    //clear the numbers of playable hand
    for (j = 0; j < MAX_PIP+1;j++) {
        pPlayer->frequencyHand[j] = 0;
    }
}

void playerAddBlock (Player *pPlayer, Block *pBlock) {
//remove an empty list in the unused list and add it to the players hand
    assert (pPlayer != NULL);
    assert (pBlock != NULL);
    assert (pBlock->upper >= 0 && pBlock->upper <= MAX_PIP);
    assert (pBlock->lower >= 0 && pBlock->lower <= MAX_PIP);
    assert (pPlayer->inHand < BLOCK_PER_PLAYER);
    
    if (pPlayer->unused != NULL) {
        List *finger = pPlayer->unused;
        pPlayer->unused = (pPlayer->unused)->next;
        
        //assign the block to the list and add it to the players hand
        finger->block = pBlock;
        //update the hand
        finger->next = pPlayer->hand;
        pPlayer->hand = finger;
        pPlayer->inHand += 1;
        
        //analyse the hand
        playerAnalyzeHand (pPlayer, pBlock, ADD_BLOCK);
    }
    else {
        printf ("Error: unused is NULL\n");
    }
}

void playerRemoveBlock (Player *pPlayer, int n) {
    //remove the block at position n form player pPlayer's hand
    assert (pPlayer != NULL);
    assert ( (n >= 0) && (n < pPlayer->inHand) );
    
    //1. Find the nth elemet in 'hand' of Player
    //2. with previous of nth element, and next of nth element
    
    //3. put the freed list into the unused section
    //4. reduce inHand
    
    List *previous = NULL;
    List *finger = pPlayer->hand;
    int i = 0;
    
    //step 1
    while (i != n) {
        previous = finger;
        finger = finger->next;
        i++;
    }
    
    data *pBlock = finger->block;   //need a reference to update the hand
    //analyse the players hand
    playerAnalyzeHand (pPlayer, pBlock, REMOVE_BLOCK);
    
    //step 2
    //lets see if we remove the nth block
    if (previous == NULL) {
        //n == 0, we need to remove the first element
        pPlayer->hand = finger->next;
    }
    else {
        //nth is not the head, it's somewhere on after the head
        previous->next = finger->next;
    }
    
    finger->block = NULL;
    
    //step 3
    //put the freed list into the unused hand
    finger->next = pPlayer->unused;
    pPlayer->unused = finger;
    
    //step 4
    //decrement inHand
    pPlayer->inHand -= 1;  
}

void playerClearHand (Player *pPlayer) {
    //remove all the blocks in player and put it to the unused hand
    assert (pPlayer != NULL);
    int i;
    List * last = pPlayer->hand;
    
    for (i = 0; i < (pPlayer->inHand)-1; i++) {
        //remove the data from the hand and put it to the unused list
        last->block = NULL;
        last = last->next;
    }
    
    //if hand was not 0 to begin with
    if (last != NULL) {
        last->block = NULL;
        last->next = pPlayer->unused;
        pPlayer->unused = pPlayer->hand;
        pPlayer->hand = NULL;
        pPlayer->inHand = 0;
    }
    //reset the player analysis
    pPlayer->playableHand = 0;
    for (i = 0; i < MAX_PIP+1; i++) {
        pPlayer->frequencyHand[i] = 0;
    }

}

data* playerGetBlock(Player *pPlayer, int n) {
    assert (pPlayer != NULL);
    assert ( (n >= 0) && (n < pPlayer->inHand) );
    
    //get the nth element and return that
    int i = 0;
    List *finger = pPlayer->hand;
    
    while (i < n) {
        finger = finger->next;
        i++;
    }
    return finger->block;
}

int playerCountInHand(Player *pPlayer) {
    assert (pPlayer != NULL);
    assert (pPlayer->inHand >= 0);
    assert (pPlayer->inHand <= BLOCK_PER_PLAYER);
    
    int i,total;
    total = 0;
    List *finger = pPlayer->hand;
    for (i = 0; i < (pPlayer->inHand); i++) {
        total += finger->block->upper + finger->block->lower;
        finger = finger->next;
    }
    return total;
}

int playerCanPlay (Player *pPlayer, int openFirst, int openSecond) {
    assert (pPlayer != NULL);
    assert (pPlayer->hand != NULL);
    
    int canPlay = 0;
    if (openFirst == START_GAME_BLOCK) {
        canPlay = 1;
    }
    else {
        assert (openFirst >=0);
        assert (openSecond >= 0);
        
        assert (openFirst <= MAX_PIP);
        assert (openSecond <= MAX_PIP);
        
        //lets see if we have some playable hand
        if (pPlayer->playableHand & (1<<openFirst)) {
            canPlay = 1;
        }
        else if (pPlayer->playableHand & (1<<openSecond)) {
            canPlay = 1;
        }
    }
    return canPlay;
}

void playerAnalyzeHand (Player *pPlayer, data *pBlock, int mode) {
    assert (pPlayer != NULL);
    assert (pPlayer->hand != NULL);
    assert (pPlayer->inHand >= 0);
    assert (pPlayer->inHand <= BLOCK_PER_PLAYER);
    assert (pBlock != NULL);
    assert (mode == ADD_BLOCK || mode == REMOVE_BLOCK);
    
    if (mode == ADD_BLOCK) {
        //we are in add mode
        int upper, lower;
        upper = pBlock->upper;
        lower = pBlock->lower;
        
        pPlayer->playableHand |= (1 << upper);
        pPlayer->playableHand |= (1 << lower);
        //update the frequency
        pPlayer->frequencyHand[upper] += 1;
        pPlayer->frequencyHand[lower] += 1;
    }
    
    else {
        //we are in remove mode
        int upper, lower;
        upper = pBlock->upper;
        lower = pBlock->lower;
        
        //update the frequency
        pPlayer->frequencyHand[upper] -= 1;
        pPlayer->frequencyHand[lower] -= 1;
        
        if (pPlayer->frequencyHand[upper] == 0) {
            //toggle that bit off
            pPlayer->playableHand &= ~(1 << upper);
        }
        
        if (pPlayer->frequencyHand[lower] == 0) {
            //toggle that bit off
            pPlayer->playableHand &= ~(1 << lower);
        }
    }
}
