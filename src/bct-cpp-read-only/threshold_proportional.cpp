#include "bct.h"
#include <cmath>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Preserves a given proportion of the strongest weights in a directed graph.
 * All other weights, as well as those on the main diagonal, are set to zero.
 */
gsl_matrix* bct::threshold_proportional_dir(const gsl_matrix* W, double p) {
	if (safe_mode) check_status(W, SQUARE | DIRECTED, "threshold_proportional_dir");
	
	// n=size(W,1);
	int n = W->size1;
	
	// W(1:n+1:end)=0;
	gsl_matrix* W_thr = copy(W);
	for (int i = 0; i < (int)W_thr->size1; i++) {
		gsl_matrix_set(W_thr, i, i, 0.0);
	}
	
	// ind=find(W);
	gsl_vector* ind = find(W_thr);
	
	// E=sortrows([ind W(ind)], -2);
	gsl_vector* W_thr_ind = ordinal_index(W_thr, ind);
	gsl_vector* sort_ind;
	gsl_vector* sort_W_thr_ind = sort(W_thr_ind, "descend", &sort_ind);
	gsl_vector_free(W_thr_ind);
	gsl_vector_free(sort_W_thr_ind);
	
	// en=round((n^2-n)*p);
	int en = (int)std::floor(n * (n - 1) * p + 0.5);
	
	// W(E(en+1:end,1))=0;
	for (int i = en; i < (int)sort_ind->size; i++) {
		int index = gsl_vector_get(ind, gsl_vector_get(sort_ind, i));
		ordinal_index_assign(W_thr, index, 0.0);
	}
	
	gsl_vector_free(ind);
	gsl_vector_free(sort_ind);
	return W_thr;
}

/*
 * Preserves a given proportion of the strongest weights in an undirected graph.
 * All other weights, as well as those on the main diagonal, are set to zero.
 */
gsl_matrix* bct::threshold_proportional_und(const gsl_matrix* W, double p) {
	if (safe_mode) check_status(W, SQUARE | UNDIRECTED, "threshold_proportional_und");
	
	// n=size(W,1);
	int n = W->size1;
	
	// W(1:n+1:end)=0;
	gsl_matrix* W_thr = triu(W, 1);
	
	// ind=find(W);
	gsl_vector* ind = find(W_thr);
	
	// E=sortrows([ind W(ind)], -2);
	gsl_vector* W_thr_ind = ordinal_index(W_thr, ind);
	gsl_vector* sort_ind;
	gsl_vector* sort_W_thr_ind = sort(W_thr_ind, "descend", &sort_ind);
	gsl_vector_free(W_thr_ind);
	gsl_vector_free(sort_W_thr_ind);
	
	// en=round((n^2-n)*p);
	int en = (int)std::floor(0.5 * n * (n - 1) * p + 0.5);
	
	// W(E(en+1:end,1))=0;
	for (int i = en; i < (int)sort_ind->size; i++) {
		int index = gsl_vector_get(ind, gsl_vector_get(sort_ind, i));
		ordinal_index_assign(W_thr, index, 0.0);
	}
	for (int i = 0; i < (int)W_thr->size1; i++) {
		for (int j = i + 1; j < (int)W_thr->size2; j++) {
			double value = gsl_matrix_get(W_thr, i, j);
			gsl_matrix_set(W_thr, j, i, value);
		}
	}
	
	gsl_vector_free(ind);
	gsl_vector_free(sort_ind);
	return W_thr;
}
