# Define the makefile variables that will be used in building system

F90 = gfortran
CC  = gcc
FFLAGS = -cpp -O2
CFLAGS = -Ofast -fopenmp
MYLIBS = -L./serial -lserial_bblas -L./lbstime -llbstime
SYSLIBS = -lgomp -lpthread

TEST = -DDOT
DOT = 
MVV =  
VVM = 
MMM = 