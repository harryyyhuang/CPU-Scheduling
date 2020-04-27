#ifndef CPU_CONTROL_H
#define CPU_CONTROL_H

#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdio.h>

#define PARENT_CPU 0
#define CHILD_CPU  1

// set of priority number
#define PRIORITY_INIT (int)50
#define PRIORITY_LOW (int)10
#define PRIORITY_HIGH (int)90

// grab the specific core the run the given process specified
// by the number pid 
#define SET_CPU(pid, core) do{                              \
    cpu_set_t mask;                                         \
    CPU_ZERO(&mask);                                        \
    CPU_SET(core, &mask);                                   \
                                                            \
    if(sched_setaffinity(pid, sizeof(mask), &mask) < 0){    \
        perror("sched_setaffinity fails");                  \
        exit(1);                                            \
    }                                                       \
    } while(0)                           


// set the process to a priority with given policy 
// detail for the priority is not describe here 
#define SET_PRIORITY(pid, policy, priority) do{             \
    struct sched_param param;                               \
    param.sched_priority = priority;                        \
    if(sched_setscheduler(pid, policy, &param) < 0){        \
        perror("sched_setscheduler fails");                 \
        exit(1);                                            \
    }                                                       \
    } while(0)

// write before scheduling to handle the child process
// ended, which to prevent the zimbie process.
#define SIGNAL_HANDLE() do{                                 \
    struct sigaction act;                                   \
    act.sa_flags = 0;                                       \
    act.sa_handler = signal_children;                       \
    sigfillset(&act.sa_mask);                               \
    sigaction(SIGCHLD, &act, NULL);                         \
} while(0)

#endif