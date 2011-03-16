#include <cmath>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <limits>

FP_TYPE matlab::FP_ID(epsilon) = std::numeric_limits<FP_TYPE>::epsilon();

/*
 * Compares two floating-point numbers.
 */
int matlab::fp_compare(FP_TYPE x, FP_TYPE y) {
	if (fp_zero(x) && fp_zero(y)) {
		return 0;
	} else {
		int exponent;
		FP_TYPE max = (std::abs(x) > std::abs(y)) ? x : y;
		std::frexp(max, &exponent);
		FP_TYPE delta = std::ldexp(FP_ID(epsilon), exponent);
		FP_TYPE difference = x - y;
		if (difference > delta) {
			return 1;
		} else if (difference < -delta) {
			return -1;
		} else {
			return 0;
		}
	}
}

bool matlab::fp_zero(FP_TYPE x) { return std::abs(x) < FP_ID(epsilon); }
bool matlab::fp_nonzero(FP_TYPE x) { return std::abs(x) > FP_ID(epsilon); }
bool matlab::fp_equal(FP_TYPE x, FP_TYPE y) { return fp_compare(x, y) == 0; }
bool matlab::fp_not_equal(FP_TYPE x, FP_TYPE y) { return fp_compare(x, y) != 0; }
bool matlab::fp_less(FP_TYPE x, FP_TYPE y) { return fp_compare(x, y) == -1; }
bool matlab::fp_less_or_equal(FP_TYPE x, FP_TYPE y) { return fp_compare(x, y) <= 0; }
bool matlab::fp_greater(FP_TYPE x, FP_TYPE y) { return fp_compare(x, y) == 1; }
bool matlab::fp_greater_or_equal(FP_TYPE x, FP_TYPE y) { return fp_compare(x, y) >= 0; }

/*
 * Compares two vectors lexicographically, returning -1, 0, or 1 if the first
 * vector is less than, equal to, or greater than the second.
 */
int matlab::compare_vectors(const VECTOR_TYPE* v1, const VECTOR_TYPE* v2) {
	for (int i = 0; i < (int)v1->size; i++) {
		if (i >= (int)v2->size) {
			return 1;
		}
		int result = fp_compare((FP_TYPE)VECTOR_ID(get)(v1, i), (FP_TYPE)VECTOR_ID(get)(v2, i));
		if (result != 0) {
			return result;
		}
	}
	if (v1->size < v2->size) {
		return -1;
	} else {
		return 0;
	}
}

/*
 * Returns whether the first vector comes before the second vector in a strict
 * weak ordering.
 */
bool matlab::vector_less(VECTOR_TYPE* v1, VECTOR_TYPE* v2) {
	return compare_vectors(v1, v2) == -1;
}

/*
 * Compares two matrices lexicographically, returning -1, 0, or 1 if the first
 * matrix is less than, equal to, or greater than the second.
 */
int matlab::compare_matrices(const MATRIX_TYPE* m1, const MATRIX_TYPE* m2) {
	int size1 = (int)m1->size1 * (int)m1->size2;
	int size2 = (int)m2->size1 * (int)m2->size2;
	for (int i = 0; i < size1; i++) {
		if (i >= size2) {
			return 1;
		}
		int result = fp_compare((FP_TYPE)ordinal_index(m1, i), (FP_TYPE)ordinal_index(m2, i));
		if (result != 0) {
			return result;
		}
	}
	if (size1 < size2) {
		return -1;
	} else {
		return 0;
	}
}

/*
 * Returns whether the first matrix comes before the second matrix in a strict
 * weak ordering.
 */
bool matlab::matrix_less(MATRIX_TYPE* m1, MATRIX_TYPE* m2) {
	return compare_matrices(m1, m2) == -1;
}

/*
 * Emulates (v op x), where op is a binary comparison operator.
 */
VECTOR_TYPE* matlab::compare_elements(const VECTOR_TYPE* v, FP_ID(fp_cmp_fn) compare, FP_TYPE x) {
	VECTOR_TYPE* cmp_v = VECTOR_ID(alloc)(v->size);
	for (int i = 0; i < (int)v->size; i++) {
		FP_TYPE value = VECTOR_ID(get)(v, i);
		VECTOR_ID(set)(cmp_v, i, (FP_TYPE)compare((FP_TYPE)value, (FP_TYPE)x));
	}
	return cmp_v;
}

/*
 * Emulates (v1 op v2), where op is a binary comparison operator.
 */
VECTOR_TYPE* matlab::compare_elements(const VECTOR_TYPE* v1, FP_ID(fp_cmp_fn) compare, const VECTOR_TYPE* v2) {
	if (v1->size != v2->size) {
		return NULL;
	}
	VECTOR_TYPE* cmp_v = VECTOR_ID(alloc)(v1->size);
	for (int i = 0; i < (int)v1->size; i++) {
		FP_TYPE value1 = VECTOR_ID(get)(v1, i);
		FP_TYPE value2 = VECTOR_ID(get)(v2, i);
		VECTOR_ID(set)(cmp_v, i, (FP_TYPE)compare((FP_TYPE)value1, (FP_TYPE)value2));
	}
	return cmp_v;
}

/*
 * Emulates (m op x), where op is a binary comparison operator.
 */
MATRIX_TYPE* matlab::compare_elements(const MATRIX_TYPE* m, FP_ID(fp_cmp_fn) compare, FP_TYPE x) {
	MATRIX_TYPE* cmp_m = MATRIX_ID(alloc)(m->size1, m->size2);
	for (int i = 0; i < (int)m->size1; i++) {
		for (int j = 0; j < (int)m->size2; j++) {
			FP_TYPE value = MATRIX_ID(get)(m, i, j);
			MATRIX_ID(set)(cmp_m, i, j, (FP_TYPE)compare((FP_TYPE)value, (FP_TYPE)x));
		}
	}
	return cmp_m;
}

/*
 * Emulates (m1 op m2), where op is a binary comparison operator.
 */
MATRIX_TYPE* matlab::compare_elements(const MATRIX_TYPE* m1, FP_ID(fp_cmp_fn) compare, const MATRIX_TYPE* m2) {
	if (m1->size1 != m2->size1 || m1->size2 != m2->size2) {
		return NULL;
	}
	MATRIX_TYPE* cmp_m = MATRIX_ID(alloc)(m1->size1, m1->size2);
	for (int i = 0; i < (int)m1->size1; i++) {
		for (int j = 0; j < (int)m1->size2; j++) {
			FP_TYPE value1 = MATRIX_ID(get)(m1, i, j);
			FP_TYPE value2 = MATRIX_ID(get)(m2, i, j);
			MATRIX_ID(set)(cmp_m, i, j, (FP_TYPE)compare((FP_TYPE)value1, (FP_TYPE)value2));
		}
	}
	return cmp_m;
}
