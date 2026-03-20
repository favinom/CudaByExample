#include <omp.h>
#include <vector>
#include <complex>

#include "config.h"

int main() {
    const std::complex<Real> c(c_re, c_im);

    std::vector<int> img(N * N);

    const Real dx = Lx/(N-1);
    const Real dy = Ly/(N-1);

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);


    #pragma omp parallel for collapse(2) schedule(static)
    for (int j = 0; j < N; ++j) {
        for (int i = 0; i < N; ++i) {
            Real re = x0 + dx * i;
            Real im = y0 + dy * j;

            std::complex<Real> z(re, im);
            int k = 0;

            while (std::norm(z) < r2 && k < MAX_IT) {
                z = z * z + c;
                ++k;
            }

            img[j * N + i] = k;
        }
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
        fprintf(fp, "%d\n", img[i]);

    fclose(fp);

    return 0;
}
