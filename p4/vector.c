#include "vector.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

Vector * create_vector(int size){
	Vector * vector = malloc(sizeof(vector));
	vector->values = calloc(sizeof(double), size);

}