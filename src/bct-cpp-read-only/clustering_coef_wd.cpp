#include "bct.h"
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Computes the clustering coefficient for a weighted directed graph.
 */
gsl_vector* bct::clustering_coef_wd(const gsl_matrix* W) {
	if (safe_mode) check_status(W, SQUARE | WEIGHTED | DIRECTED, "clustering_coef_wd");
	
	// A=W~=0;
	gsl_matrix* A = compare_elements(W, fp_not_equal, 0.0);
	
	// S=W.^(1/3)+(W.').^(1/3);
	gsl_matrix* S = pow_elements(W, 1.0 / 3.0);
	gsl_matrix* W_transpose = gsl_matrix_alloc(W->size2, W->size1);
	gsl_matrix_transpose_memcpy(W_transpose, W);
	gsl_matrix* W_transpose_pow_1_3 = pow_elements(W_transpose, 1.0 / 3.0);
	gsl_matrix_free(W_transpose);
	gsl_matrix_add(S, W_transpose_pow_1_3);
	gsl_matrix_free(W_transpose_pow_1_3);
	
	// K=sum(A+A.',2);
	gsl_matrix* A_add_A_transpose = gsl_matrix_alloc(A->size2, A->size1);
	gsl_matrix_transpose_memcpy(A_add_A_transpose, A);
	gsl_matrix_add(A_add_A_transpose, A);
	gsl_vector* K = sum(A_add_A_transpose, 2);
	gsl_matrix_free(A_add_A_transpose);
	
	// cyc3=diag(S^3)/2;
	gsl_matrix* S_pow_3_div_2 = pow(S, 3);
	gsl_matrix_free(S);
	gsl_matrix_scale(S_pow_3_div_2, 0.5);
	gsl_vector_view cyc3 = gsl_matrix_diagonal(S_pow_3_div_2);
	
	// K(cyc3==0)=inf;
	gsl_vector* cyc3_eq_0 = compare_elements(&cyc3.vector, fp_equal, 0.0);
	logical_index_assign(K, cyc3_eq_0, GSL_POSINF);
	gsl_vector_free(cyc3_eq_0);
	
	// CYC3=K.*(K-1)-2*diag(A^2);
	gsl_vector* K_sub_1 = copy(K);
	gsl_vector_add_constant(K_sub_1, -1.0);
	gsl_matrix* A_pow_2_mul_2 = pow(A, 2);
	gsl_matrix_free(A);
	gsl_matrix_scale(A_pow_2_mul_2, 2.0);
	gsl_vector_view diag_A_pow_2_mul_2 = gsl_matrix_diagonal(A_pow_2_mul_2);
	gsl_vector* CYC3 = K;
	gsl_vector_mul(CYC3, K_sub_1);
	gsl_vector_free(K_sub_1);
	gsl_vector_sub(CYC3, &diag_A_pow_2_mul_2.vector);
	gsl_matrix_free(A_pow_2_mul_2);
	
	// C=cyc3./CYC3
	gsl_vector* C = copy(&cyc3.vector);
	gsl_matrix_free(S_pow_3_div_2);
	gsl_vector_div(C, CYC3);
	gsl_vector_free(CYC3);
	
	return C;
}
