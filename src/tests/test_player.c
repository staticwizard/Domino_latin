#include <stdio.h>
#include <stdlib.h>
#include "../headers/player.h"
#include "../headers/defines.h"
#include <check.h>



START_TEST (testPlayerCreate) {
    Player aPlayer;
    
    playerCreate (&aPlayer);
    
    fail_unless (aPlayer.name == NULL, "player name creation failed");
    fail_unless (aPlayer.hand == NULL, "player hand creation failed");
    fail_unless (aPlayer.unused != NULL, "player unused creation failed");
    
    //test for BLOCK_PER_PLAYER unused blocks
    int j,i = 0;
    List *finger = aPlayer.unused;
    while (finger != NULL) {
        i++;
        finger = finger->next;
    }
    fail_unless (i == BLOCK_PER_PLAYER, "Unused is not equal to BLOCK_PER_PLAYER");
    fail_unless (aPlayer.inHand == 0, "started with more than 0 block in hand");
    
    fail_unless (aPlayer.playableHand == 0, "started with a playable hand");
    
    for (j = 0; j < MAX_PIP+1; j++) {
        fail_unless (aPlayer.frequencyHand[j] == 0, "started with a frequency hand that is not 0");
    }
    playerFreeResource (&aPlayer);
}
END_TEST


START_TEST  (testPlayerAddBlock) {
    //printf ("testing player add block\n");
    
    //create a block and add it to the playerCreate
    Block aBlock;
    Player aPlayer;
    playerCreate (&aPlayer);
    
    aBlock.upper = 0;
    aBlock.lower = 0;
    
    //add the block to the player
    playerAddBlock (&aPlayer, &aBlock);
    
    fail_unless (aPlayer.inHand == 1, NULL);
    
    //test the unused has decreased with 1
    int i;
    i = 0;
    List *finger = aPlayer.unused;
    while (finger != NULL) {
        i++;
        finger = finger->next;
    }
    fail_unless (i == BLOCK_PER_PLAYER - 1, NULL);
    
    //fail_unless the hand statistics
    fail_unless (aPlayer.playableHand == 0x01, NULL);
    fail_unless (aPlayer.frequencyHand[0] == 2, NULL);
    
    
    //insert a new blocks
    Block bBlock;
    bBlock.upper = 4;
    bBlock.lower = 3;
    playerAddBlock (&aPlayer, &bBlock);
    
    //fail_unless the info again
    fail_unless (aPlayer.inHand == 2, NULL);
    
    //test the unused has decreased with 1
    i = 0;
    finger = aPlayer.unused;
    while (finger != NULL) {
        i++;
        finger = finger->next;
    }
    fail_unless (i == BLOCK_PER_PLAYER - 2, NULL);
    
    //fail_unless the hand statistics
    fail_unless (aPlayer.playableHand == 0x19, NULL);
    fail_unless (aPlayer.frequencyHand[0] == 2, NULL);
    fail_unless (aPlayer.frequencyHand[3] == 1, NULL);
    fail_unless (aPlayer.frequencyHand[4] == 1, NULL);
    
    playerFreeResource (&aPlayer);
    //printf ("player add block test succesfull\n\n");
}
END_TEST

START_TEST (testPlayerRemoveBlock) {
    //printf ("testing player remove block\n");
    
    //add some block
    
    //create a block and add it to the playerCreate
    Block aBlock;
    Player aPlayer;
    playerCreate (&aPlayer);
    
    aBlock.upper = 0;
    aBlock.lower = 0;
    
    //add the block to the player
    playerAddBlock (&aPlayer, &aBlock);
    
    //insert a new blocks
    Block bBlock;
    bBlock.upper = 4;
    bBlock.lower = 3;
        
    playerAddBlock (&aPlayer, &bBlock);
    //than remove a block and fail_unless its info again
    
    playerRemoveBlock (&aPlayer, 0);
    fail_unless (aPlayer.inHand == 1, NULL);
    fail_unless (aPlayer.playableHand == 0x01, NULL);

    fail_unless (aPlayer.frequencyHand[4] == 0, NULL);
    fail_unless (aPlayer.frequencyHand[3] == 0, NULL);
    fail_unless (aPlayer.frequencyHand[0] == 2, NULL);
    
    //remove another block
    playerRemoveBlock (&aPlayer, 0);
    fail_unless (aPlayer.inHand == 0, NULL);
    fail_unless (aPlayer.playableHand == 0x00, NULL);
    fail_unless (aPlayer.frequencyHand[0] == 0, NULL);
    
    playerFreeResource (&aPlayer);
    //printf ("testing player remove block succesfull\n\n");
}
END_TEST

START_TEST (testPlayerFreeResource) {
   // printf ("testing player free resource\n");
    
    Block aBlock;
    Player aPlayer;
    playerCreate (&aPlayer);
    
    aBlock.upper = 0;
    aBlock.lower = 0;
    
    //add the block to the player
    playerAddBlock (&aPlayer, &aBlock);
    
    //insert a new blocks
    Block bBlock;
    bBlock.upper = 4;
    bBlock.lower = 3;
    
    playerAddBlock (&aPlayer, &bBlock);
    
    playerFreeResource (&aPlayer);
    
    //fail_unless the usual stuff
    fail_unless (aPlayer.name == NULL, NULL);
    fail_unless (aPlayer.hand == NULL, NULL);
    fail_unless (aPlayer.unused == NULL, NULL);
    fail_unless (aPlayer.inHand == 0, NULL);
    fail_unless (aPlayer.playableHand == 0, NULL);
    
    int i;
    
    for (i = 0; i < MAX_PIP+1; i++) {
        fail_unless (aPlayer.frequencyHand[i] == 0, NULL);
    }
    
    playerFreeResource (&aPlayer);
    //printf ("testing player free resource sucessfully\n\n");
}
END_TEST

START_TEST (testPlayerClearHand) {
   // printf ("testing player clear hand\n");
    Block aBlock;
    Player aPlayer;
    playerCreate (&aPlayer);
    
    aBlock.upper = 0;
    aBlock.lower = 0;
    
    //add the block to the player
    playerAddBlock (&aPlayer, &aBlock);
    
    //insert a new blocks
    Block bBlock;
    bBlock.upper = 4;
    bBlock.lower = 3;
    
    playerAddBlock (&aPlayer, &bBlock);
    
    //clear the players hand
    playerClearHand (&aPlayer);
    
    fail_unless (aPlayer.inHand == 0, NULL);
    fail_unless (aPlayer.hand == NULL, NULL);
    
    //test for BLOCK_PER_PLAYER unused blocks
    int j,i = 0;
    List *finger = aPlayer.unused;
    while (finger != NULL) {
        i++;
        finger = finger->next;
    }
    fail_unless (i == BLOCK_PER_PLAYER, NULL);
    fail_unless (aPlayer.playableHand == 0x00, NULL);
    
    for (j = 0; j < MAX_PIP+1; j++) {
        fail_unless (aPlayer.frequencyHand[j] == 0, NULL);
    }
    
    playerFreeResource (&aPlayer);
    //printf ("testing player clear hand succesfull\n\n");
}
END_TEST

START_TEST (testPlayerGetBlock) {
   // printf ("testing player get block\n");
    
    Block aBlock;
    Player aPlayer;
    playerCreate (&aPlayer);
    
    aBlock.upper = 0;
    aBlock.lower = 0;
    
    //add the block to the player
    playerAddBlock (&aPlayer, &aBlock);
    
    //insert a new blocks
    Block bBlock;
    bBlock.upper = 4;
    bBlock.lower = 3;
    playerAddBlock (&aPlayer, &bBlock);
    
    Block *someBlock = playerGetBlock (&aPlayer, 1);
    fail_unless (someBlock->upper == 0, NULL);
    fail_unless (someBlock->lower == 0, NULL);
    
    someBlock = playerGetBlock (&aPlayer, 0);
    fail_unless (someBlock->upper == 4, NULL);
    fail_unless (someBlock->lower == 3, NULL);
    //than remove a block and fail_unless its info again
    
    playerFreeResource (&aPlayer);
    //printf ("testing player get block succesfully\n\n");
}
END_TEST

START_TEST (testPlayerCountInHand) {
    //printf ("testing player count\n");
    
    
    Block aBlock;
    Player aPlayer;
    playerCreate (&aPlayer);
    
    aBlock.upper = 0;
    aBlock.lower = 0;
    
    //add the block to the player
    
    
    //insert a new blocks
    Block bBlock;
    bBlock.upper = 4;
    bBlock.lower = 3;
    
    int sum;
    
    sum = playerCountInHand (&aPlayer);
    fail_unless (sum == 0, NULL);
    
    //add a 0-0 block
    playerAddBlock (&aPlayer, &aBlock);
    sum = playerCountInHand (&aPlayer);
    fail_unless (sum == 0, NULL);
    
    //add ad 4-3 block
    playerAddBlock (&aPlayer, &bBlock);
    sum = playerCountInHand (&aPlayer);
    fail_unless (sum == 7, NULL);
    
    playerFreeResource (&aPlayer);
    //printf ("testing player count sucessfully\n\n");
}
END_TEST

START_TEST (testPlayerCanPlay) {
    //printf ("testing player can play\n");
    
    Block aBlock;
    Player aPlayer;
    playerCreate (&aPlayer);
    
    aBlock.upper = 0;
    aBlock.lower = 0;
    
    //add the block to the player
    
    
    //insert a new blocks
    Block bBlock;
    bBlock.upper = 4;
    bBlock.lower = 3;   
 
    //playerAddBlock (&aPlayer, &aBlock);
    playerAddBlock (&aPlayer, &bBlock);
    
    //see if the player can play black pips
    fail_unless (playerCanPlay(&aPlayer, 0, 0) == 0, "Blank pip test failed");
    
    playerAddBlock (&aPlayer, &aBlock);
    
    int i;
    
    for (i = 0; i < MAX_PIP+1; i++) {
        fail_unless (playerCanPlay (&aPlayer, 1, 1) == 0, NULL);
    }
    
    //playerAddBlock (&aPlayer, &aBlock);
    playerAddBlock (&aPlayer, &bBlock);
    
    i = playerCanPlay (&aPlayer, 1,2);
    fail_unless (i == 0, NULL);
    
    i = playerCanPlay (&aPlayer, 0,2);
    fail_unless (i == 1, NULL);
    
    i = playerCanPlay (&aPlayer, 1,0);
    fail_unless (i == 1, NULL);
    
    //lets try playing a game
    i = playerCanPlay (&aPlayer, 1, 2);
    fail_unless (i == 0, NULL);
    
    //lets try playing a game
    i = playerCanPlay (&aPlayer, 3, 0);
    fail_unless (i == 1, NULL);
    
    
    //lets try playing a game
    i = playerCanPlay (&aPlayer, 4, 0);
    fail_unless (i == 1, NULL);
    
    //lets try playing a game
    i = playerCanPlay (&aPlayer, 3, 4);
    fail_unless (i == 1, NULL);

    //lets try playing a game
    i = playerCanPlay (&aPlayer, 4, 3);
    fail_unless (i == 1, NULL);
    
    //lets try playing a game
    i = playerCanPlay (&aPlayer, 6, 4);
    fail_unless (i == 1, NULL);
    //lets try playing a game
    i = playerCanPlay (&aPlayer, 6, 3);
    fail_unless (i == 1, NULL);
    
    
    playerFreeResource (&aPlayer);
  //  printf ("testing player can play sucessfully\n\n");
}
END_TEST


START_TEST (testPlayerAnalyzeHand) {
    //create some blocks and create some players
    Block aBlock, bBlock;
    Player aPlayer;
    
    //initialize the blocks and players
    aBlock.upper = 0;
    aBlock.lower = 0;
  
    bBlock.upper = 4;
    bBlock.lower = 6;
   
    playerCreate (&aPlayer);    //create the player for testing
    aPlayer.hand = aPlayer.unused;
    aPlayer.unused = aPlayer.unused->next;
    
    playerAnalyzeHand (&aPlayer, &aBlock, ADD_BLOCK);

    fail_unless (aPlayer.playableHand == 0x01, "Playable hand failed test");
    fail_unless (aPlayer.frequencyHand[0] == 2, "Frequency hand failed test");
    
    int i;
    for (i = 1; i <= MAX_PIP; i++) {
        fail_unless (aPlayer.frequencyHand[i] == 0, "Frequency hand failed test 1");
    }
    
    //add another blocks
    playerAnalyzeHand(&aPlayer, &bBlock, ADD_BLOCK);
    fail_unless (aPlayer.playableHand == 0x51, "Playable hand failed test 2a");
    fail_unless (aPlayer.frequencyHand[0] == 2, "Frequency hand failed test 2b");
    fail_unless (aPlayer.frequencyHand[4] == 1, "Frequency hand failed test 2c");
    fail_unless (aPlayer.frequencyHand[6] == 1, "Frequency hand failed test 2d");
    
    
    //now we remove the blocks
    playerAnalyzeHand(&aPlayer, &aBlock, REMOVE_BLOCK);
    fail_unless (aPlayer.playableHand == 0x50, "Playable hand failed test 3");
    fail_unless (aPlayer.frequencyHand[0] == 0, "Frequency hand failed test 3");
    fail_unless (aPlayer.frequencyHand[4] == 1, "Frequency hand failed test 3");
    fail_unless (aPlayer.frequencyHand[6] == 1, "Frequency hand failed test 3");
    
    playerAnalyzeHand(&aPlayer, &bBlock, REMOVE_BLOCK);
    fail_unless (aPlayer.playableHand == 0x00, "Playable hand failed test 3");
    
    for (i = 0; i <=MAX_PIP; i++) {
        fail_unless (aPlayer.frequencyHand[i] == 0, "Frequency hand failed test 4");
    }
    
    //done verifying
    playerFreeResource (&aPlayer);
}
END_TEST



Suite *player_suite (void) {
      Suite *s = suite_create ("Player");
   
      /* Core test case */
      TCase *tc_core = tcase_create ("Core");
      
      //add the test cases
      tcase_add_test (tc_core, testPlayerCreate);
      tcase_add_test (tc_core, testPlayerAddBlock);
      tcase_add_test (tc_core, testPlayerRemoveBlock);
      tcase_add_test (tc_core, testPlayerFreeResource);
      tcase_add_test (tc_core, testPlayerClearHand);
      tcase_add_test (tc_core, testPlayerGetBlock);
      tcase_add_test (tc_core, testPlayerCountInHand);
      tcase_add_test (tc_core, testPlayerCanPlay);
      tcase_add_test (tc_core, testPlayerAnalyzeHand);
      
      //add the test case to the suite
      suite_add_tcase (s, tc_core);
    
      return s;
}
