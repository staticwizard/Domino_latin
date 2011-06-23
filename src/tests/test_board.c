#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../headers/defines.h"
#include "../headers/board.h"
#include <check.h>



START_TEST (testInitializeBoard) {
    printf ("Running tests for initialize board\n");
    
    Board *aBoard = malloc (sizeof (Board));
    boardInitialize (aBoard);
    
    fail_unless (aBoard->unused != NULL, NULL);
    fail_unless (aBoard->headState == NULL, NULL);
    fail_unless (aBoard->playedInOrder == NULL, NULL);
    fail_unless (aBoard->lastInOrder == NULL, NULL);
    fail_unless (aBoard->tailState == NULL, NULL);
    fail_unless (aBoard->items == 0);
    fail_unless (aBoard->numPlayed == 0);
    fail_unless (aBoard->openFirst == START_GAME_BLOCK, NULL);
    fail_unless (aBoard->openSecond == START_GAME_BLOCK, NULL);
    
    //fail_unless that we have enough unused lists
    int i, max;
    i = 0;
    max = ( (NUM_PLAYERS * BLOCK_PER_PLAYER)*2 ) + 15;
    List *finger = aBoard->unused;
    while (finger != NULL) {
        finger = finger->next;
        i++;
    }
    fail_unless (i == max, NULL);
        
    //destroy board
    boardDestroy (aBoard);
    
    printf ("Running tests for initialize board succesfull\n\n");
}
END_TEST

START_TEST (testPrintBoard) {
    printf ("Running tests print board\n");
    
    Board *aBoard = malloc (sizeof (Board));
    boardInitialize (aBoard);
    boardPrint (aBoard);
    
    //add some blocks and print them
    Block aBlock;
    Block bBlock;
    Block cBlock;
    Block dBlock;
    
    //ass a pass move
    Block eBlock;
    
    //make block 1
    aBlock.upper = 1;
    aBlock.lower = 6;
    
    //make block 2
    bBlock.upper = 6;
    bBlock.lower = 6;
    
    //make block 3
    cBlock.upper = 6;
    cBlock.lower = 4;
    
    //make block 4
    dBlock.upper = 2;
    dBlock.lower = 1;
    
    //make pass block
    eBlock.upper = PASS_MOVE;
    eBlock.lower = PASS_MOVE;
    
    //add block 1 and print it
    boardAddBlock (aBoard, &aBlock);
    boardPrint (aBoard);
    
    //add block 2 and print it
    boardAddBlock (aBoard, &bBlock);
    boardPrint (aBoard);
    
    //add block 3 and print it
    boardAddBlock (aBoard, &cBlock);
    boardPrint (aBoard);
    
    //add the pass block
    boardAddBlock (aBoard, &eBlock);
    boardPrint (aBoard);
    
    //add block 4 and print it
    boardAddBlock (aBoard, &dBlock);
    boardPrint (aBoard);
    
    boardDestroy (aBoard);
   
    //printf ("Running tests for board succesfull\n\n");
}
END_TEST

START_TEST (testGetFirstEnd) {
    //printf ("Running tests get open ends\n");
    
    Board *aBoard = malloc (sizeof (Board));
    boardInitialize (aBoard);

    //add some blocks and print them
    Block aBlock;
    Block bBlock;
    Block cBlock;
    
    //make block 1
    aBlock.upper = 1;
    aBlock.lower = 6;
    
    //make block 2
    bBlock.upper = 6;
    bBlock.lower = 6;
    
    //make block 3
    cBlock.upper = 6;
    cBlock.lower = 4;
    
    int openEnd = boardGetEnd(aBoard,FIRST_END);
    int openEnd2 = boardGetEnd(aBoard, SECOND_END);
    fail_unless (openEnd == START_GAME_BLOCK, NULL);
    fail_unless (openEnd2 = START_GAME_BLOCK, NULL);
    
    //add block 1 and fail_unless it
    boardAddBlock (aBoard, &aBlock);
    openEnd = boardGetEnd(aBoard,FIRST_END);
    openEnd2 = boardGetEnd(aBoard, SECOND_END);
    fail_unless (openEnd == 1, NULL);
    fail_unless (openEnd2 = 6, NULL);
    
    
    
    
    //add block 2 and fail_unless it
    boardAddBlock (aBoard, &bBlock);
    openEnd = boardGetEnd(aBoard,FIRST_END);
    openEnd2 = boardGetEnd(aBoard, SECOND_END);
    fail_unless (openEnd == 1, NULL);
    fail_unless (openEnd2 = 6, NULL);
    
    //add block 3 and fail_unless it
    boardAddBlock (aBoard, &cBlock);
    openEnd = boardGetEnd(aBoard,FIRST_END);
    openEnd2 = boardGetEnd(aBoard, SECOND_END);
    fail_unless (openEnd == 1, NULL);
    fail_unless (openEnd2 = 4, NULL);
     
    boardDestroy (aBoard);
    
   // printf ("Running tests for board succesfull\n\n");
}
END_TEST

START_TEST (testLastState) {
    //printf ("Running tests get open ends\n");
    
    Board *aBoard = malloc (sizeof (Board));
    boardInitialize (aBoard);
    
    //add some blocks and print them
    Block aBlock;
    Block bBlock;
    Block cBlock;
    Block dBlock;
    
    //make block 1
    aBlock.upper = 1;
    aBlock.lower = 6;
    
    //make block 2
    bBlock.upper = 6;
    bBlock.lower = 6;
    
    //make block 3
    cBlock.upper = 6;
    cBlock.lower = 4;
    
    //make block 4
    dBlock.upper = 2;
    dBlock.lower = 1;
    
    Block *reference, *reference2;
    
    int openEnd = boardGetEnd(aBoard,FIRST_END);
    int openEnd2 = boardGetEnd(aBoard, SECOND_END);
    fail_unless (openEnd == START_GAME_BLOCK, NULL);
    fail_unless (openEnd2 = START_GAME_BLOCK, NULL);
    
    //add block 1 and fail_unless it
    boardAddBlock (aBoard, &aBlock);
    openEnd = boardGetEnd(aBoard,FIRST_END);
    openEnd2 = boardGetEnd(aBoard, SECOND_END);
    fail_unless (openEnd == 1, NULL);
    fail_unless (openEnd2 = 6, NULL);
   
    //update the reference
    reference = aBoard->tailState->block;
    reference2 = aBoard->lastInOrder->block;
    
    fail_unless (reference->upper == 1, "upper is not correct");
    fail_unless (reference->lower == 6, "upper is not correct");
    
    
    fail_unless (reference2->upper == 1, "upper is not correct");
    fail_unless (reference2->lower == 6, "upper is not correct");
   

    //add block 2 and fail_unless it
    boardAddBlock (aBoard, &bBlock);
    openEnd = boardGetEnd(aBoard,FIRST_END);
    openEnd2 = boardGetEnd(aBoard, SECOND_END);
    fail_unless (openEnd == 1, NULL);
    fail_unless (openEnd2 = 6, NULL);
    
    reference = aBoard->tailState->block;
    reference2 = aBoard->lastInOrder->block;
    
    fail_unless (reference->upper == 6, "upper is not correct");
    fail_unless (reference->lower == 6, "upper is not correct");
    
    
    fail_unless (reference2->upper == 6, "upper is not correct");
    fail_unless (reference2->lower == 6, "upper is not correct");
    
    //add block 3 and fail_unless it
    boardAddBlock (aBoard, &cBlock);
    openEnd = boardGetEnd(aBoard,FIRST_END);
    openEnd2 = boardGetEnd(aBoard, SECOND_END);
    fail_unless (openEnd == 1, NULL);
    fail_unless (openEnd2 = 4, NULL);
    
    reference = aBoard->tailState->block;
    reference2 = aBoard->lastInOrder->block;
    
    fail_unless (reference->upper == 6, "upper is not correct");
    fail_unless (reference->lower == 4, "upper is not correct");
    
    
    fail_unless (reference2->upper == 6, "upper is not correct");
    fail_unless (reference2->lower == 4, "upper is not correct");
    
    
    //add block 4 and fail_unless it
    boardAddBlock (aBoard, &dBlock);
    openEnd = boardGetEnd(aBoard,FIRST_END);
    openEnd2 = boardGetEnd(aBoard, SECOND_END);
    fail_unless (openEnd == 2, NULL);
    fail_unless (openEnd2 = 4, NULL);
    
    reference = aBoard->tailState->block;
    reference2 = aBoard->lastInOrder->block;
    
    fail_unless (reference->upper == 6, "upper is not correct");
    fail_unless (reference->lower == 4, "upper is not correct");
    
    
    fail_unless (reference2->upper == 2, "upper is not correct");
    fail_unless (reference2->lower == 1, "upper is not correct");
    
    reference = aBoard->headState->block;
    fail_unless (reference->upper == 2, "upper is not correct");
    fail_unless (reference->lower == 1, "upper is not correct");
    
    boardDestroy (aBoard);
    
    
    //check the first state
    
    // printf ("Running tests for board succesfull\n\n");
}
END_TEST

START_TEST (testFindOpenEnd) {
    //test both sides and fault data
    //printf ("Running tests find open end\n");
    
    //add some blocks and print them
    Block aBlock;

    
    //make block 1
    aBlock.upper = 1;
    aBlock.lower = 6;
    
    int open = boardFindOpenEnd (&aBlock, 1);
    fail_unless (open == 6, NULL);
    
    open = boardFindOpenEnd (&aBlock, 6);
    fail_unless (open == 1, NULL);
    
    open = boardFindOpenEnd (&aBlock, 5);
    fail_unless (open == -1, NULL);
    
   // printf ("Running tests for board succesfull\n\n");
}
END_TEST

START_TEST (testCanPlayBlock) {
    
    //printf ("Running tests for can play block\n");
    
    Board *aBoard = malloc (sizeof (Board));
    boardInitialize (aBoard);
    //boardPrint (aBoard);
    
    //add some blocks and print them
    Block aBlock;
    Block bBlock;
    Block cBlock;
    Block dBlock;
    Block eBlock;
    Block fBlock;
    
    //make block 1
    aBlock.upper = 1;
    aBlock.lower = 6;
    
    //make block 2
    bBlock.upper = 6;
    bBlock.lower = 6;
    
    //make block 3
    cBlock.upper = 5;
    cBlock.lower = 4;
    
    dBlock.upper = PASS_MOVE;
    dBlock.lower = 0;
    
    eBlock.upper = -1;
    eBlock.lower = -1;
    
    fBlock.upper = MAX_PIP+1;
    fBlock.lower = MAX_PIP+1;
    
  
    int able;
    
    //see if we are able to play a block game
    able = boardCanPlayBlock (aBoard, &aBlock);
    fail_unless (able == true, NULL);
    boardAddBlock (aBoard, &aBlock);
    
    able = boardCanPlayBlock (aBoard, &bBlock);
    fail_unless (able == true, NULL);
    boardAddBlock (aBoard, &bBlock);
    
    able = boardCanPlayBlock (aBoard, &cBlock);
    fail_unless (able == false, NULL);
    
    able = boardCanPlayBlock (aBoard, &dBlock);
    fail_unless (able == true, NULL);
    
    able = boardCanPlayBlock (aBoard, &eBlock);
    fail_unless (able == false, NULL);
    
    able = boardCanPlayBlock (aBoard, &fBlock);
    fail_unless (able == false, NULL);
    
    boardDestroy (aBoard);
    
    //printf ("Running tests for board succesfull\n\n"); 
}
END_TEST

START_TEST (testDestroyBoard) {
    //printf ("Running tests for destroy board\n");
    
    Board *aBoard = malloc (sizeof (Board));
    
    boardInitialize (aBoard);
    
    Block aBlock;
    aBlock.upper = 6;
    aBlock.lower = 6;
    
    boardAddBlock (aBoard, &aBlock);
    boardDestroy (aBoard);
    
    /*
    fail_unless (aBoard->unused = NULL);
    fail_unless (aBoard->headState = NULL);
    fail_unless (aBoard->playedInOrder = NULL);
    fail_unless (aBoard->lastInOrder = NULL);
    fail_unless (aBoard->tailState = NULL);
    */
    //printf ("Running tests for destroy board succesfull\n\n");
}
END_TEST

START_TEST (testReinitializeBoard) {
    
    //initialize a board
    Board *aBoard = malloc (sizeof (Board));
    boardInitialize (aBoard);
    
    Block aBlock;
    Block bBlock;
    Block cBlock;
    Block dBlock;
    
    //make block 1
    aBlock.upper = 1;
    aBlock.lower = 6;
    
    //make block 2
    bBlock.upper = 6;
    bBlock.lower = 6;
    
    //make block 3
    cBlock.upper = 6;
    cBlock.lower = 4;
    
    //make block 4
    dBlock.upper = 2;
    dBlock.lower = 1;
    
    boardAddBlock (aBoard, &aBlock);
    boardAddBlock (aBoard, &bBlock);
    boardAddBlock (aBoard, &cBlock);
    boardAddBlock (aBoard, &dBlock);
    
    boardReInitialize (aBoard);
    
    //test for start values
    fail_unless (aBoard->items == 0, "failed reinit test");
    fail_unless (aBoard->numPlayed == 0, "failed reinit test");
    fail_unless (aBoard->openFirst == START_GAME_BLOCK, "failed reinit test");
    fail_unless (aBoard->openSecond == START_GAME_BLOCK, "failed reinit test");
    fail_unless (aBoard->tailState == NULL, "failed reinit test");
    fail_unless (aBoard->lastInOrder == NULL, "failed reinit test");
    fail_unless (aBoard->playedInOrder == NULL, "failed reinit test");
    fail_unless (aBoard->headState == NULL, "failed reinit test");
    fail_unless (aBoard->unused != NULL, "failed reinit test");
    
    int i,max;
    i = 0;
    max = ( (NUM_PLAYERS * BLOCK_PER_PLAYER) *2 ) + 15;
    List *finger = aBoard->unused;
    while (finger != NULL) {
        finger = finger->next;
        i++;
    }
    fail_unless (i == max, NULL);
    
    boardDestroy (aBoard);
    
    //add some blocks 
}
END_TEST
//testing getters and setters for board{Set/Get}CanPlayBothEnds(Board, Bool);

START_TEST(testBoardSetCanPlayBothEnds) {
    
    Board *aBoard = malloc (sizeof(Board));
    boardInitialize (aBoard);
    
    bool canPlay = boardGetCanPlayBothEnds (aBoard);
    
    fail_unless (canPlay == false, "initial value of canPlayBothEnds failed\n");
    
    boardSetCanPlayBothEnds (aBoard, true);
    canPlay = boardGetCanPlayBothEnds (aBoard);
    
    fail_unless (canPlay == true, "setting value of canPlayBothEnds to true failed\n");
    
    boardSetCanPlayBothEnds (aBoard, false);
    canPlay = boardGetCanPlayBothEnds (aBoard);
    
    fail_unless (canPlay == false, "setting value of canPlayBothEnds to false failed\n");
    
    //clear the board
    boardDestroy (aBoard);
}
END_TEST

START_TEST(testBoardGetCanPlayBothEnds) {
    //fail ("implement me\n");
}
END_TEST

//testing getters and setters for board{set/Get}EndToPlay
START_TEST(testBoardSetEndToPlay) {
    Board *aBoard = malloc (sizeof(Board));
    boardInitialize (aBoard);
    
    //get the initial value
    int end = boardGetEndToPlay (aBoard); 
    
    fail_unless(end == FIRST_END, "endToPlay failed initial value test\n");
    
    //testing the setters
    boardSetEndToPlay (aBoard, FIRST_END);
    end = boardGetEndToPlay (aBoard);
    fail_unless(end == FIRST_END, "endToPlay failed setter to FIRST_END value\n");
    
    //testing the setters
    boardSetEndToPlay (aBoard, SECOND_END);
    end = boardGetEndToPlay (aBoard);
    fail_unless(end == SECOND_END, "endToPlay failed setter to SECOND_END value\n");
    
    //testing the setters
    boardSetEndToPlay (aBoard, FIRST_END);
    end = boardGetEndToPlay (aBoard);
    fail_unless(end == FIRST_END, "endToPlay failed setter to FIRST_END value\n");
    
    //release the resources
    boardDestroy (aBoard);
}
END_TEST

START_TEST(testBoardGetEndToPlay) {
    //fail ("implement me\n");
}
END_TEST

START_TEST (testCanPlayBothEnds) {
    Board *aBoard = malloc(sizeof(Board));
    boardInitialize (aBoard);

    Block aBlock, bBlock, cBlock, dBlock;
    aBlock.upper = 5;
    aBlock.lower = 5;

    bBlock.upper = 5;
    bBlock.lower = 1;

    cBlock.upper = 5;
    cBlock.lower = 6;

    dBlock.upper = 6;
    dBlock.lower = 1;

    bool bothEnds = false;

    bothEnds = boardCanPlayOnEnd(aBoard, &aBlock, FIRST_END);
    fail_unless (bothEnds == false,NULL);
    bothEnds = boardCanPlayOnEnd(aBoard, &aBlock,SECOND_END);
    fail_unless (bothEnds == false, NULL);

    //adding a block
    boardAddBlock(aBoard, &aBlock);

    int ends = -1;
    
    ends = boardGetEnd (aBoard, FIRST_END);
    fail_unless(ends == 5, NULL);
    ends = boardGetEnd (aBoard, SECOND_END);
    fail_unless(ends == 5, NULL);

    bothEnds = boardCanPlayOnEnd(aBoard, &bBlock, FIRST_END);
    fail_unless (bothEnds == true,NULL);
    bothEnds = boardCanPlayOnEnd(aBoard, &bBlock,SECOND_END);
    fail_unless (bothEnds == true, NULL);

    //adding a bBlock to second end
    boardSetEndToPlay (aBoard, SECOND_END);
    boardSetCanPlayBothEnds(aBoard, true);

    boardAddBlock (aBoard, &bBlock);
    //assert some stuff
    fail_unless (boardGetCanPlayBothEnds(aBoard) == false, NULL);
    fail_unless (boardGetEnd(aBoard,FIRST_END) == 5, NULL);
    fail_unless (boardGetEnd(aBoard, SECOND_END) == 1, NULL);

    //add another block
    boardAddBlock (aBoard, &cBlock);
    fail_unless (boardGetEnd(aBoard,FIRST_END) == 6, NULL);
    fail_unless (boardGetEnd(aBoard, SECOND_END) == 1, NULL);

    //add the last block
    bothEnds = boardCanPlayOnEnd (aBoard, &dBlock,FIRST_END);
    fail_unless (bothEnds == true, NULL);

    bothEnds = boardCanPlayOnEnd (aBoard, &dBlock,SECOND_END);
    fail_unless (bothEnds == true, NULL);

    boardSetEndToPlay(aBoard, FIRST_END);
    boardSetCanPlayBothEnds(aBoard, true);
    boardAddBlock (aBoard, &dBlock);

    fail_unless (boardGetCanPlayBothEnds(aBoard) == false, NULL);
    fail_unless (boardGetEnd(aBoard,FIRST_END) == 1, NULL);
    fail_unless (boardGetEnd(aBoard, SECOND_END) == 1, NULL);
        
    boardDestroy (aBoard);
}
END_TEST

START_TEST (testLRChoices) 
{

    Board *aBoard = malloc(sizeof(Board));
    boardInitialize (aBoard);

    Block aBlock, bBlock, cBlock, dBlock, eBlock, fBlock;
    aBlock.upper = 5;
    aBlock.lower = 5;

    bBlock.upper = 5;
    bBlock.lower = 3;

    cBlock.upper = 5;
    cBlock.lower = 4;

    dBlock.upper = 3;
    dBlock.lower = 6;

    eBlock.upper = 4;
    eBlock.lower = 6;

    fBlock.upper = 6;
    fBlock.lower = 6;
    

    bool bothEnds = true;

    //add all the block from a to e
    boardAddBlock (aBoard, &aBlock);
    boardAddBlock (aBoard, &bBlock);
    boardAddBlock (aBoard, &cBlock);
    boardAddBlock (aBoard, &dBlock);

    //add it to the first end
    bothEnds &= boardCanPlayOnEnd (aBoard, &eBlock, FIRST_END);
    bothEnds &= boardCanPlayOnEnd (aBoard, &eBlock, SECOND_END);
    fail_unless (bothEnds == true, "Can play both ends failed 1");
    
    boardSetEndToPlay (aBoard, SECOND_END);
    boardSetCanPlayBothEnds (aBoard, true);
    
    boardAddBlock (aBoard, &eBlock);

    int firstEnd = boardGetEnd (aBoard, FIRST_END);
    int secondEnd = boardGetEnd (aBoard, SECOND_END);
    //printf ("first: %d\t second: %d\n", firstEnd, secondEnd);
    fail_unless (firstEnd == 6 && secondEnd == 6, "Failed eblock add");

     //test the LRChoices queue for changes
    Queue *q = &(aBoard->LRChoices);
    fail_unless (queueElements(q) == 1, "Queue elements failed test");
    fail_unless (q->queue[0] == 'R', "Failed char 'L' test in addBlock");
    
 
    //add the fBlock
    bothEnds &= boardCanPlayOnEnd (aBoard, &fBlock, FIRST_END);
    bothEnds &= boardCanPlayOnEnd (aBoard, &fBlock, SECOND_END);
    
    boardSetEndToPlay (aBoard, FIRST_END);
    boardSetCanPlayBothEnds (aBoard, true);
    boardAddBlock (aBoard, &fBlock);

    //test the LRChoices queue for changes
    fail_unless (queueElements(q) == 2, "Queue elements failed test");
    fail_unless (q->queue[1] == 'L', "Failed char 'L' test in addBlock");
    
    boardDestroy (aBoard);
}
END_TEST



Suite *boardSuite() {
    
    Suite *s = suite_create ("Board");
    TCase *tc = tcase_create ("board_core");
    
    //add the test cases
    tcase_add_test (tc, testInitializeBoard);
    tcase_add_test (tc, testPrintBoard);
    tcase_add_test (tc, testGetFirstEnd);
    tcase_add_test (tc, testFindOpenEnd);
    tcase_add_test (tc, testCanPlayBlock);
    tcase_add_test (tc, testDestroyBoard);
    tcase_add_test (tc, testReinitializeBoard);
    tcase_add_test (tc, testLastState);

    tcase_add_test (tc, testLRChoices);
        
    tcase_add_test (tc, testBoardSetCanPlayBothEnds);
    tcase_add_test (tc, testBoardGetCanPlayBothEnds);
    
    tcase_add_test (tc, testBoardSetEndToPlay);
    tcase_add_test (tc, testBoardGetEndToPlay);
    tcase_add_test (tc, testCanPlayBothEnds);
    
    suite_add_tcase (s, tc);
    return s;
}
