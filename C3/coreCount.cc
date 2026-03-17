#include <omp.h>
#include <iostream>

int main() {
    #pragma omp parallel
    {
        #pragma omp single
        {
            std::cout << "OpenMP threads used = "
                      << omp_get_num_threads() << std::endl;
        }
    }
    return 0;
}
