#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector.h>

/*
 * Converts a vector to an array.
 */
void matlab::to_array(const VECTOR_TYPE* v, FP_TYPE* array) {
	for (int i = 0; i < (int)v->size; i++) {
		array[i] = VECTOR_ID(get)(v, i);
	}
}

/*
 * Converts a vector to a boolean: true if all elements are nonzero, false
 * otherwise.
 */
bool matlab::to_bool(const VECTOR_TYPE* v) {
	return all(v) == 1;
}

/*
 * Converts a matrix to a boolean: true if all elements are nonzero, false
 * otherwise.
 */
bool matlab::to_bool(const MATRIX_TYPE* m) {
	VECTOR_TYPE* all_v = all(m);
	bool ret = all(all_v);
	VECTOR_ID(free)(all_v);
	return ret;
}

/*
 * Converts a vector to float precision.
 */
gsl_vector_float* matlab::to_vector_float(const VECTOR_TYPE* v) {
	gsl_vector_float* v_f = gsl_vector_float_alloc(v->size);
	for (int i = 0; i < (int)v->size; i++) {
		gsl_vector_float_set(v_f, i, (float)VECTOR_ID(get)(v, i));
	}
	return v_f;
}

/*
 * Converts a vector to double precision.
 */
gsl_vector* matlab::to_vector_double(const VECTOR_TYPE* v) {
	gsl_vector* v_d = gsl_vector_alloc(v->size);
	for (int i = 0; i < (int)v->size; i++) {
		gsl_vector_set(v_d, i, (double)VECTOR_ID(get)(v, i));
	}
	return v_d;
}

/*
 * Converts a vector to long double precision.
 */
gsl_vector_long_double* matlab::to_vector_long_double(const VECTOR_TYPE* v) {
	gsl_vector_long_double* v_ld = gsl_vector_long_double_alloc(v->size);
	for (int i = 0; i < (int)v->size; i++) {
		gsl_vector_long_double_set(v_ld, i, (long double)VECTOR_ID(get)(v, i));
	}
	return v_ld;
}

/*
 * Converts a matrix to a vector.  The vector is constructed by consecutively
 * appending columns.
 */
VECTOR_TYPE* matlab::to_vector(const MATRIX_TYPE* m) {
	VECTOR_TYPE* v = VECTOR_ID(alloc)(m->size1 * m->size2);
	for (int j = 0; j < (int)m->size2; j++) {
		for (int i = 0; i < (int)m->size1; i++) {
			FP_TYPE value = MATRIX_ID(get)(m, i, j);
			VECTOR_ID(set)(v, j * m->size1 + i, value);
		}
	}
	return v;
}

/*
 * Converts a matrix to float precision.
 */
gsl_matrix_float* matlab::to_matrix_float(const MATRIX_TYPE* m) {
	gsl_matrix_float* m_f = gsl_matrix_float_alloc(m->size1, m->size2);
	for (int i = 0; i < (int)m->size1; i++) {
		for (int j = 0; j < (int)m->size2; j++) {
			gsl_matrix_float_set(m_f, i, j, (float)MATRIX_ID(get)(m, i, j));
		}
	}
	return m_f;
}

/*
 * Converts a matrix to double precision.
 */
gsl_matrix* matlab::to_matrix_double(const MATRIX_TYPE* m) {
	gsl_matrix* m_d = gsl_matrix_alloc(m->size1, m->size2);
	for (int i = 0; i < (int)m->size1; i++) {
		for (int j = 0; j < (int)m->size2; j++) {
			gsl_matrix_set(m_d, i, j, (double)MATRIX_ID(get)(m, i, j));
		}
	}
	return m_d;
}

/*
 * Converts a matrix to long double precision.
 */
gsl_matrix_long_double* matlab::to_matrix_long_double(const MATRIX_TYPE* m) {
	gsl_matrix_long_double* m_ld = gsl_matrix_long_double_alloc(m->size1, m->size2);
	for (int i = 0; i < (int)m->size1; i++) {
		for (int j = 0; j < (int)m->size2; j++) {
			gsl_matrix_long_double_set(m_ld, i, j, (long double)MATRIX_ID(get)(m, i, j));
		}
	}
	return m_ld;
}

/*
 * Converts a vector to a single-column matrix.
 */
MATRIX_TYPE* matlab::to_column_matrix(const VECTOR_TYPE* v) {
	MATRIX_TYPE* m = MATRIX_ID(alloc)(v->size, 1);
	for (int i = 0; i < (int)v->size; i++) {
		MATRIX_ID(set)(m, i, 0, VECTOR_ID(get)(v, i));
	}
	return m;
}

/*
 * Converts a vector to a single-row matrix.
 */
MATRIX_TYPE* matlab::to_row_matrix(const VECTOR_TYPE* v) {
	MATRIX_TYPE* m = MATRIX_ID(alloc)(1, v->size);
	for (int i = 0; i < (int)v->size; i++) {
		MATRIX_ID(set)(m, 0, i, VECTOR_ID(get)(v, i));
	}
	return m;
}

/*
 * Converts a permutation to a vector.
 */
VECTOR_TYPE* matlab::FP_ID(to_vector)(const gsl_permutation* p) {
	VECTOR_TYPE* v = VECTOR_ID(alloc)(p->size);
	for (int i = 0; i < (int)p->size; i++) {
		VECTOR_ID(set)(v, i, (FP_TYPE)gsl_permutation_get(p, i));
	}
	return v;
}

/*
 * Converts a vector to a permutation.
 */
gsl_permutation* matlab::to_permutation(const VECTOR_TYPE* v) {
	gsl_permutation* p = gsl_permutation_alloc(v->size);
	for (int i = 0; i < (int)v->size; i++) {
		p->data[i] = (int)VECTOR_ID(get)(v, i);
	}
	if (gsl_permutation_valid(p) == 1) {
		gsl_permutation_free(p);
		return NULL;
	} else {
		return p;
	}
}
