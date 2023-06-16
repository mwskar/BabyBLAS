#ifdef __cplusplus
extern "C" {
#endif
    void mvv_(int *threads, int *len, double *va, double *vb, double *ma);
#ifdef __cplusplus
    }
#endif

// Vector B wil be the resultant vector

void mvv_(int *threads, int *len, double *va, double *vb, double *ma)
{
    int i,j;
    int length = *len;

#ifdef STRIDE8

    const int stride = 8;
    const int mod = length % stride;

    for (i=0; i<mod;i++)
    {
        *(vb + i) = 0.0;
    }
    for (i=mod; i<length; i+=stride)
    {
        *(vb + i) = 0.0;
        *(vb + (i+1)) = 0.0;
        *(vb + (i+2)) = 0.0;
        *(vb + (i+3)) = 0.0;
        *(vb + (i+4)) = 0.0;
        *(vb + (i+5)) = 0.0;
        *(vb + (i+6)) = 0.0;
        *(vb + (i+7)) = 0.0;
    }

    for (i=0; i<length; i++)
    {
        for(j=0;j<mod;j++)
        {
            *(vb + i) += *(vb + i) + ( *(va + j) * *(ma + (length * i) + j) );
        }
        
        for(j=mod; j<length; j+=stride)
        {
            *(vb + i) += ( *(va + j) * *(ma + (length * i) + j) )
                      +  ( *(va + (j+1)) * *(ma + (length * i) + (j+1)) )
                      +  ( *(va + (j+2)) * *(ma + (length * i) + (j+2)) )
                      +  ( *(va + (j+3)) * *(ma + (length * i) + (j+3)) )
                      +  ( *(va + (j+4)) * *(ma + (length * i) + (j+4)) )
                      +  ( *(va + (j+5)) * *(ma + (length * i) + (j+5)) )
                      +  ( *(va + (j+6)) * *(ma + (length * i) + (j+6)) )
                      +  ( *(va + (j+7)) * *(ma + (length * i) + (j+7)) );
        }
    }

#elif STRIDE4
    const int stride = 4;
    const int mod = length % stride;
    
    for (i=0; i<mod;i++)
    {
        *(vb + i) = 0.0;
    }
    for (i=mod; i<length; i+=stride)
    {
        *(vb + i) = 0.0;
        *(vb + (i+1)) = 0.0;
        *(vb + (i+2)) = 0.0;
        *(vb + (i+3)) = 0.0;
    }

    for (i=0; i<length; i++)
    {
        for(j=0;j<mod;j++)
        {
            *(vb + i) += *(vb + i) + ( *(va + j) * *(ma + (length * i) + j) );
        }
        
        for(j=mod; j<length; j+=stride)
        {
            *(vb + i) += ( *(va + j) * *(ma + (length * i) + j) )
                      +  ( *(va + (j+1)) * *(ma + (length * i) + (j+1)) )
                      +  ( *(va + (j+2)) * *(ma + (length * i) + (j+2)) )
                      +  ( *(va + (j+3)) * *(ma + (length * i) + (j+3)) );
        }
    }

#else
    for (i = 0; i < length; i++) // row
    {
        *(vb + i) = 0.0;
        for (j = 0; j < length; j++) // col
        {
            *(vb + i) += ( *(va + j) * *(ma + (length * i) + j) );
        }

    }
#endif
}