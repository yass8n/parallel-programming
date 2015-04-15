/**
Yaseen Aniss
This is the serial matrix multiplication in C */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
//http://stackoverflow.com/questions/2128728/allocate-matrix-in-c
//using the above reference for a simulation of a matrix...example below
/*
arr[offset] where offset is = i * num_cols + j
instead of arr[row][cols] //row major

AND

arr[offset] where offset is = i + num_rows * j
instead of arr[cols][rows] //column major
*/

typedef struct Details {
    char matrix_form[4];//buffer for the ijk, ikj, or kij
    char flag[1];//buffer for the flag (R or I)
    int n; //matrix is size n X n
} Details;
void create_matrices(int **A, int **B, int **C, int n){
    *A = malloc(sizeof(int));
    *B = malloc(sizeof(int));
    *C = malloc(sizeof(int));
    (*A) = calloc(sizeof(int), n*n); //using calloc to zero initialize the buffer
    (*B) = calloc(sizeof(int), n*n);
    (*C) = calloc(sizeof(int), n*n);
}
void initialize_random_matrix(int **M, int n){
    for (int i = 0; i < (n*n); i++){
        (*M)[i] = (rand() % 10);
    }
}
void initialize_input_matrix(int **M, int n){
    int count = 0;
    int number = 0;
    while (!feof(stdin)){
        fscanf(stdin, "%d", &number);
        (*M)[count] = number;
        count ++;
        if (count == (n*n)){
            break;
        }
    }
}
void print_matrix(int *M, int n){
    for (int i = 0; i < n; i ++){
        for (int j = 0; j < n; j++){
            printf("%d ", * ( M + (i * n) + j) ); 
        }
        printf("%s\n", "");
    }
}
void multiply_matricies(int *A, int *B, int *C, Details *details){
    int n = details->n;
    if (strncmp(details->matrix_form, "ijk", 3) == 0){
        for (int i = 0; i < n; i ++){ 
            for (int j = 0; j < n; j ++){
                for (int k = 0; k < n; k++){
                    (*(C + (i * n) + j)) += (*(A + (i*n) + k)) * (*(B + (k * n) + j));
                }
            }
        }

    }
    else if (strncmp(details->matrix_form, "ikj", 3) == 0){
        for (int i = 0; i < n; i ++){ 
            for (int k = 0; k < n; k ++){
                for (int j = 0; j < n; j++){
                    (*(C + (i * n) + j)) += (*(A + (i*n) + k)) * (*(B + (k * n) + j));
                }
            }
        }
    }
    else if (strncmp(details->matrix_form, "kij", 3) == 0){
        for (int k = 0; k < n; k ++){ 
            for (int i = 0; i < n; i ++){
                for (int j = 0; j < n; j++){
                    (*(C + (i * n) + j)) += (*(A + (i*n) + k)) * (*(B + (k * n) + j));
                }
            }
        }

    }
}
void get_user_input(int **A, int **B, int **C, Details *details) {
	fscanf(stdin, "%s", details->matrix_form); //get form
    if (strncmp(details->matrix_form, "ijk", 3) != 0 && 
        strncmp(details->matrix_form, "ikj", 3) != 0 &&
        strncmp(details->matrix_form, "kij", 3) != 0){
        printf("%s\n", "Illegal matrix form. Must be 'ijk', 'ikj', or 'kij'");
        exit(0);
    }
     
	fscanf(stdin, "%s", details->flag); //get flag
    if (strncmp(details->flag, "I", 1) != 0 && 
        strncmp(details->flag, "i", 1) != 0 && 
        strncmp(details->flag, "r", 1) != 0 && 
        strncmp(details->flag, "R", 1) != 0){
        printf("%s\n", "Illegal flag. Must be 'I' or 'R'");
        exit(0);
    }
	fscanf(stdin, "%d", &(details->n)); //get N
    if (details->n < 2){
        printf("%s\n", "Size Must be Greater than 1!");
        exit(0);
    }

	create_matrices(A, B, C, details->n);
	int j = details->n * details->n;
	int i = details->n * details->n;
    if (strncmp(details->flag, "R", 1) == 0){
        srand(time(NULL));
        initialize_random_matrix(A,details->n);
        initialize_random_matrix(B,details->n);
    }else {
        initialize_input_matrix(A,details->n );
        initialize_input_matrix(B,details->n );
    }

}

int main(int argc, char * argv[]){
	int *A, *B, *C;
	Details details;
	get_user_input(&A, &B, &C, &details);
    multiply_matricies(A, B, C, &details);
    print_matrix(C, details.n);
    free(A);
    free(B);
    free(C);
}
