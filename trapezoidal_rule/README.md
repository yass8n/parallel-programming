In order to compile, simply run 'make' from the command line and two executables will be created ( ./find_n and ./trap_run)...if that does not work, then run "clang find_n.c project.c -lm -o find_n " to create the find_n executable and run "mpicc trap_run.c project.c -std=c99 -o trap_run" to create the trap_run executable in the current directory

