#ifndef PROCESS_H
#define PROCESS_H

#define _GNU_SOURCE
#include <sys/types.h>
#include <stdbool.h>
#include <sched.h>

#define GET_TIME 351
#define PRINTK   352

typedef struct processInfo
{
    char name[33];
    int  ready_time   ;
    int  execution_time;
    pid_t pid    ;
}processInfo;

// running one unit of time
#define UNIT_TIME() do{                    \
    volatile unsigned long i;              \
    for(i = 0; i < 1000000UL; i++);        \
} while (0)


// fork the porcess and execute it 
int process_execute(struct processInfo* process);

// check if the current process is ended
bool processEnd(struct processInfo* runningProcess);                                      

// execute the process
void execprocess(struct processInfo* runningProcess);

// postpone the process to let other run first
void posponeProcess(struct processInfo* process);

// resume the process that has been postpone
void resumeProcess(struct processInfo* process);

void setcpu(pid_t pid, int core);

#endif