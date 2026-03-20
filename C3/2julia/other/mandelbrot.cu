#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "config.h"

// --- KERNEL E FUNZIONI DEVICE ---

// Usiamo nomi per i parametri che NON siano presenti in config.h (es. p_ invece di nomi semplici)
__device__ int mandelbrot_gpu(Real p_cre, Real p_cim) {
    int ret = (int)MAX_IT + 1;

    Real z_re = 0.0;
    Real z_im = 0.0;
    Real z_re2 = 0.0;
    Real z_im2 = 0.0;

    for (int it = 0; it < MAX_IT; ++it) {
        // Formula: z = z^2 + c
        z_im = 2.0 * z_re * z_im + p_cim;
        z_re = z_re2 - z_im2 + p_cre;

        z_re2 = z_re * z_re;
        z_im2 = z_im * z_im;

        // Nota: r2 deve essere una macro in config.h
        if (z_re2 + z_im2 > (Real)r2) {
            ret = it;
            break;
        }
    }
    return ret;
}

__global__ void mandelbrot_kernel(int *p_A, int p_n, Real p_dx, Real p_dy) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

    if (i < p_n && j < p_n) {
        int id = j * p_n + i;

        // x0 e y0 devono essere definiti in config.h
        Real c_re_pix = (Real)x0 + (Real)i * p_dx;
        Real c_im_pix = (Real)y0 + (Real)j * p_dy;

        p_A[id] = mandelbrot_gpu(c_re_pix, c_im_pix);
    }
}

// --- MAIN ---

int main(void) {
    // Usiamo variabili locali per evitare conflitti con macro N, Lx, Ly
    int n_val = (int)N;
    size_t total_size = (size_t)n_val * n_val * sizeof(int);

    int *h_A = (int *)malloc(total_size);
    if (h_A == NULL) return 1;

    int *d_A;
    cudaMalloc((void **)&d_A, total_size);

    // Calcolo dx e dy separato per evitare errori "expected identifier"
    Real val_Lx = (Real)Lx;
    Real val_Ly = (Real)Ly;
    Real dx_local = val_Lx / (Real)(n_val - 1);
    Real dy_local = val_Ly / (Real)(n_val - 1);

    dim3 threads(16, 16);
    dim3 blocks((n_val + threads.x - 1) / threads.x,
                (n_val + threads.y - 1) / threads.y);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start);

    // Chiamata al kernel con 4 argomenti puliti
    mandelbrot_kernel<<<blocks, threads>>>(d_A, n_val, dx_local, dy_local);

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float ms = 0;
    cudaEventElapsedTime(&ms, start, stop);

    cudaMemcpy(h_A, d_A, total_size, cudaMemcpyDeviceToHost);

    printf("Mandelbrot GPU time: %.6f s\n", ms / 1000.0);

    FILE *fp = fopen("output_mandelbrot.txt", "w");
    if (fp) {
        for (int i = 0; i < n_val * n_val; ++i) fprintf(fp, "%d\n", h_A[i]);
        fclose(fp);
    }

    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    cudaFree(d_A);
    free(h_A);

    return 0;
}
