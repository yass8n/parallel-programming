#!/bin/sh

    #PBS -N p-30_n-8000_run-5

    #PBS -j oe

    #PBS -l walltime=1:00:00

    #PBS -l select=1:ncpus=30

    export OMP_NUM_THREADS=30

    ./parallel_execute 8000 30