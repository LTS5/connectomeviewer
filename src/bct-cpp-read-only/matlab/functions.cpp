#include <cmath>
#include <cstddef>
#include <ctime>
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_vector.h>
#include <string>

/*
 * See MATLAB documentation for descriptions of these functions.  We will
 * document instances where our version differs from the MATLAB version.
 */

VECTOR_TYPE* matlab::abs(const VECTOR_TYPE* v) {
	VECTOR_TYPE* abs_v = VECTOR_ID(alloc)(v->size);
	for (int i = 0; i < (int)v->size; i++) {
		VECTOR_ID(set)(abs_v, i, std::abs(VECTOR_ID(get)(v, i)));
	}
	return abs_v;
}

MATRIX_TYPE* matlab::abs(const MATRIX_TYPE* m) {
	MATRIX_TYPE* abs_m = MATRIX_ID(alloc)(m->size1, m->size2);
	for (int i = 0; i < (int)m->size1; i++) {
		for (int j = 0; j < (int)m->size2; j++) {
			MATRIX_ID(set)(abs_m, i, j, std::abs(MATRIX_ID(get)(m, i, j)));
		}
	}
	return abs_m;
}

int matlab::all(const VECTOR_TYPE* v) {
	for (int i = 0; i < (int)v->size; i++) {
		if (fp_zero(VECTOR_ID(get)(v, i))) {
			return 0;
		}
	}
	return 1;
}

VECTOR_TYPE* matlab::all(const MATRIX_TYPE* m, int dim) {
	if (dim == 1) {
		VECTOR_TYPE* all_v = VECTOR_ID(alloc)(m->size2);
		for (int i = 0; i < (int)m->size2; i++) {
			VECTOR_ID(const_view) m_col_i = MATRIX_ID(const_column)(m, i);
			VECTOR_ID(set)(all_v, i, all(&m_col_i.vector));
		}
		return all_v;
	} else if (dim == 2) {
		VECTOR_TYPE* all_v = VECTOR_ID(alloc)(m->size1);
		for (int i = 0; i < (int)m->size1; i++) {
			VECTOR_ID(const_view) m_row_i = MATRIX_ID(const_row)(m, i);
			VECTOR_ID(set)(all_v, i, all(&m_row_i.vector));
		}
		return all_v;
	} else {
		return NULL;
	}
}

int matlab::any(const VECTOR_TYPE* v) {
	for (int i = 0; i < (int)v->size; i++) {
		if (fp_nonzero(VECTOR_ID(get)(v, i))) {
			return 1;
		}
	}
	return 0;
}

VECTOR_TYPE* matlab::any(const MATRIX_TYPE* m, int dim) {
	if (dim == 1) {
		VECTOR_TYPE* any_v = VECTOR_ID(alloc)(m->size2);
		for (int i = 0; i < (int)m->size2; i++) {
			VECTOR_ID(const_view) m_col_i = MATRIX_ID(const_column)(m, i);
			VECTOR_ID(set)(any_v, i, any(&m_col_i.vector));
		}
		return any_v;
	} else if (dim == 2) {
		VECTOR_TYPE* any_v = VECTOR_ID(alloc)(m->size1);
		for (int i = 0; i < (int)m->size1; i++) {
			VECTOR_ID(const_view) m_row_i = MATRIX_ID(const_row)(m, i);
			VECTOR_ID(set)(any_v, i, any(&m_row_i.vector));
		}
		return any_v;
	} else {
		return NULL;
	}
}

MATRIX_TYPE* matlab::diag(const VECTOR_TYPE* v, int k) {
	int i0;
	int j0;
	if (k >= 0) { i0 = 0; j0 = k; }
	else { i0 = -k; j0 = 0; }
	int n = (int)v->size + (int)std::abs(k);
	MATRIX_TYPE* diag_m = MATRIX_ID(calloc)(n, n);
	for (int i = 0; i < (int)v->size; i++) {
		MATRIX_ID(set)(diag_m, i0 + i, j0 + i, VECTOR_ID(get)(v, i));
	}
	return diag_m;
}

VECTOR_TYPE* matlab::diag(const MATRIX_TYPE* m, int k) {
	if (k <= -(int)m->size1 || k >= (int)m->size2) {
		return NULL;
	}
	int i0;
	int j0;
	if (k >= 0) { i0 = 0; j0 = k; }
	else { i0 = -k; j0 = 0; }
	int n_rows = m->size1 - i0;
	int n_cols = m->size2 - j0;
	int n = (n_rows < n_cols) ? n_rows : n_cols;
	VECTOR_TYPE* diag_v = VECTOR_ID(alloc)(n);
	for (int i = 0; i < n; i++) {
		VECTOR_ID(set)(diag_v, i, MATRIX_ID(get)(m, i0 + i, j0 + i));
	}
	return diag_v;
}

MATRIX_TYPE* matlab::FP_ID(eye)(int size) {
	return FP_ID(eye)(size, size);
}

MATRIX_TYPE* matlab::FP_ID(eye)(int size1, int size2) {
	MATRIX_TYPE* eye_m = MATRIX_ID(calloc)(size1, size2);
	VECTOR_ID(view) diag_eye_m = MATRIX_ID(diagonal)(eye_m);
	VECTOR_ID(set_all)(&diag_eye_m.vector, 1.0);
	return eye_m;
}

VECTOR_TYPE* matlab::find(const VECTOR_TYPE* v, int n, const std::string& direction) {
	int n_find = nnz(v);
	if (n_find == 0 || n < 1) {
		return NULL;
	}
	VECTOR_TYPE* find_v = VECTOR_ID(alloc)((n < n_find) ? n : n_find);
	if (direction == "first") {
		int position = 0;
		for (int i = 0; i < (int)v->size && position < (int)find_v->size; i++) {
			if (fp_nonzero(VECTOR_ID(get)(v, i))) {
				VECTOR_ID(set)(find_v, position, i);
				position++;
			}
		}
		return find_v;
	} else if (direction == "last") {
		int position = find_v->size - 1;
		for (int i = v->size - 1; i >= 0 && position >= 0; i--) {
			if (fp_nonzero(VECTOR_ID(get)(v, i))) {
				VECTOR_ID(set)(find_v, position, i);
				position--;
			}
		}
		return find_v;
	} else {
		VECTOR_ID(free)(find_v);
		return NULL;
	}
}

VECTOR_TYPE* matlab::find(const MATRIX_TYPE* m, int n, const std::string& direction) {
	VECTOR_TYPE* v = to_vector(m);
	VECTOR_TYPE* find_v = find(v, n, direction);
	VECTOR_ID(free)(v);
	return find_v;
}

/*
 * Emulates the two-return version of "find".
 */
MATRIX_TYPE* matlab::find_ij(const MATRIX_TYPE* m, int n, const std::string& direction) {
	VECTOR_TYPE* find_v = find(m, n, direction);
	if (find_v == NULL) {
		return NULL;
	} else {
		MATRIX_TYPE* find_m = MATRIX_ID(alloc)(find_v->size, 2);
		for (int i = 0; i < (int)find_v->size; i++) {
			int index = (int)VECTOR_ID(get)(find_v, i);
			int row = index % (int)m->size1;
			int column = index / (int)m->size1;
			MATRIX_ID(set)(find_m, i, 0, (FP_TYPE)row);
			MATRIX_ID(set)(find_m, i, 1, (FP_TYPE)column);
		}
		VECTOR_ID(free)(find_v);
		return find_m;
	}
}

VECTOR_TYPE* matlab::hist(const VECTOR_TYPE* v, int n) {
	VECTOR_TYPE* centers = VECTOR_ID(alloc)(n);
	FP_TYPE min_value = min(v);
	FP_TYPE max_value = max(v);
	FP_TYPE width = (max_value - min_value) / (FP_TYPE)n;
	for (int i = 0; i < n; i++) {
		VECTOR_ID(set)(centers, i, min_value + (i + 0.5) * width);
	}
	VECTOR_TYPE* hist_v = hist(v, centers);
	VECTOR_ID(free)(centers);
	return hist_v;
}

VECTOR_TYPE* matlab::hist(const VECTOR_TYPE* v, const VECTOR_TYPE* centers) {
	int n = centers->size;
	VECTOR_TYPE* hist_v = VECTOR_ID(calloc)(n);
	for (int i = 0; i < (int)v->size; i++) {
		FP_TYPE value = VECTOR_ID(get)(v, i);
		int index = n - 1;
		for (int j = 0; j < n - 1; j++) {
			FP_TYPE left = VECTOR_ID(get)(centers, j);
			FP_TYPE right = VECTOR_ID(get)(centers, j + 1);
			if (value < left) {
				index = j;
				break;
			} else if (value < right) {
				FP_TYPE middle = (left + right) / 2.0;
				if (fp_less_or_equal(value, middle)) {
					index = j;
				} else {
					index = j + 1;
				}
				break;
			}
		}
		VECTOR_ID(set)(hist_v, index, VECTOR_ID(get)(hist_v, index) + 1.0);
	}
	return hist_v;
}

int matlab::length(const VECTOR_TYPE* v) {
	return v->size;
}

int matlab::length(const MATRIX_TYPE* m) {
	return (m->size1 > m->size2) ? m->size1 : m->size2;
}

FP_TYPE matlab::max(FP_TYPE x, FP_TYPE y) {
	if (gsl_isnan((double)x) == 1) {
		return y;
	} else if (gsl_isnan((double)y) == 1) {
		return x;
	} else {
		return (x > y) ? x : y;
	}
}

FP_TYPE matlab::max(const VECTOR_TYPE* v) {
	FP_TYPE max = (FP_TYPE)GSL_NAN;
	int i = 0;
	for ( ; i < (int)v->size; i++) {
		FP_TYPE value = VECTOR_ID(get)(v, i);
		if (gsl_isnan((double)value) == 0) {
			max = value;
			break;
		}
	}
	for ( ; i < (int)v->size; i++) {
		FP_TYPE value = VECTOR_ID(get)(v, i);
		if (gsl_isnan((double)value) == 0 && value > max) {
			max = value;
		}
	}
	return max;
}

/*
 * Emulates (max(m)) or (max(m')).
 */
VECTOR_TYPE* matlab::max(const MATRIX_TYPE* m, int dim) {
	if (dim == 1) {
		VECTOR_TYPE* max_v = VECTOR_ID(alloc)(m->size2);
		for (int i = 0; i < (int)m->size2; i++) {
			VECTOR_ID(const_view) m_col_i = MATRIX_ID(const_column)(m, i);
			FP_TYPE value = max(&m_col_i.vector);
			VECTOR_ID(set)(max_v, i, value);
		}
		return max_v;
	} else if (dim == 2) {
		VECTOR_TYPE* max_v = VECTOR_ID(alloc)(m->size1);
		for (int i = 0; i < (int)m->size1; i++) {
			VECTOR_ID(const_view) m_row_i = MATRIX_ID(const_row)(m, i);
			FP_TYPE value = max(&m_row_i.vector);
			VECTOR_ID(set)(max_v, i, value);
		}
		return max_v;
	} else {
		return NULL;
	}
}

FP_TYPE matlab::mean(const VECTOR_TYPE* v, const std::string& opt) {
	if (opt == "a") {
		FP_TYPE sum = 0.0;
		for (int i = 0; i < (int)v->size; i++) {
			sum += VECTOR_ID(get)(v, i);
		}
		return sum / (FP_TYPE)v->size;
	} else if (opt == "g") {
		FP_TYPE product = 1.0;
		for (int i = 0; i < (int)v->size; i++) {
			product *= VECTOR_ID(get)(v, i);
		}
		return std::pow(product, (FP_TYPE)1.0 / (FP_TYPE)v->size);
	} else if (opt == "h") {
		FP_TYPE sum = 0.0;
		for (int i = 0; i < (int)v->size; i++) {
			sum += 1.0 / VECTOR_ID(get)(v, i);
		}
		return (FP_TYPE)v->size / sum;
	} else {
		return GSL_NAN;
	}
}

VECTOR_TYPE* matlab::mean(const MATRIX_TYPE* m, int dim, const std::string& opt) {
	if (dim == 1) {
		VECTOR_TYPE* mean_v = VECTOR_ID(alloc)(m->size2);
		for (int i = 0; i < (int)m->size2; i++) {
			VECTOR_ID(const_view) m_col_i = MATRIX_ID(const_column)(m, i);
			FP_TYPE value = mean(&m_col_i.vector, opt);
			VECTOR_ID(set)(mean_v, i, value);
		}
		return mean_v;
	} else if (dim == 2) {
		VECTOR_TYPE* mean_v = VECTOR_ID(alloc)(m->size1);
		for (int i = 0; i < (int)m->size1; i++) {
			VECTOR_ID(const_view) m_row_i = MATRIX_ID(const_row)(m, i);
			FP_TYPE value = mean(&m_row_i.vector, opt);
			VECTOR_ID(set)(mean_v, i, value);
		}
		return mean_v;
	} else {
		return NULL;
	}
}

FP_TYPE matlab::min(FP_TYPE x, FP_TYPE y) {
	if (gsl_isnan((double)x) == 1) {
		return y;
	} else if (gsl_isnan((double)y) == 1) {
		return x;
	} else {
		return (x < y) ? x : y;
	}
}

FP_TYPE matlab::min(const VECTOR_TYPE* v) {
	FP_TYPE min = (FP_TYPE)GSL_NAN;
	int i = 0;
	for ( ; i < (int)v->size; i++) {
		FP_TYPE value = VECTOR_ID(get)(v, i);
		if (gsl_isnan((double)value) == 0) {
			min = value;
			break;
		}
	}
	for ( ; i < (int)v->size; i++) {
		FP_TYPE value = VECTOR_ID(get)(v, i);
		if (gsl_isnan((double)value) == 0 && value < min) {
			min = value;
		}
	}
	return min;
}

/*
 * Emulates (min(m)) or (min(m')).
 */
VECTOR_TYPE* matlab::min(const MATRIX_TYPE* m, int dim) {
	if (dim == 1) {
		VECTOR_TYPE* min_v = VECTOR_ID(alloc)(m->size2);
		for (int i = 0; i < (int)m->size2; i++) {
			VECTOR_ID(const_view) m_col_i = MATRIX_ID(const_column)(m, i);
			FP_TYPE value = min(&m_col_i.vector);
			VECTOR_ID(set)(min_v, i, value);
		}
		return min_v;
	} else if (dim == 2) {
		VECTOR_TYPE* min_v = VECTOR_ID(alloc)(m->size1);
		for (int i = 0; i < (int)m->size1; i++) {
			VECTOR_ID(const_view) m_row_i = MATRIX_ID(const_row)(m, i);
			FP_TYPE value = min(&m_row_i.vector);
			VECTOR_ID(set)(min_v, i, value);
		}
		return min_v;
	} else {
		return NULL;
	}
}

int matlab::nnz(const VECTOR_TYPE* v) {
	int nnz = 0;
	for (int i = 0; i < (int)v->size; i++) {
		if (fp_nonzero(VECTOR_ID(get)(v, i))) {
			nnz++;
		}
	}
	return nnz;
}

int matlab::nnz(const MATRIX_TYPE* m) {
	VECTOR_TYPE* v = to_vector(m);
	int nnz_v = nnz(v);
	VECTOR_ID(free)(v);
	return nnz_v;
}

VECTOR_TYPE* matlab::nonzeros(const MATRIX_TYPE* m) {
	VECTOR_TYPE* nz_v = find(m);
	if (nz_v != NULL) {
		for (int i = 0; i < (int)nz_v->size; i++) {
			int i_m = (int)VECTOR_ID(get)(nz_v, i);
			FP_TYPE value = ordinal_index(m, i_m);
			VECTOR_ID(set)(nz_v, i, value);
		}
	}
	return nz_v;
}

VECTOR_TYPE* matlab::normpdf(const VECTOR_TYPE* v, FP_TYPE mean, FP_TYPE stdev) {
	VECTOR_TYPE* pdf_v = VECTOR_ID(alloc)(v->size);
	for (int i = 0; i < (int)v->size; i++) {
		double x = (double)VECTOR_ID(get)(v, i);
		double p = gsl_ran_gaussian_pdf(x - mean, stdev);
		VECTOR_ID(set)(pdf_v, i, (FP_TYPE)p);
	}
	return pdf_v;
}

MATRIX_TYPE* matlab::FP_ID(ones)(int size) {
	return FP_ID(ones)(size, size);
}

MATRIX_TYPE* matlab::FP_ID(ones)(int size1, int size2) {
	MATRIX_TYPE* ones_m = MATRIX_ID(alloc)(size1, size2);
	MATRIX_ID(set_all)(ones_m, 1.0);
	return ones_m;
}

/*
 * Emulates (ones(size, 1)) or (ones(1, size)).
 */
VECTOR_TYPE* matlab::FP_ID(ones_vector)(int size) {
	VECTOR_TYPE* ones_v = VECTOR_ID(alloc)(size);
	VECTOR_ID(set_all)(ones_v, 1.0);
	return ones_v;
}

FP_TYPE matlab::prod(const VECTOR_TYPE* v) {
	FP_TYPE prod = 1.0;
	for (int i = 0; i < (int)v->size; i++) {
		prod *= VECTOR_ID(get)(v, i);
	}
	return prod;
}

VECTOR_TYPE* matlab::prod(const MATRIX_TYPE* m, int dim) {
	if (dim == 1) {
		VECTOR_TYPE* prod_v = VECTOR_ID(alloc)(m->size2);
		VECTOR_ID(set_all)(prod_v, 1.0);
		for (int i = 0; i < (int)m->size1; i++) {
			VECTOR_ID(const_view) m_row_i = MATRIX_ID(const_row)(m, i);
			VECTOR_ID(mul)(prod_v, &m_row_i.vector);
		}
		return prod_v;
	} else if (dim == 2) {
		VECTOR_TYPE* prod_v = VECTOR_ID(alloc)(m->size1);
		VECTOR_ID(set_all)(prod_v, 1.0);
		for (int i = 0; i < (int)m->size2; i++) {
			VECTOR_ID(const_view) m_col_i = MATRIX_ID(const_column)(m, i);
			VECTOR_ID(mul)(prod_v, &m_col_i.vector);
		}
		return prod_v;
	} else {
		return NULL;
	}
}

MATRIX_TYPE* matlab::FP_ID(rand)(int size) {
	return FP_ID(rand)(size, size);
}

MATRIX_TYPE* matlab::FP_ID(rand)(int size1, int size2) {
	gsl_rng* rng = get_gsl_rng();
	MATRIX_TYPE* rand_m = MATRIX_ID(alloc)(size1, size2);
	for (int i = 0; i < size1; i++) {
		for (int j = 0; j < size2; j++) {
			MATRIX_ID(set)(rand_m, i, j, (FP_TYPE)gsl_rng_uniform(rng));
		}
	}
	return rand_m;
}

VECTOR_TYPE* matlab::FP_ID(rand_vector)(int size) {
	gsl_rng* rng = get_gsl_rng();
	VECTOR_TYPE* rand_v = VECTOR_ID(alloc)(size);
	for (int i = 0; i < size; i++) {
		VECTOR_ID(set)(rand_v, i, (FP_TYPE)gsl_rng_uniform(rng));
	}
	return rand_v;
}

VECTOR_TYPE* matlab::reverse(const VECTOR_TYPE* v) {
	VECTOR_TYPE* rev_v = VECTOR_ID(alloc)(v->size);
	for (int i = 0; i < (int)v->size; i++) {
		VECTOR_ID(set)(rev_v, i, VECTOR_ID(get)(v, v->size - 1 - i));
	}
	return rev_v;
}

VECTOR_TYPE* matlab::setxor(const VECTOR_TYPE* v1, const VECTOR_TYPE* v2) {
	if (v1 == NULL && v2 == NULL) {
		return NULL;
	}
	if (v1 == NULL) {
		return unique(v2);
	}
	if (v2 == NULL) {
		return unique(v1);
	}
	VECTOR_TYPE* unique_v1 = unique(v1);
	VECTOR_TYPE* unique_v2 = unique(v2);
	VECTOR_TYPE* unsized_v = VECTOR_ID(alloc)(v1->size + v2->size);
	int n = 0;
	for (int i = 0; i < (int)unique_v1->size; i++) {
		bool found = false;
		FP_TYPE v1_value = VECTOR_ID(get)(unique_v1, i);
		for (int j = 0; j < (int)unique_v2->size; j++) {
			FP_TYPE v2_value = VECTOR_ID(get)(unique_v2, j);
			if (fp_equal(v1_value, v2_value)) {
				found = true;
				break;
			}
		}
		if (!found) {
			VECTOR_ID(set)(unsized_v, n++, v1_value);
		}
	}
	for (int i = 0; i < (int)unique_v2->size; i++) {
		bool found = false;
		FP_TYPE v2_value = VECTOR_ID(get)(unique_v2, i);
		for (int j = 0; j < (int)unique_v1->size; j++) {
			FP_TYPE v1_value = VECTOR_ID(get)(unique_v1, j);
			if (fp_equal(v2_value, v1_value)) {
				found = true;
				break;
			}
		}
		if (!found) {
			VECTOR_ID(set)(unsized_v, n++, v2_value);
		}
	}
	VECTOR_ID(free)(unique_v1);
	VECTOR_ID(free)(unique_v2);
	if (n > 0) {
		VECTOR_TYPE* unsorted_v = VECTOR_ID(alloc)(n);
		VECTOR_ID(view) unsized_subv = VECTOR_ID(subvector)(unsized_v, 0, n);
		VECTOR_ID(memcpy)(unsorted_v, &unsized_subv.vector);
		VECTOR_ID(free)(unsized_v);
		VECTOR_TYPE* setxor_v = sort(unsorted_v);
		VECTOR_ID(free)(unsorted_v);
		return setxor_v;
	} else {
		VECTOR_ID(free)(unsized_v);
		return NULL;
	}
}

VECTOR_TYPE* matlab::sort(const VECTOR_TYPE* v, const std::string& mode, VECTOR_TYPE** ind) {
	if (mode != "ascend" && mode != "descend") {
		return NULL;
	}
	FP_TYPE elements[v->size];
	to_array(v, elements);
	std::size_t indices[v->size];
	if (mode == "ascend") {
		stable_sort_index(indices, elements, v->size);
	} else {
		stable_sort_index(indices, elements, v->size, fp_greater);
	}
	VECTOR_TYPE* sort_v = VECTOR_ID(alloc)(v->size);
	if (ind != NULL) {
		*ind = VECTOR_ID(alloc)(v->size);
	}
	for (int i = 0; i < (int)v->size; i++) {
		int index = indices[i];
		VECTOR_ID(set)(sort_v, i, elements[index]);
		if (ind != NULL) {
			VECTOR_ID(set)(*ind, i, (FP_TYPE)index);
		}
	}
	return sort_v;
}

MATRIX_TYPE* matlab::sort(const MATRIX_TYPE* m, int dim, const std::string& mode, MATRIX_TYPE** ind) {
	if (mode != "ascend" && mode != "descend") {
		return NULL;
	}
	if (dim == 1) {
		MATRIX_TYPE* sort_m = MATRIX_ID(alloc)(m->size1, m->size2);
		if (ind != NULL) {
			*ind = MATRIX_ID(alloc)(m->size1, m->size2);
		}
		for (int i = 0; i < (int)m->size2; i++) {
			VECTOR_ID(const_view) m_col_i = MATRIX_ID(const_column)(m, i);
			VECTOR_TYPE* sort_m_col_i;
			if (ind == NULL) {
				sort_m_col_i = sort(&m_col_i.vector, mode);
			} else {
				VECTOR_TYPE* ind_col_i;
				sort_m_col_i = sort(&m_col_i.vector, mode, &ind_col_i);
				MATRIX_ID(set_col)(*ind, i, ind_col_i);
				VECTOR_ID(free)(ind_col_i);
			}
			MATRIX_ID(set_col)(sort_m, i, sort_m_col_i);
			VECTOR_ID(free)(sort_m_col_i);
		}
		return sort_m;
	} else if (dim == 2) {
		MATRIX_TYPE* sort_m = MATRIX_ID(alloc)(m->size1, m->size2);
		if (ind != NULL) {
			*ind = MATRIX_ID(alloc)(m->size1, m->size2);
		}
		for (int i = 0; i < (int)m->size1; i++) {
			VECTOR_ID(const_view) m_row_i = MATRIX_ID(const_row)(m, i);
			VECTOR_TYPE* sort_m_row_i;
			if (ind == NULL) {
				sort_m_row_i = sort(&m_row_i.vector, mode);
			} else {
				VECTOR_TYPE* ind_row_i;
				sort_m_row_i = sort(&m_row_i.vector, mode, &ind_row_i);
				MATRIX_ID(set_row)(*ind, i, ind_row_i);
				VECTOR_ID(free)(ind_row_i);
			}
			MATRIX_ID(set_row)(sort_m, i, sort_m_row_i);
			VECTOR_ID(free)(sort_m_row_i);
		}
		return sort_m;
	} else {
		return NULL;
	}
}

/*
 * Emulates (sortrows(v)) for a column vector.
 */
VECTOR_TYPE* matlab::sortrows(const VECTOR_TYPE* v, VECTOR_TYPE** ind) {
	return sort(v, "ascend", ind);
}

MATRIX_TYPE* matlab::sortrows(const MATRIX_TYPE* m, VECTOR_TYPE** ind) {
	VECTOR_TYPE* rows[m->size1];
	for (int i = 0; i < (int)m->size1; i++) {
		rows[i] = VECTOR_ID(alloc)(m->size2);
		MATRIX_ID(get_row)(rows[i], m, i);
	}
	std::size_t indices[m->size1];
	stable_sort_index(indices, rows, m->size1, vector_less);
	for (int i = 0; i < (int)m->size1; i++) {
		VECTOR_ID(free)(rows[i]);
	}
	MATRIX_TYPE* sort_m = MATRIX_ID(alloc)(m->size1, m->size2);
	if (ind != NULL) {
		*ind = VECTOR_ID(alloc)(m->size1);
	}
	for (int i = 0; i < (int)m->size1; i++) {
		int index = indices[i];
		VECTOR_ID(const_view) m_row_index = MATRIX_ID(const_row)(m, index);
		MATRIX_ID(set_row)(sort_m, i, &m_row_index.vector);
		if (ind != NULL) {
			VECTOR_ID(set)(*ind, i, (FP_TYPE)index);
		}
	}
	return sort_m;
}

FP_TYPE matlab::std(const VECTOR_TYPE* v, int opt) {
	FP_TYPE mu = mean(v);
	FP_TYPE err = 0.0;
	for (int i = 0; i < (int)v->size; i++) {
		err += std::pow(VECTOR_ID(get)(v, i) - mu, 2);
	}
	if (opt == 0) {
		return std::sqrt(err / (FP_TYPE)(v->size - 1));
	} else if (opt == 1) {
		return std::sqrt(err / (FP_TYPE)v->size);
	} else {
		return GSL_NAN;
	}
}

VECTOR_TYPE* matlab::std(const MATRIX_TYPE* m, int opt, int dim) {
	if (dim == 1) {
		VECTOR_TYPE* std_v = VECTOR_ID(alloc)(m->size2);
		for (int i = 0; i < (int)m->size2; i++) {
			VECTOR_ID(const_view) m_col_i = MATRIX_ID(const_column)(m, i);
			FP_TYPE value = matlab::std(&m_col_i.vector, opt);
			VECTOR_ID(set)(std_v, i, value);
		}
		return std_v;
	} else if (dim == 2) {
		VECTOR_TYPE* std_v = VECTOR_ID(alloc)(m->size1);
		for (int i = 0; i < (int)m->size1; i++) {
			VECTOR_ID(const_view) m_row_i = MATRIX_ID(const_row)(m, i);
			FP_TYPE value = matlab::std(&m_row_i.vector, opt);
			VECTOR_ID(set)(std_v, i, value);
		}
		return std_v;
	} else {
		return NULL;
	}
}

FP_TYPE matlab::sum(const VECTOR_TYPE* v) {
	FP_TYPE sum = 0.0;
	for (int i = 0; i < (int)v->size; i++) {
		sum += VECTOR_ID(get)(v, i);
	}
	return sum;
}

VECTOR_TYPE* matlab::sum(const MATRIX_TYPE* m, int dim) {
	if (dim == 1) {
		VECTOR_TYPE* sum_v = VECTOR_ID(calloc)(m->size2);
		for (int i = 0; i < (int)m->size1; i++) {
			VECTOR_ID(const_view) m_row_i = MATRIX_ID(const_row)(m, i);
			VECTOR_ID(add)(sum_v, &m_row_i.vector);
		}
		return sum_v;
	} else if (dim == 2) {
		VECTOR_TYPE* sum_v = VECTOR_ID(calloc)(m->size1);
		for (int i = 0; i < (int)m->size2; i++) {
			VECTOR_ID(const_view) m_col_i = MATRIX_ID(const_column)(m, i);
			VECTOR_ID(add)(sum_v, &m_col_i.vector);
		}
		return sum_v;
	} else {
		return NULL;
	}
}

MATRIX_TYPE* matlab::toeplitz(const VECTOR_TYPE* column, const VECTOR_TYPE* row) {
	const VECTOR_TYPE* _row;
	if (row == NULL) {
		_row = column;
	} else {
		_row = row;
	}
	MATRIX_TYPE* toe_m = MATRIX_ID(alloc)(column->size, _row->size);
	for (int i = 0; i < (int)column->size; i++) {
		for (int j = 0; j < (int)_row->size; j++) {
			FP_TYPE value;
			if (i - j >= 0) {
				value = VECTOR_ID(get)(column, i - j);
			} else {
				value = VECTOR_ID(get)(_row, j - i);
			}
			MATRIX_ID(set)(toe_m, i, j, value);
		}
	}
	return toe_m;
}

MATRIX_TYPE* matlab::tril(const MATRIX_TYPE* m, int k) {
	if (k < -(int)m->size1 || k > (int)m->size2) {
		return NULL;
	}
	MATRIX_TYPE* tril_m = copy(m);
	for (int i = 0; i < (int)m->size1; i++) {
		for (int j = i + k + 1; j < (int)m->size2; j++) {
			if (j >= 0) {
				MATRIX_ID(set)(tril_m, i, j, 0.0);
			}
		}
	}
	return tril_m;
}

MATRIX_TYPE* matlab::triu(const MATRIX_TYPE* m, int k) {
	if (k < -(int)m->size1 || k > (int)m->size2) {
		return NULL;
	}
	MATRIX_TYPE* triu_m = copy(m);
	for (int i = 0; i < (int)m->size1; i++) {
		for (int j = i + k - 1; j >= 0; j--) {
			if (j < (int)m->size2) {
				MATRIX_ID(set)(triu_m, i, j, 0.0);
			}
		}
	}
	return triu_m;
}

VECTOR_TYPE* matlab::unique(const VECTOR_TYPE* v, const std::string& first_or_last, VECTOR_TYPE** i, VECTOR_TYPE** j) {
	if (first_or_last != "first" && first_or_last != "last") {
		return NULL;
	}
	VECTOR_TYPE* sort_v = sort(v);
	VECTOR_TYPE* unsized_v = VECTOR_ID(alloc)(v->size);
	VECTOR_ID(set)(unsized_v, 0, VECTOR_ID(get)(sort_v, 0));
	int n = 1;
	for (int x = 1; x < (int)v->size; x++) {
		FP_TYPE prev_value = VECTOR_ID(get)(sort_v, x - 1);
		FP_TYPE value = VECTOR_ID(get)(sort_v, x);
		if (fp_not_equal(prev_value, value)) {
			VECTOR_ID(set)(unsized_v, n++, value);
		}
	}
	VECTOR_ID(free)(sort_v);
	VECTOR_TYPE* unique_v = VECTOR_ID(alloc)(n);
	VECTOR_ID(view) unsized_subv = VECTOR_ID(subvector)(unsized_v, 0, n);
	VECTOR_ID(memcpy)(unique_v, &unsized_subv.vector);
	VECTOR_ID(free)(unsized_v);
	if (i != NULL) {
		*i = VECTOR_ID(alloc)(n);
		for (int x = 0; x < n; x++) {
			for (int y = 0; y < (int)v->size; y++) {
				if (fp_equal(VECTOR_ID(get)(unique_v, x), VECTOR_ID(get)(v, y))) {
					VECTOR_ID(set)(*i, x, y);
					if (first_or_last == "first") {
						break;
					}
				}
			}
		}
	}
	if (j != NULL) {
		*j = VECTOR_ID(alloc)(v->size);
		for (int x = 0; x < (int)v->size; x++) {
			for (int y = 0; y < n; y++) {
				if (fp_equal(VECTOR_ID(get)(v, x), VECTOR_ID(get)(unique_v, y))) {
					VECTOR_ID(set)(*j, x, y);
					break;
				}
			}
		}
	}
	return unique_v;
}

VECTOR_TYPE* matlab::unique(const MATRIX_TYPE* m, const std::string& first_or_last, VECTOR_TYPE** i, VECTOR_TYPE** j) {
	VECTOR_TYPE* v = to_vector(m);
	VECTOR_TYPE* unique_v = unique(v, first_or_last, i, j);
	VECTOR_ID(free)(v);
	return unique_v;
}

/*
 * Emulates (unique(m, "rows", first_or_last)).
 */
MATRIX_TYPE* matlab::unique_rows(const MATRIX_TYPE* m, const std::string& first_or_last, VECTOR_TYPE** i, VECTOR_TYPE** j) {
	if (first_or_last != "first" && first_or_last != "last") {
		return NULL;
	}
	MATRIX_TYPE* sort_m = sortrows(m);
	MATRIX_TYPE* unsized_m = MATRIX_ID(alloc)(m->size1, m->size2);
	VECTOR_ID(view) first_row = MATRIX_ID(row)(sort_m, 0);
	MATRIX_ID(set_row)(unsized_m, 0, &first_row.vector);
	int n_unique = 1;
	for (int x = 1; x < (int)m->size1; x++) {
		VECTOR_ID(view) sort_m_row_x_sub_1 = MATRIX_ID(row)(sort_m, x - 1);
		VECTOR_ID(view) sort_m_row_x = MATRIX_ID(row)(sort_m, x);
		if (compare_vectors(&sort_m_row_x_sub_1.vector, &sort_m_row_x.vector) != 0) {
			MATRIX_ID(set_row)(unsized_m, n_unique++, &sort_m_row_x.vector);
		}
	}
	MATRIX_ID(free)(sort_m);
	MATRIX_TYPE* unique_m = MATRIX_ID(alloc)(n_unique, m->size2);
	MATRIX_ID(view) unsized_subm = MATRIX_ID(submatrix)(unsized_m, 0, 0, n_unique, m->size2);
	MATRIX_ID(memcpy)(unique_m, &unsized_subm.matrix);
	MATRIX_ID(free)(unsized_m);
	if (i != NULL) {
		*i = VECTOR_ID(alloc)(n_unique);
		for (int x = 0; x < n_unique; x++) {
			VECTOR_ID(view) unique_m_row_x = MATRIX_ID(row)(unique_m, x);
			for (int y = 0; y < (int)m->size1; y++) {
				VECTOR_ID(const_view) m_row_y = MATRIX_ID(const_row)(m, y);
				if (compare_vectors(&unique_m_row_x.vector, &m_row_y.vector) == 0) {
					VECTOR_ID(set)(*i, x, y);
					if (first_or_last == "first") {
						break;
					}
				}
			}
		}
	}
	if (j != NULL) {
		*j = VECTOR_ID(alloc)(m->size1);
		for (int x = 0; x < (int)m->size1; x++) {
			VECTOR_ID(const_view) m_row_x = MATRIX_ID(const_row)(m, x);
			for (int y = 0; y < n_unique; y++) {
				VECTOR_ID(view) unique_m_row_y = MATRIX_ID(row)(unique_m, y);
				if (compare_vectors(&m_row_x.vector, &unique_m_row_y.vector) == 0) {
					VECTOR_ID(set)(*j, x, y);
					break;
				}
			}
		}
	}
	return unique_m;
}

MATRIX_TYPE* matlab::FP_ID(zeros)(int size) {
	return MATRIX_ID(calloc)(size, size);
}

MATRIX_TYPE* matlab::FP_ID(zeros)(int size1, int size2) {
	return MATRIX_ID(calloc)(size1, size2);
}

/*
 * Emulates (zeros(size, 1)) or (zeros(1, size)).
 */
VECTOR_TYPE* matlab::FP_ID(zeros_vector)(int size) {
	return VECTOR_ID(calloc)(size);
}
