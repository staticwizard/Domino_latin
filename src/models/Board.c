/**
* A board describing a domino board
* \file board.c
* \author W.k.Tse 
* \version 0.1
* \date 28-04-10
*/

#include "Board.h"
#include "../interface.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../defines.h"
#include "queue.h"

//private variables
static char text[1024];  //reserving 1KByte for printing text

Board* boardInitialize (Board *pBoard) {
    assert (pBoard != NULL);
    
    //1. initialize the unused list
    pBoard->unused = NULL;
    pBoard->headState = NULL;
    pBoard->playedInOrder = NULL;
    
    pBoard->lastInOrder = NULL;
    //pBoard->preLastState = NULL;
    pBoard->tailState = NULL;
    
    //get the ammount of players in the game
    //the maximum empty list that we need is the blocks per player
    //times the players in the game - (players-1)
    int max_list =( (NUM_PLAYERS * BLOCK_PER_PLAYER) * 2) + 15; //enough free list for the
    //board order and the played order. +15 for people that got passed. 15 is a very safe
    //estimate
    int i;
    for (i = 0; i < max_list; i++) {
        List *newList = malloc (sizeof (List));
        //add the newly created list to the unsued list
        newList->next = pBoard->unused;
        pBoard->unused = newList;
    }
    //initialize all the other lists
    
    //initialize items to 0
    pBoard->items = 0;
    pBoard->numPlayed = 0;
    pBoard->openFirst = START_GAME_BLOCK;
    pBoard->openSecond = START_GAME_BLOCK;
    
    //initialize the onBoard items
    for (i = 0; i <= MAX_PIP; i++) {
        pBoard->onBoard[i] = 0;
    }

    //initialize the queue info
    queueInitialize (&(pBoard->LRChoices), 10);
    
    //initialize the endToPlay info's
    pBoard->canPlayBothEnds = false;
    pBoard->endToPlay = FIRST_END;
    return pBoard;

   
}
void boardPrint (Board *pBoard) {
    assert (pBoard != NULL);

    //printLine ("---------------------------------------------------------------------\n");
    sprintf (text,"The game has %d moves\n", pBoard->numPlayed);
    printLine (text);
    sprintf (text,"The open ends of the game are: %d\t %d\n", pBoard->openFirst, pBoard->openSecond);
    printLine (text);
    sprintf (text, "The played state is:\n");
    printLine (text);
    int i;
    List *finger = pBoard->headState;;
    for (i = 1; i <= pBoard->items; i++) {
        sprintf (text, "%d: upper: %d\t lower: %d\n",i, finger->block->upper, finger->block->lower);
        printLine (text);
        finger = finger->next;
    }
    
    //print the game in played order
    sprintf (text, "\nThe game in played order: \n");
    printLine (text);
    finger = pBoard->playedInOrder;
    for (i = 1; i <= pBoard->numPlayed; i++) {
        if (finger->block->upper == PASS_MOVE) {
            sprintf (text, "%d. PASSED\n", i);
            printLine (text);
        }
        else {
            sprintf (text, "%d: upper: %d\t lower: %d\n",i, finger->block->upper,finger->block->lower);
            printLine (text);
        }
        finger = finger->next;
    }
    
    //print the onBoard statistics
    sprintf (text, "The onBoard statistics:\n");
    printLine (text);
    for (i = 0; i <= MAX_PIP; i++) {
        sprintf(text,"%d: %d\n", i, pBoard->onBoard[i]);
        printLine (text);
    }
    sprintf (text, "\n");
    printLine (text);
}

int32_t boardGetEnd (Board *pBoard, int32_t pEnd) {
    assert (pBoard != NULL);
    assert (pEnd == FIRST_END || pEnd == SECOND_END);
    
    int openEnd = 0;

    if (pEnd == FIRST_END) {
        openEnd = pBoard->openFirst;
    }
    if (pEnd == SECOND_END) {
        openEnd =  pBoard->openSecond;
    }
    
    return openEnd;
}

int32_t boardFindOpenEnd (Block *open, int closed) {
    //the blocks to find the open one
    //the closedEnd is closed, we need to find the open end on the open block
    assert (open != NULL);
    assert (closed >= 0);
    assert (closed <= MAX_PIP);
    
    int openEnd = -1;
    
    //now that we have the 2 adjacent blocks, we should compare and see
    //which number is the open end
    if (open->upper == PASS_MOVE) {
        openEnd = closed;
    }
    if ( open->upper == closed) {
        openEnd = open->lower;
    }
    else if (open->lower == closed) {
        openEnd = open->upper;
    }
    return openEnd;
}

void boardAddBlock(Board *pBoard, Block *pBlock) {
    assert (pBoard != NULL);
    assert (pBlock != NULL);
    assert (pBoard->unused != NULL);
    assert (boardCanPlayBlock(pBoard, pBlock) == true);//LEGAL_MOVE
    assert (pBoard->items <= ((NUM_PLAYERS * BLOCK_PER_PLAYER) - (NUM_PLAYERS-1)));
    assert (! (pBlock->upper == PASS_MOVE && pBoard->items == 0));

            
    //1. fetch 1 unsued list from the collection
    //2. determine the end it belongs to (first or second in our case)
    //3. add it the the correct end in played state
    //5. update the open ends
    //4. increment items
    
    List *new = pBoard->unused;
    pBoard->unused = new->next;
    new->block = pBlock;
    new->next = NULL;
    
    List *played = pBoard->unused;
    pBoard->unused = played->next;
    played->block = pBlock;
    played->next = NULL;
    
    //determine the end it belongs to
    if (pBoard->items == 0) {
        //there are no items in the list, just add it and update the ends
        pBoard->headState = new;
        pBoard->tailState = new;
        
        //update the played in order state
        pBoard->playedInOrder = played;
        pBoard->lastInOrder = played;
        
        //update the items
        pBoard->items += 1;
        pBoard->numPlayed += 1;
        
        //update the open ends
        pBoard->openFirst = pBlock->upper;
        pBoard->openSecond = pBlock->lower;
    }
    //add a block move
    else if (pBoard->items >0 && pBlock->upper == PASS_MOVE) {
        //add a pass move into the played order
        pBoard->lastInOrder->next = played;
        pBoard->lastInOrder = played;
        pBoard->numPlayed += 1;
        
    }
    else {
        int open;   //the number of the new open end
        //there is 1 or more items in the lists
        
        //we can find the hand, determine the side
        //pBlock->upper == pBoard->openFirst || pBlock->lower == pBoard->openFirst
        
        //pre-calculate the values if both ends can be played on
        bool playFirst = boardGetCanPlayBothEnds(pBoard) &&
            (boardGetEndToPlay(pBoard) == FIRST_END);
        bool playSecond = boardGetCanPlayBothEnds(pBoard) &&
            (boardGetEndToPlay(pBoard) == SECOND_END);
        bool selective = (playFirst || playSecond);//used to tell if we choose to play a specific end
        
        if ( (boardCanPlayOnEnd(pBoard,pBlock, FIRST_END) && !selective) || playFirst) {
            //the block is going to the head
            
            open = boardFindOpenEnd (pBlock, pBoard->openFirst);
            
            //add the block to the played list
            new->next = pBoard->headState;
            pBoard->headState = new;
            //update the open end first
            pBoard->openFirst = open;
        }
        else if ( (boardCanPlayOnEnd(pBoard, pBlock, SECOND_END) && !selective) || playSecond) {
            //the block is going to the back
            open = boardFindOpenEnd (pBlock, pBoard->openSecond);
            //add the block to the game state
            pBoard->tailState->next = new;
            pBoard->tailState = new;
            //update the open end second
            pBoard->openSecond = open;
        }
        
        //update the list InOrder
        pBoard->lastInOrder->next = played;
        pBoard->lastInOrder = played;
        
        //update the items in list
        pBoard->items += 1;
        pBoard->numPlayed += 1;

        //update the queue of LRChoices
        if (playFirst) {
            queuePush(&(pBoard->LRChoices), 'L');
        }
        else if (playSecond) {
            queuePush(&(pBoard->LRChoices), 'R');
        }
        
    }
    //update the onBoard info
    updateOnBoard(pBoard, pBlock);
    
    //set the canPlayBothEnds flag
    boardSetCanPlayBothEnds(pBoard,false);
}

void updateOnBoard(Board *pBoard, Block *pBlock) {
    assert (pBoard != NULL);
    assert (pBlock != NULL);
    
    //the block is not a passmove
    if (pBlock->upper != PASS_MOVE) {
        //a non double is being played
        if (pBlock->upper != pBlock->lower) {
            pBoard->onBoard[pBlock->upper] += 1;
            pBoard->onBoard[pBlock->lower] += 1;
        }
        else { //a double is being played
            pBoard->onBoard[pBlock->upper] += 1;
        }
    }
    //else, the block is a block move, ignore
}

int32_t boardGetBlocksStats(Board* pBoard, int32_t pPos) {
    assert (pBoard != NULL);
    assert ( (pPos >= 0 && pPos <= MAX_PIP) || (pPos == START_GAME_BLOCK));

    
    int stats = 0;
    
    if (pPos == START_GAME_BLOCK) {
        stats = 0;
    }
    else {
        stats = pBoard->onBoard[pPos];
    }
    return stats;
}
 
bool boardCanPlayBlock (Board *pBoard, Block *pBlock) {
    assert (pBoard != NULL);
    assert (pBlock != NULL);
   
    bool able = false;
    
    if (boardGetEnd(pBoard, FIRST_END) == START_GAME_BLOCK) {
        able = true;
    }
    else if (boardCanPlayOnEnd(pBoard,pBlock, FIRST_END) ) {
        able = true;
    }
    else if (boardCanPlayOnEnd(pBoard,pBlock, SECOND_END) ) {
        able = true;
    }
    else if (pBlock->upper == PASS_MOVE) {
        able = true;
    }
    //printLine ("upper: %d lower: %d code: %d\n",pBlock->upper, pBlock->lower, able);
    return able;
}

bool boardCanPlayOnEnd (Board *pBoard, Block *pBlock, int32_t pEnd) {
    assert (pBoard != NULL);
    assert (pBlock != NULL);
    assert (pEnd == FIRST_END || pEnd == SECOND_END);

    bool canPlay = false;
    int32_t openEnd = 0;
    
    if (pEnd == FIRST_END) {
        openEnd = boardGetEnd(pBoard, FIRST_END);
    }
    else if (pEnd == SECOND_END) {
        openEnd = boardGetEnd(pBoard, SECOND_END);
    }
    //else EMPTY

    //do the check here
    canPlay = (openEnd == pBlock->upper || openEnd == pBlock->lower);
    
    return canPlay;
}

bool boardGetCanPlayBothEnds(Board* pBoard) {
    assert (pBoard != NULL);
    return pBoard->canPlayBothEnds;
}

void boardSetCanPlayBothEnds(Board* pBoard, bool pValue) {
    assert (pBoard !=NULL);
    
    pBoard-> canPlayBothEnds = pValue;
}

int32_t boardGetEndToPlay (Board* pBoard) {
    assert (pBoard != NULL);
    return pBoard->endToPlay;
}

void boardSetEndToPlay (Board* pBoard, int32_t pEnd) {
    assert (pBoard != NULL);
    assert (pEnd == FIRST_END || pEnd == SECOND_END);
    
    pBoard->endToPlay = pEnd;
}

void boardDestroy (Board *pBoard) {
    //reclaim all the resources
    assert (pBoard != NULL);
    
    //1. clear the unused list
    //2. clear the playedState list
    //3. clear the playedInOrder lists
    //4. clear the queue list
    List *finger = pBoard->unused;
    List *tmpFinger, *finger2, *tmpFinger2;
    while (finger != NULL) {
        tmpFinger = finger->next;
        free (finger);
        finger = tmpFinger;
    }
    pBoard->unused = NULL;
    
    //clear the played state
    int i;
    finger = pBoard->headState;
    finger2 = pBoard->playedInOrder;
    for (i = 0; i < pBoard->items; i++) {
        tmpFinger = finger->next;
        tmpFinger2 = finger2->next;
        
        free (finger);
        free (finger2);
        
        finger = tmpFinger;
        finger2 = tmpFinger2;
    }
    
    pBoard->headState = NULL;
    pBoard->playedInOrder = NULL;
    
    //free the last things
    pBoard->lastInOrder = NULL;
    //List *preLastState;
    pBoard->tailState = NULL;

    //clear the LRChoices list
    queueDestroy(&(pBoard->LRChoices));
        
    //free the board
    free (pBoard); 
}

void boardReInitialize (Board *pBoard) {
    assert (pBoard != NULL);
    
    //put all the items back in the unused stack
    if (pBoard->tailState != NULL) {
        pBoard->tailState->next = pBoard->playedInOrder;
    }
    if (pBoard->lastInOrder != NULL) {
        pBoard->lastInOrder->next = pBoard->unused;
    }
    
    if ( pBoard->headState != NULL) {
        pBoard->unused = pBoard->headState;
    }
    //the lists are now chained
    //clear all the list beside unused and the ammount of items
    pBoard->headState = NULL;
    pBoard->playedInOrder = NULL;
    pBoard->items = 0;
    pBoard->numPlayed = 0;
        
    pBoard->lastInOrder = NULL;
    pBoard->tailState = NULL;
    
    pBoard->openFirst = START_GAME_BLOCK;
    pBoard->openSecond = START_GAME_BLOCK;
    
    //reset the onboard statistics
    int i;
    for (i = 0; i <= MAX_PIP; i++) {
        pBoard->onBoard[i] = 0;
    }
    
    //canPlayBothEnds
    pBoard->canPlayBothEnds = false;
    pBoard->endToPlay = FIRST_END;

    //reset the LRChoices queue
    pBoard->LRChoices.elements = 0;
}
