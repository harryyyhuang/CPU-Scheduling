#define _GNU_SOURCE
#include "scheduler.h"
#include "process.h"
#include "cpucontrol.h"
#include "queue.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// read the whole file of the input 
void readFile(){
#ifdef DEBUG
            fprintf(stderr, "start reading the file\n");
#endif

    char algo[5];
    scanf("%s", algo);
    determineAlgo(algo);

    scanf("%d", &processNum);

    processList = (struct processInfo*)malloc(processNum * sizeof(struct  processInfo));
    readProcess();
    

#ifdef DEBUG
            fprintf(stderr, "end reading the file\n");
#endif
}

// determine the scheduling algorithm strategy
void determineAlgo(char* algoString){
    if(strcmp(algoString, "FIFO")){
        algorithm = FIFO;
    }

    else if(strcmp(algoString, "RR")){
        algorithm = RR;
    }

    else if(strcmp(algoString, "SJF")){
        algorithm = SJF;
    }

    else if(strcmp(algoString, "PSJF")){
        algorithm = PSJF;
    }
}

// read the process name, ready time and 
// execution time and store it into the 
// processlist and initialize the pid to
// -1 which means the process hasn't been
// created
void readProcess(){
#ifdef DEBUG
                fprintf(stderr, "start reading the process\n");
#endif
    for(int i = 0; i < processNum; ++i){
        scanf("%s", processList[i].name);
        scanf("%d", &processList[i].ready_time);
        scanf("%d", &processList[i].execution_time);
        processList[i].pid = -1;
    }
#ifdef DEBUG
                fprintf(stderr, "end reading the process\n");
#endif
}

// compare the ready time and sort the process list
int compare(const void* a, const void* b){
    if(((processInfo* )a)->ready_time < ((processInfo*)b)->ready_time) return -1;
    else if(((processInfo*)a)->ready_time == ((processInfo*)b)->ready_time) return 0;
    else return 1;
}

// just to load the ready queue which simply use the function
// in queue.h
void load_queue(){
    processQueue readyQueue = initQueue(processNum);
}

// handle the end of the process and choose the next
// thing to do for each scheduling policy
// void signal_children(int signo){
//     wait(NULL);

//     //implement different policy
// }


void handleRR(processQueue* readyQueue, processInfo* process){
    if(!pushQueue(readyQueue, process)){
        perror("ready queue out of bound");
        exit(1);
    }
#ifdef DEBUG
                fprintf(stderr, "posponing %s process at time %d.\n", process->name, ntime);
#endif
    posponeProcess(process);

}

void handlePSJF(processQueue* readyQueue, processInfo* process){
    if(!insertQueue(readyQueue, process)){
        perror("ready queue out of bound");
        exit(1);
    }
#ifdef DEBUG
                fprintf(stderr, "posponing %s process at time %d.\n", process->name, ntime);
#endif
    posponeProcess(process);
}





// start schedule every thing
void scheduler(){

    // sort the process list
    qsort(processList, processNum, sizeof(processInfo), compare);

    // init the ready queue
    load_queue();

    // set the scheduler process to parent cpu
    SET_CPU(getpid(), PARENT_CPU);

    // set high priority to scheduler so it won't be preempted
    SET_PRIORITY(getpid(), SCHED_FIFO, PRIORITY_INIT);

    // set the signal that handle the child signal 
    //SIGNAL_HANDLE();

    // initial the scheduler
    ntime = 0;
    running = -1;
    finishNum = 0;
    processInfo* runningProcess = 0;

    // start looping
    while (1)
    {
        // check if ntime reach for any process's ready time
        for(int i = finishNum; i < processNum; i++){
            if (processList[i].ready_time == ntime){
#ifdef DEBUG
                fprintf(stderr, "%s is getting into the ready queue at time %d.\n", processList[i].name, ntime);
#endif
                if(algorithm == SJF || algorithm == PSJF){
                    if(!insertQueue(&readyQueue, &processList[i])){
                        perror("insert ready queue out of bound");
                        exit(1);
                    }
                }
                else{
                    if(!pushQueue(&readyQueue, &processList[i])){
                        perror("push ready queue out of bound");
                        exit(1);
                    }
                }
            }
        }

        
        if(runningProcess){
            // check if current running process finish
            if(processEnd(runningProcess)){
                waitpid(runningProcess->pid, NULL, 0);
                runningProcess = 0;
                ++finishNum;

                if(finishNum == processNum) break ; 
            }

            // if policy is RR then should check the time slice
            // and let the handleRR handle it HAHA~
            else if(algorithm == RR && ntime%500 == 0){
                handleRR(runningProcess, &readyQueue);
                runningProcess = 0 ;
            }

            // if policy is PSJF then should check if the process
            // is to be preempted
            else if(algorithm == PSJF && isPreempt(&readyQueue, runningProcess)){
                handlePSJF(runningProcess, &readyQueue);
                runningProcess = 0 ;
            }
        }

        // check the ready process and execute it
        // if process hasn't been fork then fork it
        // if process had been postponed then resume it
        if(!runningProcess){
            if(!emptyQueue(readyQueue)){
                processInfo* toExecProcess = pullQueue(&readyQueue);
                if(toExecProcess->pid == -1){
                    toExecProcess->pid = process_execute(toExecProcess);
                    printf("%s %d\n", toExecProcess->name, toExecProcess->pid);
                }
                else{
                    resumeProcess(toExecProcess);
                }
                runningProcess = toExecProcess ;
            }
        }

        // Run an unit of time
        UNIT_TIME();
        ++ntime;
        if(runningProcess) execprocess(runningProcess);
    }
    

    return 0;
}



