#include "bct.h"
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Computes the distance matrix for a binary graph.
 */
gsl_matrix* bct::distance_bin(const gsl_matrix* G) {
	if (safe_mode) check_status(G, SQUARE | BINARY, "distance_bin");
	
	// D=eye(length(G));
	gsl_matrix* D = eye_double(length(G));
	
	// n=1;
	int n = 1;
	
	// nPATH=G;
	gsl_matrix* nPATH = copy(G);
	
	// L=(nPATH~=0);
	gsl_matrix* L = compare_elements(nPATH, fp_not_equal, 0.0);
	
	// while find(L,1);
	gsl_vector* find_L = find(L, 1);
	while (find_L != NULL) {
		gsl_vector_free(find_L);
		
		// D=D+n.*L;
		gsl_matrix_scale(L, (double)n);
		gsl_matrix_add(D, L);
		
		// n=n+1;
		n++;
		
		// nPATH=nPATH*G;
		gsl_matrix* temp = mul(nPATH, G);
		gsl_matrix_free(nPATH);
		nPATH = temp;
		
		// L=(nPATH~=0).*(D==0);
		gsl_matrix_free(L);
		L = compare_elements(nPATH, fp_not_equal, 0.0);
		gsl_matrix* D_eq_0 = compare_elements(D, fp_equal, 0.0);
		gsl_matrix_mul_elements(L, D_eq_0);
		gsl_matrix_free(D_eq_0);
		
		find_L = find(L, 1);
	}
	
	gsl_matrix_free(nPATH);
	gsl_matrix_free(L);
	
	// D(~D)=inf;
	gsl_matrix* not_D = logical_not(D);
	logical_index_assign(D, not_D, GSL_POSINF);
	gsl_matrix_free(not_D);
	
	// D=D-eye(length(G));
	gsl_matrix* eye_length_G = eye_double(length(G));
	gsl_matrix_sub(D, eye_length_G);
	gsl_matrix_free(eye_length_G);
	
	return D;
}
