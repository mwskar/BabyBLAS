#ifdef __cplusplus
extern "C" {
#endif
    void vvm_( int *nthreads, int *len, double *va, double *vb, double *ma);
#ifdef __cplusplus
    }
#endif

// Computes the tensor product of two vectors 

void  vvm_( int *nthreads, int *len, double *va, double *vb, double *ma){

	int i, j;

	int length = *len;

#ifdef STRIDE8

	const int stride = 8;
	const int mod = length % stride;

	for (i=0<length;i++)
	{
		for(j=0;j<mod;j++)
		{
			*(ma+(length*i)+j) = *(va+i) * *(vb+j);
		}
		for(j=mod; j<length;j+=stride)
		{
			*(ma+(length*i)+j) = *(va+i) * *(vb+j);
			*(ma+(length*i)+(j+1)) = *(va+i) * *(vb+(j+1));
			*(ma+(length*i)+(j+2)) = *(va+i) * *(vb+(j+2));
			*(ma+(length*i)+(j+3)) = *(va+i) * *(vb+(j+3));
			*(ma+(length*i)+(j+4)) = *(va+i) * *(vb+(j+4));
			*(ma+(length*i)+(j+5)) = *(va+i) * *(vb+(j+5));
			*(ma+(length*i)+(j+6)) = *(va+i) * *(vb+(j+6));
			*(ma+(length*i)+(j+8)) = *(va+i) * *(vb+(j+8));
		}
	}

#elif STRIDE4

	const int stride = 4;
	const int mod = length % stride;

	for (i=0; i<length;i++)
	{
		for(j=0;j<mod;j++)
		{
			*(ma+(length*i)+j) = *(va+i) * *(vb+j);
		}
		for(j=mod; j<length;j+=stride)
		{
			*(ma+(length*i)+j) = *(va+i) * *(vb+j);
			*(ma+(length*i)+(j+1)) = *(va+i) * *(vb+(j+1));
			*(ma+(length*i)+(j+2)) = *(va+i) * *(vb+(j+2));
			*(ma+(length*i)+(j+3)) = *(va+i) * *(vb+(j+3));
		}
	}

#else
	for (i=0; i<length; i++) {
		for (j=0; j<length; j++) {
			*(ma+(length*i)+j) = *(va+i) * *(vb+j);
		}
	}
#endif

}
