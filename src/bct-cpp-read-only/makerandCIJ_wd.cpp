#include "bct.h"
#include <ctime>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_permute_vector.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_vector.h>

/*
 * Generates a random directed weighted graph with N nodes and K edges.  Weights
 * are chosen uniformly between wmin and wmax.  No edges are placed on the main
 * diagonal.
 */
gsl_matrix* bct::makerandCIJ_wd(int N, int K, double wmin, double wmax) {
	gsl_rng* rng = get_gsl_rng();
	gsl_vector* w = gsl_vector_alloc(K);
	for (int i = 0; i < K; i++) {
		gsl_vector_set(w, i, gsl_rng_uniform(rng) * (wmax - wmin) + wmin);
	}
	
	// ind = ~eye(N);
	gsl_matrix* eye_N = eye_double(N);
	gsl_matrix* ind = logical_not(eye_N);
	gsl_matrix_free(eye_N);
	
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
	
	// CIJ(irp(1:K)) = w;
	gsl_vector_view irp_subv = gsl_vector_subvector(irp, 0, K);
	ordinal_index_assign(CIJ, &irp_subv.vector, w);
	gsl_vector_free(w);
	gsl_vector_free(irp);
	
	return CIJ;
}

/*
 * Generates a random directed weighted graph with the same number of nodes,
 * number of edges, and weight distribution as the given graph.  No edges are
 * placed on the main diagonal.  The given matrix should therefore not contain
 * nonzero entries on the main diagonal.
 */
gsl_matrix* bct::makerandCIJ_wd_wp(const gsl_matrix* m) {
	if (safe_mode) check_status(m, SQUARE | NO_LOOPS, "makerandCIJ_wd_wp");
	int N = m->size1;
	int K = N * (N - 1);
	double* w = new double[K];
	for (int i = 0, k = 0; i < (int)m->size1; i++) {
		for (int j = 0; j < (int)m->size2; j++) {
			if (i != j) {
				w[k++] = gsl_matrix_get(m, i, j);
			}
		}
	}
	gsl_rng* rng = get_gsl_rng();
	gsl_ran_shuffle(rng, w, K, sizeof(double));
	gsl_matrix* rand_m = gsl_matrix_alloc(m->size1, m->size2);
	for (int i = 0, k = 0; i < (int)m->size1; i++) {
		for (int j = 0; j < (int)m->size2; j++) {
			if (i == j) {
				gsl_matrix_set(rand_m, i, j, 0.0);
			} else {
				gsl_matrix_set(rand_m, i, j, w[k++]);
			}
		}
	}
	delete[] w;
	return rand_m;
}
