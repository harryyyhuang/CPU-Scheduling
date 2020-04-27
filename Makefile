FLAG = -DDEBUG -Wall -g 

main: main.o scheduler.o process.o queue.o
	gcc $(FLAG) main.o scheduler.o process.o queue.o

main.o: main.c Makefile
	gcc $(FLAG) main.c -c

scheduler.o: scheduler.c scheduler.h cpucontrol.h Makefile
	gcc $(FLAG) scheduler.c -c

process.o: process.c process.h cpucontrol.h Makefile
	gcc $(FLAG) process.c -c

queue.o: queue.c queue.h cpucontrol.h Makefile
	gcc $(FLAG) queue.c -c

clean:
	rm -rf *o
