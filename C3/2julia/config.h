#ifndef CONFIG_H
#define CONFIG_H

#define N 24001
#define MAX_IT 10000

#define r2  4.0

#define x0 -1.0
#define y0 -1.0

#define Lx 2.0
#define Ly 2.0

#define c_re -0.8
#define c_im  0.156

typedef double Real;

//#define WRITE_TO_FILE

#define TIC(name) \
    struct timespec name##_t0, name##_t1; \
    clock_gettime(CLOCK_MONOTONIC, &name##_t0)

#define TOC(name, label) \
    clock_gettime(CLOCK_MONOTONIC, &name##_t1); \
    printf("%s: %.6f s\n", (label), \
           (double)(name##_t1.tv_sec - name##_t0.tv_sec) + \
           1e-9 * (double)(name##_t1.tv_nsec - name##_t0.tv_nsec))


#ifdef _OPENMP
    #if OMP_SCHED_CHUNK
        #define OMP_FOR _Pragma("omp parallel for schedule(static)")
    #else
        #define OMP_FOR _Pragma("omp parallel for schedule(static,1)")
    #endif
#else
  #define OMP_FOR
#endif


#endif

//clock_t t0 = clock();
//clock_t t1 = clock();
//double elapsed = (double)(t1 - t0) / CLOCKS_PER_SEC;
//printf("Loop time: %.6f s\n", elapsed);
