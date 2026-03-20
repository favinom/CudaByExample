uno: julia_c.c julia_neon.c
	gcc   julia_c.c         -O3 -march=native                   -o julia_c
	clang julia_neon.c      -O3 -march=armv8-a+simd -ffast-math -o julia_neon
	gcc   julia_omp.c       -O3 -fopenmp                        -o julia_omp
	gcc   julia_omp_chunk.c -O3 -fopenmp                        -o julia_omp_chunk
	gcc   julia_gnuvec.c    -O3                                 -o julia_gnuvec
	#g++   julia_cpp.cc      -O3          -Wl,-ld_classic        -o julia_cpp	
	#g++   julia_cpp.cc      -O3 -fopenmp -Wl,-ld_classic        -o julia_cpp_omp
	

clean:
	rm -rf julia_c
	rm -rf julia_neon
	rm -rf julia_omp
	rm -rf julia_omp_chunk
	rm -rf julia_gnuvec
	#rm -rf julia_cpp
	#rm -rf julia_cpp_omp
	rm -rf *txt


	