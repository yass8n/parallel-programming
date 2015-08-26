all: parallel_mult
    
parallel_mult: parallel_mult.o 
	mpicc parallel_mult.o -lm -o parallel_mult

parallel_mult.o: parallel_mult.c
	mpicc -g -Wall -std=c99 -c parallel_mult.c

clean:
	rm ./parallel_mult ./parallel_mult.o

