#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_blas.h>
#include <cmath>
#include "bct.h"

gsl_vector* bct::eigenvector_centrality(const gsl_matrix* G) {
  //uses power iteration to find dominant eigenvector
  //adapted from NetworkX 1.4 eigenvector_centrality function
  //see also: http://www.mathworks.de/matlabcentral/fx_files/7978/1/mPowerEig.c
  //see also: http://en.wikipedia.org/wiki/Power_iteration

  if (safe_mode) check_status(G, SQUARE, "eigenvector_centrality");

  double tol = .000001; //ensures that the average value is stable at 1e-6
  int maxiter=1000;
  int N = G->size1;
  gsl_vector* x = gsl_vector_alloc(N);
  gsl_vector* xlast=gsl_vector_alloc(N);

  //note that this starting vector is already normalized (elements sum to 1)
  double startval = 1.0/double(N);
  gsl_vector_set_all(x, startval);

  double evec_norm;
  double err;
  double newval;

  for (int iter=0; iter < maxiter; iter++) {
    err = 0.0;
    gsl_vector_memcpy(xlast, x);
    gsl_vector_set_zero(x);

#ifdef _OPENMP
#pragma omp parallel for private(newval) shared(x,xlast)
#endif
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
	newval = gsl_vector_get(x, i) + gsl_vector_get(xlast, j) * gsl_matrix_get(G, i, j);
	gsl_vector_set(x, i, newval);
      }
    }

    //normalize vector
    evec_norm = gsl_blas_dnrm2(x); // -- calculate vector norm    
    gsl_vector_scale(x, 1.0/evec_norm);

    //check convergence
    for (int i = 0; i < (int)x->size; i++) {
      err += abs(gsl_vector_get(x, i) - gsl_vector_get(xlast, i));
    }

    if (err < double(N) * tol) {
      break; //end power iteration
    }
    
  }

  //deallocate xlast vector
  gsl_vector_free(xlast);

  return(x); //x contains best estimate of eigenvector centrality
}

/*float version*/
gsl_vector_float* bct::eigenvector_centrality(const gsl_matrix_float* G) {
  //uses power iteration to find dominant eigenvector
  //adapted from NetworkX 1.4 eigenvector_centrality function
  //see also: http://www.mathworks.de/matlabcentral/fx_files/7978/1/mPowerEig.c
  //see also: http://en.wikipedia.org/wiki/Power_iteration

  if (safe_mode) check_status(G, SQUARE, "eigenvector_centrality");

  double tol = .000001; //ensures that the average value is stable at 1e-6
  int maxiter=1000;
  int N = G->size1;
  gsl_vector_float* x = gsl_vector_float_alloc(N);
  gsl_vector_float* xlast=gsl_vector_float_alloc(N);

  //note that this starting vector is already normalized (elements sum to 1)
  double startval = 1.0/double(N);
  gsl_vector_float_set_all(x, (float)startval);

  float evec_norm;
  double err;
  double newval;

  for (int iter=0; iter < maxiter; iter++) {
    err = 0.0;
    gsl_vector_float_memcpy(xlast, x);
    gsl_vector_float_set_zero(x);

#ifdef _OPENMP
#pragma omp parallel for private(newval) shared(x,xlast)
#endif
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
	newval = gsl_vector_float_get(x, i) + gsl_vector_float_get(xlast, j) * gsl_matrix_float_get(G, i, j);
	gsl_vector_float_set(x, i, newval);
      }
    }

    //normalize vector
    evec_norm = gsl_blas_snrm2(x); // -- calculate vector norm (use snrm because x is float)
    gsl_vector_float_scale(x, 1.0/evec_norm);

    //check convergence
    for (int i = 0; i < (int)x->size; i++) {
      err += abs(gsl_vector_float_get(x, i) - gsl_vector_float_get(xlast, i));
    }

    if (err < double(N) * tol) {
      break; //end power iteration
    }
    
  }

  //deallocate xlast vector
  gsl_vector_float_free(xlast);

  return(x); //x contains best estimate of eigenvector centrality
}
