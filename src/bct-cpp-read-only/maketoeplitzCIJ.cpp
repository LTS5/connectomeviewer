#include "bct.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Generates a random directed binary graph with a Toeplitz organization.
 */
gsl_matrix* bct::maketoeplitzCIJ(int N, int K, double s) {
	
	// profile = normpdf([1:N-1],0.5,s);
	gsl_vector* indices = sequence_double(1, N - 1);
	gsl_vector* profile = normpdf(indices, 0.5, s);
	gsl_vector_free(indices);
	
	// template = toeplitz([0 profile],[0 profile]);
	gsl_vector* temp = concatenate(0.0, profile);
	gsl_vector_free(profile);
	profile = temp;
	gsl_matrix* _template = toeplitz(profile, profile);
	gsl_vector_free(profile);
	
	// template = template.*(K./sum(sum(template)))
	gsl_vector* sum__template = sum(_template);
	double sum_sum__template = sum(sum__template);
	gsl_vector_free(sum__template);
	gsl_matrix_scale(_template, (double)K / sum_sum__template);
	
	// CIJ = zeros(N);
	gsl_matrix* CIJ = zeros_double(N);
	
	// while ((sum(sum(CIJ)) ~= K))
	gsl_vector* sum_CIJ = sum(CIJ);
	double sum_sum_CIJ = sum(sum_CIJ);
	gsl_vector_free(sum_CIJ);
	while ((int)sum_sum_CIJ != K) {
		
		// CIJ = (rand(N)<template);
		gsl_matrix* rand_N = rand_double(N);
		gsl_matrix_free(CIJ);
		CIJ = compare_elements(rand_N, fp_less, _template);
		gsl_matrix_free(rand_N);
		
		sum_CIJ = sum(CIJ);
		sum_sum_CIJ = sum(sum_CIJ);
		gsl_vector_free(sum_CIJ);
	}
	
	gsl_matrix_free(_template);
	return CIJ;
}
