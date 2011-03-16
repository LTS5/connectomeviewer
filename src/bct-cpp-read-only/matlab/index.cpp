#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Our indexing-with-assignment functions do not automatically resize vectors
 * and matrices as MATLAB's do.  Thus, while the MATLAB code
 * (A = [1 2 3]; A([4 5]) = 5) yields (A = [1 2 3 5 5]), the equivalent C++ code
 * will result in an error.  In general, it is the caller's responsibility to
 * ensure that a call to one of these functions does not attempt to index past
 * the end of a vector or matrix.
 *
 * GSL does not support vectors of size 0 nor matrices with 0 rows or columns.
 * Therefore, in situations where MATLAB code would return an empty structure
 * (i.e., a vector/matrix having dimensions 0x0, 0xN, or Nx0), the equivalent
 * C++ code will return NULL.
 */

// Vector-by-vector indexing

VECTOR_TYPE* matlab::ordinal_index(const VECTOR_TYPE* v, const VECTOR_TYPE* indices) {
	VECTOR_TYPE* index_v = VECTOR_ID(alloc)(indices->size);
	for (int i = 0; i < (int)indices->size; i++) {
		int index = (int)VECTOR_ID(get)(indices, i);
		FP_TYPE value = VECTOR_ID(get)(v, index);
		VECTOR_ID(set)(index_v, i, value);
	}
	return index_v;
}

void matlab::ordinal_index_assign(VECTOR_TYPE* v, const VECTOR_TYPE* indices, FP_TYPE value) {
	for (int i = 0; i < (int)indices->size; i++) {
		int index = (int)VECTOR_ID(get)(indices, i);
		VECTOR_ID(set)(v, index, value);
	}
}

void matlab::ordinal_index_assign(VECTOR_TYPE* v, const VECTOR_TYPE* indices, const VECTOR_TYPE* values) {
	for (int i = 0; i < (int)indices->size; i++) {
		int index = (int)VECTOR_ID(get)(indices, i);
		FP_TYPE value = VECTOR_ID(get)(values, i);
		VECTOR_ID(set)(v, index, value);
	}
}

VECTOR_TYPE* matlab::logical_index(const VECTOR_TYPE* v, const VECTOR_TYPE* logical_v) {
	int n_index = nnz(logical_v);
	if (n_index == 0) {
		return NULL;
	}
	VECTOR_TYPE* index_v = VECTOR_ID(alloc)(n_index);
	for (int i = 0, index = 0; i < (int)logical_v->size; i++) {
		if (fp_nonzero(VECTOR_ID(get)(logical_v, i))) {
			FP_TYPE value = VECTOR_ID(get)(v, i);
			VECTOR_ID(set)(index_v, index++, value);
		}
	}
	return index_v;
}

void matlab::logical_index_assign(VECTOR_TYPE* v, const VECTOR_TYPE* logical_v, FP_TYPE value) {
	for (int i = 0; i < (int)logical_v->size; i++) {
		if (fp_nonzero(VECTOR_ID(get)(logical_v, i))) {
			VECTOR_ID(set)(v, i, value);
		}
	}
}

void matlab::logical_index_assign(VECTOR_TYPE* v, const VECTOR_TYPE* logical_v, const VECTOR_TYPE* values) {
	for (int i = 0, index = 0; i < (int)logical_v->size; i++) {
		if (fp_nonzero(VECTOR_ID(get)(logical_v, i))) {
			FP_TYPE value = VECTOR_ID(get)(values, index++);
			VECTOR_ID(set)(v, i, value);
		}
	}
}

// Matrix-by-integer indexing

FP_TYPE matlab::ordinal_index(const MATRIX_TYPE* m, int index) {
	int row = index % (int)m->size1;
	int column = index / (int)m->size1;
	return MATRIX_ID(get)(m, row, column);
}

void matlab::ordinal_index_assign(MATRIX_TYPE* m, int index, FP_TYPE value) {
	int row = index % (int)m->size1;
	int column = index / (int)m->size1;
	MATRIX_ID(set)(m, row, column, value);
}

// Matrix-by-vector indexing

VECTOR_TYPE* matlab::ordinal_index(const MATRIX_TYPE* m, const VECTOR_TYPE* indices) {
	VECTOR_TYPE* index_v = VECTOR_ID(alloc)(indices->size);
	for (int i = 0; i < (int)indices->size; i++) {
		int index = (int)VECTOR_ID(get)(indices, i);
		FP_TYPE value = ordinal_index(m, index);
		VECTOR_ID(set)(index_v, i, value);
	}
	return index_v;
}

void matlab::ordinal_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* indices, FP_TYPE value) {
	for (int i = 0; i < (int)indices->size; i++) {
		int index = (int)VECTOR_ID(get)(indices, i);
		ordinal_index_assign(m, index, value);
	}
}

void matlab::ordinal_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* indices, const VECTOR_TYPE* values) {
	for (int i = 0; i < (int)indices->size; i++) {
		int index = (int)VECTOR_ID(get)(indices, i);
		FP_TYPE value = VECTOR_ID(get)(values, i);
		ordinal_index_assign(m, index, value);
	}
}

VECTOR_TYPE* matlab::logical_index(const MATRIX_TYPE* m, const VECTOR_TYPE* logical_v) {
	int n_index = nnz(logical_v);
	if (n_index == 0) {
		return NULL;
	}
	VECTOR_TYPE* index_v = VECTOR_ID(alloc)(n_index);
	for (int i = 0, index = 0; i < (int)logical_v->size; i++) {
		if (fp_nonzero(VECTOR_ID(get)(logical_v, i))) {
			FP_TYPE value = ordinal_index(m, i);
			VECTOR_ID(set)(index_v, index++, value);
		}
	}
	return index_v;
}

void matlab::logical_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* logical_v, FP_TYPE value) {
	for (int i = 0; i < (int)logical_v->size; i++) {
		if (fp_nonzero(VECTOR_ID(get)(logical_v, i))) {
			ordinal_index_assign(m, i, value);
		}
	}
}

void matlab::logical_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* logical_v, const VECTOR_TYPE* values) {
	for (int i = 0, index = 0; i < (int)logical_v->size; i++) {
		if (fp_nonzero(VECTOR_ID(get)(logical_v, i))) {
			FP_TYPE value = VECTOR_ID(get)(values, index++);
			ordinal_index_assign(m, i, value);
		}
	}
}

// Matrix-by-two-vectors indexing (non-mixed)

MATRIX_TYPE* matlab::ordinal_index(const MATRIX_TYPE* m, const VECTOR_TYPE* rows, const VECTOR_TYPE* columns) {
	MATRIX_TYPE* index_m = MATRIX_ID(alloc)(rows->size, columns->size);
	for (int i = 0; i < (int)rows->size; i++) {
		int row = (int)VECTOR_ID(get)(rows, i);
		for (int j = 0; j < (int)columns->size; j++) {
			int column = (int)VECTOR_ID(get)(columns, j);
			FP_TYPE value = MATRIX_ID(get)(m, row, column);
			MATRIX_ID(set)(index_m, i, j, value);
		}
	}
	return index_m;
}

void matlab::ordinal_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* rows, const VECTOR_TYPE* columns, FP_TYPE value) {
	for (int i = 0; i < (int)rows->size; i++) {
		int row = (int)VECTOR_ID(get)(rows, i);
		for (int j = 0; j < (int)columns->size; j++) {
			int column = (int)VECTOR_ID(get)(columns, j);
			MATRIX_ID(set)(m, row, column, value);
		}
	}
}

void matlab::ordinal_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* rows, const VECTOR_TYPE* columns, const MATRIX_TYPE* values) {
	for (int i = 0; i < (int)rows->size; i++) {
		int row = (int)VECTOR_ID(get)(rows, i);
		for (int j = 0; j < (int)columns->size; j++) {
			int column = (int)VECTOR_ID(get)(columns, j);
			FP_TYPE value = MATRIX_ID(get)(values, i, j);
			MATRIX_ID(set)(m, row, column, value);
		}
	}
}

MATRIX_TYPE* matlab::logical_index(const MATRIX_TYPE* m, const VECTOR_TYPE* logical_rows, const VECTOR_TYPE* logical_columns) {
	int n_rows = nnz(logical_rows);
	int n_columns = nnz(logical_columns);
	if (n_rows == 0 || n_columns == 0) {
		return NULL;
	}
	MATRIX_TYPE* index_m = MATRIX_ID(alloc)(n_rows, n_columns);
	for (int i = 0, row = 0; i < (int)logical_rows->size; i++) {
		if (fp_nonzero(VECTOR_ID(get)(logical_rows, i))) {
			for (int j = 0, column = 0; j < (int)logical_columns->size; j++) {
				if (fp_nonzero(VECTOR_ID(get)(logical_columns, j))) {
					FP_TYPE value = MATRIX_ID(get)(m, i, j);
					MATRIX_ID(set)(index_m, row, column, value);
					column++;
				}
			}
			row++;
		}
	}
	return index_m;
}

void matlab::logical_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* logical_rows, const VECTOR_TYPE* logical_columns, FP_TYPE value) {
	for (int i = 0; i < (int)logical_rows->size; i++) {
		if (fp_nonzero(VECTOR_ID(get)(logical_rows, i))) {
			for (int j = 0; j < (int)logical_columns->size; j++) {
				if (fp_nonzero(VECTOR_ID(get)(logical_columns, j))) {
					MATRIX_ID(set)(m, i, j, value);
				}
			}
		}
	}
}

void matlab::logical_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* logical_rows, const VECTOR_TYPE* logical_columns, const MATRIX_TYPE* values) {
	for (int i = 0, row = 0; i < (int)logical_rows->size; i++) {
		if (fp_nonzero(VECTOR_ID(get)(logical_rows, i))) {
			for (int j = 0, column = 0; j < (int)logical_columns->size; j++) {
				if (fp_nonzero(VECTOR_ID(get)(logical_columns, j))) {
					FP_TYPE value = MATRIX_ID(get)(values, row, column);
					MATRIX_ID(set)(m, i, j, value);
					column++;
				}
			}
			row++;
		}
	}
}

// Matrix-by-two-vectors indexing (mixed)

MATRIX_TYPE* matlab::ord_log_index(const MATRIX_TYPE* m, const VECTOR_TYPE* rows, const VECTOR_TYPE* logical_columns) {
	int n_columns = nnz(logical_columns);
	if (n_columns == 0) {
		return NULL;
	}
	MATRIX_TYPE* index_m = MATRIX_ID(alloc)(rows->size, n_columns);
	for (int j = 0, column = 0; j < (int)logical_columns->size; j++) {
		if (fp_nonzero(VECTOR_ID(get)(logical_columns, j))) {
			for (int i = 0; i < (int)rows->size; i++) {
				int row = (int)VECTOR_ID(get)(rows, i);
				FP_TYPE value = MATRIX_ID(get)(m, row, j);
				MATRIX_ID(set)(index_m, i, column, value);
			}
			column++;
		}
	}
	return index_m;
}

void matlab::ord_log_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* rows, const VECTOR_TYPE* logical_columns, FP_TYPE value) {
	for (int j = 0; j < (int)logical_columns->size; j++) {
		if (fp_nonzero(VECTOR_ID(get)(logical_columns, j))) {
			for (int i = 0; i < (int)rows->size; i++) {
				int row = (int)VECTOR_ID(get)(rows, i);
				MATRIX_ID(set)(m, row, j, value);
			}
		}
	}
}

void matlab::ord_log_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* rows, const VECTOR_TYPE* logical_columns, const MATRIX_TYPE* values) {
	for (int j = 0, column = 0; j < (int)logical_columns->size; j++) {
		if (fp_nonzero(VECTOR_ID(get)(logical_columns, j))) {
			for (int i = 0; i < (int)rows->size; i++) {
				int row = (int)VECTOR_ID(get)(rows, i);
				FP_TYPE value = MATRIX_ID(get)(values, i, column);
				MATRIX_ID(set)(m, row, j, value);
			}
			column++;
		}
	}
}

MATRIX_TYPE* matlab::log_ord_index(const MATRIX_TYPE* m, const VECTOR_TYPE* logical_rows, const VECTOR_TYPE* columns) {
	int n_rows = nnz(logical_rows);
	if (n_rows == 0) {
		return NULL;
	}
	MATRIX_TYPE* index_m = MATRIX_ID(alloc)(n_rows, columns->size);
	for (int i = 0, row = 0; i < (int)logical_rows->size; i++) {
		if (fp_nonzero(VECTOR_ID(get)(logical_rows, i))) {
			for (int j = 0; j < (int)columns->size; j++) {
				int column = (int)VECTOR_ID(get)(columns, j);
				FP_TYPE value = MATRIX_ID(get)(m, i, column);
				MATRIX_ID(set)(index_m, row, j, value);
			}
			row++;
		}
	}
	return index_m;
}

void matlab::log_ord_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* logical_rows, const VECTOR_TYPE* columns, FP_TYPE value) {
	for (int i = 0; i < (int)logical_rows->size; i++) {
		if (fp_nonzero(VECTOR_ID(get)(logical_rows, i))) {
			for (int j = 0; j < (int)columns->size; j++) {
				int column = (int)VECTOR_ID(get)(columns, j);
				MATRIX_ID(set)(m, i, column, value);
			}
		}
	}
}

void matlab::log_ord_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* logical_rows, const VECTOR_TYPE* columns, const MATRIX_TYPE* values) {
	for (int i = 0, row = 0; i < (int)logical_rows->size; i++) {
		if (fp_nonzero(VECTOR_ID(get)(logical_rows, i))) {
			for (int j = 0; j < (int)columns->size; j++) {
				int column = (int)VECTOR_ID(get)(columns, j);
				FP_TYPE value = MATRIX_ID(get)(values, row, j);
				MATRIX_ID(set)(m, i, column, value);
			}
			row++;
		}
	}
}

// Matrix-by-matrix indexing

MATRIX_TYPE* matlab::ordinal_index(const MATRIX_TYPE* m, const MATRIX_TYPE* indices) {
	MATRIX_TYPE* index_m = MATRIX_ID(alloc)(indices->size1, indices->size2);
	for (int i = 0; i < (int)indices->size1; i++) {
		for (int j = 0; j < (int)indices->size2; j++) {
			int index = (int)MATRIX_ID(get)(indices, i, j);
			FP_TYPE value = ordinal_index(m, index);
			MATRIX_ID(set)(index_m, i, j, value);
		}
	}
	return index_m;
}

void matlab::ordinal_index_assign(MATRIX_TYPE* m, const MATRIX_TYPE* indices, FP_TYPE value) {
	for (int i = 0; i < (int)indices->size1; i++) {
		for (int j = 0; j < (int)indices->size2; j++) {
			int index = (int)MATRIX_ID(get)(indices, i, j);
			ordinal_index_assign(m, index, value);
		}
	}
}

void matlab::ordinal_index_assign(MATRIX_TYPE* m, const MATRIX_TYPE* indices, const MATRIX_TYPE* values) {
	for (int i = 0; i < (int)indices->size1; i++) {
		for (int j = 0; j < (int)indices->size2; j++) {
			int index = (int)MATRIX_ID(get)(indices, i, j);
			FP_TYPE value = MATRIX_ID(get)(values, i, j);
			ordinal_index_assign(m, index, value);
		}
	}
}

VECTOR_TYPE* matlab::logical_index(const MATRIX_TYPE* m, const MATRIX_TYPE* logical_m) {
	int n_index = nnz(logical_m);
	if (n_index == 0) {
		return NULL;
	}
	VECTOR_TYPE* index_v = VECTOR_ID(alloc)(n_index);
	for (int j = 0, index = 0; j < (int)logical_m->size2; j++) {
		for (int i = 0; i < (int)logical_m->size1; i++) {
			if (fp_nonzero(MATRIX_ID(get)(logical_m, i, j))) {
				FP_TYPE value = MATRIX_ID(get)(m, i, j);
				VECTOR_ID(set)(index_v, index++, value);
			}
		}
	}
	return index_v;
}

void matlab::logical_index_assign(MATRIX_TYPE* m, const MATRIX_TYPE* logical_m, FP_TYPE value) {
	for (int j = 0; j < (int)logical_m->size2; j++) {
		for (int i = 0; i < (int)logical_m->size1; i++) {
			if (fp_nonzero(MATRIX_ID(get)(logical_m, i, j))) {
				MATRIX_ID(set)(m, i, j, value);
			}
		}
	}
}

void matlab::logical_index_assign(MATRIX_TYPE* m, const MATRIX_TYPE* logical_m, const VECTOR_TYPE* values) {
	for (int j = 0, index = 0; j < (int)logical_m->size2; j++) {
		for (int i = 0; i < (int)logical_m->size1; i++) {
			if (fp_nonzero(MATRIX_ID(get)(logical_m, i, j))) {
				FP_TYPE value = VECTOR_ID(get)(values, index++);
				MATRIX_ID(set)(m, i, j, value);
			}
		}
	}
}
