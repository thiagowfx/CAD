#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef MPI_ENABLE
#include <mpi.h>
#endif

#define M_PI 3.14159265358979323846 
#define MPI_DUMMY_TAG 1

int main(int argc, char *argv[]) {
#ifdef MPI_ENABLE
	MPI_Init(&argc, &argv);

	double starttime, endtime;
	starttime = MPI_Wtime();

	int rank, p;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
#else
	clock_t starttime, endtime;
	starttime = clock();
#endif

	char* usage_fmt = "usage: %s number_of_samples seed\n"; 
	char* end_ptr_for_strtol; 
 
	/* Obtendo parametros por linha de comando  */ 
	if (argc != 3)  { 
		fprintf(stderr, usage_fmt, argv[0]); 
#ifdef MPI_ENABLE
		MPI_Finalize();
#endif
		exit(EXIT_FAILURE); 
	} 

	long num_samples = strtol(argv[1], &end_ptr_for_strtol, 10); 
	if (*end_ptr_for_strtol != '\0') { 
		fprintf(stderr, usage_fmt, argv[0]); 
#ifdef MPI_ENABLE
		MPI_Finalize();
#endif
		exit(EXIT_FAILURE); 
	} 

	long seed = strtol(argv[2], &end_ptr_for_strtol, 10); 
	if (*end_ptr_for_strtol != '\0') { 
		fprintf(stderr, usage_fmt, argv[0]); 
#ifdef MPI_ENABLE
		MPI_Finalize();
#endif
		exit(EXIT_FAILURE); 
	} 
 
#ifdef MPI_ENABLE
	srand((unsigned int) rank * seed); 
	long num_samples_for_each = num_samples / p;
#endif
	int count = 0; 
	double x, y; 

#ifdef MPI_ENABLE
	for (int i = 0; i < num_samples_for_each; ++i) { 
#else
	for (int i = 0; i < num_samples; ++i) { 
#endif
		x = (double) rand() / (double) (RAND_MAX); 
		y = (double) rand() / (double) (RAND_MAX); 
		if ((x*x + y*y) <= 1.0) 
			++count; 
	} 

	double pi = 0.0;

	// MPI_Allreduce(&count, &count, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
#ifdef MPI_ENABLE
	if(rank != 0) {
		MPI_Request ireq;
		MPI_Isend(&count, 1, MPI_INT, 0, MPI_DUMMY_TAG, MPI_COMM_WORLD, &ireq);
	}
	else {
		for(int i = 1; i < p; ++i) {
			int _count = 0;
			MPI_Recv(&_count, 1, MPI_INT, i, MPI_DUMMY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			count += _count;
		}
		pi = 4.0 * (double) count / (double) (num_samples_for_each * p);
	}
#else
		pi = 4.0 * (double) count / (double) (num_samples);
#endif

#ifdef MPI_ENABLE
	endtime = MPI_Wtime();
#else
	endtime = clock();
#endif

#ifdef MPI_ENABLE
	if(rank == 0) {
#endif
		printf("Resultado sequencial:\n"); 
#ifdef MPI_ENABLE
		printf("Total de pontos = %ld, seed = %ld\n", num_samples_for_each * p, seed); 
#else
		printf("Total de pontos = %ld, seed = %ld\n", num_samples, seed); 
#endif
		printf("Pi estimado = %12.10f\n", pi); 
		printf("Erro  = %12.10f\n", fabs(pi - M_PI)); 
#ifdef MPI_ENABLE
	}
#endif

#ifdef MPI_ENABLE
	if(rank == 0) {
		printf("Process #%d: total time: %lf seconds\n", rank, endtime - starttime);
	}
#else
	printf("Total execution time: %lf\n", ((double)(endtime - starttime) / CLOCKS_PER_SEC) / 1000.0);
#endif

	/* clean up and return */ 
#ifdef MPI_ENABLE
	MPI_Finalize();
#endif

	return EXIT_SUCCESS;
}
