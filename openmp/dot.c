#ifdef __cplusplus
extern "C" {
#endif
    double dot_( int *threads, int *len,  double *a, double *b, double*c );
#ifdef __cplusplus
    }
#endif

/*  O p e n M P     C O D E  */

#include <omp.h>

double dot_( int *threads, int *len,  double *a, double *b){

    int i;
    int veclen = *len;
    double sum = 0;

// Set the number of threads to use here

    omp_set_num_threads(*threads);


#pragma omp parallel shared(veclen) private(i) reduction(+:sum)
{
    for (i = 0; i < veclen; i++)
    {
        sum = sum + ( *(vec1 + i) * *(vec2 + i) );
    }
}

    return sum;
}


