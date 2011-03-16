#include "bct.h"
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

void reachdist2(const gsl_matrix*, gsl_matrix**, gsl_matrix**, gsl_matrix**, int, int*, gsl_vector*, gsl_vector*);

/*
 * Computes reachability and distance matrices based on the power of the
 * adjacency matrix.
 */
gsl_matrix* bct::reachdist(const gsl_matrix* CIJ, gsl_matrix** D) {
	if (safe_mode) check_status(CIJ, SQUARE, "reachdist");
	
	// R = CIJ;
	gsl_matrix* R = copy(CIJ);
	
	// D = CIJ;
	if (D != NULL) {
		*D = copy(CIJ);
	}
	
	// powr = 2;
	int powr = 2;
	
	// N = size(CIJ,1);
	int N = CIJ->size1;
	
	// CIJpwr = CIJ;
	gsl_matrix* CIJpwr = copy(CIJ);
	
	// id = sum(CIJ,1);
	gsl_vector* id = sum(CIJ, 1);
	
	// od = sum(CIJ,2)';
	gsl_vector* od = sum(CIJ, 2);
	
	// id_0 = find(id==0);
	gsl_vector* id_eq_0 = compare_elements(id, fp_equal, 0.0);
	gsl_vector_free(id);
	gsl_vector* id_0 = find(id_eq_0);
	gsl_vector_free(id_eq_0);
	
	// od_0 = find(od==0);
	gsl_vector* od_eq_0 = compare_elements(od, fp_equal, 0.0);
	gsl_vector_free(od);
	gsl_vector* od_0 = find(od_eq_0);
	gsl_vector_free(od_eq_0);
	
	// col = setxor(1:N,id_0);
	gsl_vector* i_all = sequence_double(0, N - 1);
	gsl_vector* col = setxor(i_all, id_0);
	
	// row = setxor(1:N,od_0);
	gsl_vector* row = setxor(i_all, od_0);
	gsl_vector_free(i_all);
	
	// [R,D,powr] = reachdist2(CIJ,CIJpwr,R,D,N,powr,col,row);
	reachdist2(CIJ, &CIJpwr, &R, D, N, &powr, col, row);
	gsl_matrix_free(CIJpwr);
	if (col != NULL) {
		gsl_vector_free(col);
	}
	if (row != NULL) {
		gsl_vector_free(row);
	}
	
	if (D != NULL) {
		
		// D = powr - D+1;
		gsl_matrix_scale(*D, -1.0);
		gsl_matrix_add_constant(*D, (double)(powr + 1));
		
		// D(D==(N+2)) = Inf;
		gsl_matrix* D_eq_N_add_2 = compare_elements(*D, fp_equal, (double)(N + 2));
		logical_index_assign(*D, D_eq_N_add_2, GSL_POSINF);
		gsl_matrix_free(D_eq_N_add_2);
		
		// D(:,id_0) = Inf;
		gsl_vector* D_rows_cols = sequence_double(0, N - 1);
		if (id_0 != NULL) {
			ordinal_index_assign(*D, D_rows_cols, id_0, GSL_POSINF);
		}
		
		// D(od_0,:) = Inf;
		if (od_0 != NULL) {
			ordinal_index_assign(*D, od_0, D_rows_cols, GSL_POSINF);
		}
		gsl_vector_free(D_rows_cols);
	}
	
	if (id_0 != NULL) {
		gsl_vector_free(id_0);
	}
	if (od_0 != NULL) {
		gsl_vector_free(od_0);
	}
	return R;
}

void reachdist2(const gsl_matrix* CIJ, gsl_matrix** CIJpwr, gsl_matrix** R, gsl_matrix** D, int N, int* powr, gsl_vector* col, gsl_vector* row) {
	using namespace bct;
	
	// CIJpwr = CIJpwr*CIJ;
	gsl_matrix* temp = mul(*CIJpwr, CIJ);
	gsl_matrix_free(*CIJpwr);
	*CIJpwr = temp;
	
	// R = double(R | ((CIJpwr)~=0));
	gsl_matrix* CIJpwr_neq_0 = compare_elements(*CIJpwr, fp_not_equal, 0.0);
	gsl_matrix* R_or_CIJpwr_neq_0 = logical_or(*R, CIJpwr_neq_0);
	gsl_matrix_free(CIJpwr_neq_0);
	gsl_matrix_free(*R);
	*R = R_or_CIJpwr_neq_0;
	
	// D = D+R;
	if (D != NULL) {
		gsl_matrix_add(*D, *R);
	}
	
	// if ((powr<=N)&&(~isempty(nonzeros(R(row,col)==0))))
	if (*powr <= N) {
		if (row != NULL && col != NULL) {
			gsl_matrix* R_idx = ordinal_index(*R, row, col);
			gsl_matrix* R_idx_eq_0 = compare_elements(R_idx, fp_equal, 0.0);
			gsl_matrix_free(R_idx);
			gsl_vector* nonzeros_R_idx_eq_0 = nonzeros(R_idx_eq_0);
			gsl_matrix_free(R_idx_eq_0);
			if (nonzeros_R_idx_eq_0 != NULL) {
				gsl_vector_free(nonzeros_R_idx_eq_0);
				
				// powr = powr+1;
				(*powr)++;
				
				// [R,D,powr] = reachdist2(CIJ,CIJpwr,R,D,N,powr,col,row);
				reachdist2(CIJ, CIJpwr, R, D, N, powr, col, row);
			}
		}
	}
}
