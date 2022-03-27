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
    printf("In function\n");
    double sum = 0;
    for (int i = 0; i < *len; i++)
    {
        sum = sum + ( *(vec1 + i) * *(vec2 + i) );
    }

    return (sum);
}
