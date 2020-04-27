#ifndef QUEUE_H
#define QUEUE_H

#include "process.h"

struct processQueue
{
    struct processInfo** processes;
    int    in     ;
    int    out    ;
    int    num    ;
    int    maxNum ;
};

// initialize the processQueue
processQueue initQueue(int ProcNum);

// utilization for the processQueue
bool pushQueue(processQueue* readyQueue, processInfo* ready);
bool insertQueue(processQueue* readyQueue, processInfo* ready);

// boolean to check if queue is empty
bool emptyQueue(processQueue readyQueue);

// pull out the process 
processInfo* pullQueue(processQueue* readyQueue);

// push the process backward
void pushBackAll(processQueue* readyQueue, processInfo* ready, int insertIndex);

// check if to preempt
bool isPreempt(processQueue* readyQueue, processInfo* runningProcess);




#endif