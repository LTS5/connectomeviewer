#include "bct.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Computes the clustering coefficient for a binary undirected graph.
 */
gsl_vector* bct::clustering_coef_bu(const gsl_matrix* G) {
	if (safe_mode) check_status(G, SQUARE | BINARY | UNDIRECTED, "clustering_coef_bu");
	
	// n=length(G);
	int n = length(G);
	
	// C=zeros(n,1);
	gsl_vector* C = zeros_vector_double(n);
	
	// for u=1:n
	for (int u = 0; u < n; u++) {
		
		// V=find(G(u,:));
		// k=length(V);
		// if k>=2;
		gsl_vector_const_view G_row_u = gsl_matrix_const_row(G, u);
		gsl_vector* V = find(&G_row_u.vector);
		if (V != NULL) {
			int k = length(V);
			if (k >= 2) {
				
				// S=G(V,V);
				gsl_matrix* S = ordinal_index(G, V, V);
				
				// C(u)=sum(S(:))/(k^2-k);
				gsl_vector* sum_S = sum(S);
				gsl_matrix_free(S);
				gsl_vector_set(C, u, sum(sum_S) / (double)(k * (k - 1)));
				gsl_vector_free(sum_S);
			}
			
			gsl_vector_free(V);
		}
	}
	
	return C;
}
