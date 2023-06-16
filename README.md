# baby_blas

A linear algebra library for applying principles learned in 'High Performance Scientific Computing'.

Operations optimized for both single and multi-threaded applications. Multi-threaded in both Pthreads and OpenMP.
Most of the optimizations are focused on the matrix x matrix operations.

To change which version of the code is run, please go into 'Makefile.inc' and change which of the libraries comes first in the 'MyLibs' section.

To build the program binary: make
To clean the binary to be re-compiled for a specific application: make reset

Code for timing libraries and for a Pthreads template provided by Dr. Pounds at Mercer University.

Operations: Dot Product, Vector x Vector, Vector x Matrix, Matrix x Vector, and Matrix x Matrix