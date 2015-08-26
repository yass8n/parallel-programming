//Yaseen Aniss
/* This file calculates the value of n to use in the MPI runs.
It does this by starting at ‘value = 2000000’ trapezoids and calculating the integral. 
If we haven’t gotten the precision we wanted, then it increments the num_traps by 1 and it tries again 
over and over until it finally gets the precision we want (14 sig figs)...then it breakes out of the 
while loop and prints the stats.
*/

#include <stdlib.h>
#include <stdio.h>
#include "project.h"

static void find_n(){  
    double approx_val = 0;
    long num_traps = 2031000;
    long inc = 1;
    double abs_true_error;
    project * proj = malloc(sizeof(project));
    initialize_project(proj);
    int keep_trying = 1;
    while(keep_trying){

        /* get the trapazoidal estimation */
        approx_val = trapezoidal_estimate(proj, 100, 600, num_traps);

        abs_true_error = calc_abs_true_error(proj->true_val, approx_val);

        /* check if we reached our goal...if so then break out of the loop */
        if (is_goal_reached(abs_true_error, proj->min_error_val, proj->true_val, approx_val)){ 
           keep_trying = 0;
        } 
        /*Printing stats to make sure Jaguar hasn't frozen */
           printf("num traps    = %ld\n", num_traps);
           printf("actual value = %.14lg\n", proj->true_val);
           printf("trap calc    = %.14lg\n", approx_val);
           printf("abs t error  = %.14lg\n\n", abs_true_error);
           num_traps += inc;  

    } 
    free(proj);
}

int main(int argc, char * argv[]){
  find_n();
  return 0;
}
