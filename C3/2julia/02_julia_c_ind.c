#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "functions.h"

int main(void)
{
    int * restrict A = malloc(N * N * sizeof(int));
    if (A == NULL) {
        return 1;
    }

    const Real dx = Lx/(N-1);
    const Real dy = Ly/(N-1);

    TIC(loop);

    int i = 0, j = 0;
    for (int ind = 0; ind < N * N; ++ind)
    {
        Real re = x0 + (Real)i * dx;
        Real im = y0 + (Real)j * dy;

        A[ind] = julia(re, im);

        ++i;
        if (i == N)
        {
            i = 0;
            ++j;
        }
    }

//    for (int ind = 0; ind < N*N; ++ind)
//    {
//        int i=ind%N;
//        int j=ind/N;
        
//        Real re = x0 + i * dx;
//        Real im = y0 + j * dy;

//        A[ind] = julia(re, im);
//    }

    TOC(loop,"Loop time");

#ifdef WRITE_TO_FILE
    write_array_to_file("output_ser.txt", A, N * N);
#else
    TIC(sum);
    printf("Sum = %lld\n", sum_array(A, N * N));
    TOC(sum,"Sum time");
#endif

    free(A);
    return 0;
}
