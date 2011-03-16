#include "bct.h"
#include <cmath>
#include <ctime>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_vector.h>

/*
 * Returns a latticized graph with equivalent degree sequence to the original
 * weighted directed graph, and with preserved connectedness.  On average, each
 * edge is rewired ITER times.  Out-strength is preserved for weighted graphs,
 * while in-strength is not.
 */
gsl_matrix* bct::latmio_dir_connected(const gsl_matrix* R, int ITER) {
	if (safe_mode) check_status(R, SQUARE | DIRECTED, "latmio_dir_connected");
	
	gsl_rng* rng = get_gsl_rng();
	
	// n=length(R);
	int n = length(R);
	
	// D=zeros(n);
	gsl_matrix* D = zeros_double(n);
	
	// u=[0 min([mod(1:n-1,n);mod(n-1:-1:1,n)])];
	gsl_vector* seq1 = sequence_double(1, n - 1);
	gsl_vector* seq2 = sequence_double(n - 1, -1, 1);
	gsl_matrix* seq1_seq2 = concatenate_columns(seq1, seq2);
	gsl_vector_free(seq1);
	gsl_vector_free(seq2);
	gsl_vector* min_seq1_seq2 = min(seq1_seq2);
	gsl_matrix_free(seq1_seq2);
	gsl_vector* u = concatenate(0.0, min_seq1_seq2);
	gsl_vector_free(min_seq1_seq2);
	
	// for v=1:ceil(n/2)
	for (int v = 1; v <= (int)std::ceil((double)n / 2.0); v++) {
		
		// D(n-v+1,:)=u([v+1:n 1:v]);
		gsl_vector* u_indices1 = sequence_double(v, n - 1);
		gsl_vector* u_indices2 = sequence_double(0, v - 1);
		gsl_vector* u_indices = concatenate(u_indices1, u_indices2);
		gsl_vector_free(u_indices1);
		gsl_vector_free(u_indices2);
		gsl_vector* u_idx = ordinal_index(u, u_indices);
		gsl_vector_free(u_indices);
		gsl_matrix_set_row(D, n - v, u_idx);
		gsl_vector_free(u_idx);
		
		// D(v,:)=D(n-v+1,n:-1:1);
		gsl_vector* D_rows = gsl_vector_alloc(1);
		gsl_vector_set(D_rows, 0, (double)(n - v));
		gsl_vector* D_cols = sequence_double(n - 1, -1, 0);
		gsl_matrix* D_idx = ordinal_index(D, D_rows, D_cols);
		gsl_vector_free(D_rows);
		gsl_vector_free(D_cols);
		gsl_vector* D_idx_v = to_vector(D_idx);
		gsl_matrix_free(D_idx);
		gsl_matrix_set_row(D, v - 1, D_idx_v);
		gsl_vector_free(D_idx_v);
	}
	
	// [i j]=find(R);
	gsl_matrix* find_R = find_ij(R);
	gsl_vector_view i = gsl_matrix_column(find_R, 0);
	gsl_vector_view j = gsl_matrix_column(find_R, 1);
	
	// K=length(i);
	int K = length(&i.vector);
	
	// ITER=K*ITER;
	ITER = K * ITER;
	
	gsl_matrix* _R = copy(R);
	
	// for iter=1:ITER
	for (int iter = 1; iter <= ITER; iter++) {
		
		// while 1
		while (true) {
			
			// rewire = 1
			bool rewire = true;
			
			int e1, e2;
			int a, b, c, d;
			
			// while 1
			while (true) {
				
				// e1=ceil(K*rand);
				e1 = gsl_rng_uniform_int(rng, K);
				
				// e2=ceil(K*rand);
				e2 = gsl_rng_uniform_int(rng, K);
				
				// while (e2==e1),
				while (e2 == e1) {
					
					// e2=ceil(K*rand);
					e2 = gsl_rng_uniform_int(rng, K);
				}
				
				// a=i(e1); b=j(e1);
				a = (int)gsl_vector_get(&i.vector, e1);
				b = (int)gsl_vector_get(&j.vector, e1);
				
				// c=i(e2); d=j(e2);
				c = (int)gsl_vector_get(&i.vector, e2);
				d = (int)gsl_vector_get(&j.vector, e2);
				
				// if all(a~=[c d]) && all(b~=[c d]);
				if (a != c && a != d && b != c && b != d) {
					
					// break
					break;
				}
			}
			
			// if ~(R(a,d) || R(c,b))
			if (fp_zero(gsl_matrix_get(_R, a, d)) && fp_zero(gsl_matrix_get(_R, c, b))) {
				
				// if (D(a,b)+D(c,d))>=(D(a,d)+D(c,b))
				if (fp_greater_or_equal(gsl_matrix_get(D, a, b) + gsl_matrix_get(D, c, d),
										gsl_matrix_get(D, a, d) + gsl_matrix_get(D, c, b))) {
					
					// if ~(any([R(a,c) R(d,b) R(d,c)]) && any([R(c,a) R(b,d) R(b,a)]))
					gsl_vector* _R_idx_1 = gsl_vector_alloc(3);
					gsl_vector_set(_R_idx_1, 0, gsl_matrix_get(_R, a, c));
					gsl_vector_set(_R_idx_1, 1, gsl_matrix_get(_R, d, b));
					gsl_vector_set(_R_idx_1, 2, gsl_matrix_get(_R, d, c));
					gsl_vector* _R_idx_2 = gsl_vector_alloc(3);
					gsl_vector_set(_R_idx_2, 0, gsl_matrix_get(_R, c, a));
					gsl_vector_set(_R_idx_2, 1, gsl_matrix_get(_R, b, d));
					gsl_vector_set(_R_idx_2, 2, gsl_matrix_get(_R, b, a));
					bool any_R_idx_1_and_any_R_idx_2 = any(_R_idx_1) && any(_R_idx_2);
					gsl_vector_free(_R_idx_1);
					gsl_vector_free(_R_idx_2);
					if (!any_R_idx_1_and_any_R_idx_2) {
						
						// P=R([a c],:);
						gsl_vector* _R_rows = gsl_vector_alloc(2);
						gsl_vector_set(_R_rows, 0, (double)a);
						gsl_vector_set(_R_rows, 1, (double)c);
						gsl_vector* _R_cols = sequence_double(0, _R->size2 - 1);
						gsl_matrix* P = ordinal_index(_R, _R_rows, _R_cols);
						gsl_vector_free(_R_rows);
						gsl_vector_free(_R_cols);
						
						// P(1,b)=0; P(1,d)=1;
						gsl_matrix_set(P, 0, b, 0.0);
						gsl_matrix_set(P, 0, d, 1.0);
						
						// P(2,d)=0; P(2,b)=1;
						gsl_matrix_set(P, 1, d, 0.0);
						gsl_matrix_set(P, 1, b, 1.0);
						
						// PN=P;
						gsl_matrix* PN = copy(P);
						
						// PN(1,a)=1; PN(2,c)=1;
						gsl_matrix_set(PN, 0, a, 1.0);
						gsl_matrix_set(PN, 1, c, 1.0);
						
						// while 1
						while (true) {
							
							// P(1,:)=any(R(P(1,:)~=0,:),1);
							gsl_vector_view P_row_0 = gsl_matrix_row(P, 0);
							gsl_vector* P_row_0_neq_0 = compare_elements(&P_row_0.vector, fp_not_equal, 0.0);
							gsl_vector* _R_cols = sequence_double(0, _R->size2 - 1);
							gsl_matrix* _R_idx = log_ord_index(_R, P_row_0_neq_0, _R_cols);
							gsl_vector_free(P_row_0_neq_0);
							if (_R_idx != NULL) {
								gsl_vector* any__R_idx = any(_R_idx, 1);
								gsl_matrix_free(_R_idx);
								gsl_matrix_set_row(P, 0, any__R_idx);
								gsl_vector_free(any__R_idx);
							} else {
								gsl_vector_set_zero(&P_row_0.vector);
							}
							
							// P(2,:)=any(R(P(2,:)~=0,:),1);
							gsl_vector_view P_row_1 = gsl_matrix_row(P, 0);
							gsl_vector* P_row_1_neq_0 = compare_elements(&P_row_1.vector, fp_not_equal, 0.0);
							_R_idx = log_ord_index(_R, P_row_1_neq_0, _R_cols);
							gsl_vector_free(P_row_1_neq_0);
							gsl_vector_free(_R_cols);
							if (_R_idx != NULL) {
								gsl_vector* any__R_idx = any(_R_idx, 1);
								gsl_matrix_free(_R_idx);
								gsl_matrix_set_row(P, 1, any__R_idx);
								gsl_vector_free(any__R_idx);
							} else {
								gsl_vector_set_zero(&P_row_1.vector);
							}
							
							// P=P.*(~PN);
							gsl_matrix* not_PN = logical_not(PN);
							gsl_matrix_mul_elements(P, not_PN);
							gsl_matrix_free(not_PN);
							
							// PN=PN+P
							gsl_matrix_add(PN, P);
							
							// if ~all(any(P,2))
							gsl_vector* any_P = any(P, 2);
							bool all_any_P = all(any_P);
							gsl_vector_free(any_P);
							if (!all_any_P) {
								
								// rewire=0;
								rewire = false;
								
								// break
								break;
							}
							
							// elseif any(PN(1,[b c])) && any(PN(2,[d a]))
							gsl_vector* PN_idx_1 = gsl_vector_alloc(2);
							gsl_vector_set(PN_idx_1, 0, gsl_matrix_get(PN, 0, b));
							gsl_vector_set(PN_idx_1, 1, gsl_matrix_get(PN, 0, c));
							gsl_vector* PN_idx_2 = gsl_vector_alloc(2);
							gsl_vector_set(PN_idx_2, 0, gsl_matrix_get(PN, 1, d));
							gsl_vector_set(PN_idx_2, 1, gsl_matrix_get(PN, 1, a));
							bool any_PN_idx_1_and_any_PN_idx_2 = any(PN_idx_1) && any(PN_idx_2);
							gsl_vector_free(PN_idx_1);
							gsl_vector_free(PN_idx_2);
							if (any_PN_idx_1_and_any_PN_idx_2) {
								
								// break
								break;
							}
						}
						
						gsl_matrix_free(P);
						gsl_matrix_free(PN);
					}
					
					// if rewire
					if (rewire) {
						
						// R(a,d)=R(a,b); R(a,b)=0;
						gsl_matrix_set(_R, a, d, gsl_matrix_get(_R, a, b));
						gsl_matrix_set(_R, a, b, 0.0);
						
						// R(c,b)=R(c,d); R(c,d)=0;
						gsl_matrix_set(_R, c, b, gsl_matrix_get(_R, c, d));
						gsl_matrix_set(_R, c, d, 0.0);
						
						// j(e1) = d;
						gsl_vector_set(&j.vector, e1, (double)d);
						
						// j(e2) = b;
						gsl_vector_set(&j.vector, e2, (double)b);
						
						// break;
						break;
					}
				}
			}
		}
	}
	
	gsl_matrix_free(D);
	gsl_matrix_free(find_R);
	return _R;
}
