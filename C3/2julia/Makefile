uno:
	gcc  01_julia_c_ij.c   -O3 -march=native                             -o julia_c_ij
	gcc  01_julia_c_ij.c   -O3 -march=native -fopenmp                    -o julia_c_ij_omp
	gcc  01_julia_c_ij.c   -O3 -march=native -fopenmp -DOMP_SCHED_CHUNK  -o julia_c_ij_omp_chunk
	gcc  02_julia_c_ind.c  -O3 -march=native                             -o julia_c_ind
	gcc  02_julia_c_ind.c  -O3 -march=native -fopenmp                    -o julia_c_ind_omp
	gcc  02_julia_c_ind.c  -O3 -march=native -fopenmp -DOMP_SCHED_CHUNK  -o julia_c_ind_omp_chunk
	gcc  03_julia_gnuvec.c -O3 -march=native                             -o julia_gnuvec
	gcc  03_julia_gnuvec.c -O3 -march=native -fopenmp                    -o julia_gnuvec_omp
	gcc  03_julia_gnuvec.c -O3 -march=native -fopenmp -DOMP_SCHED_CHUNK  -o julia_gnuvec_omp_chunk	

clean:
	rm -rf julia_c_ij
	rm -rf julia_c_ij_omp
	rm -rf julia_c_ij_omp_chunk
	rm -rf julia_c_ind
	rm -rf julia_c_ind_omp
	rm -rf julia_c_ind_omp_chunk
	rm -rf julia_gnuvec
	rm -rf julia_gnuvec_omp
	rm -rf julia_gnuvec_omp_chunk
	rm -rf *txt

run:
	./julia_c_ij
	./julia_c_ij_omp
	./julia_c_ij_omp_chunk
	./julia_c_ind
	./julia_c_ind_omp
	./julia_c_ind_omp_chunk
	./julia_gnuvec
	./julia_gnuvec_omp
	./julia_gnuvec_omp_chunk
