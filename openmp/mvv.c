#ifdef __cplusplus
extern "C" {
#endif
    void mvv_( int *threads, int *len,  double *a, double *b, double*c );
#ifdef __cplusplus
    }
#endif

/*  O p e n M P     C O D E  */

#include <omp.h>

void mvv_( int *threads, int *len,  double *a, double *b, double *ma ){

    int i, j;
    int veclen = *len;

// Set the number of threads to use here

    omp_set_num_threads(*threads);

#pragma omp parallel shared(veclen) private(i,j)
{
    for (int i = 0; i < veclen; i++) // row
    {
        *(bv + i) = 0.0;
        for (int j = 0; j < veclen; j++) // col
        {
            *(vb + i) = *(vb + i) + ( *(va + j) * *(ma + (length * i) + j) );
        }

    }
}
}


