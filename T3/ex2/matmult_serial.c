#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int** alloc_square_matrix(int n) {
	int **m = malloc(n * sizeof(int*));

	for(int i = 0; i < n; ++i) {
		m[i] = malloc(n * sizeof(int));
	}

	return m;
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

int** mult_square_matrix(int **m1, int **m2, int n) {
	int **m = alloc_square_matrix(n);

	for(int i = 0; i < n; ++i) {
		for(int j = 0; j < n; ++j) {
			int cell = 0;
			for(int k = 0; k < n; ++k) {
				cell += m1[i][k] * m2[k][j];
			}
			m[i][j] = cell;
		}
	}

	return m;
}

int main(int argc, char *argv[]) {
	if(argc != 4) {
		printf("Usage: %s <matrix1.txt> <matrix2.txt> <outfile.txt>\n", argv[0]);
		exit(0);
	}

	int n, n2;
	clock_t begin, end;

	begin = clock();
	int **m1 = read_square_matrix(argv[1], &n);
	int **m2 = read_square_matrix(argv[2], &n2);

	if(n != n2) {
		fprintf(stderr, "Error: the matrices must have the same	order!\n");
		exit(1);
	}

	int **m3 = mult_square_matrix(m1, m2, n);
	write_square_matrix(m3, n, argv[3]);
	end = clock();

#ifdef DEBUG
	print_square_matrix(m1, n);
	print_square_matrix(m2, n);
	print_square_matrix(m3, n);
#endif

	printf("Execution time: %lf s\n", ((double)(end - begin)) / CLOCKS_PER_SEC);

	unalloc_square_matrix(m1, n);
	unalloc_square_matrix(m2, n);
	unalloc_square_matrix(m3, n);

	return 0;
}
