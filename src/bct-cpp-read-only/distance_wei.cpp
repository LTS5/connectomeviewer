#include "bct.h"
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Computes the distance matrix for a weighted graph.
 */
gsl_matrix* bct::distance_wei(const gsl_matrix* G) {
	if (safe_mode) check_status(G, SQUARE | WEIGHTED, "distance_wei");
	
	// n=length(G);
	int n = length(G);
	
	// D=zeros(n); D(~eye(n))=inf;
	gsl_matrix* D = gsl_matrix_alloc(n, n);
	gsl_matrix_set_all(D, GSL_POSINF);
	gsl_vector_view diag_D = gsl_matrix_diagonal(D);
	gsl_vector_set_all(&diag_D.vector, 0.0);
	
	// for u=1:n
	for (int u = 0; u < n; u++) {
		
		// S=true(1,n);
		gsl_vector* S = gsl_vector_alloc(n);
		gsl_vector_set_all(S, 1.0);
		
		// G1=G;
		gsl_matrix* G1 = copy(G);
		
		// V=u;
		gsl_vector* V = gsl_vector_alloc(1);
		gsl_vector_set(V, 0, (double)u);
		
		// while 1
		while (true) {
			
			// S(V)=0;
			ordinal_index_assign(S, V, 0.0);
			
			// G1(:,V)=0;
			gsl_vector* G1_rows = sequence_double(0, G1->size1 - 1);
			ordinal_index_assign(G1, G1_rows, V, 0.0);
			gsl_vector_free(G1_rows);
			
			// for v=V
			for (int i_V = 0; i_V < (int)V->size; i_V++) {
				int v = (int)gsl_vector_get(V, i_V);
				
				// W=find(G1(v,:));
				gsl_vector_view G1_row_v = gsl_matrix_row(G1, v);
				gsl_vector* W = find(&G1_row_v.vector);
				if (W != NULL) {
					
					// D(u,W)=min([D(u,W);D(u,v)+G1(v,W)]);
					gsl_vector* DG1_row = gsl_vector_alloc(1);
					gsl_vector_set(DG1_row, 0, (double)u);
					gsl_matrix* D_u_W = ordinal_index(D, DG1_row, W);
					double D_u_v = gsl_matrix_get(D, u, v);
					gsl_vector_set(DG1_row, 0, (double)v);
					gsl_matrix* G1_v_W = ordinal_index(G1, DG1_row, W);
					gsl_matrix_add_constant(G1_v_W, D_u_v);
					gsl_matrix* cat = concatenate_columns(D_u_W, G1_v_W);
					gsl_matrix_free(D_u_W);
					gsl_matrix_free(G1_v_W);
					gsl_vector* min_cat_v = min(cat);
					gsl_matrix_free(cat);
					gsl_matrix* min_cat = to_row_matrix(min_cat_v);
					gsl_vector_free(min_cat_v);
					gsl_vector_set(DG1_row, 0, (double)u);
					ordinal_index_assign(D, DG1_row, W, min_cat);
					gsl_vector_free(W);
					gsl_vector_free(DG1_row);
					gsl_matrix_free(min_cat);
				}
			}
			
			// minD=min(D(u,S));
			// if isempty(minD)||isinf(minD), break, end;
			gsl_vector* D_row = gsl_vector_alloc(1);
			gsl_vector_set(D_row, 0, (double)u);
			gsl_matrix* D_u_S = ord_log_index(D, D_row, S);
			gsl_vector_free(D_row);
			if (D_u_S == NULL) {
				break;
			} else {
				double minD = gsl_matrix_min(D_u_S);
				gsl_matrix_free(D_u_S);
				if (gsl_isinf(minD) == 1) {
					break;
				}
				
				// V=find(D(u,:)==minD);
				gsl_vector_free(V);
				gsl_vector_view D_row_u = gsl_matrix_row(D, u);
				gsl_vector* D_row_u_eq_minD = compare_elements(&D_row_u.vector, fp_equal, minD);
				V = find(D_row_u_eq_minD);
				gsl_vector_free(D_row_u_eq_minD);
			}
		}
		
		gsl_vector_free(S);
		gsl_matrix_free(G1);
		gsl_vector_free(V);
	}
	
	return D;
}
