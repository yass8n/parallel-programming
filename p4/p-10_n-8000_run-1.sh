#!/bin/sh

	#PBS -N p-10_n-8000_run-1

	#PBS -j oe

	#PBS -l walltime=1:00:00

	#PBS -l select=1:ncpus=10

	export OMP_NUM_THREADS=10

	./parallel_execute 8000 10
