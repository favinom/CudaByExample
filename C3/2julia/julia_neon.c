#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <arm_neon.h>

#include "config.h"

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

    const float32x4_t v_c_re = vdupq_n_f32(c_re);
    const float32x4_t v_c_im = vdupq_n_f32(c_im);
    const float32x4_t v_r2   = vdupq_n_f32(r2);

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    for (int id = 0; id < NN_pad; id += 4)
    {
        int i0 = (id + 0) % N;  int j0 = (id + 0) / N;
        int i1 = (id + 1) % N;  int j1 = (id + 1) / N;
        int i2 = (id + 2) % N;  int j2 = (id + 2) / N;
        int i3 = (id + 3) % N;  int j3 = (id + 3) / N;

        float32x4_t v_re = {
            x0 + i0 * dx,
            x0 + i1 * dx,
            x0 + i2 * dx,
            x0 + i3 * dx
        };

        float32x4_t v_im = {
            y0 + j0 * dy,
            y0 + j1 * dy,
            y0 + j2 * dy,
            y0 + j3 * dy
        };

int32x4_t v_A = vdupq_n_s32(MAX_IT + 1);

float32x4_t v_re2 = vmulq_f32(v_re, v_re);
float32x4_t v_im2 = vmulq_f32(v_im, v_im);

for (int it = 0; it < MAX_IT; ++it)
{
    v_im = vmlaq_n_f32(v_c_im, vmulq_f32(v_re, v_im), 2.0f);
    v_re = vaddq_f32(vsubq_f32(v_re2, v_im2), v_c_re);

    v_re2 = vmulq_f32(v_re, v_re);
    v_im2 = vmulq_f32(v_im, v_im);

    float32x4_t v_norm2 = vaddq_f32(v_re2, v_im2);

    uint32x4_t mask_out   = vcgtq_f32(v_norm2, v_r2);
    uint32x4_t mask_unset = vceqq_s32(v_A, vdupq_n_s32(MAX_IT + 1));
    uint32x4_t mask       = vandq_u32(mask_out, mask_unset);

    v_A = vbslq_s32(mask, vdupq_n_s32(it), v_A);

    if (vmaxvq_u32(mask_unset) == 0)
    {
        break;
    }
}

vst1q_s32(&A[id], v_A);
}

    clock_gettime(CLOCK_MONOTONIC, &t1);

    double elapsed =
        (double)(t1.tv_sec - t0.tv_sec) +
        1e-9 * (double)(t1.tv_nsec - t0.tv_nsec);

    printf("Loop time: %.6f s\n", elapsed);

    FILE *fp = fopen("output_neo.txt", "w");
    if (fp == NULL) {
        perror("fopen");
        return 1;
    }

    for (int i = 0; i < N*N; ++i)
        fprintf(fp, "%d\n", A[i]);

fclose(fp);

    free(A);
    return 0;
}