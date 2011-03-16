#include "bct.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Given a distance matrix, computes characteristic path length.
 */
double bct::charpath_lambda(const gsl_matrix* D) {
	
	// lambda = sum(sum(D(D~=Inf)))/length(nonzeros(D~=Inf));
	gsl_matrix* D_neq_inf = compare_elements(D, fp_not_equal, GSL_POSINF);
	gsl_vector* D_idx = logical_index(D, D_neq_inf);
	double sum_D_idx = sum(D_idx);
	gsl_vector_free(D_idx);
	double ret = sum_D_idx / (double)nnz(D_neq_inf);
	gsl_matrix_free(D_neq_inf);
	return ret;
}

/*
 * Given a distance matrix, computes eccentricity, radius, and diameter.
 */
gsl_vector* bct::charpath_ecc(const gsl_matrix* D, double* radius, double* diameter) {
	
	// ecc = max(D.*(D~=Inf),[],2);
	gsl_matrix* D_finite = copy(D);
	gsl_matrix* D_eq_inf = compare_elements(D, fp_equal, GSL_POSINF);
	logical_index_assign(D_finite, D_eq_inf, 0.0);
	gsl_matrix_free(D_eq_inf);
	gsl_vector* ecc = max(D_finite, 2);
	gsl_matrix_free(D_finite);
	
	// radius = min(ecc);
	if (radius != NULL) {
		*radius = min(ecc);
	}
	
	// diameter = max(ecc);
	if (diameter != NULL) {
		*diameter = max(ecc);
	}
	
	return ecc;
}
