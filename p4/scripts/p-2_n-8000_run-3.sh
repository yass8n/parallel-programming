#!/bin/sh

    #PBS -N p-2_n-8000_run-3

    #PBS -j oe

    #PBS -l walltime=1:00:00

    #PBS -l select=1:ncpus=2

    export OMP_NUM_THREADS=2

    ./parallel_execute 8000 2