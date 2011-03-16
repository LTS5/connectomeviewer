#include "bct.h"
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Computes z-score for a binary graph and its corresponding community
 * structure.  For a directed graph, computes out-neighbor z-score.
 */
gsl_vector* bct::module_degree_zscore(const gsl_matrix* A, const gsl_vector* Ci) {
	if (safe_mode) check_status(A, SQUARE | BINARY, "module_degree_zscore");
	
	// n=length(A);
	int n = length(A);
	
	// Z=zeros(n,1);
	gsl_vector* Z = zeros_vector_double(n);
	
	// for i=1:max(Ci)
	for (int i = 1; i <= (int)max(Ci); i++) {
		
		// Koi=sum(A(Ci==i,Ci==i),2);
		gsl_vector* Ci_eq_i = compare_elements(Ci, fp_equal, (double)i);
		gsl_matrix* A_idx = logical_index(A, Ci_eq_i, Ci_eq_i);
		gsl_vector* Koi = sum(A_idx, 2);
		gsl_matrix_free(A_idx);
		
		// Z(Ci==i)=(Koi-mean(Koi))./std(Koi);
		double std_Koi = matlab::std(Koi);
		gsl_vector_add_constant(Koi, -mean(Koi));
		gsl_vector_scale(Koi, 1.0 / std_Koi);
		logical_index_assign(Z, Ci_eq_i, Koi);
		gsl_vector_free(Ci_eq_i);
		gsl_vector_free(Koi);
	}
	
	// Z(isnan(Z))=0;
	for (int i = 0; i < (int)Z->size; i++) {
		if (gsl_isnan(gsl_vector_get(Z, i)) == 1) {
			gsl_vector_set(Z, i, 0.0);
		}
	}
	
	return Z;
}
