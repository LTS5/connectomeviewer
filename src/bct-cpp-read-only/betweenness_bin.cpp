#include "bct.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Computes node betweenness for a binary graph.
 */
gsl_vector* bct::betweenness_bin(const gsl_matrix* G) {
	gsl_vector* BC;
	gsl_matrix* EBC = edge_betweenness_bin(G, &BC);
	gsl_matrix_free(EBC);
	return BC;
}

/*
 * Computes node and edge betweenness for a binary graph.
 */
gsl_matrix* bct::edge_betweenness_bin(const gsl_matrix* G, gsl_vector** BC) {
	if (safe_mode) check_status(G, SQUARE | BINARY, "edge_betweenness_bin");
	
	// n=length(G);
	int n = length(G);
	
	// BC=zeros(n,1);
	if (BC != NULL) {
		*BC = zeros_vector_double(n);
	}
	
	// EBC=zeros(n);
	gsl_matrix* EBC = zeros_double(n);
	
	// for u=1:n
	for (int u = 0; u < n; u++) {
		
		// D=false(1,n); D(u) = 1;
		gsl_vector* D = gsl_vector_calloc(n);
		gsl_vector_set(D, u, 1.0);
		
		// NP=zeros(1,n); NP(u)=1;
		gsl_vector* NP = zeros_vector_double(n);
		gsl_vector_set(NP, u, 1.0);
		
		// P=false(n);
		gsl_matrix* P = gsl_matrix_calloc(n, n);
		
		// Q=zeros(1,n); q=n;
		gsl_vector* Q = zeros_vector_double(n);
		int q = n - 1;
		
		// Gu=G;
		gsl_matrix* Gu = copy(G);
		
		// V=u;
		gsl_vector* V = gsl_vector_alloc(1);
		gsl_vector_set(V, 0, (double)u);
		
		// while V
		while (V != NULL) {
			
			// Gu(:,V)=0;
			gsl_vector* Gu_rows = sequence_double(0, Gu->size1 - 1);
			ordinal_index_assign(Gu, Gu_rows, V, 0.0);
			gsl_vector_free(Gu_rows);
			
			// for v=V
			for (int i_V = 0; i_V < (int)V->size; i_V++) {
				int v = (int)gsl_vector_get(V, i_V);
				
				// Q(q)=v; q=q-1;
				gsl_vector_set(Q, q--, (double)v);
				
				// W=find(Gu(v,:));
				gsl_vector_view Gu_row_v = gsl_matrix_row(Gu, v);
				gsl_vector* W = find(&Gu_row_v.vector);
				if (W != NULL) {
					
					// for w=W
					for (int i_W = 0; i_W < (int)W->size; i_W++) {
						int w = (int)gsl_vector_get(W, i_W);
						
						// if D(w)
						if (fp_nonzero(gsl_vector_get(D, w))) {
							
							// NP(w)=NP(w)+NP(v);
							gsl_vector_set(NP, w, gsl_vector_get(NP, w) + gsl_vector_get(NP, v));
							
							// P(w,v)=1;
							gsl_matrix_set(P, w, v, 1.0);
							
							// else
						} else {
							
							// D(w)=1;
							gsl_vector_set(D, w, 1.0);
							
							// NP(w)=NP(v);
							gsl_vector_set(NP, w, gsl_vector_get(NP, v));
							
							// P(w,v)=1;
							gsl_matrix_set(P, w, v, 1.0);
						}
					}
					gsl_vector_free(W);
				}
			}
			
			// V=find(any(Gu(V,:),1));
			gsl_vector* Gu_cols = sequence_double(0, G->size2 - 1);
			gsl_matrix* Gu_idx = ordinal_index(Gu, V, Gu_cols);
			gsl_vector_free(Gu_cols);
			gsl_vector* any_Gu_idx = any(Gu_idx);
			gsl_matrix_free(Gu_idx);
			gsl_vector_free(V);
			V = find(any_Gu_idx);
			gsl_vector_free(any_Gu_idx);
		}
		
		gsl_matrix_free(Gu);
		
		// if ~all(D)
		if (all(D) == 0) {
			
			// Q(1:q)=find(~D);
			gsl_vector* not_D = logical_not(D);
			gsl_vector* find_not_D = find(not_D);
			gsl_vector_free(not_D);
			gsl_vector_view Q_subv = gsl_vector_subvector(Q, 0, q + 1);
			gsl_vector_memcpy(&Q_subv.vector, find_not_D);
			gsl_vector_free(find_not_D);
		}
		
		gsl_vector_free(D);
		
		// DP=zeros(n,1);
		gsl_vector* DP = zeros_vector_double(n);
		
		// for w=Q(1:n-1);
		for (int i_Q = 0; i_Q < n - 1; i_Q++) {
			int w = (int)gsl_vector_get(Q, i_Q);
			
			// BC(w)=BC(w)+DP(w)
			if (BC != NULL) {
				gsl_vector_set(*BC, w, gsl_vector_get(*BC, w) + gsl_vector_get(DP, w));
			}
			
			// for v=find(P(w,:))
			gsl_vector_view P_row_w = gsl_matrix_row(P, w);
			gsl_vector* find_P_row_w = find(&P_row_w.vector);
			if (find_P_row_w != NULL) {
				for (int i_find_P_row_w = 0; i_find_P_row_w < (int)find_P_row_w->size; i_find_P_row_w++) {
					int v = (int)gsl_vector_get(find_P_row_w, i_find_P_row_w);
					
					// DPvw=(1+DP(w)).*NP(v)./NP(w);
					double DP_w = gsl_vector_get(DP, w);
					double NP_v = gsl_vector_get(NP, v);
					double NP_w = gsl_vector_get(NP, w);
					double DPvw = (1 + DP_w) * NP_v / NP_w;
					
					// DP(v)=DP(v)+DPvw;
					gsl_vector_set(DP, v, gsl_vector_get(DP, v) + DPvw);
					
					// EBC(v,w)=EBC(v,w)+DPvw;
					gsl_matrix_set(EBC, v, w, gsl_matrix_get(EBC, v, w) + DPvw);
				}
				gsl_vector_free(find_P_row_w);
			}
		}
		
		gsl_vector_free(NP);
		gsl_matrix_free(P);
		gsl_vector_free(Q);
		gsl_vector_free(DP);
	}
	
	return EBC;
}
