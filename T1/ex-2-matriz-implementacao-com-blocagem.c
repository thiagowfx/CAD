#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 1000
#define BLOCK_SIZE 200
#define min(x,y) (((x)<(y))?(x):(y))

int main(int argc, char *argv[]) {
	int i, j, k, ii, jj, kk;
	int A[MAX_SIZE][MAX_SIZE], B[MAX_SIZE][MAX_SIZE], C[MAX_SIZE][MAX_SIZE];

	memset(C, 0, sizeof C);
	
	for(ii = 0; ii < MAX_SIZE; ii += BLOCK_SIZE)
		for(jj = 0; jj < MAX_SIZE; jj += BLOCK_SIZE)
			for(kk = 0; kk < MAX_SIZE; kk += BLOCK_SIZE)
				for(i = ii; i < min(MAX_SIZE, ii + BLOCK_SIZE); ++i)
					for(j = jj; j < min(MAX_SIZE, jj + BLOCK_SIZE); ++j)
						for(k = kk; k < min(MAX_SIZE, kk + BLOCK_SIZE); ++k)
							C[i][j] += A[i][k] * B[k][j];

	return 0;
}
