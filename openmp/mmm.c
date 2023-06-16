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

#ifdef CACHE
	
	#pragma omp parallel for shared(veclen) private(i,j,k)
	for (i=0; i<veclen; i++) {
		for (k=0; k<veclen; k++) {
			for (j=0;j<veclen;j++){
				*(c+(i*veclen+j)) += *(a+(i*veclen+k)) * *(b+(k*veclen+j)); 
			}
		}
	}
#elif CACHE4
	
	__m256d _a, _b, _c;
	double * buff = (double*)&_c;
	const int SIMD = 4;
	const int mod = veclen % SIMD;	
	//printf("Before parallel\n");
	
	#pragma omp parallel for shared(veclen, SIMD, mod, buff, _c) private(i,j,k, _a,_b)
	for (i=0; i<veclen; i++) {
		for (k=0; k<veclen; k++) {
			_a = _mm256_set1_pd(a[i*veclen+k]);

			for (j=0;j<mod;j++){
				*(c+(i*veclen+j)) += *(a+(i*veclen+k)) * *(b+(k*veclen+j)); 
			}
			
			for (j=mod;j<veclen;j+=SIMD){
				//_c = _mm256_set1_pd(0.0);
				_b = _mm256_set_pd( *(b+(k*veclen+(j+3))), *(b+(k*veclen+(j+2))), *(b+(k*veclen+(j+1))), *(b+(k*veclen+j)) );
				_c = _mm256_mul_pd( _a, _b );
				c[i*veclen+j] += buff[3];
				c[i*veclen+j+1] += buff[2];
				c[i*veclen+j+2] += buff[1];
				c[i*veclen+j+3] += buff[0];	
			}
		}
	}
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


