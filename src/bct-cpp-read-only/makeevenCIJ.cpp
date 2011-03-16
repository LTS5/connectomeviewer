#include "bct.h"
#include <cmath>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector.h>

/*
 * Generates a random directed binary graph with equal-sized clusters placed on
 * the diagonal and the remaining connections distributed randomly among them.
 * N must be a power of 2, and the cluster size is given by (2 ^ sz_cl).
 */
gsl_matrix* bct::makeevenCIJ(int N, int K, int sz_cl) {

	// mx_lvl = floor(log2(N));
	int mx_lvl = (int)std::floor(std::log((double)N) / std::log(2.0));
	
	// sz_cl = sz_cl-1;
	sz_cl--;

	// t = ones(2).*2;
	gsl_matrix* t = gsl_matrix_alloc(2, 2);
	gsl_matrix_set_all(t, 2.0);

	// Nlvl = 2^mx_lvl;
	double Nlvl = std::pow(2.0, mx_lvl);
	
	// N = Nlvl;
	N = (int)Nlvl;
	
	gsl_matrix* CIJ = NULL;
	
	// for lvl=1:mx_lvl-1
	for (int lvl = 1; lvl <= mx_lvl - 1; lvl++) {
		
		// CIJ = ones(2^(lvl+1),2^(lvl+1));
		int n = (int)std::pow(2.0, lvl + 1);
		CIJ = ones_double(n, n);
		
		// group1 = [1:size(CIJ,1)/2];
		gsl_vector* group1 = sequence_double(0, n / 2 - 1);
		
		// group2 = [size(CIJ,1)/2+1:size(CIJ,1)];
		gsl_vector* group2 = sequence_double(n / 2, n - 1);
		
		// CIJ(group1,group1) = t;
		ordinal_index_assign(CIJ, group1, group1, t);
		gsl_vector_free(group1);
		
		// CIJ(group2,group2) = t;
		ordinal_index_assign(CIJ, group2, group2, t);
		gsl_vector_free(group2);
		
		// CIJ = CIJ+ones(size(CIJ,1),size(CIJ,1));
		gsl_matrix_add_constant(CIJ, 1.0);
		
		// t = CIJ;
		gsl_matrix_free(t);
		t = CIJ;
	}
	
	// s = size(CIJ,1);
	int s = CIJ->size1;
	
	// CIJ = CIJ-ones(s,s)-mx_lvl.*eye(s);
	gsl_matrix_add_constant(CIJ, -1.0);
	gsl_matrix* mx_lvl_mul_eye_s = eye_double(s);
	gsl_matrix_scale(mx_lvl_mul_eye_s, (double)mx_lvl);
	gsl_matrix_sub(CIJ, mx_lvl_mul_eye_s);
	gsl_matrix_free(mx_lvl_mul_eye_s);

	// CIJp = (CIJ>=(mx_lvl-sz_cl));
	gsl_matrix* CIJp = compare_elements(CIJ, fp_greater_or_equal, (double)(mx_lvl - sz_cl));
	gsl_matrix_free(CIJ);

	// CIJc = (CIJp==1);
	gsl_matrix* CIJc = compare_elements(CIJp, fp_equal, 1.0);
	gsl_matrix_free(CIJp);
	
	// remK = K-nnz(CIJc);
	int remK = K - nnz(CIJc);
	
	if (remK > 0) {
		
		// [a,b] = find(~(CIJc+eye(N)));
		gsl_matrix* CIJc_add_eye_N = copy(CIJc);
		gsl_matrix* eye_N = eye_double(N);
		gsl_matrix_add(CIJc_add_eye_N, eye_N);
		gsl_matrix_free(eye_N);
		gsl_matrix* not_CIJc_add_eye_N = logical_not(CIJc_add_eye_N);
		gsl_matrix_free(CIJc_add_eye_N);
		gsl_matrix* find_not_CIJc_add_eye_N = find_ij(not_CIJc_add_eye_N);
		gsl_matrix_free(not_CIJc_add_eye_N);
		gsl_vector_view a = gsl_matrix_column(find_not_CIJc_add_eye_N, 0);
		gsl_vector_view b = gsl_matrix_column(find_not_CIJc_add_eye_N, 1);
		
		// rp = randperm(length(a));
		gsl_permutation* rp = randperm(length(&a.vector));
		gsl_vector* rp_v = to_vector_double(rp);
		gsl_permutation_free(rp);
		gsl_vector_view rp_subv = gsl_vector_subvector(rp_v, 0, remK);
		
		// a = a(rp(1:remK));
		gsl_vector* a_rp_subv = ordinal_index(&a.vector, &rp_subv.vector);
		gsl_vector* ab_indices = sequence_double(0, remK - 1);
		ordinal_index_assign(&a.vector, ab_indices, a_rp_subv);
		gsl_vector_free(a_rp_subv);
		
		// b = b(rp(1:remK));
		gsl_vector* b_rp_subv = ordinal_index(&b.vector, &rp_subv.vector);
		gsl_vector_free(rp_v);
		ordinal_index_assign(&b.vector, ab_indices, b_rp_subv);
		gsl_vector_free(ab_indices);
		gsl_vector_free(b_rp_subv);
		
		// for i=1:remK
		for (int i = 0; i < remK; i++) {
			
			// CIJc(a(i),b(i)) = 1;
			gsl_matrix_set(CIJc, (int)gsl_vector_get(&a.vector, i), (int)gsl_vector_get(&b.vector, i), 1.0);
		}
		
		gsl_matrix_free(find_not_CIJc_add_eye_N);
	}

	// CIJ = CIJc;
	return CIJc;
}
