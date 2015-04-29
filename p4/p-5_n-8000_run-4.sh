#!/bin/sh

	#PBS -N p-5_n-8000_run-4

	#PBS -j oe

	#PBS -l walltime=1:00:00

	#PBS -l select=1:ncpus=5

	export OMP_NUM_THREADS=5

	./parallel_execute 8000 5