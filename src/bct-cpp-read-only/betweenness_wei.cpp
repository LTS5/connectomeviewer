#include "bct.h"
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Computes node betweenness for a weighted graph.
 */
gsl_vector* bct::betweenness_wei(const gsl_matrix* G) {
	gsl_vector* BC;
	gsl_matrix* EBC = edge_betweenness_wei(G, &BC);
	gsl_matrix_free(EBC);
	return BC;
}

/*
 * Computes node and edge betweenness for a weighted graph.
 */
gsl_matrix* bct::edge_betweenness_wei(const gsl_matrix* G, gsl_vector** BC) {
	if (safe_mode) check_status(G, SQUARE | WEIGHTED, "edge_betweenness_wei");
	
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
		
		// D=inf(1,n); D(u) = 0;
		gsl_vector* D = gsl_vector_alloc(n);
		gsl_vector_set_all(D, GSL_POSINF);
		gsl_vector_set(D, u, 0.0);
		
		// NP=zeros(1,n); NP(u)=1;
		gsl_vector* NP = zeros_vector_double(n);
		gsl_vector_set(NP, u, 1.0);
		
		// S=true(1,n);
		gsl_vector* S = gsl_vector_alloc(n);
		gsl_vector_set_all(S, 1.0);
		
		// P=false(n);
		gsl_matrix* P = gsl_matrix_calloc(n, n);
		
		// Q=zeros(1,n); q=n;
		gsl_vector* Q = zeros_vector_double(n);
		int q = n - 1;
		
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
				
				// Q(q)=v; q=q-1;
				gsl_vector_set(Q, q--, (double)v);
				
				// W=find(G1(v,:));
				gsl_vector_view G1_row_v = gsl_matrix_row(G1, v);
				gsl_vector* W = find(&G1_row_v.vector);
				if (W != NULL) {
					
					// for w=W
					for (int i_W = 0; i_W < (int)W->size; i_W++) {
						int w = (int)gsl_vector_get(W, i_W);
						
						// Duw=D(v)+G1(v,w);
						double Duw = gsl_vector_get(D, v) + gsl_matrix_get(G1, v, w);
						
						// if Duw<D(w)
						if (Duw < gsl_vector_get(D, w)) {
							
							// D(w)=Duw;
							gsl_vector_set(D, w, Duw);
							
							// NP(w)=NP(v);
							gsl_vector_set(NP, w, gsl_vector_get(NP, v));
							
							// P(w,:)=0;
							gsl_vector_view P_row_w = gsl_matrix_row(P, w);
							gsl_vector_set_zero(&P_row_w.vector);
							
							// P(w,v)=1;
							gsl_matrix_set(P, w, v, 1.0);
							
							// elseif Duw==D(w)
						} else if (fp_equal(Duw, gsl_vector_get(D, w))) {
							
							// NP(w)=NP(w)+NP(v);
							gsl_vector_set(NP, w, gsl_vector_get(NP, w) + gsl_vector_get(NP, v));
							
							// P(w,v)=1;
							gsl_matrix_set(P, w, v, 1.0);
						}
					}
					gsl_vector_free(W);
				}
			}
			
			// if isempty(minD), break
			if (nnz(S) == 0) {
				break;
			} else {
				
				// minD=min(D(S))
				gsl_vector* D_S = logical_index(D, S);
				double minD = gsl_vector_min(D_S);
				gsl_vector_free(D_S);
				
				// elseif isinf(minD),
				if (gsl_isinf(minD) == 1) {
					
					// Q(1:q)=find(isinf(D)); break
					gsl_vector* isinf_D = compare_elements(D, fp_equal, GSL_POSINF);
					gsl_vector* find_isinf_D = find(isinf_D);
					gsl_vector_free(isinf_D);
					gsl_vector_view Q_subv = gsl_vector_subvector(Q, 0, q + 1);
					gsl_vector_memcpy(&Q_subv.vector, find_isinf_D);
					gsl_vector_free(find_isinf_D);
					break;
				}
				
				// V=find(D==minD);
				gsl_vector_free(V);
				gsl_vector* D_eq_minD = compare_elements(D, fp_equal, minD);
				V = find(D_eq_minD);
				gsl_vector_free(D_eq_minD);
			}
		}
		
		gsl_vector_free(D);
		gsl_vector_free(S);
		gsl_matrix_free(G1);
		gsl_vector_free(V);
		
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
