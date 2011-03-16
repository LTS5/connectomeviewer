#include "bct.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Computes the joint degree distribution, a matrix in which the value of each
 * element (u, v) is the number of nodes with u outgoing connections and v
 * incoming connections.
 */
gsl_matrix* bct::jdegree(const gsl_matrix* CIJ) {
	if (safe_mode) check_status(CIJ, SQUARE, "jdegree");
	
	// CIJ = double(CIJ~=0);
	gsl_matrix* _CIJ = compare_elements(CIJ, fp_not_equal, 0.0);
	
	// N = size(CIJ,1);
	int N = _CIJ->size1;
	
	// id = sum(CIJ,1);
	gsl_vector* id = sum(_CIJ, 1);
	
	// od = sum(CIJ,2)';
	gsl_vector* od = sum(_CIJ, 2);
	gsl_matrix_free(_CIJ);
	
	// szJ = max(max(id,od))+1;
	double max_id = gsl_vector_max(id);
	double max_od = gsl_vector_max(od);
	int szJ = (int)max(max_id, max_od) + 1;
	
	// J = zeros(szJ);
	gsl_matrix* J = zeros_double(szJ);
	
	// for i=1:N
	for (int i = 0; i < N; i++) {
		
		// J(id(i)+1,od(i)+1) = J(id(i)+1,od(i)+1) + 1;
		int id_i = (int)gsl_vector_get(id, i);
		int od_i = (int)gsl_vector_get(od, i);
		gsl_matrix_set(J, id_i, od_i, gsl_matrix_get(J, id_i, od_i) + 1.0);
	}
	
	gsl_vector_free(id);
	gsl_vector_free(od);
	return J;
}

/*
 * Given a joint degree distribution matrix, returns the number of nodes with
 * in-degree = out-degree.
 */
int bct::jdegree_bl(const gsl_matrix* J) {
	
	// J_bl = sum(diag(J));
	gsl_vector_const_view diag_J = gsl_matrix_const_diagonal(J);
	return (int)sum(&diag_J.vector);
}

/*
 * Given a joint degree distribution matrix, returns the number of nodes with
 * in-degree > out-degree.
 */
int bct::jdegree_id(const gsl_matrix* J) {
	
	// J_id = sum(sum(tril(J,-1)));
	gsl_matrix* tril_J = tril(J, -1);
	gsl_vector* sum_tril_J = sum(tril_J);
	int J_id = (int)sum(sum_tril_J);
	gsl_matrix_free(tril_J);
	gsl_vector_free(sum_tril_J);
	return J_id;
}

/*
 * Given a joint degree distribution matrix, returns the number of nodes with
 * out-degree > in-degree.
 */
int bct::jdegree_od(const gsl_matrix* J) {
	
	// J_od = sum(sum(triu(J,1)));
	gsl_matrix* triu_J = triu(J, 1);
	gsl_vector* sum_triu_J = sum(triu_J);
	int J_od = (int)sum(sum_triu_J);
	gsl_matrix_free(triu_J);
	gsl_vector_free(sum_triu_J);	
	return J_od;
}
