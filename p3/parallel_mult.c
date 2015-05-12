/*
Yaseen Aniss
This is the Matrix Multiplication implementation using open MPI 

http://stackoverflow.com/questions/2128728/allocate-matrix-in-c
I used the above reference for tips on how to structure my matrix
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <mpi.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
/* struct Details
   Purpose: an object to hold the values that the user entered

   Variables: 
           matrix_form: buffer for the ijk, ikj, or kij
           flag:        buffer for the flag (R or I)
           n:           The number of columns in a row
*/
 typedef struct Details {
    char matrix_form[4];//buffer for the ijk, ikj, or kij
    char flag[1];       //buffer for the flag (R or I)
    int n;              //matrix is size n X n
} Details;
/*
  Purpose:      Allocate memory for the matricies and initialize the values to 0
  Input args:   A: pointer to Global_A matrix
                  B: pointer to Global_B matrix
                  C: pointer to final_matrix 
                  a: pointer to ocal_a matrix 

  Output args:  A: pointer to Global_A matrix (initialized to 0)
                  B: pointer to Global_B matrix (initialized to 0)
                  C: pointer to final_matrix (initialized to 0)
                  a: pointer to ocal_a matrix (initialized to 0)
 */
void create_matrices(int **A, int **B, int **C, int **a, int n){
    (*A) = calloc(sizeof(int), n*n); 
    (*B) = calloc(sizeof(int), n*n);
    (*C) = calloc(sizeof(int), n*n);
    (*a) = calloc(sizeof(int), n*n);
}
/*
  Purpose:      Initialize the Matrix using random numbers
  Input args:   n: The number of columns in a row

  Output args:  M: A filled Matrix 
 */
void initialize_random_matrix(int **M, int n){
    for (int i = 0; i < (n*n); i++){
        (*M)[i] = (rand() % 10 - 5);
    }
}
/*
  Purpose:      Initialize the Matrix using the data the user is entering
  Input args:   n: The number of columns in a row

  Output args:  M: A filled Matrix 
 */
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

/*
  Purpose:      Print a Matrix with each row on its own line
  Input args:   M: The Matrix to be printed
                n: The number of columns in a row
 */
void print_matrix(int *M, int n){
    for (int i = 0; i < n; i ++){
        for (int j = 0; j < n; j++){
            printf("%d ", * ( M + (i * n) + j) ); 
        }
        printf("%s\n", "");
    }
}
/*
  Purpose:      Multilpy the Matricies A and B to produce a matrix C
  Input args:   A: pointer to a local_A matrix that only has (n/comm_sz) rows filled
                B: pointer to a filled Global_B matrix
                comm_sz: the size of the comm_world
                details: pointer to the filled input data that the user entered
  
  Output args:  C: pointer to a section of the "final_matrix" matrix that is the result of the 
                multiplication of A and B
 */
void multiply_matricies(int *A, int *B, int *C, Details *details, int comm_sz){
    int n = details->n;
    if (strncmp(details->matrix_form, "ijk", 3) == 0){
        for (int i = 0; i < n/comm_sz; i ++){ 
            for (int j = 0; j < n; j ++){
                for (int k = 0; k < n; k++){
                    (*(C + (i * n) + j)) += (*(A + (i*n) + k)) * (*(B + (k * n) + j));
                }
            }
        }

    }
    else if (strncmp(details->matrix_form, "ikj", 3) == 0){
        for (int i = 0; i < n/comm_sz; i ++){ 
            for (int k = 0; k < n; k ++){
                for (int j = 0; j < n; j++){
                    (*(C + (i * n) + j)) += (*(A + (i*n) + k)) * (*(B + (k * n) + j));
                }
            }
        }
    }
    else if (strncmp(details->matrix_form, "kij", 3) == 0){
        for (int k = 0; k < n; k ++){ 
            for (int i = 0; i < n/comm_sz; i ++){
                for (int j = 0; j < n; j++){
                    (*(C + (i * n) + j)) += (*(A + (i*n) + k)) * (*(B + (k * n) + j));
                }
            }
        }

    }
}
/*
  Purpose: Read in the users input and assign appropriate values to all the matricies 
           according to the data that was entered
  
  Output args:  A: pointer to an initialized and filled Global_A matrix
                  B: pointer to an initialized and filled Global_B matrix
                  C: pointer to an initialized and empty final_matrix (all values are 0)
                  a: pointer to an initialized and empty local_a matrix (all values are 0)
                  details: pointer to the filled input data that the user entered
 */
void get_user_input(int **A, int **B, int**C, int **a, Details *details) {
    fscanf(stdin, "%s", details->matrix_form); //get form
    if (strncmp(details->matrix_form, "ijk", 3) != 0 && 
        strncmp(details->matrix_form, "ikj", 3) != 0 &&
        strncmp(details->matrix_form, "kij", 3) != 0){
        printf("%s\n", "Illegal matrix form. Must be 'ijk', 'ikj', or 'kij'");
        printf("\n%s entered\n", details->matrix_form);
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

    create_matrices(A, B, C, a, details->n);
    if (strncmp(details->flag, "R", 1) == 0){
        srand(time(NULL));
        initialize_random_matrix(A,details->n);
        initialize_random_matrix(B,details->n);
    }else {
        initialize_input_matrix(A,details->n );
        initialize_input_matrix(B,details->n );
    }

}

/*
  Purpose: Build a derived datatype so that the three
           values can be sent in a single MPI message.
  Input args:   details: pointer to the details input data
  Output args:  input_details:  the new MPI datatype that is created
 */
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
    int *global_A, *global_B, *local_A, *final_matrix;
    Details *details = malloc(sizeof(Details));
    double start_time;
    double end_time;
    int my_rank;
    int comm_sz;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    /* Build the derived data type so I can use 'details' in MPI_Bcast */
    MPI_Datatype input_details;
    build_mpi_type(details, &input_details);

  /* start the timer on rank 0 */
    if (my_rank == 0){
        get_user_input(&global_A, &global_B, &final_matrix, &local_A, details);
        start_time = MPI_Wtime();
    }
    /* Send the details from rank 0 to other processors/threads  */
    MPI_Bcast(details, 1, input_details, 0, MPI_COMM_WORLD);

    int row_times_col = (details->n * details->n);

    if (my_rank != 0){
        /*using the value for n given to us through details objects passed from process 0
        to create n X n matricies for all 4 relevant matricies
        global_A, global_B, local_A final_matrix;
        */
        create_matrices(&global_A, &global_B, &final_matrix, &local_A, details->n);
    }
    /* scatter global_A amongst the processors in the comm world */
    MPI_Scatter(global_A, (row_times_col) / comm_sz, MPI_INT, local_A, (row_times_col) / comm_sz, MPI_INT, 0, MPI_COMM_WORLD);

    /* broadcast Global_b amongst the processors in the comm world */
    MPI_Bcast(global_B, row_times_col, MPI_INT, 0, MPI_COMM_WORLD);

    /* Multiply the matricies using the local_a values we got from MPI_Scatter and 
    the global_B we got from MPI_Bcast */
    multiply_matricies(local_A, global_B, final_matrix, details, comm_sz);


    /*  gathering all the data into final_matrix using process 0 */
    MPI_Gather(final_matrix, (row_times_col) / comm_sz, MPI_INT, final_matrix, (row_times_col) / comm_sz , MPI_INT, 0,MPI_COMM_WORLD);

    if (my_rank == 0){
        end_time  = MPI_Wtime();
        double elapsed_time = end_time - start_time;
        printf("\nrunning on %d processor(s)\n""Elapsed time = %.7e seconds\n",
             comm_sz,elapsed_time);

        if (strncmp(details->flag, "I", 1) == 0 || strncmp(details->flag, "i", 1) == 0){
            print_matrix(final_matrix, details->n);
        }
    }

    MPI_Finalize();

}