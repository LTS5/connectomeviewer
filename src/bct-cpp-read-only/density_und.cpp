#include "bct.h"
#include <gsl/gsl_matrix.h>

/*
 * Computes density for an undirected graph.  Connection weights are ignored.
 */
double bct::density_und(const gsl_matrix* CIJ) {
	if (safe_mode) check_status(CIJ, SQUARE | UNDIRECTED, "density_und");
	
	// N = size(CIJ,1);
	int N = CIJ->size1;
	
	// K = nnz(triu(CIJ));
	gsl_matrix* triu_CIJ = triu(CIJ);
	int K = nnz(triu_CIJ);
	gsl_matrix_free(triu_CIJ);
	
	// kden = K/((N^2-N)/2);
	return (double)K / ((double)(N * (N - 1)) / 2.0);
}
