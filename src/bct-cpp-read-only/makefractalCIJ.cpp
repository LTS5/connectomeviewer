#include "bct.h"
#include <cmath>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Generates a random directed binary graph with a hierarchical (fractal)
 * cluster organization.  Cluster connection density starts at 1 and decays as
 * (1 / (E ^ n)), where n is the hierarchical level index.  Cluster size is
 * given by (2 ^ sz_cl).
 */
gsl_matrix* bct::makefractalCIJ(int mx_lvl, double E, int sz_cl, int* K) {
	
	// t = ones(2).*2;
	gsl_matrix* t = gsl_matrix_alloc(2, 2);
	gsl_matrix_set_all(t, 2.0);
	
	// N = 2^mx_lvl;
	int N = (int)std::pow(2.0, mx_lvl);
	
	// sz_cl = sz_cl-1;
	sz_cl--;
	
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
	
	// ee = mx_lvl-CIJ-sz_cl;
	gsl_matrix* ee = copy(CIJ);
	gsl_matrix_scale(ee, -1.0);
	gsl_matrix_add_constant(ee, (double)(mx_lvl - sz_cl));
	
	// ee = (ee>0).*ee;
	gsl_matrix* temp = compare_elements(ee, fp_greater, 0.0);
	gsl_matrix_mul_elements(temp, ee);
	gsl_matrix_free(ee);
	ee = temp;
	
	// prob = (1./(E.^ee)).*(ones(s,s)-eye(s));
	gsl_matrix* E_m = gsl_matrix_alloc(s, s);
	gsl_matrix_set_all(E_m, E);
	gsl_matrix* neg_ee = copy(ee);
	gsl_matrix_scale(neg_ee, -1.0);
	gsl_matrix* prob = pow_elements(E_m, neg_ee);
	gsl_matrix_free(E_m);
	gsl_matrix_free(neg_ee);
	gsl_matrix* ones_s_sub_eye_s = ones_double(s, s);
	gsl_matrix* eye_s = eye_double(s);
	gsl_matrix_sub(ones_s_sub_eye_s, eye_s);
	gsl_matrix_free(eye_s);
	gsl_matrix_mul_elements(prob, ones_s_sub_eye_s);
	gsl_matrix_free(ones_s_sub_eye_s);
	
	// CIJ = (prob>rand(N));
	gsl_matrix* rand_N = rand_double(N);
	gsl_matrix_free(CIJ);
	CIJ = compare_elements(prob, fp_greater, rand_N);
	gsl_matrix_free(rand_N);
	
	// K = sum(sum(CIJ));
	if (K != NULL) {
		gsl_vector* sum_CIJ = sum(CIJ);
		*K = sum(sum_CIJ);
		gsl_vector_free(sum_CIJ);
	}
	
	return CIJ;
}
