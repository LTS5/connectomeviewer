#include "bct.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <vector>

/*
 * Computes the fraction of all paths that are cycles.
 */
gsl_vector* bct::cycprob_fcyc(const std::vector<gsl_matrix*>& Pq) {
	
	// fcyc = zeros(1,size(Pq,3));
	gsl_vector* fcyc = zeros_vector_double(Pq.size());
	
	// for q=1:size(Pq,3)
	for (int q = 1; q < (int)Pq.size(); q++) {
		
		// if(sum(sum(Pq(:,:,q)))>0)
		gsl_vector* sum_Pq_q = sum(Pq[q]);
		double sum_sum_Pq_q = sum(sum_Pq_q);
		gsl_vector_free(sum_Pq_q);
		if (sum_sum_Pq_q > 0.0) {
			
			// fcyc(q) = sum(diag(Pq(:,:,q)))/sum(sum(Pq(:,:,q)));
			gsl_vector_view diag_Pq_q = gsl_matrix_diagonal(Pq[q]);
			double sum_diag_Pq_q = sum(&diag_Pq_q.vector);
			gsl_vector_set(fcyc, q, sum_diag_Pq_q / sum_sum_Pq_q);
		}
	}
	
	return fcyc;
}

/*
 * Computes the probability that a non-cyclic path of length (q - 1) can be
 * extended to form a cycle of length q.
 */
gsl_vector* bct::cycprob_pcyc(const std::vector<gsl_matrix*>& Pq) {
	
	// pcyc = zeros(1,size(Pq,3));
	gsl_vector* pcyc = zeros_vector_double(Pq.size());
	
	// for q=2:size(Pq,3)
	for (int q = 2; q < (int)Pq.size(); q++) {
		
		// if((sum(sum(Pq(:,:,q-1)))-sum(diag(Pq(:,:,q-1))))>0)
		gsl_vector* sum_Pq_q_sub_1 = sum(Pq[q - 1]);
		double sum_sum_Pq_q_sub_1 = sum(sum_Pq_q_sub_1);
		gsl_vector_free(sum_Pq_q_sub_1);
		gsl_vector_view diag_Pq_q_sub_1 = gsl_matrix_diagonal(Pq[q - 1]);
		double sum_diag_Pq_q_sub_1 = sum(&diag_Pq_q_sub_1.vector);
		if (sum_sum_Pq_q_sub_1 - sum_diag_Pq_q_sub_1 > 0.0) {
			
			// pcyc(q) = sum(diag(Pq(:,:,q)))/(sum(sum(Pq(:,:,q-1)))-sum(diag(Pq(:,:,q-1))));
			gsl_vector_view diag_Pq_q = gsl_matrix_diagonal(Pq[q]);
			double sum_diag_Pq_q = sum(&diag_Pq_q.vector);
			gsl_vector_set(pcyc, q, sum_diag_Pq_q / (sum_sum_Pq_q_sub_1 - sum_diag_Pq_q_sub_1));
		}
	}
	
	return pcyc;
}
