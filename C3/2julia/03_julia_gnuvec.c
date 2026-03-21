#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "config.h"
#include "functions.h"

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

    const v4f dxv = {dx, dx, dx, dx};
    const v4f dyv = {dy, dy, dy, dy};
    const v4f x0v = {x0, x0, x0, x0};
    const v4f y0v = {y0, y0, y0, y0};

    const v4i v_init = { MAX_IT + 1, MAX_IT + 1, MAX_IT + 1, MAX_IT + 1 };

    TIC(loop);

    OMP_FOR
    for (int id = 0; id < NN_pad; id += 4)
    {
        v4i idv={id + 0,id + 1,id + 2,id + 3};
        v4i iv=idv%N;
        v4i jv=idv/N;

        v4f tempx=__builtin_convertvector(iv, v4f);
        v4f tempy=__builtin_convertvector(jv, v4f);

        v4f v_re=dxv*tempx+x0v;
        v4f v_im=dyv*tempy+y0v;

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
