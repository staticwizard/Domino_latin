/**
 * @Author: W.k. Tse
 * @Date: 11-4-'11
 * @Description: Implementing the PNG-format export function. With a modified PNG file-format
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "exporter.h"

void exportToFile (char *pInput, ExportInfo *pInfo) {
    assert (pInfo != NULL);
    assert(pInput != NULL);
    
    //check that the string is atleast 1 char long
    assert(strlen (pInput) > 0);

    //open the file
    FILE *file = fopen (pInput, "w");
    if (file == NULL) {
        printf ("File open error: No write permission or file open error\n");
        exit(-1);	
    }
    else {
        //the file opened correctly
        char tagEvent[100];
        char tagSite[100];
        char tagDate[25];
        char tagRound[3];
        char tagTeam1[100];
        char tagTeam2[100];
        char tagResult[10];

        //start writing some meta info
        fprintf (file,"%s\n", pInfo->tagEvent);
        fprintf (file,"%s\n", pInfo->tagSite);
        fprintf (file,"%s\n", pInfo->tagDate);
        fprintf (file,"%s\n", pInfo->tagRound);
        fprintf (file,"%s\n", pInfo->tagTeam1);
        fprintf (file,"%s\n", pInfo->tagTeam2);
        fprintf (file,"%s\n", pInfo->tagResult);
          
        //flush the file
        fflush (file);
        fclose (file);
        
        //FIXME: implement me
    }
}
