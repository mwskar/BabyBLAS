#ifdef __cplusplus
extern "C" {
#endif
    void mmm_( int *threads, int *len,  double *a, double *b, double*c );
#ifdef __cplusplus
    }
#endif

/*  O p e n M P     C O D E  */

#include <omp.h>
#include <immintrin.h>
#include <stdio.h>

void mmm_( int *threads, int *len,  double *a, double *b, double *c ){

    int i, j, k;
    int veclen = *len;
    int sharelen = veclen;

// Set the number of threads to use here

    omp_set_num_threads(*threads);

// Changed order of access to apply basic cache optimizations
#ifdef CACHE
	
	#pragma omp parallel for shared(veclen) private(i,j,k)
	for (i=0; i<veclen; i++) {
		for (k=0; k<veclen; k++) {
			for (j=0;j<veclen;j++){
				*(c+(i*veclen+j)) += *(a+(i*veclen+k)) * *(b+(k*veclen+j)); 
			}
		}
	}

//	Currently the best working version. Transpose the matrix so both matrices
// 		are accessed in a cache optimal manner and then use SIMD.
#elif transposeSIMD
	const int SIMD = 4;
	const int mod = veclen % SIMD;
	__m256d _a, _b, _c;
	double * buff;

	double *b2 = (double *) calloc(veclen*veclen, sizeof(double));

	#pragma omp parallel for shared(veclen) private(i)
	for (i = 0; i < veclen; i++){
		for(j = 0; j < velclen; j++){
			*(b2 + (i*veclen + j) ) = *(b + (j*veclen + i));
		}
	}

	#pragma opm parallel shared(veclen, a, b2, c) private(_a, _b, _c, buff)
	buff = (double*)&_c;

	#pragma omp for
	for (i = 0; i < veclen; i++){
		for(j = 0; j < veclen; j++){
			*(c+i*veclen=j) = 0;

			for(k = 0; k < mod; k++){
				*(c+(i*veclen+j)) += *(a+(i*veclen+k)) * *(b2+(i*veclen+k))
			}

			_c = _mm256_set1_pd(0.0);

			for(k = mod; k < veclen; k++){
				_a = _mm256_set_pd( *(a+(i*veclen+(k+3))), *(a+(i*veclen+(k+2))), *(a+(i*veclen+(k+1))), *(a+(i*veclen+k)));
				_b = _mm256_set_pd( *(b2+(i*veclen+(k+3))), *(b2+(i*veclen+(k+2))),*(b2+(i*veclen+(k+1))),*(b2+(i*veclen+k)));
				_c = _mm256_fmadd_pd(_a, _b, _c);
			}

			*(c+(i*veclen+j)) += buff[0] + buff[1] + buff[2] + buff[3];
		}

	}
	free(b2);

//	Regualar matrix x matrix
#else
	#pragma omp parallel for shared(veclen) private(i,j,k)
	for (i=0; i<veclen; i++) {
      		for (j=0; j<veclen; j++) {
            		*(c+(i*veclen+j)) = 0.0;
            		for (k=0;k<veclen;k++){
                		*(c+(i*veclen+j)) += *(a+(i*veclen+k)) * *(b+(k*veclen+j)); 
            		}
        	}
    	}
#endif
}


