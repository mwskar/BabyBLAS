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
    int length = *len;

    for (int i = 0; i < length; i++)
    {
        *(vb + i) = 0.0;
    }

    for (int i = 0; i < length; i++) // row
    {
        for (int j = 0; j < length; j++) // col
        {
            *(vb + i) = *(vb + i) + ( *(va + j) * *(ma + (length * i) + j) );
        }

    }
}