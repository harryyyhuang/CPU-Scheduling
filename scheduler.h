#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "queue.h"
#include <stdbool.h>

#define FIFO 0
#define RR   1
#define SJF  2
#define PSJF 3


// set of priority number
#define PRIORITY_INIT (int)50
#define PRIORITY_LOW (int)10
#define PRIORITY_HIGH (int)90

// data member for all processes and scheduler
static int algorithm;
static int processNum;
static processInfo* processList;

// the ready for executing selection
static processQueue readyQueue;

// current unit of time
static int ntime;

// index of running process, -1 if no process running
static int running;

// number of the finished process
static int finishNum;



// functions for getting the input from files
void readFile();
void determineAlgo(char* algoString);
void readProcess();

// fuction for scheduling the process
void scheduler();

// function for quick sort
int compare();

// function for handling signal
// void signal_handle();
// void signal_children(int signo);

// function for handling the ready queue
void load_queue();

// function for handling the RR policy
void handleRR(processQueue* readyQueue, processInfo* process);

// function for handling the PSJF policy
void handlePSJF(processQueue* readyQueue, processInfo* process);

#endif