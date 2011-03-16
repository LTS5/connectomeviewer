#include "bct.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <vector>

/*
 * Finds walks.  Note that there is no twalk argument as its value may overflow
 * a C++ long.  Wq (the main return) and wlq are indexed by path length.  They
 * therefore contain no valid data at index 0.
 */
std::vector<gsl_matrix*> bct::findwalks(const gsl_matrix* CIJ, gsl_vector** wlq) {
	if (safe_mode) check_status(CIJ, SQUARE, "findwalks");
	
	// CIJ = double(CIJ~=0);
	gsl_matrix* _CIJ = compare_elements(CIJ, fp_not_equal, 0.0);
	
	// N = size(CIJ,1);
	int N = CIJ->size1;
	
	// Wq = zeros(N,N,N);
	std::vector<gsl_matrix*> Wq(N + 1);
	Wq[0] = NULL;
	for (int i = 1; i <= N; i++) {
		Wq[i] = zeros_double(N, N);
	}
	
	// CIJpwr = CIJ;
	gsl_matrix* _CIJpwr = copy(_CIJ);
	
	// Wq(:,:,1) = CIJ;
	Wq[1] = copy(_CIJ);
	
	// for q=2:N
	for (int q = 2; q <= N; q++) {
		
		// CIJpwr = CIJpwr*CIJ;
		gsl_matrix* temp = mul(_CIJpwr, _CIJ);
		gsl_matrix_free(_CIJpwr);
		_CIJpwr = temp;
		
		// Wq(:,:,q) = CIJpwr;
		Wq[q] = copy(_CIJpwr);
	}
	
	gsl_matrix_free(_CIJ);
	gsl_matrix_free(_CIJpwr);
	
	// twalk = sum(sum(sum(Wq)));
	// wlq = reshape(sum(sum(Wq)),1,N);
	if (wlq != NULL) {
		*wlq = gsl_vector_alloc(N + 1);
		gsl_vector_set(*wlq, 0, 0.0);
		for (int i = 1; i <= N; i++) {
			gsl_vector* sum_Wq_i = sum(Wq[i]);
			gsl_vector_set(*wlq, i, sum(sum_Wq_i));
			gsl_vector_free(sum_Wq_i);
		}
	}
	
	return Wq;
}
