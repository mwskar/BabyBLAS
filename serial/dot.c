#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

    double dot_(int *threads, int *len, double *vec1, double *vec2);

#ifdef __cplusplus
}
#endif

double dot_(int *threads, int *len, double *vec1, double *vec2)
{
    
    double sum = 0.0;
    int length = *len;

#ifdef STRIDE8

    const int stride = 8;
    const int mod = length % stride;
    
    for (int i=0; i<mod; i++)
    {
        sum += ( *(vec1 + i) * *(vec2 + i) );
    }
    for (int i = mod; i < length; i+=stride)
    {
        sum += ( *(vec1 + i) * *(vec2 + i) )
            + ( *(vec1 + (i+1)) * *(vec2 + (i+1)) )
            + ( *(vec1 + (i+2)) * *(vec2 + (i+2)) )
            + ( *(vec1 + (i+3)) * *(vec2 + (i+3)) )
            + ( *(vec1 + (i+4)) * *(vec2 + (i+4)) )
            + ( *(vec1 + (i+5)) * *(vec2 + (i+5)) )
            + ( *(vec1 + (i+6)) * *(vec2 + (i+6)) )
            + ( *(vec1 + (i+7)) * *(vec2 + (i+7)) );

    } 
#elif STRIDE4
    const int stride = 4;
    const int mod = length % stride;
    
    for (int i=0; i<mod; i++)
    {
        sum += ( *(vec1 + i) * *(vec2 + i) );
    }
    for (int i = mod; i < length; i+=stride)
    {
        sum += ( *(vec1 + i) * *(vec2 + i) )
            + ( *(vec1 + (i+1)) * *(vec2 + (i+1)) )
            + ( *(vec1 + (i+2)) * *(vec2 + (i+2)) )
            + ( *(vec1 + (i+3)) * *(vec2 + (i+3)) );

    }       
#else
    for (int i = 0; i < length; i++)
    {
        sum += ( *(vec1 + i) * *(vec2 + i) );
    }    
#endif

    return (sum);
}
