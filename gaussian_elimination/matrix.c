/*
Yaseen Aniss
This is matrix.c file. It is used to define the functions associated with matricies
*/
#include "matrix.h"
#include <stdlib.h>
#include "vector.h"
#include <stdio.h>
#include <string.h>
/*
  Purpose:      Allocate memory for the Matrix
  
  Input args:   Size: the number of values to allocate
                  initialize: can be 0 or 1.
                          if 0, dont initialzie the values
                          if 1, initialize the values to random numbers

  Output args:  Matrix *: pointer to a new Matrix object
 */
Matrix * create_matrix(int size, int initialize){
    Matrix * matrix = malloc(sizeof(Matrix));
    matrix->size = size;
    int i = 0, j = 0;
    matrix->values = malloc(sizeof(double *) * matrix->size);
    for (i; i < matrix->size; i++){
        matrix->values[i] = calloc(sizeof(double), matrix->size);
        if (initialize == 1){
            for (j = 0; j < matrix->size; j++){
                matrix->values[i][j] = drand48() * 2e6 - 1e6;
            }
        }
    }
    return matrix;
}
/*
  Purpose:      Print a Matrix's values

  Input args:   vector: pointer to the matrix we will be operating on
 */
void print_matrix(Matrix * matrix){
    int i = 0, j = 0;
    for (i; i < matrix->size; i++){
        for (j = 0; j < matrix->size; j++){
            printf("%f ", matrix->values[i][j]);
        }
        puts("\n");
    }
}
/*
  Purpose:      Swap the rows of a matrix

  Input args:   matrix: pointer to the matrix we will be operating on
                initial_row: the index of one of the rows we will be swapping
                swap_row: the index of the other row we will be swapping

  Output args:  Matrix *: the new matrix with the updated swaps         
 */
void swap_matrix_row(Matrix **matrix, int initial_row, int swap_row){
    double * initial_row_pointer = (*matrix)->values[initial_row];
    (*matrix)->values[initial_row] = (*matrix)->values[swap_row];
    (*matrix)->values[swap_row] = initial_row_pointer;
}
/*
  Purpose:      Create an exact copy of a Matrix
  Input args:   m: pointer to the matrix we will be operating on

  Output args:  Matrix *: the new matrix that is a copy of the "m" Matrix      
 */
Matrix * copy_matrix(Matrix * m){
    Matrix * matrix = malloc(sizeof(Matrix));
    
    matrix->size = m->size;
    matrix->values = malloc(sizeof(double *) * matrix->size);
    int i=0;
    for (i; i < matrix->size; i++)
    {
        matrix->values[i] = malloc(sizeof(double) * matrix->size);
        memcpy(matrix->values[i], m->values[i], sizeof(double) * m->size); 
    }
    return matrix;
}