#define GSL_DOUBLE
#include "macros.h"

#include <cmath>
#include <ctime>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_vector.h>
#include "matlab.h"
#include <string>

/*
 * This file contains functions for MATLAB emulation that do not depend on the
 * desired floating-point precision.
 */

std::string matlab::dec2bin(int n) {
	return dec2bin(n, 0);
}

std::string matlab::dec2bin(int n, int len) {
	if (n < 0) {
		return "";
	}
	int binlen = (int)(std::floor(1.0 + std::log(n) / std::log(2)));
	std::string bin((len > binlen) ? len : binlen, '0');
	for (int i = bin.size() - 1; i >= 0; i--) {
		int remainder = n % 2;
		if (remainder) {
			bin[i] = '1';
		}
		n >>= 1;
	}
	return bin;
}

gsl_matrix* matlab::inv(const gsl_matrix* m) {
	if (m->size1 != m->size2) {
		return NULL;
	}
	gsl_matrix* LU = copy(m);
	gsl_permutation* p = gsl_permutation_alloc(m->size1);
	int signum;
	gsl_linalg_LU_decomp(LU, p, &signum);
	gsl_matrix* inv_m = NULL;
	if (fp_nonzero(gsl_linalg_LU_det(LU, signum))) {
		inv_m = gsl_matrix_alloc(m->size1, m->size2);
		gsl_linalg_LU_invert(LU, p, inv_m);
	}
	gsl_matrix_free(LU);
	gsl_permutation_free(p);
	return inv_m;
}

/*
 * Generates a permutation of the integers 0 to (size - 1), whereas the MATLAB
 * version uses the integers 1 to size.
 */
gsl_permutation* matlab::randperm(int size) {
	gsl_rng* rng = get_gsl_rng();
	double values[size];
	for (int i = 0; i < size; i++) {
		values[i] = (double)i;
	}
	gsl_ran_shuffle(rng, values, size, sizeof(double));
	gsl_vector_view values_vv = gsl_vector_view_array(values, size);
	gsl_permutation* values_p = to_permutation(&values_vv.vector);
	return values_p;
}

/*
 * Emulates (m1 \ m2) = (inv(m1) * m2).
 */
gsl_matrix* matlab::div_left(const gsl_matrix* m1, const gsl_matrix* m2) {
	if (m1->size1 != m1->size2 || m2->size1 != m2->size2 || m1->size1 != m2->size1) {
		return NULL;
	}
	gsl_matrix* inv_m1 = inv(m1);
	gsl_matrix* div_m = mul(inv_m1, m2);
	gsl_matrix_free(inv_m1);
	return div_m;
}

/*
 * Emulates (m1 / m2) = ((inv(m2') * m1')').
 */
gsl_matrix* matlab::div_right(const gsl_matrix* m1, const gsl_matrix* m2) {
	if (m1->size1 != m1->size2 || m2->size1 != m2->size2 || m1->size1 != m2->size1) {
		return NULL;
	}
	gsl_matrix* m2_transpose = gsl_matrix_alloc(m2->size2, m2->size1);
	gsl_matrix_transpose_memcpy(m2_transpose, m2);
	gsl_matrix* inv_m2_transpose = inv(m2_transpose);
	gsl_matrix_free(m2_transpose);
	gsl_matrix* m1_transpose = gsl_matrix_alloc(m1->size2, m1->size1);
	gsl_matrix_transpose_memcpy(m1_transpose, m1);
	gsl_matrix* div_m = mul(inv_m2_transpose, m1_transpose);
	gsl_matrix_free(inv_m2_transpose);
	gsl_matrix_free(m1_transpose);
	gsl_matrix_transpose(div_m);
	return div_m;
}

/*
 * Returns a random number generator that is guaranteed to be seeded only once
 * during program execution.  This generator should not be freed by the caller.
 */
gsl_rng* matlab::get_gsl_rng() {
	static gsl_rng* rng = NULL;
	if (rng == NULL) {
		gsl_rng_default_seed = std::time(NULL);
		rng = gsl_rng_alloc(gsl_rng_default);
	}
	return rng;
}

/*
 * Seeds the given random number generator.
 */
void matlab::seed_rng(const gsl_rng* rng, unsigned long seed) {
	gsl_rng_set(rng, seed);
}
