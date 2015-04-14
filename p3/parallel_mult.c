#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
#include <mpi.h>

//http://stackoverflow.com/questions/2128728/allocate-matrix-in-c
//using the above reference for a simulation of a matrix...example below
/*
arr[offset] where offset is = i * num_cols + j
instead of arr[row][cols] //row major

AND

arr[offset] where offset is = i + num_rows * j
instead of arr[cols][rows] //column major
*/
/*
 * Purpose: Build a derived datatype so that the three
 *          values can be sent in a single MPI message.
 * Input args:   g_values: pointer to the trapezoidal_values input data
 * Output args:  input_mpi_t_p:  the new MPI datatype that is created
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

static void build_mpi_type(Details *details, MPI_Datatype * input_details){
    int array_of_blocklengths[3] = {4, 1, 1};
    MPI_Datatype array_of_types[3] = {MPI_CHAR, MPI_CHAR, MPI_INT};
    MPI_Aint m_addr, f_addr, n_addr;
    MPI_Aint array_of_displacements[3] = {0};

    MPI_Get_address(&details->matrix_form, &m_addr);
    MPI_Get_address(&details->flag, &f_addr);
    MPI_Get_address(&details->n, &n_addr);
    array_of_displacements[1] = f_addr-m_addr;
    array_of_displacements[2] = n_addr-m_addr;
    MPI_Type_create_struct(3,array_of_blocklengths,array_of_displacements, 
                           array_of_types,input_details);
    MPI_Type_commit(input_details);
}

int main(){
	int *global_A, *local_A, *global_B, *final_matrix;

	/* Initialize MPI */
    MPI_Init(NULL, NULL);

    // declaring the rank and comm size variables for all processors to have
	int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	int comm_sz;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    MPI_Datatype input_details;
	Details *details = malloc(sizeof(Details));
    build_mpi_type(details, &input_details);

  /* start the timer on rank 0 */
    double start_time;
    if (my_rank == 0){
		get_user_input(&global_A, &global_B, &final_matrix, details);
		multiply_matricies(global_A, global_B, final_matrix, details);
		// start_time = MPI_Wtime();
    }
    /* Send the data from rank 0 to other processors/threads  */
    MPI_Bcast(details, 1, input_details, 0, MPI_COMM_WORLD);

    /* MPI_Barrier blocks until all processors have reached here so everyone can have
    the value for 'details' before we try to dereference it*/
    MPI_Barrier(MPI_COMM_WORLD);
    if (my_rank != 0){
	    create_matrices(&global_A, &global_B, &final_matrix, details->n);
	}

    MPI_Bcast(final_matrix, (details->n * details->n) , MPI_INT, 0, MPI_COMM_WORLD);

    if (my_rank == 1){
    	puts("hey\n");
        print_matrix(final_matrix, details->n);
    	// printf("%d\n", details->n);
    	// printf("%s\n", details->matrix_form);
    	// printf("%s\n", details->flag);
    }

    MPI_Finalize();

}