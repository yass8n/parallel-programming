executable: executable.o matrix.o vector.o
		gcc -g -lm -fopenmp executable.o matrix.o vector.o -o execute
			
executable.o: executable.c
		gcc -c -g -fopenmp executable.c

matrix.o: matrix.c
		gcc -c -g matrix.c

vector.o: vector.c
		gcc -c -g vector.c

clean:
		rm *.o ./a.out ./execute

