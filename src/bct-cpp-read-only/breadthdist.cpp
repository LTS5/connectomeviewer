#include "bct.h"
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Computes reachability and distance matrices using breadth-first search.
 */
gsl_matrix* bct::breadthdist(const gsl_matrix* CIJ, gsl_matrix** D) {
	if (safe_mode) check_status(CIJ, SQUARE, "breadthdist");
	
	// N = size(CIJ,1);
	int N = CIJ->size1;
	
	// D = zeros(N);
	gsl_matrix* _D = zeros_double(N);
	
	// for i=1:N
	for (int i = 0; i < N; i++) {
		
		// D(i,:) = breadth(CIJ,i);
		gsl_vector* distance = breadth(CIJ, i);
		gsl_matrix_set_row(_D, i, distance);
		gsl_vector_free(distance);
	}
	
	// D(D==0) = Inf;
	gsl_matrix* D_eq_0 = compare_elements(_D, fp_equal, 0.0);
	logical_index_assign(_D, D_eq_0, GSL_POSINF);
	gsl_matrix_free(D_eq_0);
	
	// R = double(D~=Inf);
	gsl_matrix* R = compare_elements(_D, fp_not_equal, GSL_POSINF);
	
	if (D != NULL) *D = _D; else gsl_matrix_free(_D);
	return R;
}
