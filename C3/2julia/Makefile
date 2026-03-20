uno:
	gcc  01_julia_c_ij.c               -O3 -march=native            -o julia_c_ij
	gcc  02_julia_c_ind.c              -O3 -march=native            -o julia_c_ind
	gcc  04_julia_omp_ind.c            -O3 -march=native -fopenmp   -o julia_omp_ind
	gcc  05_julia_omp_ind_chunk.c      -O3 -march=native -fopenmp   -o julia_omp_ind_chunk
	gcc  06_julia_omp_ind_chunk_auto.c -O3 -march=native -fopenmp   -o julia_omp_ind_chunk_auto

	#clang julia_neon.c      -O3 -march=armv8-a+simd -ffast-math -o julia_neon

	#gcc   julia_omp_chunk.c -O3 -fopenmp                        -o julia_omp_chunk
	#gcc   julia_gnuvec.c    -O3                                 -o julia_gnuvec
	#g++   julia_cpp.cc      -O3          -Wl,-ld_classic        -o julia_cpp	
	#g++   julia_cpp.cc      -O3 -fopenmp -Wl,-ld_classic        -o julia_cpp_omp
	

clean:
	rm -rf julia_c_ij
	rm -rf julia_c_ind
	rm -rf julia_omp_ind
	rm -rf julia_omp_ind_chunk
	rm -rf julia_omp_ind_chunk_auto
	rm -rf julia_omp_chunk
	rm -rf julia_gnuvec
	#rm -rf julia_cpp
	#rm -rf julia_cpp_omp
	rm -rf *txt

run:
	./julia_c_ij
	./julia_c_ind
	./julia_omp_ind
	./julia_omp_ind_chunk
	./julia_omp_ind_chunk_auto

	