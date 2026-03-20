#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#include "config.h"

int julia(Real re, Real im)
{
    int ret = MAX_IT + 1;

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
    int *restrict A = malloc(N * N * sizeof(int));
    if (A == NULL) {
        return 1;
    }

    const Real dx = Lx / (N - 1);
    const Real dy = Ly / (N - 1);

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nth = omp_get_num_threads();

        for (int id = tid; id < N * N; id += nth)
        {
            int j = id / N;
            int i = id % N;

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
    printf("%d\n", A[9240]);

    FILE *fp = fopen("output_omp.txt", "w");
    if (fp == NULL) {
        perror("fopen");
        free(A);
        return 1;
    }

    for (int i = 0; i < N * N; ++i)
        fprintf(fp, "%d\n", A[i]);

    fclose(fp);
    free(A);
    return 0;
}
