#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

void jacobi (int n, double dx, double dy, double omega, double *u, double *f, double tol, int maxit);

double* dalloc_matrix(int n, int m) {
	double* mat = (double*) malloc (n * m * sizeof(double));
	return mat;
}


void dfree_matrix(double *mat) {
	free(mat);
}

double cpu_time() {
   return clock()/((double) CLOCKS_PER_SEC);
}

/*****************************************************
* Inicializando os dados
******************************************************/
void initialize(  
                int n,    
                double *dx,
                double *dy,
                double *u,
		double *f)
{
  int i,j;
  double xx,yy;
  double *up = u;
  double *fp = f;

  *dx = 1.0 / (n-1);
  *dy = 1.0 / (n-1);

  /* Inicializando condicao inicial e RHS */
  for (i=0; i<n; i++){
    xx = exp((*dx) * (i-1));
    for (j=0; j<n; j++){
      yy = (*dy) * (j-1);
      *up++ = 0.0;
      *fp++ = xx * yy;
    }
  }
      
}


/************************************************************
* Verificacao do erro entre a solucao numerica e exata 
************************************************************/
void error_check(
                 int n,
                 double dx,
                 double dy,
                 double *u)
{
  int i,j;
  double xx, yy, temp, error;
  double *up = u;

  error = 0.0;

  for (j=0; j<n; j++){
    xx = exp(dx * (i-1));
    for (i=0; i<n; i++){
      yy =  exp(-2.0 * (dy * (j-1)));
      temp = *up++ - xx*yy;
      error += temp*temp;
    }
  }

  error = sqrt(error)/(n*n);

  printf("Erro : %g\n", error);

}




int main(int argc, char **argv) {
    int n, m, mits;
    double tol, relax, alpha;
    double *u, *f, dx, dy;
    double dt, start, end, mflops;

    m = n = 100;
    relax = 1.0;
    tol   = 1.0e-5;
    mits  = 1000;
    if(argc == 4) {
      n     = atoi(argv[1]);
      tol   = atof(argv[2]);
      mits  = atoi(argv[3]);
    }
    else {
      printf("Use: ./jacobi <n> <tol> <mits>\n");
      printf("where\n");
      printf("   <n>     : numero de pontos em X e Y (default 100)\n"); 
      printf("   <tol>   : tolerancia do erro (default 1e-5)\n");
      printf("   <mits>  : numero maximo de iteracoes (default 1000)\n");
    }

    printf("-> %d, %g, %g, %d\n", n, relax, tol, mits);
    
    // Alocando as estruturas de dados
    u = dalloc_matrix(n,m);
    f = dalloc_matrix(n,m);


    /* Dados inicializados */
    initialize(n, &dx, &dy, u, f);
    
    double pi = M_PI;

    relax = 2.0/(1.0 + sin(pi*dx));

    printf("relax parameter: %f \n", relax);

    /* Resolvendo a equação */
    start = cpu_time();
    jacobi(n, dx, dy, relax, u,f, tol, mits);
    end   = cpu_time();

    dt = end-start;

    printf(" elapsed time : %12.6f\n", dt);
    mflops = (0.000001*mits*(m-2)*(n-2)*13) / dt;
    printf(" MFlops       : %12.6g (%d, %d, %d, %g)\n",mflops, mits, m, n, dt);

    // Verificando o erro.
    error_check(n, dx, dy, u);

    // Liberando memória.
    dfree_matrix(u);
    dfree_matrix(f);
       
  return 0;
}

/* 
  subroutine jacobi (n,dx,dy,alpha,omega,u,f,tol,maxit)
******************************************************************
* Resolve equação de poisson em um grid retangular assumindo : 
* (1) discretização uniforme em cada direção, e 
* (2) condiçoes de contorno de Dirichlect 
* 
* Metodo de jacobi é usado nesta rotina
*
* Input : n    número de pontos nas direcoes X/Y 
*         dx,dy espacamento entre os pontos 
*         omega  fator de relaxação SOR 
*         f(n,m) vetor RHS 
*         u(n,m) vetor soluçao
*         tol    tolerancia do metodo iterativo
*         maxit  numero máximo de iteracoes
*
* Output : u(n,m) - solucao
*****************************************************************
*/

void jacobi (int n, double dx, double dy, double omega, double *u, double *f, double tol, int maxit )
{

	int i,j,k;
	double error, resid, h2, b;
	double *uold;
	double *up;
        double *uoldp;

	uold = dalloc_matrix(n,n);

	h2 = (dx*dx);   /* X-direction coef */

  	error = 10.0 * tol;
  	k = 1;
  	while (k <= maxit && error > tol) 
  	{
      		error = 0.0;

      		/* copia solução corrente para solução antiga */
		uoldp = uold;
		up = u;
      		for (j=0; j<n; j++)
         		for (i=0; i<n; i++)
				*uoldp++ = *up++;
		   
      		/* calcula o residuo por diferencas finitas */
      		for (i=1; i<n-1; i++)
	 		for (j=1; j<n-1; j++)
         		{
				#define F(i,j) (*(f + n * (i) + (j)))
				#define U(i,j) (*(u + n * (i) + (j)))
				#define Uold(i,j) (*(uold + n * (i) + (j)))
		
				/* atualiza a solução */
				U(i,j) = (1.0-omega)*U(i,j) + 
                                            omega*( U(i-1,j) + U(i+1,j)  +
  		    			            U(i,j-1) + U(i,j+1) + h2*F(i,j))/4.0;

				/* acumula o erro relativo */
			        resid = U(i,j)-Uold(i,j);
				error += resid*resid;
          		}

		/* verifica o erro  */
		k++;
    		error = sqrt(error);

	} /* while */

	printf("Numero total de iteracoes:  %d\n", k);
	printf("Error                  : %10.15f\n", error);
  	dfree_matrix(uold);
} 
