#include "process.h"
#include <unistd.h>
#include "cpucontrol.h"


int process_execute(struct processInfo* process){
    pid_t pid = fork();

    // pid == -1 means fail to fork
    if(pid < 0){
        perror("fork fail");
        exit(1);
    }
    // pid == 0 means it's child process
    if(pid == 0){
        pid = getpid();
        long start_sec, start_nsec, end_sec, end_nsec;
        syscall(GET_TIME, start_sec, start_nsec);
        for(int i = 0; i < process->execution_time; ++i ){
            UNIT_TIME();
        }
        syscall(GET_TIME, end_sec, end_nsec);

        syscall(PRINTK, pid, start_sec, start_nsec, end_sec, end_nsec);


        exit(0);
    }

    // parent process
    else{
        // set the child process cpu and priority
        setcpu(pid, CHILD_CPU);


        SET_PRIORITY(pid, SCHED_FIFO, PRIORITY_INIT);
    }

    return pid;
}

void setcpu(pid_t pid, int core){
    cpu_set_t mask;                                         
    CPU_ZERO(&mask);                                        
    CPU_SET(core, &mask);                                   
                                                            
    if(sched_setaffinity(pid, sizeof(mask), &mask) < 0){    
        perror("sched_setaffinity fails");                  
        exit(1);    
    }
}

// check if the executing time of process has reach the process's
// given execution time, and reutrn true as it end, otherwise return 
// false
bool processEnd(struct processInfo* runningProcess){
    if(runningProcess->execution_time == 0) return true;
    else return false;
}

// add the process executing time as the process is being executed
void execprocess(struct processInfo* runningProcess){
    --runningProcess->execution_time;
}

// stop the running process
void posponeProcess(struct processInfo* process){
    SET_PRIORITY(process->pid, SCHED_IDLE, 0) ;
}

// resume the process
void resumeProcess(struct processInfo* process){
    SET_PRIORITY(process->pid, SCHED_FIFO, PRIORITY_HIGH);
}
