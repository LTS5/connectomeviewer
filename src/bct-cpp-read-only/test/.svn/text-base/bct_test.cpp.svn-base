#ifndef BCT_TEST_CPP
#define BCT_TEST_CPP

#include "bct_test.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <octave/oct.h>
#include <vector>

/*
 * Converts a GSL vector to an Octave matrix.
 */
Matrix bct_test::from_gsl(const gsl_vector* gslv, int start) {
	Matrix m = Matrix(1, gslv->size - start);
	for (int i = start; i < (int)gslv->size; i++) {
		m(0, i - start) = gsl_vector_get(gslv, i);
	}
	return m;
}

/*
 * Converts a GSL matrix to an Octave matrix.
 */
Matrix bct_test::from_gsl(const gsl_matrix* gslm, int start_row, int start_col) {
	Matrix m = Matrix(gslm->size1 - start_row, gslm->size2 - start_col);
	for (int i = start_row; i < (int)gslm->size1; i++) {
		for (int j = start_col; j < (int)gslm->size2; j++) {
			m(i - start_row, j - start_col) = gsl_matrix_get(gslm, i, j);
		}
	}
	return m;
}

/*
 * Converts a GSL 3D matrix to an Octave 3D matrix.
 */
NDArray bct_test::from_gsl(const std::vector<gsl_matrix*> gslm, int start) {
	dim_vector dim_v(3);
	dim_v.resize(3);
	dim_v(0) = gslm[start]->size1;
	dim_v(1) = gslm[start]->size2;
	dim_v(2) = gslm.size() - start;
	NDArray m(dim_v);
	for (int k = start; k < (int)gslm.size(); k++) {
		for (int i = 0; i < (int)gslm[start]->size1; i++) {
			for (int j = 0; j < (int)gslm[start]->size2; j++) {
				m(i, j, k - start) = gsl_matrix_get(gslm[k], i, j);
			}
		}
	}
	return m;
}

/*
 * Converts an Octave matrix to a GSL vector.
 */
gsl_vector* bct_test::to_gslv(const Matrix m, int start) {
	gsl_vector* gslv;
	if (m.dims()(0) == 1) {
		gslv = gsl_vector_alloc(m.dims()(1) + start);
		for (int i = 0; i < m.dims()(1); i++) {
			gsl_vector_set(gslv, i + start, m(0, i));
		}
	} else {
		gslv = gsl_vector_alloc(m.dims()(0) + start);
		for (int i = 0; i < m.dims()(0); i++) {
			gsl_vector_set(gslv, i + start, m(i, 0));
		}
	}
	return gslv;
}

/*
 * Converts an Octave matrix to a GSL matrix.
 */
gsl_matrix* bct_test::to_gslm(const Matrix m, int start_row, int start_col) {
	gsl_matrix* gslm = gsl_matrix_alloc(m.dims()(0) + start_row, m.dims()(1) + start_col);
	for (int i = 0; i < m.dims()(0); i++) {
		for (int j = 0; j < m.dims()(1); j++) {
			gsl_matrix_set(gslm, i + start_row, j + start_col, m(i, j));
		}
	}
	return gslm;
}

/*
 * Converts an Octave 3D matrix to a GSL 3D matrix.
 */
std::vector<gsl_matrix*> bct_test::to_gsl(const NDArray m, int start) {
	std::vector<gsl_matrix*> gslm(m.dims()(2) + start);
	for (int k = 0; k < start; k++) {
		gslm[k] = NULL;
	}
	for (int k = 0; k < m.dims()(2); k++) {
		gslm[k + start] = gsl_matrix_alloc(m.dims()(0), m.dims()(1));
		for (int i = 0; i < m.dims()(0); i++) {
			for (int j = 0; j < m.dims()(1); j++) {
				gsl_matrix_set(gslm[k + start], i, j, m(i, j, k));
			}
		}
	}
	return gslm;
}

#endif
