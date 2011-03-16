#include "bct.h"
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Computes the range for each edge (i.e., the shortest path length between the
 * nodes it connects after the edge has been removed from the graph)
 */
gsl_matrix* bct::erange(const gsl_matrix* CIJ, double* eta, gsl_matrix** Eshort, double* fs) {
	if (safe_mode) check_status(CIJ, SQUARE, "erange");
	
	// N = size(CIJ,1);
	int N = CIJ->size1;
	
	// K = length(nonzeros(CIJ));
	int k = nnz(CIJ);
	
	// Erange = zeros(N,N);
	gsl_matrix* Erange = zeros_double(N, N);
	
	// [i,j] = find(CIJ==1);
	gsl_matrix* CIJ_eq_1 = compare_elements(CIJ, fp_equal, 1.0);
	gsl_matrix* find_CIJ_eq_1 = find_ij(CIJ_eq_1);
	gsl_matrix_free(CIJ_eq_1);	
	gsl_vector_view i = gsl_matrix_column(find_CIJ_eq_1, 0);
	gsl_vector_view j = gsl_matrix_column(find_CIJ_eq_1, 1);
	
	// for c=1:length(i)
	for (int c = 0; c < length(&i.vector); c++) {
		
		// CIJcut = CIJ;
		gsl_matrix* CIJcut = copy(CIJ);
		
		// CIJcut(i(c),j(c)) = 0;
		int i_c = (int)gsl_vector_get(&i.vector, c);
		int j_c = (int)gsl_vector_get(&j.vector, c);
		gsl_matrix_set(CIJcut, i_c, j_c, 0.0);
		
		// [R,D] = reachdist(CIJcut);
		gsl_matrix* D;
		gsl_matrix* R = reachdist(CIJcut, &D);
		gsl_matrix_free(CIJcut);
		gsl_matrix_free(R);
		
		// Erange(i(c),j(c)) = D(i(c),j(c))
		gsl_matrix_set(Erange, i_c, j_c, gsl_matrix_get(D, i_c, j_c));
		gsl_matrix_free(D);
	}
	
	gsl_matrix_free(find_CIJ_eq_1);
	
	// eta = sum(Erange((Erange>0)&(Erange<Inf)))/length(Erange((Erange>0)&(Erange<Inf)));
	if (eta != NULL) {
		gsl_matrix* Erange_gt_0 = compare_elements(Erange, fp_greater, 0.0);
		gsl_matrix* Erange_lt_inf = compare_elements(Erange, fp_less, GSL_POSINF);
		gsl_matrix* Erange_gt_0_and_Erange_lt_inf = logical_and(Erange_gt_0, Erange_lt_inf);
		gsl_vector* Erange_idx = logical_index(Erange, Erange_gt_0_and_Erange_lt_inf);
		gsl_matrix_free(Erange_gt_0);
		gsl_matrix_free(Erange_lt_inf);
		gsl_matrix_free(Erange_gt_0_and_Erange_lt_inf);
		*eta = sum(Erange_idx) / (double)length(Erange_idx);
		gsl_vector_free(Erange_idx);
	}
	
	// Eshort = Erange>2;
	gsl_matrix* _Eshort = compare_elements(Erange, fp_greater, 2.0);
	
	// fs = length(nonzeros(Eshort))/K;
	if (fs != NULL) {
		*fs = (double)nnz(_Eshort) / (double)k;
	}
	
	if (Eshort != NULL) *Eshort = _Eshort; else gsl_matrix_free(_Eshort);
	return Erange;
}
