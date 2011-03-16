#include "bct.h"
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>

/*
 * Given a distance matrix, computes Marchiori and Latora's connectivity length.
 */
double bct::connectivity_length(const gsl_matrix* D) {
	int N = D->size1;
	double sum = 0.0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			double value = gsl_matrix_get(D, i, j);
			if (gsl_finite(value) == 1 && fp_nonzero(value)) {
				sum += 1.0 / value;
			}
		}
	}
	if (fp_zero(sum)) {
		return GSL_POSINF;
	} else {
		return (double)(N * (N - 1)) / sum;
	}
}
