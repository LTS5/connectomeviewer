#include "bct.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

double matching_ind(const gsl_vector*, const gsl_vector*, int, int, int);

/*
 * Computes matching index for all connections.
 */
gsl_matrix* bct::matching_ind(const gsl_matrix* CIJ) {
	if (safe_mode) check_status(CIJ, SQUARE, "matching_ind");
	
	// N = size(CIJ,1);
	int N = CIJ->size1;
	
	// Mall = zeros(N,N);
	gsl_matrix* Mall = zeros_double(N, N);
	
	// for i=1:N-1
	for (int i = 0; i < N - 1; i++) {
		
		// c1 = [CIJ(:,i)' CIJ(i,:)];
		gsl_vector_const_view CIJ_col_i = gsl_matrix_const_column(CIJ, i);
		gsl_vector_const_view CIJ_row_i = gsl_matrix_const_row(CIJ, i);
		gsl_vector* c1 = concatenate(&CIJ_col_i.vector, &CIJ_row_i.vector);
		
		// for j=i+1:N
		for (int j = i + 1; j < N; j++) {
			
			// c2 = [CIJ(:,j)' CIJ(j,:)];
			gsl_vector_const_view CIJ_col_j = gsl_matrix_const_column(CIJ, j);
			gsl_vector_const_view CIJ_row_j = gsl_matrix_const_row(CIJ, j);
			gsl_vector* c2 = concatenate(&CIJ_col_j.vector, &CIJ_row_j.vector);
			
			gsl_matrix_set(Mall, i, j, matching_ind(c1, c2, i, j, N));
			gsl_vector_free(c2);
		}
		
		gsl_vector_free(c1);
	}
	
	return Mall;
}

/*
 * Computes matching index for incoming connections.
 */
gsl_matrix* bct::matching_ind_in(const gsl_matrix* CIJ) {
	if (safe_mode) check_status(CIJ, SQUARE, "matching_ind_in");
	
	// N = size(CIJ,1);
	int N = CIJ->size1;
	
	// Min = zeros(N,N);
	gsl_matrix* Min = zeros_double(N, N);
	
	// for i=1:N-1
	for (int i = 0; i < N - 1; i++) {
		
		// c1 = CIJ(:,i);
		gsl_vector_const_view c1 = gsl_matrix_const_column(CIJ, i);
		
		// for j=i+1:N
		for (int j = i + 1; j < N; j++) {
			
			// c2 = CIJ(:,j);
			gsl_vector_const_view c2 = gsl_matrix_const_column(CIJ, j);
			
			gsl_matrix_set(Min, i, j, matching_ind(&c1.vector, &c2.vector, i, j, 0));
		}
	}
	
	return Min;
}

/*
 * Computes matching index for outgoing connections.
 */
gsl_matrix* bct::matching_ind_out(const gsl_matrix* CIJ) {
	if (safe_mode) check_status(CIJ, SQUARE, "matching_ind_out");
	
	// N = size(CIJ,1);
	int N = CIJ->size1;
	
	// Mout = zeros(N,N);
	gsl_matrix* Mout = zeros_double(N, N);
	
	// for i=1:N-1
	for (int i = 0; i < N - 1; i++) {
		
		// c1 = CIJ(:,i);
		gsl_vector_const_view c1 = gsl_matrix_const_row(CIJ, i);
		
		// for j=i+1:N
		for (int j = i + 1; j < N; j++) {
			
			// c2 = CIJ(:,j);
			gsl_vector_const_view c2 = gsl_matrix_const_row(CIJ, j);
			
			gsl_matrix_set(Mout, i, j, matching_ind(&c1.vector, &c2.vector, i, j, 0));
		}
	}
	
	return Mout;
}

double matching_ind(const gsl_vector* c1, const gsl_vector* c2, int i, int j, int N) {
	using namespace bct;
	
	// use = ~(~c1&~c2);
	gsl_vector* use = logical_or(c1, c2);
	
	// use(i) = 0;  use(i+N) = 0;
	gsl_vector_set(use, i, 0.0);
	gsl_vector_set(use, i + N, 0.0);
	
	// use(j) = 0;  use(j+N) = 0;
	gsl_vector_set(use, j, 0.0);
	gsl_vector_set(use, j + N, 0.0);
	
	// ncon = sum(c1(use))+sum(c2(use));
	gsl_vector* c1_use = logical_index(c1, use);
	gsl_vector* c2_use = logical_index(c2, use);
	gsl_vector_free(use);
	double ncon = 0.0;
	if (c1_use != NULL) {
		ncon += sum(c1_use);
	}
	if (c2_use != NULL) {
		ncon += sum(c2_use);
	}
	
	// if (ncon==0)
	double ret;
	if (fp_zero(ncon)) {
		
		// Mall(i,j) = 0;
		ret = 0.0;
	} else {
		
		// Mall(i,j) = 2*(sum(c1(use)&c2(use))/ncon);
		if (c1_use == NULL || c2_use == NULL) {
			ret = 0.0;
		} else {
			gsl_vector* log_and = logical_and(c1_use, c2_use);
			ret = 2.0 * sum(log_and) / ncon;
			gsl_vector_free(log_and);
		}
	}
	
	if (c1_use != NULL) gsl_vector_free(c1_use);
	if (c2_use != NULL) gsl_vector_free(c2_use);
	return ret;
}
