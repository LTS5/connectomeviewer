#include "bct.h"
#include <gsl/gsl_matrix.h>

/*
 * Applies an absolute weight threshold to a graph.  All weights below this
 * threshold, as well as those on the main diagonal, are set to zero.
 */
gsl_matrix* bct::threshold_absolute(const gsl_matrix* W, double thr) {
	if (safe_mode) check_status(W, SQUARE, "threshold_absolute");
	
	gsl_matrix* W_thr = copy(W);
	
	// W(1:size(W,1)+1:end)=0;
	for (int i = 0; i < (int)W_thr->size1; i++) {
		gsl_matrix_set(W_thr, i, i, 0.0);
	}
	
	// W(W<thr)=0;
	gsl_matrix* W_thr_lt_thr = compare_elements(W_thr, fp_less, thr);
	logical_index_assign(W_thr, W_thr_lt_thr, 0.0);
	gsl_matrix_free(W_thr_lt_thr);
	
	return W_thr;
}
