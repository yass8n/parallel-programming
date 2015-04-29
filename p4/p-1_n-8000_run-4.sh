#!/bin/sh

	#PBS -N p-1_n-8000_run-4

	#PBS -j oe

	#PBS -l walltime=1:00:00

	#PBS -l select=1:ncpus=1

	export OMP_NUM_THREADS=1

	./parallel_execute 8000 1