// Optimization Example
// v1.0 - 2010/03/20 - carlos@tacc.utexas.edu
// 
// Example of a naively written code
// This code contains data dependencies that prevent vecorization, 
// as well as type-mixed math and a naive mathematical implementation.
// Optimize the computational kernel in lines 34-36 of this file
// as much as possible

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_SIZE 50000

int main(int argc, char *argv[])
{
  int i, j, k;
  double tStart, tSetup, tEnd, setup, kernel;
  double x[MAX_SIZE];
  double y[MAX_SIZE];

  setup = 0.0;
  kernel = 0.0;
  /* External loop is only used to make things take longer */
  /* DO NOT CHANGE THIS EXTERNAL LOOP */
  for( k = 1; k < 1000; k++){

    /* Initialize the arrays */
    tStart = (double)clock()/(double)CLOCKS_PER_SEC;
    for(i = 0; i < MAX_SIZE; i++){
        x[i] = 2.0*i;
    }
    tSetup = (double)clock()/(double)CLOCKS_PER_SEC;

    /* KERNEL */
    for(i = 0; i < MAX_SIZE; i++){
	y[i] = x[i];
    }

    for(i = 1; i < MAX_SIZE-1; i++){
        x[i] = y[i-1]/4 + y[i]/2 + y[i+1]/4;
    }
    tEnd = (double)clock()/(double)CLOCKS_PER_SEC;

  setup  += tSetup - tStart;
  kernel += tEnd - tSetup;
  }

  printf( "\nTwo values for validation");
  printf( "\n=========================");
  printf( "\nX[1]  = %e", x[1]);
  printf( "\nX[%d] = %e\n", MAX_SIZE-2, x[MAX_SIZE-2]);

  printf( "\nExecution times");
  printf( "\n=========================");
  printf( "\nSetup Time  = %le", setup );
  printf( "\nKernel Time = %le\n\n", kernel );



return 0;
}
