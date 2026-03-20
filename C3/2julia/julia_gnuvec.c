#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#include "config.h"

typedef float  v4f __attribute__((vector_size(4 * sizeof(float))));
typedef int    v4i __attribute__((vector_size(4 * sizeof(int))));
typedef unsigned int v4u __attribute__((vector_size(4 * sizeof(unsigned int))));

int main(void)
{
    const int NN = N * N;
    const int NN_pad = ((NN + 3) / 4) * 4;

    int *restrict A = malloc((size_t)NN_pad * sizeof(int));
    if (A == NULL) {
        return 1;
    }

    const float dx = Lx / (N - 1);
    const float dy = Ly / (N - 1);

    const v4f v_c_re = { c_re, c_re, c_re, c_re };
    const v4f v_c_im = { c_im, c_im, c_im, c_im };
    const v4f v_r2   = { r2,   r2,   r2,   r2   };

    const v4i v_init = { MAX_IT + 1, MAX_IT + 1, MAX_IT + 1, MAX_IT + 1 };

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    for (int id = 0; id < NN_pad; id += 4)
    {
        int i0 = (id + 0) % N;  int j0 = (id + 0) / N;
        int i1 = (id + 1) % N;  int j1 = (id + 1) / N;
        int i2 = (id + 2) % N;  int j2 = (id + 2) / N;
        int i3 = (id + 3) % N;  int j3 = (id + 3) / N;

        v4f v_re = {
            x0 + i0 * dx,
            x0 + i1 * dx,
            x0 + i2 * dx,
            x0 + i3 * dx
        };

        v4f v_im = {
            y0 + j0 * dy,
            y0 + j1 * dy,
            y0 + j2 * dy,
            y0 + j3 * dy
        };

        v4i v_A = v_init;

        v4f v_re2 = v_re * v_re;
        v4f v_im2 = v_im * v_im;

        for (int it = 0; it < MAX_IT; ++it)
        {
            v_im = 2.0f * (v_re * v_im) + v_c_im;
            v_re = (v_re2 - v_im2) + v_c_re;

            v_re2 = v_re * v_re;
            v_im2 = v_im * v_im;

            v4f v_norm2 = v_re2 + v_im2;

            /* mask_out: lane = ~0 se fuori, 0 altrimenti */
            v4u mask_out = (v4u)(v_norm2 > v_r2);

            /* mask_unset: lane = ~0 se ancora non assegnato */
            v4u mask_unset = (v4u)(v_A == v_init);

            v4u mask = mask_out & mask_unset;

            /* equivalente di vbslq_s32(mask, it, v_A) */
            v4i v_it = { it, it, it, it };
            v_A = (v4i)(((v4u)v_it & mask) | ((v4u)v_A & ~mask));

            /* break se nessuna lane è ancora unset */
            if (!(mask_unset[0] | mask_unset[1] | mask_unset[2] | mask_unset[3])) {
                break;
            }
        }

        A[id + 0] = v_A[0];
        A[id + 1] = v_A[1];
        A[id + 2] = v_A[2];
        A[id + 3] = v_A[3];
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);

    double elapsed =
        (double)(t1.tv_sec - t0.tv_sec) +
        1e-9 * (double)(t1.tv_nsec - t0.tv_nsec);

    printf("Loop time: %.6f s\n", elapsed);

    FILE *fp = fopen("output_gnuvec.txt", "w");
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
