/**
 * @Author: W.k. Tse
 * @Date: 11-4-'11
 * @Description: Implementing the PNG-format export function. With a modified PNG file-format
 */
#ifndef EXPORTER_H
#define EXPORTER_H

#include <stdint.h>

//some structs for passing info to the export function

typedef struct _exportInfo {
  //Tags for some meta information about the game
  //These tags are based on the PGN (Portable Game Notation)
  char tagEvent[100];
  char tagSite[100];
  char tagDate[25];
  char tagRound[3];
  char tagTeam1[100];
  char tagTeam2[100];
  char tagResult[10];
  
  //Some extra tags for more info
  int32_t startingPlayer;
  
  //the players hand
    //  List *dataPlayer1;
    // List *dataPlayer2;
    //List *dataPlayer3;
    //List *dataPlayer4;
  
  //The order in which the game progressed
  //A chronological order of the blocks played
  //List *blocksPlayed;
  

  //FIXME implement a struct for passing info
} ExportInfo;


/**
 * Function to export the game to a file-format
 * @param pInput The file to export the game to
 * @pre pInput is not null
 * @pre pInput is writable
 * @post pInput the game is exported to the file
 * *NOTE* : if file already exists, we will overwrite the file
 */
void exportToFile (char *pInput, ExportInfo *pInfo);


#endif
