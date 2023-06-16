#ifdef __cplusplus
extern "C" {
#endif
    void mmm_( int *threads, int *len,  double *a, double *b, double *c );
#ifdef __cplusplus
    }
#endif

#include <stdio.h>
#include <immintrin.h>

/*  S E R I A L   C O D E  */

void mmm_( int *threads, int *len,  double *a, double *b, double *c ){

	/* in serial code, *threads not used. It is retained here so the code can be called
	 * identically to the threaded methods.
 	*/

    	int i, j, k;
    	const int veclen = *len;

	__m256d _a, _b, _c;
	double * buff = (double*)&_c;


	
// Changed order of access to apply basic cache optimizations
#ifdef CACHE
	for (i=0; i<veclen; i++) {
		for (k=0; k<veclen; k++) {
			for (j=0;j<veclen;j++){
				*(c+(i*veclen+j)) += *(a+(i*veclen+k)) * *(b+(k*veclen+j)); 
			}
		}
	}
// Changed order of access to apply basic cache optimizations with SIMD
#elif CACHE4 

	const int SIMD = 4;
	const int mod = veclen % SIMD;	
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
#elif SIMD4
	const int SIMD = 4;
	const int mod = veclen % SIMD;	
	for (i=0; i<veclen; i++) {
		for (j=0; j<veclen; j++) {
			*(c+(i*veclen+j)) = 0.0; 
			
			for (k=0;k<mod;k++){
				*(c+(i*veclen+j)) += *(a+(i*veclen+k)) * *(b+(k*veclen+j)); 
			}
			
			_c = _mm256_set1_pd(0.0);
			
			for (k=mod;k<veclen;k+=SIMD){
				_a = _mm256_set_pd( *(a+(i*veclen+(k+3))), *(a+(i*veclen+(k+2))), *(a+(i*veclen+(k+1))), *(a+(i*veclen+k)) );
				_b = _mm256_set_pd( *(b+((k+3)*veclen+j)), *(b+((k+2)*veclen+j)), *(b+((k+1)*veclen+j)), *(b+(k*veclen+j)) );
				_c = _mm256_fmadd_pd(_a, _b, _c);	
				
			}	
				*(c+(i*veclen+j)) += buff[0] + buff[1] + buff[2] + buff[3];
		}
	}
// Regular matrix x matrix but with increased stride (loop unrolling) to
// 		try and get the compiler to vectorize
#elif STRIDE4
const int stride = 4;

mod = veclen % stride;
for (i=0; i<veclen; i++) {
    for (j=0; j<veclen; j++) {
       *(c+(i*veclen+j)) = 0.0;
       for (k=0;k<mod;k++){
          *(c+(i*veclen+j)) += *(a+(i*veclen+k)) * *(b+(k*veclen+j)); 
       }
       #pragma simd
       for (k=mod;k<veclen;k+=stride) {
          *(c+(i*veclen+j)) += *(a+(i*veclen+k  )) * *(b+( k   *veclen+j)) 
                             + *(a+(i*veclen+k+1)) * *(b+((k+1)*veclen+j)) 
                             + *(a+(i*veclen+k+2)) * *(b+((k+2)*veclen+j)) 
                             + *(a+(i*veclen+k+3)) * *(b+((k+3)*veclen+j)); 
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

	for (i = 0; i < veclen; i++){
		for(j = 0; j < velclen; j++){
			*(b2 + (i*veclen + j) ) = *(b + (j*veclen + i));
		}
	}

	buff = (double*)&_c;

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
for (i=0; i<veclen; i++) {
		for (j=0; j<veclen; j++) {
			for (k=0;k<veclen;k++){
				*(c+(i*veclen+j)) += *(a+(i*veclen+k)) * *(b+(k*veclen+j)); 
			}
		}
	}
#endif



}


