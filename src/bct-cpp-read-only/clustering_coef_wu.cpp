#include "bct.h"
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Computes the clustering coefficient for a weighted undirected graph.
 */
gsl_vector* bct::clustering_coef_wu(const gsl_matrix* W) {
	if (safe_mode) check_status(W, SQUARE | WEIGHTED | UNDIRECTED, "clustering_coef_wu");
	
	// K=sum(W~=0,2);
	gsl_matrix* W_neq_0 = compare_elements(W, fp_not_equal, 0.0);
	gsl_vector* K = sum(W_neq_0, 2);
	gsl_matrix_free(W_neq_0);
	
	// cyc3=diag((W.^(1/3))^3);
	gsl_matrix* W_pow_1_3 = pow_elements(W, 1.0 / 3.0);
	gsl_matrix* W_pow_1_3_pow_3 = pow(W_pow_1_3, 3);
	gsl_matrix_free(W_pow_1_3);
	gsl_vector_view cyc3 = gsl_matrix_diagonal(W_pow_1_3_pow_3);
	
	// K(cyc3==0)=inf;
	gsl_vector* cyc3_eq_0 = compare_elements(&cyc3.vector, fp_equal, 0.0);
	logical_index_assign(K, cyc3_eq_0, GSL_POSINF);
	gsl_vector_free(cyc3_eq_0);
	
	// C=cyc3./(K.*(K-1));
	gsl_vector* K_sub_1 = copy(K);
	gsl_vector_add_constant(K_sub_1, -1.0);
	gsl_vector_mul(K, K_sub_1);
	gsl_vector_free(K_sub_1);
	gsl_vector* C = copy(&cyc3.vector);
	gsl_matrix_free(W_pow_1_3_pow_3);
	gsl_vector_div(C, K);
	gsl_vector_free(K);
	
	return C;
}
