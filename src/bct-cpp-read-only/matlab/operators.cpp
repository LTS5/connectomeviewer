#include <cmath>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Emulates ([v x]) for a row vector or ([v ; x]) for a column vector.
 */
VECTOR_TYPE* matlab::concatenate(const VECTOR_TYPE* v, FP_TYPE x) {
	if (v == NULL) {
		VECTOR_TYPE* cat_v = VECTOR_ID(alloc)(1);
		VECTOR_ID(set)(cat_v, 0, x);
		return cat_v;
	}
	VECTOR_TYPE* cat_v = VECTOR_ID(alloc)(v->size + 1);
	VECTOR_ID(view) cat_subv = VECTOR_ID(subvector)(cat_v, 0, v->size);
	VECTOR_ID(memcpy)(&cat_subv.vector, v);
	VECTOR_ID(set)(cat_v, v->size, x);
	return cat_v;
}

/*
 * Emulates ([x v]) for a row vector or ([x ; v]) for a column vector.
 */
VECTOR_TYPE* matlab::concatenate(FP_TYPE x, const VECTOR_TYPE* v) {
	if (v == NULL) {
		VECTOR_TYPE* cat_v = VECTOR_ID(alloc)(1);
		VECTOR_ID(set)(cat_v, 0, x);
		return cat_v;
	}
	VECTOR_TYPE* cat_v = VECTOR_ID(alloc)(v->size + 1);
	VECTOR_ID(view) cat_subv = VECTOR_ID(subvector)(cat_v, 1, v->size);
	VECTOR_ID(memcpy)(&cat_subv.vector, v);
	VECTOR_ID(set)(cat_v, 0, x);
	return cat_v;
}
 
/*
 * Emulates ([v1 v2]) for row vectors or ([v1 ; v2]) for column vectors.
 */
VECTOR_TYPE* matlab::concatenate(const VECTOR_TYPE* v1, const VECTOR_TYPE* v2) {
	if (v1 == NULL && v2 == NULL) {
		return NULL;
	} else if (v1 == NULL) {
		return copy(v2);
	} else if (v2 == NULL) {
		return copy(v1);
	}
	VECTOR_TYPE* cat_v = VECTOR_ID(alloc)(v1->size + v2->size);
	VECTOR_ID(view) cat_subv1 = VECTOR_ID(subvector)(cat_v, 0, v1->size);
	VECTOR_ID(view) cat_subv2 = VECTOR_ID(subvector)(cat_v, v1->size, v2->size);
	VECTOR_ID(memcpy)(&cat_subv1.vector, v1);
	VECTOR_ID(memcpy)(&cat_subv2.vector, v2);
	return cat_v;
}

/*
 * Emulates ([v1 ; v2]) for row vectors.
 */
MATRIX_TYPE* matlab::concatenate_columns(const VECTOR_TYPE* v1, const VECTOR_TYPE* v2) {
	if (v1 == NULL && v2 == NULL) {
		return NULL;
	} else if (v1 == NULL) {
		return to_row_matrix(v2);
	} else if (v2 == NULL) {
		return to_row_matrix(v1);
	} else if (v1->size != v2->size) {
		return NULL;
	}
	MATRIX_TYPE* cat_m = MATRIX_ID(alloc)(2, v1->size);
	MATRIX_ID(set_row)(cat_m, 0, v1);
	MATRIX_ID(set_row)(cat_m, 1, v2);
	return cat_m;
}

/*
 * Emulates ([m ; v]) for a row vector.
 */
MATRIX_TYPE* matlab::concatenate_columns(const MATRIX_TYPE* m, const VECTOR_TYPE* v) {
	if (m == NULL && v == NULL) {
		return NULL;
	} else if (m == NULL) {
		return to_row_matrix(v);
	} else if (v == NULL) {
		return copy(m);
	} else if (m->size2 != v->size) {
		return NULL;
	}
	MATRIX_TYPE* cat_m = MATRIX_ID(alloc)(m->size1 + 1, m->size2);
	MATRIX_ID(view) cat_subm = MATRIX_ID(submatrix)(cat_m, 0, 0, m->size1, m->size2);
	MATRIX_ID(memcpy)(&cat_subm.matrix, m);
	MATRIX_ID(set_row)(cat_m, m->size1, v);
	return cat_m;
}

/*
 * Emulates ([v ; m]) for a row vector.
 */
MATRIX_TYPE* matlab::concatenate_columns(const VECTOR_TYPE* v, const MATRIX_TYPE* m) {
	if (m == NULL && v == NULL) {
		return NULL;
	} else if (m == NULL) {
		return to_row_matrix(v);
	} else if (v == NULL) {
		return copy(m);
	} else if (m->size2 != v->size) {
		return NULL;
	}
	MATRIX_TYPE* cat_m = MATRIX_ID(alloc)(m->size1 + 1, m->size2);
	MATRIX_ID(set_row)(cat_m, 0, v);
	MATRIX_ID(view) cat_subm = MATRIX_ID(submatrix)(cat_m, 1, 0, m->size1, m->size2);
	MATRIX_ID(memcpy)(&cat_subm.matrix, m);
	return cat_m;
}

/*
 * Emulates ([m1 ; m2]).
 */
MATRIX_TYPE* matlab::concatenate_columns(const MATRIX_TYPE* m1, const MATRIX_TYPE* m2) {
	if (m1 == NULL && m2 == NULL) {
		return NULL;
	} else if (m1 == NULL) {
		return copy(m2);
	} else if (m2 == NULL) {
		return copy(m1);
	} else if (m1->size2 != m2->size2) {
		return NULL;
	}
	MATRIX_TYPE* cat_m = MATRIX_ID(alloc)(m1->size1 + m2->size1, m1->size2);
	MATRIX_ID(view) cat_subm1 = MATRIX_ID(submatrix)(cat_m, 0, 0, m1->size1, m1->size2);
	MATRIX_ID(view) cat_subm2 = MATRIX_ID(submatrix)(cat_m, m1->size1, 0, m2->size1, m2->size2);
	MATRIX_ID(memcpy)(&cat_subm1.matrix, m1);
	MATRIX_ID(memcpy)(&cat_subm2.matrix, m2);
	return cat_m;
}

/*
 * Emulates ([v1 v2]) for column vectors.
 */
MATRIX_TYPE* matlab::concatenate_rows(const VECTOR_TYPE* v1, const VECTOR_TYPE* v2) {
	if (v1 == NULL && v2 == NULL) {
		return NULL;
	} else if (v1 == NULL) {
		return to_column_matrix(v2);
	} else if (v2 == NULL) {
		return to_column_matrix(v1);
	} else if (v1->size != v2->size) {
		return NULL;
	}
	MATRIX_TYPE* cat_m = MATRIX_ID(alloc)(v1->size, 2);
	MATRIX_ID(set_col)(cat_m, 0, v1);
	MATRIX_ID(set_col)(cat_m, 1, v2);
	return cat_m;
}

/*
 * Emulates ([m v]) for a column vector.
 */
MATRIX_TYPE* matlab::concatenate_rows(const MATRIX_TYPE* m, const VECTOR_TYPE* v) {
	if (m == NULL && v == NULL) {
		return NULL;
	} else if (m == NULL) {
		return to_column_matrix(v);
	} else if (v == NULL) {
		return copy(m);
	} else if (m->size1 != v->size) {
		return NULL;
	}
	MATRIX_TYPE* cat_m = MATRIX_ID(alloc)(m->size1, m->size2 + 1);
	MATRIX_ID(view) cat_subm = MATRIX_ID(submatrix)(cat_m, 0, 0, m->size1, m->size2);
	MATRIX_ID(memcpy)(&cat_subm.matrix, m);
	MATRIX_ID(set_col)(cat_m, m->size2, v);
	return cat_m;
}

/*
 * Emulates ([v m]) for a column vector.
 */
MATRIX_TYPE* matlab::concatenate_rows(const VECTOR_TYPE* v, const MATRIX_TYPE* m) {
	if (m == NULL && v == NULL) {
		return NULL;
	} else if (m == NULL) {
		return to_column_matrix(v);
	} else if (v == NULL) {
		return copy(m);
	} else if (m->size1 != v->size) {
		return NULL;
	}
	MATRIX_TYPE* cat_m = MATRIX_ID(alloc)(m->size1, m->size2 + 1);
	MATRIX_ID(set_col)(cat_m, 0, v);
	MATRIX_ID(view) cat_subm = MATRIX_ID(submatrix)(cat_m, 0, 1, m->size1, m->size2);
	MATRIX_ID(memcpy)(&cat_subm.matrix, m);
	return cat_m;
}

/*
 * Emulates ([m1 m2]).
 */
MATRIX_TYPE* matlab::concatenate_rows(const MATRIX_TYPE* m1, const MATRIX_TYPE* m2) {
	if (m1 == NULL && m2 == NULL) {
		return NULL;
	} else if (m1 == NULL) {
		return copy(m2);
	} else if (m2 == NULL) {
		return copy(m1);
	} else if (m1->size1 != m2->size1) {
		return NULL;
	}
	MATRIX_TYPE* cat_m = MATRIX_ID(alloc)(m1->size1, m1->size2 + m2->size2);
	MATRIX_ID(view) cat_subm1 = MATRIX_ID(submatrix)(cat_m, 0, 0, m1->size1, m1->size2);
	MATRIX_ID(view) cat_subm2 = MATRIX_ID(submatrix)(cat_m, 0, m1->size2, m2->size1, m2->size2);
	MATRIX_ID(memcpy)(&cat_subm1.matrix, m1);
	MATRIX_ID(memcpy)(&cat_subm2.matrix, m2);
	return cat_m;
}

/*
 * Emulates copy assignment.
 */
VECTOR_TYPE* matlab::copy(const VECTOR_TYPE* v) {
	VECTOR_TYPE* copy_v = VECTOR_ID(alloc)(v->size);
	VECTOR_ID(memcpy)(copy_v, v);
	return copy_v;
}

/*
 * Emulates copy assignment.
 */
MATRIX_TYPE* matlab::copy(const MATRIX_TYPE* m) {
	MATRIX_TYPE* copy_m = MATRIX_ID(alloc)(m->size1, m->size2);
	MATRIX_ID(memcpy)(copy_m, m);
	return copy_m;
}

/*
 * Emulates (v1 & v2).
 */
VECTOR_TYPE* matlab::logical_and(const VECTOR_TYPE* v1, const VECTOR_TYPE* v2) {
	if (v1->size != v2->size) {
		return NULL;
	}
	VECTOR_TYPE* and_v = VECTOR_ID(alloc)(v1->size);
	for (int i = 0; i < (int)v1->size; i++) {
		bool nz1 = fp_nonzero(VECTOR_ID(get)(v1, i));
		bool nz2 = fp_nonzero(VECTOR_ID(get)(v2, i));
		VECTOR_ID(set)(and_v, i, (FP_TYPE)(nz1 && nz2));
	}
	return and_v;
}

/*
 * Emulates (m1 & m2).
 */
MATRIX_TYPE* matlab::logical_and(const MATRIX_TYPE* m1, const MATRIX_TYPE* m2) {
	if (m1->size1 != m2->size1 || m1->size2 != m2->size2) {
		return NULL;
	}
	MATRIX_TYPE* and_m = MATRIX_ID(alloc)(m1->size1, m1->size2);
	for (int i = 0; i < (int)m1->size1; i++) {
		for (int j = 0; j < (int)m1->size2; j++) {
			bool nz1 = fp_nonzero(MATRIX_ID(get)(m1, i, j));
			bool nz2 = fp_nonzero(MATRIX_ID(get)(m2, i, j));
			MATRIX_ID(set)(and_m, i, j, (FP_TYPE)(nz1 && nz2));
		}
	}
	return and_m;
}

/*
 * Emulates (~v).
 */
VECTOR_TYPE* matlab::logical_not(const VECTOR_TYPE* v) {
	VECTOR_TYPE* not_v = VECTOR_ID(alloc)(v->size);
	for (int i = 0; i < (int)v->size; i++) {
		bool z = fp_zero(VECTOR_ID(get)(v, i));
		VECTOR_ID(set)(not_v, i, (FP_TYPE)z);
	}
	return not_v;
}

/*
 * Emulates (~m)
 */
MATRIX_TYPE* matlab::logical_not(const MATRIX_TYPE* m) {
	MATRIX_TYPE* not_m = MATRIX_ID(alloc)(m->size1, m->size2);
	for (int i = 0; i < (int)m->size1; i++) {
		for (int j = 0; j < (int)m->size2; j++) {
			bool z = fp_zero(MATRIX_ID(get)(m, i, j));
			MATRIX_ID(set)(not_m, i, j, (FP_TYPE)z);
		}
	}
	return not_m;
}

/*
 * Emulates (v1 | v2).
 */
VECTOR_TYPE* matlab::logical_or(const VECTOR_TYPE* v1, const VECTOR_TYPE* v2) {
	if (v1->size != v2->size) {
		return NULL;
	}
	VECTOR_TYPE* or_v = VECTOR_ID(alloc)(v1->size);
	for (int i = 0; i < (int)v1->size; i++) {
		bool nz1 = fp_nonzero(VECTOR_ID(get)(v1, i));
		bool nz2 = fp_nonzero(VECTOR_ID(get)(v2, i));
		VECTOR_ID(set)(or_v, i, (FP_TYPE)(nz1 || nz2));
	}
	return or_v;
}

/*
 * Emulates (m1 | m2).
 */
MATRIX_TYPE* matlab::logical_or(const MATRIX_TYPE* m1, const MATRIX_TYPE* m2) {
	if (m1->size1 != m2->size1 || m1->size2 != m2->size2) {
		return NULL;
	}
	MATRIX_TYPE* or_m = MATRIX_ID(alloc)(m1->size1, m1->size2);
	for (int i = 0; i < (int)m1->size1; i++) {
		for (int j = 0; j < (int)m1->size2; j++) {
			bool nz1 = fp_nonzero(MATRIX_ID(get)(m1, i, j));
			bool nz2 = fp_nonzero(MATRIX_ID(get)(m2, i, j));
			MATRIX_ID(set)(or_m, i, j, (FP_TYPE)(nz1 || nz2));
		}
	}
	return or_m;
}

/*
 * Emulates (m1 * m2).
 */
#ifdef GSL_FLOAT
gsl_matrix_float* matlab::mul(const gsl_matrix_float* m1, const gsl_matrix_float* m2) {
	if (m1->size2 != m2->size1) {
		return NULL;
	}
	gsl_matrix_float* mul_m = gsl_matrix_float_alloc(m1->size1, m2->size2);
	gsl_blas_sgemm(CblasNoTrans, CblasNoTrans, 1.0, m1, m2, 0.0, mul_m);
	return mul_m;
}
#endif
#ifdef GSL_DOUBLE
gsl_matrix* matlab::mul(const gsl_matrix* m1, const gsl_matrix* m2) {
	if (m1->size2 != m2->size1) {
		return NULL;
	}
	gsl_matrix* mul_m = gsl_matrix_alloc(m1->size1, m2->size2);
	gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, m1, m2, 0.0, mul_m);
	return mul_m;
}
#endif
#ifdef GSL_LONG_DOUBLE
gsl_matrix_long_double* matlab::mul(const gsl_matrix_long_double* m1, const gsl_matrix_long_double* m2) {
	if (m1->size2 != m2->size1) {
		return NULL;
	}
	gsl_matrix_long_double* mul_m = gsl_matrix_long_double_alloc(m1->size1, m2->size2);
	for (int i = 0; i < (int)m1->size1; i++) {
		gsl_vector_long_double_const_view row = gsl_matrix_long_double_const_row(m1, i);
		for (int j = 0; j < (int)m2->size2; j++) {
			gsl_vector_long_double_const_view col = gsl_matrix_long_double_const_column(m2, j);
			long double value = 0.0L;
			for (int k = 0; k < (int)m1->size2; k++) {
				value += gsl_vector_long_double_get(&row.vector, k) * gsl_vector_long_double_get(&col.vector, k);
			}
			gsl_matrix_long_double_set(mul_m, i, j, value);
		}
	}
	return mul_m;
}
#endif

/*
 * Emulates (m ^ power).
 */
MATRIX_TYPE* matlab::pow(const MATRIX_TYPE* m, int power) {
	if (m->size1 != m->size2 || power < 1) {
		return NULL;
	}
	MATRIX_TYPE* pow_m = copy(m);
	for (int i = 2; i <= power; i++) {
		MATRIX_TYPE* temp_m = mul(pow_m, m);
		MATRIX_ID(free)(pow_m);
		pow_m = temp_m;
	}
	return pow_m;
}

/*
 * Emulates (v .^ power).
 */
VECTOR_TYPE* matlab::pow_elements(const VECTOR_TYPE* v, FP_TYPE power) {
	VECTOR_TYPE* pow_v = VECTOR_ID(alloc)(v->size);
	for (int i = 0; i < (int)v->size; i++) {
		FP_TYPE value = std::pow(VECTOR_ID(get)(v, i), power);
		VECTOR_ID(set)(pow_v, i, value);
	}
	return pow_v;
}

/*
 * Emulates (v .^ powers).
 */
VECTOR_TYPE* matlab::pow_elements(const VECTOR_TYPE* v, const VECTOR_TYPE* powers) {
	if (v->size != powers->size) {
		return NULL;
	}
	VECTOR_TYPE* pow_v = VECTOR_ID(alloc)(v->size);
	for (int i = 0; i < (int)v->size; i++) {
		FP_TYPE value = std::pow(VECTOR_ID(get)(v, i), VECTOR_ID(get)(powers, i));
		VECTOR_ID(set)(pow_v, i, value);
	}
	return pow_v;
}

/*
 * Emulates (m .^ power).
 */
MATRIX_TYPE* matlab::pow_elements(const MATRIX_TYPE* m, FP_TYPE power) {
	MATRIX_TYPE* pow_m = MATRIX_ID(alloc)(m->size1, m->size2);
	for (int i = 0; i < (int)m->size1; i++) {
		for (int j = 0; j < (int)m->size2; j++) {
			FP_TYPE value = std::pow(MATRIX_ID(get)(m, i, j), power);
			MATRIX_ID(set)(pow_m, i, j, value);
		}
	}
	return pow_m;
}

/*
 * Emulates (m .^ powers).
 */
MATRIX_TYPE* matlab::pow_elements(const MATRIX_TYPE* m, const MATRIX_TYPE* powers) {
	if (m->size1 != powers->size1 || m->size2 != powers->size2) {
		return NULL;
	}
	MATRIX_TYPE* pow_m = MATRIX_ID(alloc)(m->size1, m->size2);
	for (int i = 0; i < (int)m->size1; i++) {
		for (int j = 0; j < (int)m->size2; j++) {
			FP_TYPE value = std::pow(MATRIX_ID(get)(m, i, j), MATRIX_ID(get)(powers, i, j));
			MATRIX_ID(set)(pow_m, i, j, value);
		}
	}
	return pow_m;
}

/* 
 * Emulates (start:end).
 */
VECTOR_TYPE* matlab::FP_ID(sequence)(int start, int end) {
	return FP_ID(sequence)(start, 1, end);
}

/*
 * Emulates (start:step:end).
 */
VECTOR_TYPE* matlab::FP_ID(sequence)(int start, int step, int end) {
	int n_seq = (end - start) / step + 1;
	if (n_seq <= 0) {
		return NULL;
	}
	VECTOR_TYPE* seq_v = VECTOR_ID(alloc)(n_seq);
	for (int i = 0, value = start; i < n_seq; i++, value += step) {
		VECTOR_ID(set)(seq_v, i, value);
	}
	return seq_v;
}
