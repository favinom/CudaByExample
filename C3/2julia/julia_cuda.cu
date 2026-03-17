#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "config.h"

// --- KERNEL E FUNZIONI DEVICE ---

// Funzione che calcola l'iterazione di Julia per un singolo punto
// Il qualificatore __device__ indica che gira sulla GPU
__device__ int julia_gpu(Real re, Real im) {
    int ret = MAX_IT + 1;
    Real re2 = re * re;
    Real im2 = im * im;

    for (int it = 0; it < MAX_IT; ++it) {
        // Calcolo im prima di aggiornare re per non perdere il valore originale
        Real next_im = 2.0 * re * im + c_im;
        re = re2 - im2 + c_re;
        im = next_im;

        re2 = re * re;
        im2 = im * im;

        if (re2 + im2 > r2) {
            ret = it;
            break;
        }
    }
    return ret;
}

// Kernel principale che distribuisce il lavoro sulla griglia di thread
__global__ void julia_kernel(int *A, int n_val, Real dx_val, Real dy_val) {
    // Mappatura dei thread CUDA sulle coordinate (i, j) della matrice
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

    // Controllo boundary per evitare accessi fuori memoria se N non è multiplo del blocco
    if (i < n_val && j < n_val) {
        int id = j * n_val + i;

        Real re = x0 + i * dx_val;
        Real im = y0 + j * dy_val;

        A[id] = julia_gpu(re, im);
    }
}

// --- MAIN ---

int main(void) {
    size_t size = N * N * sizeof(int);

    // Allocazione memoria Host (CPU)
    int *h_A = (int *)malloc(size);
    if (h_A == NULL) {
        fprintf(stderr, "Errore allocazione CPU\n");
        return 1;
    }

    // Allocazione memoria Device (GPU)
    int *d_A;
    cudaError_t err = cudaMalloc((void **)&d_A, size);
    if (err != cudaSuccess) {
        fprintf(stderr, "Errore allocazione GPU: %s\n", cudaGetErrorString(err));
        return 1;
    }

    const Real dx = Lx / (N - 1);
    const Real dy = Ly / (N - 1);

    // Configurazione della griglia CUDA (blocchi da 16x16 thread)
    dim3 threadsPerBlock(16, 16);
    dim3 blocksPerGrid((N + threadsPerBlock.x - 1) / threadsPerBlock.x,
                       (N + threadsPerBlock.y - 1) / threadsPerBlock.y);

    // --- TIMING CON CUDA EVENTS ---
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start); // Inizio timer

    // Lancio del kernel
    julia_kernel<<<blocksPerGrid, threadsPerBlock>>>(d_A, N, dx, dy);

    cudaEventRecord(stop); // Fine timer

    // Sincronizzazione: attendiamo che la GPU finisca tutto il lavoro
    cudaEventSynchronize(stop);

    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);

    // --- RICEZIONE DATI ---
    // Copiamo i risultati dalla memoria GPU alla memoria RAM della CPU
    cudaMemcpy(h_A, d_A, size, cudaMemcpyDeviceToHost);

    printf("GPU Kernel time: %.6f s\n", milliseconds / 1000.0);
    printf("Risultato test A[9240]: %d\n", h_A[9240]);

    // Scrittura su file (struttura identica al tuo originale)
    FILE *fp = fopen("output_cuda.txt", "w");
    if (fp != NULL) {
        for (int i = 0; i < N * N; ++i) {
            fprintf(fp, "%d\n", h_A[i]);
        }
        fclose(fp);
    }

    // --- PULIZIA ---
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    cudaFree(d_A);
    free(h_A);

    return 0;
}
