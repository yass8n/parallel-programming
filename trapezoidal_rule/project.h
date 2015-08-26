//Yaseen Aniss
/*
 Project is the object used for many of the common functions needed for this assignment.
 It is used by the find_n program as well as the trap_run program. Its main objective
 is to calculate the integral and the relative true error 
 */

#ifndef _project_H
#define _project_H

#include <complex.h>

typedef struct {
    double (*f_of_x)(double); //this is a function pointer that will point to 'func_to_integrate'
    double true_val;
    double min_error_val;
} project;


void initialize_project(project *);

double calc_h(double, double, int);

double calc_abs_true_error(double, double);

double trapezoidal_estimate(project *, double, double , double );

int is_goal_reached(double, double, double, double);

int fourteen_digit_comparison(double, double);

#endif