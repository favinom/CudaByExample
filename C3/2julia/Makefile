uno: julia_c.c julia_neon.c julia_cpp.cc
	gcc   julia_c.c    -O3 -march=native                   -o julia_c
	clang julia_neon.c -O3 -march=armv8-a+simd -ffast-math -o julia_neon
	g++   julia_cpp.cc -O3 -fopenmp -Wl,-ld_classic        -o julia_cpp_omp
	g++   julia_cpp.cc -O3          -Wl,-ld_classic        -o julia_cpp
	
