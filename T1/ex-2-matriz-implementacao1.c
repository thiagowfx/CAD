#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 4000

int main(int argc, char *argv[]) {
	int i, j, k;
	int A[MAX_SIZE][MAX_SIZE], B[MAX_SIZE][MAX_SIZE], C[MAX_SIZE][MAX_SIZE];

	memset(C, 0, sizeof C);
	
	for(j = 0; j < MAX_SIZE; ++j)
		for(i = 0; i < MAX_SIZE; ++i)
			for(k = 0; k < MAX_SIZE; ++k)
				C[i][j] += A[i][k] * B[k][j];

	return 0;
}
