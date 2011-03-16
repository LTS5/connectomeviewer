#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_vector.h>
#include <limits>
#include "sort.h"
#include <string>

namespace matlab {
	
	// ---------------------------------
	// Precision-independent definitions
	// ---------------------------------
	
#ifndef MATLAB_H
#define MATLAB_H
	
	// Functions
	std::string dec2bin(int n);
	std::string dec2bin(int n, int len);
	gsl_matrix* inv(const gsl_matrix* m);
	gsl_permutation* randperm(int size);
	
	// Operators
	gsl_matrix* div_left(const gsl_matrix* m1, const gsl_matrix* m2);
	gsl_matrix* div_right(const gsl_matrix* m1, const gsl_matrix* m2);
	
	// Utility
	gsl_rng* get_gsl_rng();
	void seed_rng(const gsl_rng* rng, unsigned long seed);
#endif
	
	// -------------------------------
	// Precision-dependent definitions
	// -------------------------------
	
	// Functions
	VECTOR_TYPE* abs(const VECTOR_TYPE* v);
	MATRIX_TYPE* abs(const MATRIX_TYPE* m);
	int all(const VECTOR_TYPE* v);
	VECTOR_TYPE* all(const MATRIX_TYPE* m, int dim = 1);
	int any(const VECTOR_TYPE* v);
	VECTOR_TYPE* any(const MATRIX_TYPE* m, int dim = 1);
	MATRIX_TYPE* diag(const VECTOR_TYPE* v, int k = 0);
	VECTOR_TYPE* diag(const MATRIX_TYPE* m, int k = 0);
	MATRIX_TYPE* FP_ID(eye)(int size);
	MATRIX_TYPE* FP_ID(eye)(int size1, int size2);
	VECTOR_TYPE* find(const VECTOR_TYPE* v, int n = std::numeric_limits<int>::max(), const std::string& direction = "first");
	VECTOR_TYPE* find(const MATRIX_TYPE* m, int n = std::numeric_limits<int>::max(), const std::string& direction = "first");
	MATRIX_TYPE* find_ij(const MATRIX_TYPE* m, int n = std::numeric_limits<int>::max(), const std::string& direction = "first");
	VECTOR_TYPE* hist(const VECTOR_TYPE* v, int n = 10);
	VECTOR_TYPE* hist(const VECTOR_TYPE* v, const VECTOR_TYPE* centers);
	int length(const VECTOR_TYPE* v);
	int length(const MATRIX_TYPE* m);
	FP_TYPE max(FP_TYPE x, FP_TYPE y);
	FP_TYPE max(const VECTOR_TYPE* v);
	VECTOR_TYPE* max(const MATRIX_TYPE* m, int dim = 1);
	FP_TYPE mean(const VECTOR_TYPE* v, const std::string& opt = "a");
	VECTOR_TYPE* mean(const MATRIX_TYPE* m, int dim = 1, const std::string& opt = "a");
	FP_TYPE min(FP_TYPE x, FP_TYPE y);
	FP_TYPE min(const VECTOR_TYPE* v);
	VECTOR_TYPE* min(const MATRIX_TYPE* m, int dim = 1);
	int nnz(const VECTOR_TYPE* v);
	int nnz(const MATRIX_TYPE* m);
	VECTOR_TYPE* nonzeros(const MATRIX_TYPE* m);
	VECTOR_TYPE* normpdf(const VECTOR_TYPE* v, FP_TYPE mean, FP_TYPE stdev);
	MATRIX_TYPE* FP_ID(ones)(int size);
	MATRIX_TYPE* FP_ID(ones)(int size1, int size2);
	VECTOR_TYPE* FP_ID(ones_vector)(int size);
	FP_TYPE prod(const VECTOR_TYPE* v);
	VECTOR_TYPE* prod(const MATRIX_TYPE* m, int dim = 1);
	MATRIX_TYPE* FP_ID(rand)(int size);
	MATRIX_TYPE* FP_ID(rand)(int size1, int size2);
	VECTOR_TYPE* FP_ID(rand_vector)(int size);
	VECTOR_TYPE* reverse(const VECTOR_TYPE* v);
	VECTOR_TYPE* setxor(const VECTOR_TYPE* v1, const VECTOR_TYPE* v2);
	VECTOR_TYPE* sort(const VECTOR_TYPE* v, const std::string& mode = "ascend", VECTOR_TYPE** ind = NULL);
	MATRIX_TYPE* sort(const MATRIX_TYPE* m, int dim = 1, const std::string& mode = "ascend", MATRIX_TYPE** ind = NULL);
	VECTOR_TYPE* sortrows(const VECTOR_TYPE* v, VECTOR_TYPE** ind = NULL);
	MATRIX_TYPE* sortrows(const MATRIX_TYPE* m, VECTOR_TYPE** ind = NULL);
	FP_TYPE std(const VECTOR_TYPE* v, int opt = 0);
	VECTOR_TYPE* std(const MATRIX_TYPE* m, int opt = 0, int dim = 1);
	FP_TYPE sum(const VECTOR_TYPE* v);
	VECTOR_TYPE* sum(const MATRIX_TYPE* m, int dim = 1);
	MATRIX_TYPE* toeplitz(const VECTOR_TYPE* column, const VECTOR_TYPE* row = NULL);
	MATRIX_TYPE* tril(const MATRIX_TYPE* m, int k = 0);
	MATRIX_TYPE* triu(const MATRIX_TYPE* m, int k = 0);
	VECTOR_TYPE* unique(const VECTOR_TYPE* v, const std::string& first_or_last = "last", VECTOR_TYPE** i = NULL, VECTOR_TYPE** j = NULL);
	VECTOR_TYPE* unique(const MATRIX_TYPE* m, const std::string& first_or_last = "last", VECTOR_TYPE** i = NULL, VECTOR_TYPE** j = NULL);
	MATRIX_TYPE* unique_rows(const MATRIX_TYPE* m, const std::string& first_or_last = "last", VECTOR_TYPE** i = NULL, VECTOR_TYPE** j = NULL);
	MATRIX_TYPE* FP_ID(zeros)(int size);
	MATRIX_TYPE* FP_ID(zeros)(int size1, int size2);
	VECTOR_TYPE* FP_ID(zeros_vector)(int size);
	
	// Operators
	VECTOR_TYPE* concatenate(const VECTOR_TYPE* v, FP_TYPE x);
	VECTOR_TYPE* concatenate(FP_TYPE x, const VECTOR_TYPE* v);
	VECTOR_TYPE* concatenate(const VECTOR_TYPE* v1, const VECTOR_TYPE* v2);
	MATRIX_TYPE* concatenate_columns(const VECTOR_TYPE* v1, const VECTOR_TYPE* v2);
	MATRIX_TYPE* concatenate_columns(const MATRIX_TYPE* m, const VECTOR_TYPE* v);
	MATRIX_TYPE* concatenate_columns(const VECTOR_TYPE* v, const MATRIX_TYPE* m);
	MATRIX_TYPE* concatenate_columns(const MATRIX_TYPE* m1, const MATRIX_TYPE* m2);
	MATRIX_TYPE* concatenate_rows(const VECTOR_TYPE* v1, const VECTOR_TYPE* v2);
	MATRIX_TYPE* concatenate_rows(const MATRIX_TYPE* m, const VECTOR_TYPE* v);
	MATRIX_TYPE* concatenate_rows(const VECTOR_TYPE* v, const MATRIX_TYPE* m);
	MATRIX_TYPE* concatenate_rows(const MATRIX_TYPE* m1, const MATRIX_TYPE* m2);
	VECTOR_TYPE* copy(const VECTOR_TYPE* v);
	MATRIX_TYPE* copy(const MATRIX_TYPE* m);
	VECTOR_TYPE* logical_and(const VECTOR_TYPE* v1, const VECTOR_TYPE* v2);
	MATRIX_TYPE* logical_and(const MATRIX_TYPE* m1, const MATRIX_TYPE* m2);
	VECTOR_TYPE* logical_not(const VECTOR_TYPE* v);
	MATRIX_TYPE* logical_not(const MATRIX_TYPE* m);
	VECTOR_TYPE* logical_or(const VECTOR_TYPE* v1, const VECTOR_TYPE* v2);
	MATRIX_TYPE* logical_or(const MATRIX_TYPE* m1, const MATRIX_TYPE* m2);
	MATRIX_TYPE* mul(const MATRIX_TYPE* m1, const MATRIX_TYPE* m2);
	MATRIX_TYPE* pow(const MATRIX_TYPE* m, int power);
	VECTOR_TYPE* pow_elements(const VECTOR_TYPE* v, FP_TYPE power);
	VECTOR_TYPE* pow_elements(const VECTOR_TYPE* v, const VECTOR_TYPE* powers);
	MATRIX_TYPE* pow_elements(const MATRIX_TYPE* m, FP_TYPE power);
	MATRIX_TYPE* pow_elements(const MATRIX_TYPE* m, const MATRIX_TYPE* powers);
	VECTOR_TYPE* FP_ID(sequence)(int start, int end);
	VECTOR_TYPE* FP_ID(sequence)(int start, int step, int end);
	
	// Floating-point comparison
	extern FP_TYPE FP_ID(epsilon);
	int fp_compare(FP_TYPE x, FP_TYPE y);
	typedef bool (*FP_ID(fp_cmp_fn))(FP_TYPE, FP_TYPE);
	bool fp_zero(FP_TYPE x);
	bool fp_nonzero(FP_TYPE x);
	bool fp_equal(FP_TYPE x, FP_TYPE y);
	bool fp_not_equal(FP_TYPE x, FP_TYPE y);
	bool fp_less(FP_TYPE x, FP_TYPE y);
	bool fp_less_or_equal(FP_TYPE x, FP_TYPE y);
	bool fp_greater(FP_TYPE x, FP_TYPE y);
	bool fp_greater_or_equal(FP_TYPE x, FP_TYPE y);
	
	// Vector/matrix comparison
	int compare_vectors(const VECTOR_TYPE* v1, const VECTOR_TYPE* v2);
	bool vector_less(VECTOR_TYPE* v1, VECTOR_TYPE* v2);
	int compare_matrices(const MATRIX_TYPE* m1, const MATRIX_TYPE* m2);
	bool matrix_less(MATRIX_TYPE* m1, MATRIX_TYPE* m2);
	VECTOR_TYPE* compare_elements(const VECTOR_TYPE* v, FP_ID(fp_cmp_fn) compare, FP_TYPE x);
	VECTOR_TYPE* compare_elements(const VECTOR_TYPE* v1, FP_ID(fp_cmp_fn) compare, const VECTOR_TYPE* v2);
	MATRIX_TYPE* compare_elements(const MATRIX_TYPE* m, FP_ID(fp_cmp_fn) compare, FP_TYPE x);
	MATRIX_TYPE* compare_elements(const MATRIX_TYPE* m1, FP_ID(fp_cmp_fn) compare, const MATRIX_TYPE* m2);
	
	// Vector-by-vector indexing
	VECTOR_TYPE* ordinal_index(const VECTOR_TYPE* v, const VECTOR_TYPE* indices);
	void ordinal_index_assign(VECTOR_TYPE* v, const VECTOR_TYPE* indices, FP_TYPE value);
	void ordinal_index_assign(VECTOR_TYPE* v, const VECTOR_TYPE* indices, const VECTOR_TYPE* values);
	VECTOR_TYPE* logical_index(const VECTOR_TYPE* v, const VECTOR_TYPE* logical_v);
	void logical_index_assign(VECTOR_TYPE* v, const VECTOR_TYPE* logical_v, FP_TYPE values);
	void logical_index_assign(VECTOR_TYPE* v, const VECTOR_TYPE* logical_v, const VECTOR_TYPE* values);
	
	// Matrix-by-integer indexing
	FP_TYPE ordinal_index(const MATRIX_TYPE* m, int index);
	void ordinal_index_assign(MATRIX_TYPE* m, int index, FP_TYPE value);
	
	// Matrix-by-vector indexing
	VECTOR_TYPE* ordinal_index(const MATRIX_TYPE* m, const VECTOR_TYPE* indices);
	void ordinal_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* indices, FP_TYPE value);
	void ordinal_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* indices, const VECTOR_TYPE* values);
	VECTOR_TYPE* logical_index(const MATRIX_TYPE* m, const VECTOR_TYPE* logical_v);
	void logical_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* logical_v, FP_TYPE value);
	void logical_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* logical_v, const VECTOR_TYPE* values);
	
	// Matrix-by-two-vectors indexing (non-mixed)
	MATRIX_TYPE* ordinal_index(const MATRIX_TYPE* m, const VECTOR_TYPE* rows, const VECTOR_TYPE* columns);
	void ordinal_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* rows, const VECTOR_TYPE* columns, FP_TYPE value);
	void ordinal_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* rows, const VECTOR_TYPE* columns, const MATRIX_TYPE* values);
	MATRIX_TYPE* logical_index(const MATRIX_TYPE* m, const VECTOR_TYPE* logical_rows, const VECTOR_TYPE* logical_columns);
	void logical_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* logical_rows, const VECTOR_TYPE* logical_columns, FP_TYPE value);
	void logical_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* logical_rows, const VECTOR_TYPE* logical_columns, const MATRIX_TYPE* values);
	
	// Matrix-by-two-vectors indexing (mixed)
	MATRIX_TYPE* ord_log_index(const MATRIX_TYPE* m, const VECTOR_TYPE* rows, const VECTOR_TYPE* logical_columns);
	void ord_log_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* rows, const VECTOR_TYPE* logical_columns, FP_TYPE value);
	void ord_log_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* rows, const VECTOR_TYPE* logical_columns, const MATRIX_TYPE* values);
	MATRIX_TYPE* log_ord_index(const MATRIX_TYPE* m, const VECTOR_TYPE* logical_rows, const VECTOR_TYPE* columns);
	void log_ord_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* logical_rows, const VECTOR_TYPE* columns, FP_TYPE value);
	void log_ord_index_assign(MATRIX_TYPE* m, const VECTOR_TYPE* logical_rows, const VECTOR_TYPE* columns, const MATRIX_TYPE* values);
	
	// Matrix-by-matrix indexing
	MATRIX_TYPE* ordinal_index(const MATRIX_TYPE* m, const MATRIX_TYPE* indices);
	void ordinal_index_assign(MATRIX_TYPE* m, const MATRIX_TYPE* indices, FP_TYPE value);
	void ordinal_index_assign(MATRIX_TYPE* m, const MATRIX_TYPE* indices, const MATRIX_TYPE* values);
	VECTOR_TYPE* logical_index(const MATRIX_TYPE* m, const MATRIX_TYPE* logical_m);
	void logical_index_assign(MATRIX_TYPE* m, const MATRIX_TYPE* logical_m, FP_TYPE value);
	void logical_index_assign(MATRIX_TYPE* m, const MATRIX_TYPE* logical_m, const VECTOR_TYPE* values);
	
	// Vector/matrix conversion
	void to_array(const VECTOR_TYPE* v, FP_TYPE* array);
	bool to_bool(const VECTOR_TYPE* v);
	bool to_bool(const MATRIX_TYPE* m);
	gsl_vector_float* to_vector_float(const VECTOR_TYPE* v);
	gsl_vector* to_vector_double(const VECTOR_TYPE* v);
	gsl_vector_long_double* to_vector_long_double(const VECTOR_TYPE* v);
	VECTOR_TYPE* to_vector(const MATRIX_TYPE* m);
	gsl_matrix_float* to_matrix_float(const MATRIX_TYPE* m);
	gsl_matrix* to_matrix_double(const MATRIX_TYPE* m);
	gsl_matrix_long_double* to_matrix_long_double(const MATRIX_TYPE* m);
	MATRIX_TYPE* to_column_matrix(const VECTOR_TYPE* v);
	MATRIX_TYPE* to_row_matrix(const VECTOR_TYPE* v);
	VECTOR_TYPE* FP_ID(to_vector)(const gsl_permutation* p);
	gsl_permutation* to_permutation(const VECTOR_TYPE* v);
	
	// Utility
	MATRIX_TYPE* permute_columns(const gsl_permutation* p, const MATRIX_TYPE* m);
	MATRIX_TYPE* permute_rows(const gsl_permutation* p, const MATRIX_TYPE* m);
}
