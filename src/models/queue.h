/**
 * A queue interface for a queue implementation
 * \file queue.h
 * \author W.k.Tse 
 * \version 0.1
 * \date 12-04-11
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>

typedef struct _Queue 
{
    int32_t size;
    int32_t elements;
    char *queue;
}Queue;

void queueInitialize(Queue *pQueue, int32_t pInitialSize);

void queueDestroy(Queue *pQueue);

void queuePushItem(Queue *pQueue, char pItem);

char queuePopItem(Queue *pQueue);

void queuePush(Queue *pQueue, char pItem);

char queuePop(Queue *pQueue);

void queueExpand(Queue *pQueue);

int32_t queueSize(Queue *pQueue);

int32_t queueElements(Queue *pQueue);


#endif

