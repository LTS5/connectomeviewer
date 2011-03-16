#include "bct.h"
#include <cmath>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <vector>

/*
 * Finds paths from a set of source nodes up to a given length.  Note that there
 * is no savepths argument; if all paths are desired, pass a valid pointer as
 * the allpths argument.  There is also no tpath argument as its value may
 * overflow a C++ long.  Since 0 is a valid node index in C++, -1 is used as the
 * "filler" value in allpths rather than 0 as in MATLAB.  Pq (the main return),
 * plq, and util are indexed by path length.  They therefore have (qmax + 1)
 * elements and contain no valid data at index 0.
 */
std::vector<gsl_matrix*> bct::findpaths(const gsl_matrix* CIJ, const gsl_vector* sources, int qmax, gsl_vector** plq, int* qstop, gsl_matrix** allpths, gsl_matrix** util) {
	if (safe_mode) check_status(CIJ, SQUARE, "findpaths");
	
	// CIJ = double(CIJ~=0);
	gsl_matrix* _CIJ = compare_elements(CIJ, fp_not_equal, 0.0);
	
	// N = size(CIJ,1);
	int N = CIJ->size1;
	
	// pths = [];
	gsl_matrix* pths = NULL;
	
	// Pq = zeros(N,N,qmax);
	std::vector<gsl_matrix*> Pq(qmax + 1);
	Pq[0] = NULL;
	for (int i = 1; i <= qmax; i++) {
		Pq[i] = zeros_double(N, N);
	}
	
	// util = zeros(N,qmax);
	if (util != NULL) {
		*util = zeros_double(N, qmax + 1);
	}
	
	// q = 1;
	int q = 1;
	
	gsl_vector* _CIJ_cols = sequence_double(0, N - 1);
	gsl_matrix* _CIJ_idx = ordinal_index(_CIJ, sources, _CIJ_cols);
	gsl_vector_free(_CIJ_cols);
	gsl_matrix* _CIJ_idx_ij = find_ij(_CIJ_idx);
	gsl_matrix_free(_CIJ_idx);
	pths = gsl_matrix_alloc(2, _CIJ_idx_ij->size1);
	for (int i = 0; i < (int)_CIJ_idx_ij->size1; i++) {
		int i_row = (int)gsl_matrix_get(_CIJ_idx_ij, i, 0);
		int i_start = (int)gsl_vector_get(sources, i_row);
		int i_end = (int)gsl_matrix_get(_CIJ_idx_ij, i, 1);
		gsl_matrix_set(pths, 0, i, (double)i_start);
		gsl_matrix_set(pths, 1, i, (double)i_end);
	}
	gsl_matrix_free(_CIJ_idx_ij);
	
	// util(1:N,q) = util(1:N,q)+hist(reshape(pths,1,size(pths,1)*size(pths,2)),1:N)';
	if (util != NULL) {
		gsl_vector* reshape_pths = to_vector(pths);
		gsl_vector* centers = sequence_double(0, N - 1);
		gsl_vector* hist_reshape_pths = hist(reshape_pths, centers);
		gsl_vector_free(reshape_pths);
		gsl_vector_free(centers);
		gsl_vector_view util_col_q = gsl_matrix_column(*util, q);
		gsl_vector_add(&util_col_q.vector, hist_reshape_pths);
		gsl_vector_free(hist_reshape_pths);
	}
	
	// for np=1:size(pths,2)
	for (int np = 0; np < (int)pths->size2; np++) {
		
		// Pq(pths(1,np),pths(q+1,np),q) = Pq(pths(1,np),pths(q+1,np),q) + 1;
		int i = (int)gsl_matrix_get(pths, 0, np);
		int j = (int)gsl_matrix_get(pths, q, np);
		gsl_matrix_set(Pq[q], i, j, gsl_matrix_get(Pq[q], i, j) + 1.0);
	}
	
	// if (savepths==1)
	if (allpths != NULL) {
		
		// allpths = pths;
		*allpths = copy(pths);
	}
	
	// for q=2:qmax
	for (q = 2; q <= qmax; q++) {
		
		// npths = zeros(q+1,len_npths);
		// Handle as a std::vector<gsl_vector*> rather than preallocating a gsl_matrix*
		std::vector<gsl_vector*> npths_v;
		
		// endp = unique(pths(q,:));
		gsl_vector_view pths_row_q_sub_1 = gsl_matrix_row(pths, q - 1);
		gsl_vector* endp = unique(&pths_row_q_sub_1.vector);
		
		// npthscnt = 0;
		int npthscnt = 0;
		
		// for ii=1:length(endp)
		for (int ii = 0; ii < length(endp); ii++) {
			
			// i = endp(ii);
			int i = (int)gsl_vector_get(endp, ii);
			
			// [pa,pb] = find(pths(q,:) == i);
			gsl_vector* pths_row_q_sub_1_eq_i = compare_elements(&pths_row_q_sub_1.vector, fp_equal, (double)i);
			gsl_vector* pb = find(pths_row_q_sub_1_eq_i);
			gsl_vector_free(pths_row_q_sub_1_eq_i);
			if (pb != NULL) {
				
				// nendp = find(CIJ(i,:)==1);
				gsl_vector_const_view _CIJ_row_i = gsl_matrix_const_row(_CIJ, i);
				gsl_vector* _CIJ_row_i_eq_1 = compare_elements(&_CIJ_row_i.vector, fp_equal, 1.0);
				gsl_vector* nendp = find(_CIJ_row_i_eq_1);
				gsl_vector_free(_CIJ_row_i_eq_1);
				
				// if (~isempty(nendp))
				if (nendp != NULL) {
					
					// for jj=1:length(nendp)
					for (int jj = 0; jj < length(nendp); jj++) {
						
						// j = nendp(jj);
						int j = (int)gsl_vector_get(nendp, jj);
						
						// pb_temp = pb(sum(j==pths(2:q,pb),1)==0);
						gsl_vector* pths_rows = sequence_double(1, q - 1);
						gsl_matrix* pths_idx = ordinal_index(pths, pths_rows, pb);
						gsl_vector_free(pths_rows);
						gsl_matrix* pths_idx_eq_j = compare_elements(pths_idx, fp_equal, (double)j);
						gsl_matrix_free(pths_idx);
						gsl_vector* sum_pths_idx_eq_j = sum(pths_idx_eq_j, 1);
						gsl_matrix_free(pths_idx_eq_j);
						gsl_vector* sum_pths_idx_eq_j_eq_0 = compare_elements(sum_pths_idx_eq_j, fp_equal, 0.0);
						gsl_vector_free(sum_pths_idx_eq_j);
						gsl_vector* pb_temp = logical_index(pb, sum_pths_idx_eq_j_eq_0);
						gsl_vector_free(sum_pths_idx_eq_j_eq_0);
						if (pb_temp != NULL) {
						
							// npths(:,npthscnt+1:npthscnt+length(pb_temp)) = [pths(:,pb_temp)' ones(length(pb_temp),1)*j]';
							pths_rows = sequence_double(0, pths->size1 - 1);
							pths_idx = ordinal_index(pths, pths_rows, pb_temp);
							gsl_vector_free(pths_rows);
							gsl_matrix* temp = gsl_matrix_alloc(pths_idx->size1 + 1, pths_idx->size2);
							gsl_matrix_view temp_subm = gsl_matrix_submatrix(temp, 0, 0, pths_idx->size1, pths_idx->size2);
							gsl_matrix_memcpy(&temp_subm.matrix, pths_idx);
							gsl_matrix_free(pths_idx);
							pths_idx = temp;
							gsl_vector* last_row = gsl_vector_alloc(length(pb_temp));
							gsl_vector_set_all(last_row, (double)j);
							gsl_matrix_set_row(pths_idx, pths_idx->size1 - 1, last_row);
							gsl_vector_free(last_row);
							for (int i = 0; i < length(pb_temp); i++) {
								npths_v.push_back(zeros_vector_double(q + 1));
								gsl_vector_view pths_idx_col_i = gsl_matrix_column(pths_idx, i);
								gsl_vector_view npths_v_i_subv = gsl_vector_subvector(npths_v[npthscnt + i], 0, pths_idx->size1);
								gsl_vector_memcpy(&npths_v_i_subv.vector, &pths_idx_col_i.vector);
							}
							gsl_matrix_free(pths_idx);
							
							// npthscnt = npthscnt+length(pb_temp);
							npthscnt += length(pb_temp);
							
							// Pq(1:N,j,q) = Pq(1:N,j,q)+(hist(pths(1,pb_temp),1:N))';
							gsl_vector_view pths_row_0 = gsl_matrix_row(pths, 0);
							gsl_vector* pths_row_0_idx = ordinal_index(&pths_row_0.vector, pb_temp);
							gsl_vector_free(pb_temp);
							gsl_vector* centers = sequence_double(0, N - 1);
							gsl_vector* hist_pths_idx = hist(pths_row_0_idx, centers);
							gsl_vector_free(pths_row_0_idx);
							gsl_vector_free(centers);
							gsl_vector_view Pq_q_col_j = gsl_matrix_column(Pq[q], j);
							gsl_vector_add(&Pq_q_col_j.vector, hist_pths_idx);
							gsl_vector_free(hist_pths_idx);
						}
					}
					
					gsl_vector_free(nendp);
				}
				
				gsl_vector_free(pb);
			}
		}
		
		gsl_vector_free(endp);
		gsl_matrix* npths = gsl_matrix_alloc(q + 1, npthscnt);
		for (int i = 0; i < npthscnt; i++) {
			gsl_matrix_set_col(npths, i, npths_v[i]);
			gsl_vector_free(npths_v[i]);
		}
		
		// if (savepths==1)
		if (allpths != NULL) {
			
			// allpths = [allpths; zeros(1,size(allpths,2))];
			// allpths = [allpths npths(:,1:npthscnt)];
			gsl_matrix* temp = gsl_matrix_alloc((*allpths)->size1 + 1, (*allpths)->size2 + npthscnt);
			gsl_matrix_set_all(temp, -1.0);
			gsl_matrix_view temp_subm = gsl_matrix_submatrix(temp, 0, 0, (*allpths)->size1, (*allpths)->size2);
			gsl_matrix_memcpy(&temp_subm.matrix, *allpths);
			temp_subm = gsl_matrix_submatrix(temp, 0, (*allpths)->size2, (*allpths)->size1 + 1, npthscnt);
			gsl_matrix_view npths_subm = gsl_matrix_submatrix(npths, 0, 0, npths->size1, npthscnt);
			gsl_matrix_memcpy(&temp_subm.matrix, &npths_subm.matrix);
			gsl_matrix_free(*allpths);
			*allpths = temp;
		}
		
		// util(1:N,q) = util(1:N,q) + hist(reshape(npths,1,size(npths,1)*size(npths,2)),1:N)' - diag(Pq(:,:,q));
		if (util != NULL) {
			gsl_vector* reshape_npths = to_vector(npths);
			gsl_vector* centers = sequence_double(0, N - 1);
			gsl_vector* hist_reshape_npths = hist(reshape_npths, centers);
			gsl_vector_free(reshape_npths);
			gsl_vector_free(centers);
			gsl_vector_view util_col_q = gsl_matrix_column(*util, q);
			gsl_vector_add(&util_col_q.vector, hist_reshape_npths);
			gsl_vector_free(hist_reshape_npths);
			gsl_vector_view diag_Pq_q = gsl_matrix_diagonal(Pq[q]);
			gsl_vector_sub(&util_col_q.vector, &diag_Pq_q.vector);
		}
		
		// pths = npths(:,npths(1,:)~=npths(q+1,:));
		gsl_vector* npths_rows = sequence_double(0, npths->size1 - 1);
		gsl_vector_view npths_row_0 = gsl_matrix_row(npths, 0);
		gsl_vector_view npths_row_q = gsl_matrix_row(npths, q);
		gsl_vector* npths_cols = compare_elements(&npths_row_0.vector, fp_not_equal, &npths_row_q.vector);
		gsl_matrix_free(pths);
		pths = ord_log_index(npths, npths_rows, npths_cols);
		gsl_matrix_free(npths);
		gsl_vector_free(npths_rows);
		gsl_vector_free(npths_cols);
		
		// if (isempty(pths))
		// ...
		if (pths == NULL) {
			break;
		}
	}
	
	gsl_matrix_free(_CIJ);
	if (pths != NULL) {
		gsl_matrix_free(pths);
	}
	
	// qstop = q;
	if (qstop != NULL) {
		*qstop = (q <= qmax) ? q : qmax;
	}
	
	// tpath = sum(sum(sum(Pq)));
	// plq = reshape(sum(sum(Pq)),1,qmax)
	if (plq != NULL) {
		*plq = gsl_vector_alloc(qmax + 1);
		gsl_vector_set(*plq, 0, 0.0);
		for (int i = 1; i <= qmax; i++) {
			gsl_vector* sum_Pq_i = sum(Pq[i]);
			gsl_vector_set(*plq, i, sum(sum_Pq_i));
			gsl_vector_free(sum_Pq_i);
		}
	}
	
	return Pq;
}
