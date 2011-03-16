#include "bct.h"
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Computes nodal participation coefficient for a binary graph and its
 * corresponding community structure.  For a directed graph, computes "out-
 * neighbor" participation coefficient.
 */
gsl_vector* bct::participation_coef(const gsl_matrix* A, const gsl_vector* Ci) {
	if (safe_mode) check_status(A, SQUARE | BINARY, "participation_coef");
	
	// n=length(A);
	int n = length(A);
	
	// Ko=sum(A,2);
	gsl_vector* Ko = sum(A, 2);
	
	// Ko(~Ko)=inf;
	gsl_vector* not_Ko = logical_not(Ko);
	logical_index_assign(Ko, not_Ko, GSL_POSINF);
	gsl_vector_free(not_Ko);
	
	// Gc=A*diag(Ci);
	gsl_matrix* diag_Ci = diag(Ci);
	gsl_matrix* Gc = mul(A, diag_Ci);
	gsl_matrix_free(diag_Ci);
	
	// Kc2=zeros(n,1);
	gsl_vector* Kc2 = zeros_vector_double(n);
	
	// for i=1:max(Ci);
	for (int i = 1; i <= (int)max(Ci); i++) {
		
		// Kc2=Kc2+(sum(Gc==i,2).^2);
		gsl_matrix* Gc_eq_i = compare_elements(Gc, fp_equal, (double)i);
		gsl_vector* sum_Gc_eq_i = sum(Gc_eq_i, 2);
		gsl_matrix_free(Gc_eq_i);
		gsl_vector* sum_Gc_eq_i_pow_2 = pow_elements(sum_Gc_eq_i, 2.0);
		gsl_vector_free(sum_Gc_eq_i);
		gsl_vector_add(Kc2, sum_Gc_eq_i_pow_2);
		gsl_vector_free(sum_Gc_eq_i_pow_2);
	}
	
	gsl_matrix_free(Gc);
	
	// P=ones(n,1)-Kc2./(Ko.^2);
	gsl_vector* Ko_pow_2 = pow_elements(Ko, 2.0);
	gsl_vector_free(Ko);
	gsl_vector_div(Kc2, Ko_pow_2);
	gsl_vector_free(Ko_pow_2);
	gsl_vector* P = ones_vector_double(n);
	gsl_vector_sub(P, Kc2);
	gsl_vector_free(Kc2);
	
	return P;
}
