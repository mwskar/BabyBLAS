#ifdef __cplusplus
extern "C" {
#endif
    void vvm_( int *threads, int *len,  double *a, double *b, double*ma );
#ifdef __cplusplus
    }
#endif

/*  O p e n M P     C O D E  */

#include <omp.h>

void vvm_( int *threads, int *len,  double *a, double *b, double *ma ){

    int i, j;
    int veclen = *len;
    int sharelen = veclen;

// Set the number of threads to use here

    omp_set_num_threads(*threads);

#pragma omp parallel for shared(veclen) private(i,j)
	for (i=0; i<veclen; i++) {
		for (j=0; j<veclen; j++) {
			*(ma+(veclen*i)+j) = *(a+i) * *(b+j);
		}
	}

}


