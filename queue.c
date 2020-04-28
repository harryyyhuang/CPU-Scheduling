#include "queue.h"
#include "process.h"
#include <stdio.h>


// dynamclly allocate the process queue list set the max number 
// to the all number of processes get from input, set the index 
// and the num to the default zero
processQueue initQueue(int ProcNum){
    processQueue QueueInit ;
    QueueInit.processes = (struct processInfo**)malloc(ProcNum * sizeof(struct  processInfo*));
    QueueInit.maxNum    = ProcNum    ;
    QueueInit.in        = 0          ;
    QueueInit.out       = 0          ;
    QueueInit.num       = 0          ;

    return QueueInit;
}

// push the process into the ready queue for execution
bool pushQueue(processQueue* readyQueue, processInfo* ready){
    if(readyQueue->num < readyQueue->maxNum){
        readyQueue->processes[readyQueue->in] = ready ;
        readyQueue->in = (readyQueue->in + 1) % readyQueue->maxNum ;
        ++readyQueue->num                    ;
        return true;
    }
    else return false;
}

// insert the process into the ready queue by its execution time
bool insertQueue(processQueue* readyQueue, processInfo* ready){
    if(readyQueue->num < readyQueue->maxNum){
        int insertIndex = readyQueue->out ;
        while (1)
        {
            if(insertIndex == readyQueue->in){
                readyQueue->processes[insertIndex] = ready;
                break;
            }

            if(ready->execution_time < readyQueue->processes[insertIndex]->execution_time){
                pushBackAll(readyQueue, ready, insertIndex);
                break;
            }
            else{
                insertIndex = (insertIndex + 1) % readyQueue->maxNum ;
            }

            
        }
        readyQueue->in = (readyQueue->in + 1) % readyQueue->maxNum ;
        ++readyQueue->num                    ;

        return true;
    }
    else return false;
}

// return true if the queue's num is zero
bool emptyQueue(processQueue* readyQueue){
    if(readyQueue->num == 0) return true;
    return false;
}

// pull the first process in the queue
processInfo* pullQueue(processQueue* readyQueue){
    processInfo* temprocess = readyQueue->processes[readyQueue->out];
    readyQueue->out = (readyQueue->out + 1) % readyQueue->maxNum ;
    --readyQueue->num                               ;
    return temprocess;
}

// recursively push back all the process until inserIndex reach the queue's in
// which mean no other should be push back
void pushBackAll(processQueue* readyQueue, processInfo* ready, int insertIndex){
    processInfo* tmpprocess = readyQueue->processes[insertIndex] ;
    readyQueue->processes[insertIndex] = ready ;
    insertIndex = (insertIndex + 1) % readyQueue->maxNum ;
    pushBackAll(readyQueue, tmpprocess, insertIndex) ;
    if(insertIndex == readyQueue->in) return ;
}

// check if the running process should be preempt by checking the queue's
// first element's execution time, if smaller than running process's 
// execution time then it should be preempt. if queue is empty then 
// there's noting to preempt
bool isPreempt(processQueue* readyQueue, processInfo* runningProcess){
    if(emptyQueue(readyQueue)) return false ;

    if(readyQueue->processes[readyQueue->out]->execution_time < runningProcess->execution_time) return true;

    return false;
}
