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


	

#ifdef CACHE
	for (i=0; i<veclen; i++) {
		for (k=0; k<veclen; k++) {
			for (j=0;j<veclen;j++){
				*(c+(i*veclen+j)) += *(a+(i*veclen+k)) * *(b+(k*veclen+j)); 
			}
		}
	}
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
#elif CACHE8
	
	const int SIMD = 8;
	const int mod = veclen % SIMD;	
	__m256d _b2, _c2;
	double * buff2 = (double*) &_c2;
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
				
				_b2 = _mm256_set_pd( *(b+(k*veclen+(j+7))), *(b+(k*veclen+(j+6))), *(b+(k*veclen+(j+5))), *(b+(k*veclen+(j+4))) );
				_c2 = _mm256_mul_pd( _a, _b2 );

				c[i*veclen+j] += buff[3];
				c[i*veclen+j+1] += buff[2];
				c[i*veclen+j+2] += buff[1];
				c[i*veclen+j+3] += buff[0];	
				
				c[i*veclen+j+4] += buff2[3];
				c[i*veclen+j+5] += buff2[2];
				c[i*veclen+j+6] += buff2[1];
				c[i*veclen+j+7] += buff2[0];	

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

#elif SIMD8
	const int SIMD = 8;
	const int mod = veclen % SIMD;
	__m256d = _a2, _b2, _c2;
	for (i=0; i<veclen; i++) {
		for (j=0; j<veclen; j++) {
			*(c+(i*veclen+j)) = 0.0; 
			
			for (k=0;k<mod;k++){
				*(c+(i*veclen+j)) += *(a+(i*veclen+k)) * *(b+(k*veclen+j)); 
			}
			
			_c = _mm256_set1_pd(0.0);
			_c2 = _mm256_set1_pd(0.0);
			
			for (k=mod;k<veclen;k+=SIMD){
				_a = _mm256_set_pd( *(a+(i*veclen+(k+3))), *(a+(i*veclen+(k+2))), *(a+(i*veclen+(k+1))), *(a+(i*veclen+k)) );
				_b = _mm256_set_pd( *(b+((k+3)*veclen+j)), *(b+((k+2)*veclen+j)), *(b+((k+1)*veclen+j)), *(b+(k*veclen+j)) );
				_c = _mm256_fmadd_pd(_a, _b, _c);	
				
				_a2 = _mm256_set_pd( *(a+(i*veclen+(k+7))), *(a+(i*veclen+(k+6))), *(a+(i*veclen+(k+5))), *(a+(i*veclen+(k+4))) );
				_b2 = _mm256_set_pd( *(b+((k+7)*veclen+j)), *(b+((k+6)*veclen+j)), *(b+((k+5)*veclen+j)), *(b+((k+4)*veclen+j)) );
				_c2 = _mm256_fmadd_pd(_a2, _b2, _c2);	

			}
				_c = _mm256_add_pd( _c, _c2 );	
				*(c+(i*veclen+j)) += buff[0] + buff[1] + buff[2] + buff[3];
		}
	}
#ifdef STRIDE8
    const int stride = 8;

    mod = veclen % stride;
    for (i=0; i<veclen; i++) {
        for (j=0; j<veclen; j++) {
            *(c+(i*veclen+j)) = 0.0;
            for (k=0;k<mod;k++){
                *(c+(i*veclen+j)) += *(a+(i*veclen+k)) * *(b+(k*veclen+j)); 
            }
            for (k=mod;k<veclen;k+=stride) {
                *(c+(i*veclen+j)) += *(a+(i*veclen+k  )) * *(b+( k   *veclen+j)) 
                                   + *(a+(i*veclen+(k+1))) * *(b+((k+1)*veclen+j)) 
                                   + *(a+(i*veclen+(k+2))) * *(b+((k+2)*veclen+j)) 
                                   + *(a+(i*veclen+(k+3))) * *(b+((k+3)*veclen+j)) 
                                   + *(a+(i*veclen+(k+4))) * *(b+((k+4)*veclen+j)) 
                                   + *(a+(i*veclen+(k+5))) * *(b+((k+5)*veclen+j)) 
                                   + *(a+(i*veclen+(k+6))) * *(b+((k+6)*veclen+j)) 
                                   + *(a+(i*veclen+(k+7))) * *(b+((k+7)*veclen+j)); 
            }
        }
    }
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

#elif NEW

const int array_size = veclen*veclen;

for (i=0; i < array_size; i++){
	 *(c+i) = 0.0;
}

for (i=0; i<veclen; i++) {
	for (j=0; j<veclen; j++) {
		#pragma unroll(3)
		#pragma GCC ivdep
		for (k=0;k<veclen;k++){
			*(c+(i*veclen+j)) += *(a+(i*veclen+k)) * *(b+(k*veclen+j)); 
		}
       	}
}
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


