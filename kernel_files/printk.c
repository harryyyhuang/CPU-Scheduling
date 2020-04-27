// syscall #352

#include <linux/linkage.h>
#include <linux/kernel.h>


asmlinkage void sys_printk(int pid, long startsec, long startnsec, long endsec, long endnsec){
    printk(KERN_INFO "[Project1] %d %ld.%09ld %ld.%09ld", pid, startsec, startnsec, endsec, endnsec);
}
