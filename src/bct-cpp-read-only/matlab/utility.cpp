#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector.h>

/*
 * Permutes the columns of a matrix.
 */
MATRIX_TYPE* matlab::permute_columns(const gsl_permutation* p, const MATRIX_TYPE* m) {
	if (p->size != m->size2) return NULL;
	MATRIX_TYPE* permuted_m = MATRIX_ID(alloc)(m->size1, m->size2);
	for (int i = 0; i < (int)p->size; i++) {
		int i_col = gsl_permutation_get(p, i);
		VECTOR_ID(const_view) m_col_i_col = MATRIX_ID(const_column)(m, i_col);
		MATRIX_ID(set_col)(permuted_m, i, &m_col_i_col.vector);
	}
	return permuted_m;
}

/*
 * Permutes the rows of a matrix.
 */
MATRIX_TYPE* matlab::permute_rows(const gsl_permutation* p, const MATRIX_TYPE* m) {
	if (p->size != m->size1) return NULL;
	MATRIX_TYPE* permuted_m = MATRIX_ID(alloc)(m->size1, m->size2);
	for (int i = 0; i < (int)p->size; i++) {
		int i_row = gsl_permutation_get(p, i);
		VECTOR_ID(const_view) m_row_i_row = MATRIX_ID(const_row)(m, i_row);
		MATRIX_ID(set_row)(permuted_m, i, &m_row_i_row.vector);
	}
	return permuted_m;
}
