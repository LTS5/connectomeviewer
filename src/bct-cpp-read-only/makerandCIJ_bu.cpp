#include "bct.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_permute_vector.h>
#include <gsl/gsl_vector.h>

/*
 * Generates a random undirected binary graph with N nodes and K edges.  No
 * edges are placed on the main diagonal.
 */
gsl_matrix* bct::makerandCIJ_bu(int N, int K) {
	
	// ind = triu(~eye(N));
	gsl_matrix* eye_N = eye_double(N);
	gsl_matrix* not_eye_N = logical_not(eye_N);
	gsl_matrix_free(eye_N);
	gsl_matrix* ind = triu(not_eye_N);
	gsl_matrix_free(not_eye_N);
	
	// i = find(ind);
	gsl_vector* i = find(ind);
	gsl_matrix_free(ind);
	
	// rp = randperm(length(i));
	gsl_permutation* rp = randperm(length(i));
	
	// irp = i(rp);
	gsl_permute_vector(rp, i);
	gsl_permutation_free(rp);
	gsl_vector* irp = i;
	
	// CIJ = zeros(N);
	gsl_matrix* CIJ = zeros_double(N);
	
	// CIJ(irp(1:K)) = 1;
	gsl_vector_view irp_subv = gsl_vector_subvector(irp, 0, K);
	ordinal_index_assign(CIJ, &irp_subv.vector, 1.0);
	gsl_vector_free(irp);
	
	// CIJ = CIJ+CIJ';
	gsl_matrix* CIJ_transpose = gsl_matrix_alloc(CIJ->size2, CIJ->size1);
	gsl_matrix_transpose_memcpy(CIJ_transpose, CIJ);
	gsl_matrix_add(CIJ, CIJ_transpose);
	gsl_matrix_free(CIJ_transpose);
	
	return CIJ;
}
