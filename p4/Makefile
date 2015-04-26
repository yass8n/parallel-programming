all: execute parallel_execute 

execute: executable.o matrix.o vector.o
		gcc -g -lm -fopenmp executable.o matrix.o vector.o -o execute

executable.o: executable.c
		gcc -c -g -fopenmp executable.c

parallel_execute: parallel_executable.o matrix.o vector.o g_elimination.o
		gcc -g -lm -fopenmp parallel_executable.o matrix.o vector.o g_elimination.o -o parallel_execute

parallel_executable.o: parallel_executable.c
		gcc -c -g -fopenmp parallel_executable.c

matrix.o: matrix.c
		gcc -c -g matrix.c

vector.o: vector.c
		gcc -c -g vector.c

g_elimination.o:  g_elimination.c
		gcc -c -g g_elimination.c

clean:
		rm *.o ./execute

