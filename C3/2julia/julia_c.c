#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"

int julia(Real re, Real im)
{
    int ret = MAX_IT+1;

    Real re2 = re * re;
    Real im2 = im * im;

    for (int it = 0; it < MAX_IT; ++it)
    {
        im = 2.0 * re * im + c_im;
        re = re2 - im2 + c_re;

        re2 = re * re;
        im2 = im * im;

        if (re2 + im2 > r2)
        {
            ret = it;
            break;
        }
    }
    return ret;
}


int main(void)
{
    int * restrict A = malloc(N * N * sizeof(int));
    if (A == NULL) {
        return 1;
    }

    const Real dx = Lx/(N-1);
    const Real dy = Ly/(N-1);

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    for (int j = 0; j < N; ++j)
    {
        for (int i = 0; i < N; ++i)
        {
            int id = j * N + i;

            Real re = x0 + i * dx;
            Real im = y0 + j * dy;

            A[id] = julia(re, im);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);

    double elapsed =
        (double)(t1.tv_sec - t0.tv_sec) +
        1e-9 * (double)(t1.tv_nsec - t0.tv_nsec);

    printf("Loop time: %.6f s\n", elapsed);

	printf("%d\n",A[9240]);

    FILE *fp = fopen("output_ser.txt", "w");
    if (fp == NULL) {
        perror("fopen");
        return 1;
    }

    for (int i = 0; i < N*N; ++i)
        fprintf(fp, "%d\n", A[i]);

    free(A);
    return 0;
}


       //clock_t t0 = clock();
    //clock_t t1 = clock();

    //double elapsed = (double)(t1 - t0) / CLOCKS_PER_SEC;
    //printf("Loop time: %.6f s\n", elapsed);
