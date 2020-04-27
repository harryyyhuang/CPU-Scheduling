// syscall #351

#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/timer.h>

asmlinkage void sys_get_time(long* second, long* nanosecond){
    struct timespec t;
    getnstimeofday(&t) ;
    *second = t.tv_sec  ;
    *nanosecond = t.tv_nsec ;
}
