/**
 * A queue implementation
 * \file queue.h
 * \author W.k.Tse 
 * \version 0.1
 * \date 12-04-11
 */

#include "queue.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

void queueInitialize(Queue *pQueue, int32_t initialSize) 
{
    assert (pQueue != NULL);
    assert (initialSize >0 );

    char *qPtr = malloc (initialSize * sizeof(char));

    if (qPtr == NULL) {
        printf ("Queue memory allocation failed\n");
        exit(-1);
    }
    else {
        pQueue->queue = qPtr;
        pQueue->size = initialSize;
        pQueue->elements = 0;
    }
}

void queueDestroy(Queue *pQueue) 
{
    assert (pQueue != NULL);
    free (pQueue->queue);
    pQueue->queue = NULL;
    pQueue->elements = 0;
    pQueue->size = 0;
}


void queuePushItem(Queue *pQueue, char pItem) 
{
    queuePush(pQueue, pItem);
}

        
char queuePopItem(Queue *pQueue) 
{
    return queuePop(pQueue);
}


void queuePush(Queue *pQueue, char pItem) 
{
    assert (pQueue != NULL);
    //check to see if we have enough space in the array
    if (queueElements(pQueue) == queueSize(pQueue) ) {
        //we need to expand the array
        queueExpand (pQueue);
    }
    //insert the item in the correct place
    pQueue->queue[queueElements(pQueue)] = pItem;
    pQueue->elements += 1;
}

char queuePop(Queue *pQueue) 
{
    assert (pQueue != NULL);

    char item = 0;
    //check to see if we have more items to remove
    if (pQueue->elements > 0) {
        item = pQueue->queue[0];

        //shift items left
        int i;
        for (i = 1; i < pQueue->elements; i++) {
            pQueue->queue[i-1] = pQueue->queue[i];
        }
        pQueue->elements -= 1;
    }
    return item;    
}


void queueExpand(Queue *pQueue) 
{
	assert (pQueue != NULL);
    //double the expansion size

    char *qPtr = malloc (pQueue->size*2 * sizeof(char));
    if (qPtr == NULL) {
        //allocation error
        printf ("Memory allocation failed: pushing to full queue failed\n");
        exit(-1);
    }
    else {
        //we have enough space in ram to allocate a bigger queue
        int i;
        for (i = 0; i < pQueue->size; i++) {
            qPtr[i] = pQueue->queue[i];
        }
    }
    pQueue->size *= 2;
		
    //free the old malloc, and assign the new malloc as the new array
    free (pQueue->queue);
    pQueue->queue = qPtr;	  
}

int32_t queueSize(Queue *pQueue) 
{
    return pQueue->size;
}


int32_t queueElements(Queue *pQueue) 
{
    return pQueue->elements;
}


