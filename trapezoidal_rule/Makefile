
all: find_n trap_run 

find_n: find_n.o project.o
	clang find_n.o project.o -lm -o find_n 

find_n.o: find_n.c project.h
	clang -c find_n.c

project.o: project.c project.h
	clang -c project.c

trap_run: trap_run.o project.o
	mpicc trap_run.o project.o -lm -o trap_run

trap_run.o: trap_run.c
	mpicc -g -Wall -std=c99 -c trap_run.c

clean:
	rm find_n trap_run *.o
