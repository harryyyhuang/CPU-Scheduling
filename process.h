#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>

struct processInfo
{
    char name[33];
    int  ready_time   ;
    int  execution_time;
    pid_t pid    ;
};

// running one unit of time
#define UNIT_TIME() do{                    \
    volatile unsigned long i;              \
    for(i = 0; i < 1000000UL; i++);        \
} while (0)


// fork the porcess and execute it 
int process_execute(processInfo* process);

// check if the current process is ended
bool processEnd(processInfo* runningProcess);                                      

// execute the process
void execprocess(processInfo* runningProcess);

// postpone the process to let other run first
void posponeProcess(processInfo* process);

// resume the process that has been postpone
void resumeProcess(processInfo* process);

#endif