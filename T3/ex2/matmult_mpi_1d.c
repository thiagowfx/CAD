#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DUMMY_TAG 1

int** alloc_matrix(int rows, int cols) {
	int **m = malloc(rows * sizeof(int*));

	for(int i = 0; i < rows; ++i) {
		m[i] = (int*) malloc(cols * sizeof(int));
	}

	return m;
}

int** alloc_square_matrix(int n) {
	return alloc_matrix(n, n);
}

void unalloc_square_matrix(int** m, int n) {
	for(int i = 0; i < n; ++i) {
		free(m[i]);
	}

	free(m);
}

/* Read the matrix from the given file and returns it. It also updates n with
 * the order of the matrix. */
int** read_square_matrix(const char* file, int* n) {
	FILE *fp;
	fp = fopen(file, "r");

	if(fp == NULL) {
		fprintf(stderr, "Error while reading the matrix.\n");
		MPI_Finalize();
		exit(1);
	}

	fscanf(fp, "%d\n", n);

	int **m = alloc_square_matrix(*n);

	for(int i = 0; i < *n; ++i) {
		for (int j = 0; j < *n; ++j) {
			fscanf(fp, "%d", &m[i][j]);
		}
	}

	return m;
}

void write_square_matrix(int **m, int n, const char* file) {
	FILE *fp;
	fp = fopen(file, "w");

	if(fp == NULL) {
		fprintf(stderr, "Error while writing the matrix.\n");
		MPI_Finalize();
		exit(1);
	}

	fprintf(fp, "%d\n", n);

	for(int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			fprintf(fp, "%d ", m[i][j]);
		}
		fprintf(fp, "\n");
	}
}

/* Print the square matrix m of order n. */
void print_square_matrix(int **m, int n) {
	for(int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			printf("%d ", m[i][j]);
		}
		puts("");
	}
}

int matrix_cell(int *r, int *c, int n) {
	int cell = 0;
	for(int i = 0; i < n; ++i) {
		cell += r[i] * c[i];
	}
	return cell;
}

int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);

	double starttime, endtime;
	starttime = MPI_Wtime();

	/* Rank of the current process. */
	int rank;

	/* Number of processes. */
	int p;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	/*
	 * Check if the input arguments are okay.
	 */
	if(argc != 4) {
		if (rank == 0) {
			printf("Usage: %s <matrix1.txt> <matrix2.txt> <outfile.txt>\n", argv[0]);
		}
		MPI_Finalize();
		exit(1);
	}

	int **m1 = NULL, **m2 = NULL;
	int **m3 = NULL;

	int _n[2];

	/*
	 * Allocate and read the input matrices.
	 */
	if (rank == 0) {
		m1 = read_square_matrix(argv[1], &_n[0]);
		m2 = read_square_matrix(argv[2], &_n[1]);
	}

	MPI_Bcast(_n, 2, MPI_INT, 0, MPI_COMM_WORLD);

	/*
	 * Check if the multiplication can be done, depending on the order of
	 * the matrices.
	 */
	if(_n[0] != _n[1]) {
		if(rank == 0) {
			fprintf(stderr, "Error: the matrices must have the same	order!\n");
		}
		MPI_Finalize();
		exit(1);
	}

	int n = _n[0];

	/*
	 * Check whether the number of processes divide the matrices order or
	 * not.
	 */
	if((n % p) != 0) {
		if(rank == 0) {
			fprintf(stderr, "Error: order of the matrix is not divisible by number of processes, as assumed");
		}
		MPI_Finalize();
		exit(1);
	}

	if(rank != 0) {
		m1 = alloc_square_matrix(n * n);
		m2 = alloc_square_matrix(n * n);
	}

	/* Broadcast the second matrix to every process. */
	for(int i = 0; i < n; ++i) {
		MPI_Bcast(&m2[i][0], n, MPI_INT, 0, MPI_COMM_WORLD);
	}
	
#ifdef DEBUG
	print_square_matrix(m2, n);
#endif

	int rows_for_each = n / p;
	MPI_Request *ireq = (MPI_Request*) malloc(n * sizeof(MPI_Request));

	/* N/A: MPI_Scatter(m1, rows_for_each * n, MPI_INT, m1cc, rows_for_each * n, MPI_INT, 0, MPI_COMM_WORLD);*/
	/*
	 * Send some rows of the first matrix to each process.
	 */
	if (rank == 0) {
		for(int i = 0; i < p; ++i) {
			for(int j = 0; j < rows_for_each; ++j) {
				MPI_Isend(&m1[i * rows_for_each + j][0], n, MPI_INT, i, DUMMY_TAG, MPI_COMM_WORLD, &ireq[i * rows_for_each + j]);
			}
		}
	}

	 /*
	  * Receive some rows of the first matrix from the master process.
	  */
	for(int j = 0; j < rows_for_each; ++j) {
		MPI_Recv(&m1[rank * rows_for_each + j][0], n, MPI_INT, 0, DUMMY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

#ifdef DEBUG
	for(int i = 0; i < n; ++i) {
		printf("Process #%d, row %d: ", rank, i);
		for (int j = 0; j < n; ++j) {
			printf("%d ", m1[i][j]);
		}
		puts("");
	}
#endif

	m3 = alloc_square_matrix(n);

	/*
	 * Here the (partial) multiplication happens in each process.
	 */
	for(int i = 0; i < rows_for_each; ++i) {
		int posicaoi = rank * rows_for_each + i;
		for(int j = 0; j < n; ++j) {
			int cell = 0;
			for(int k = 0; k < n; ++k) {
				cell += m1[posicaoi][k] * m2[k][j];
			}
			m3[posicaoi][j] = cell;
		}
	}

#ifdef DEBUG
	for(int i = 0; i < n; ++i) {
		printf("Process #%d, row %d: ", rank, i);
		for (int j = 0; j < n; ++j) {
			printf("%d ", m3[i][j]);
		}
		puts("");
	}
#endif

	/*
	 * Each process send the information back to the master process.
	 */
	for(int j = 0; j < rows_for_each; ++j) {
		MPI_Isend(&m3[rank * rows_for_each + j][0], n, MPI_INT, 0, DUMMY_TAG, MPI_COMM_WORLD, &ireq[rank * rows_for_each + j]);
	}

	/*
	 * The master process receives information from the other processes
	 * (including itself).
	 */
	if (rank == 0) {
		for(int i = 0; i < p; ++i) {
			for(int j = 0; j < rows_for_each; ++j) {
				MPI_Recv(&m3[i * rows_for_each + j][0], n, MPI_INT, i, DUMMY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}
	}

	endtime = MPI_Wtime();

	if (rank == 0) {
#ifdef DEBUG
		print_square_matrix(m3, n);
#endif
		write_square_matrix(m3, n, argv[3]);
	}

	if (rank == 0) {
		printf("Process #%d: total time: %lf seconds\n", rank, endtime - starttime);
	}

	if(ireq)
		free(ireq);

	if(m1)
		unalloc_square_matrix(m1, n);

	if(m2)
		unalloc_square_matrix(m2, n);

	if(m3)
		unalloc_square_matrix(m3, n);

	MPI_Finalize();

	return 0;
}
