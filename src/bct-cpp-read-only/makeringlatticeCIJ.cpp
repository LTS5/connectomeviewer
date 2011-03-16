#include "bct.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector.h>

/*
 * Generates a random directed binary graph with a ring lattice organization.
 */
gsl_matrix* bct::makeringlatticeCIJ(int N, int K) {
	
	// CIJ = zeros(N);
	gsl_matrix* CIJ = zeros_double(N);
	
	// CIJ1 = ones(N);
	gsl_matrix* CIJ1 = ones_double(N);
	
	// KK = 0;
	int KK = 0;
	
	// cnt = 0;
	int cnt = -1;
	
	// seq = 1:N-1;
	gsl_vector* seq = sequence_double(1, N - 1);
	
	// seq2 = N-1:-1:1;
	gsl_vector* seq2 = sequence_double(N - 1, -1, 1);
	
	gsl_matrix* dCIJ = NULL;
	
	// while (KK<K)
	while (KK < K) {
		
		// cnt = cnt + 1;
		cnt++;
		
		// dCIJ = triu(CIJ1,seq(cnt))-triu(CIJ1,seq(cnt)+1);
		if (dCIJ != NULL) {
			gsl_matrix_free(dCIJ);
		}
		dCIJ = triu(CIJ1, (int)gsl_vector_get(seq, cnt));
		gsl_matrix* triu_CIJ1_seq_cnt_add_1 = triu(CIJ1, (int)gsl_vector_get(seq, cnt) + 1);
		gsl_matrix_sub(dCIJ, triu_CIJ1_seq_cnt_add_1);
		gsl_matrix_free(triu_CIJ1_seq_cnt_add_1);
		
		// dCIJ2 = triu(CIJ1,seq2(cnt))-triu(CIJ1,seq2(cnt)+1);
		gsl_matrix* dCIJ2 = triu(CIJ1, (int)gsl_vector_get(seq2, cnt));
		gsl_matrix* triu_CIJ1_seq2_cnt_add_1 = triu(CIJ1, (int)gsl_vector_get(seq2, cnt) + 1);
		gsl_matrix_sub(dCIJ2, triu_CIJ1_seq2_cnt_add_1);
		gsl_matrix_free(triu_CIJ1_seq2_cnt_add_1);
		
		// dCIJ = dCIJ+dCIJ'+dCIJ2+dCIJ2';
		gsl_matrix* dCIJ_transpose = gsl_matrix_alloc(dCIJ->size2, dCIJ->size1);
		gsl_matrix_transpose_memcpy(dCIJ_transpose, dCIJ);
		gsl_matrix* dCIJ2_transpose = gsl_matrix_alloc(dCIJ2->size2, dCIJ2->size1);
		gsl_matrix_transpose_memcpy(dCIJ2_transpose, dCIJ2);
		gsl_matrix_add(dCIJ, dCIJ_transpose);
		gsl_matrix_free(dCIJ_transpose);
		gsl_matrix_add(dCIJ, dCIJ2);
		gsl_matrix_free(dCIJ2);
		gsl_matrix_add(dCIJ, dCIJ2_transpose);
		gsl_matrix_free(dCIJ2_transpose);
		
		// CIJ = CIJ + dCIJ;
		gsl_matrix_add(CIJ, dCIJ);
		
		// KK = sum(sum(CIJ));
		gsl_vector* sum_CIJ = sum(CIJ);
		KK = (int)sum(sum_CIJ);
		gsl_vector_free(sum_CIJ);
	}
	
	gsl_matrix_free(CIJ1);
	gsl_vector_free(seq);
	gsl_vector_free(seq2);
	
	// overby = KK-K;
	int overby = KK - K;
	
	// if(overby>0)
	if (overby > 0) {
		
		// [i j] = find(dCIJ);
		gsl_matrix* find_dCIJ = find_ij(dCIJ);
		gsl_vector_view i = gsl_matrix_column(find_dCIJ, 0);
		gsl_vector_view j = gsl_matrix_column(find_dCIJ, 1);
		
		// rp = randperm(length(i));
		gsl_permutation* rp = randperm(length(&i.vector));
		
		// for ii=1:overby
		for (int ii = 0; ii < overby; ii++) {
			
			// CIJ(i(rp(ii)),j(rp(ii))) = 0;
			int i_rp_ii = gsl_vector_get(&i.vector, gsl_permutation_get(rp, ii));
			int j_rp_ii = gsl_vector_get(&j.vector, gsl_permutation_get(rp, ii));
			gsl_matrix_set(CIJ, i_rp_ii, j_rp_ii, 0.0);
		}
		
		gsl_matrix_free(find_dCIJ);
		gsl_permutation_free(rp);
	}
	
	gsl_matrix_free(dCIJ);
	return CIJ;
}
