#ifndef FUNCTIONS_H
#define FUNCTIONS_H

static inline int julia(Real re, Real im)
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

int write_array_to_file(const char *filename, const int *A, int n)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("fopen");
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        fprintf(fp, "%d\n", A[i]);
    }

    fclose(fp);
    return 0;
}

long long sum_array(const int *A, int n)
{
    long long s = 0;

    for (int i = 0; i < n; ++i) {
        s += A[i];
    }

    return s;
}

#endif

