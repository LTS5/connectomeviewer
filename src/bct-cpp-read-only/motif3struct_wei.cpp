#include "bct.h"
#include <cmath>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Counts occurrences of three-node structural motifs in a weighted graph.
 * Returns intensity and (optionally) coherence and motif counts.
 */
gsl_matrix* bct::motif3struct_wei(const gsl_matrix* W, gsl_matrix** Q, gsl_matrix** F) {
	if (safe_mode) check_status(W, SQUARE | WEIGHTED, "motif3struct_wei");
	
	// load motif34lib M3 M3n ID3 N3
	gsl_vector* ID3;
	gsl_vector* N3;
	gsl_matrix* M3 = motif3generate(&ID3, &N3);
	
	// n=length(W);
	int n = length(W);
	
	// I=zeros(13,n);
	gsl_matrix* I = zeros_double(13, n);
	
	// Q=zeros(13,n);
	if (Q != NULL) {
		*Q = zeros_double(13, n);
	}
	
	// F=zeros(13,n);
	if (F != NULL) {
		*F = zeros_double(13, n);
	}
	
	// A=1*(W~=0);
	gsl_matrix* A = compare_elements(W, fp_not_equal, 0.0);
	
	// As=A|A.';
	gsl_matrix* A_transpose = gsl_matrix_alloc(A->size2, A->size1);
	gsl_matrix_transpose_memcpy(A_transpose, A);
	gsl_matrix* As = logical_or(A, A_transpose);
	gsl_matrix_free(A_transpose);
	
	// for u=1:n-2
	for (int u = 0; u < n - 2; u++) {
		
		// V1=[false(1,u) As(u,u+1:n)];
		gsl_vector* V1 = gsl_vector_alloc(n);
		gsl_matrix_get_row(V1, As, u);
		for (int i = 0; i <= u; i++) {
			gsl_vector_set(V1, i, 0.0);
		}
		
		// for v1=find(V1)
		gsl_vector* find_V1 = find(V1);
		if (find_V1 != NULL) {
			for (int i_find_V1 = 0; i_find_V1 < (int)find_V1->size; i_find_V1++) {
				int v1 = (int)gsl_vector_get(find_V1, i_find_V1);
				
				// V2=[false(1,u) As(v1,u+1:n)];
				gsl_vector* V2 = gsl_vector_alloc(n);
				gsl_matrix_get_row(V2, As, v1);
				for (int i = 0; i <= u; i++) {
					gsl_vector_set(V2, i, 0.0);
				}
				
				// V2(V1)=0;
				logical_index_assign(V2, V1, 0.0);
				
				// V2=([false(1,v1) As(u,v1+1:n)])|V2;
				gsl_vector* V2_1 = gsl_vector_alloc(n);
				gsl_matrix_get_row(V2_1, As, u);
				for (int i = 0; i <= v1; i++) {
					gsl_vector_set(V2_1, i, 0.0);
				}
				gsl_vector* V2_2 = V2;
				V2 = logical_or(V2_1, V2_2);
				gsl_vector_free(V2_1);
				gsl_vector_free(V2_2);
				
				// for v2=find(V2)
				gsl_vector* find_V2 = find(V2);
				if (find_V2 != NULL) {
					for (int i_find_V2 = 0; i_find_V2 < (int)find_V2->size; i_find_V2++) {
						int v2 = (int)gsl_vector_get(find_V2, i_find_V2);
						
						// w=[W(v1,u) W(v2,u) W(u,v1) W(v2,v1) W(u,v2) W(v1,v2)];
						int WA_rows[] = { v1, v2, u, v2, u, v1 };
						int WA_cols[] = { u, u, v1, v1, v2, v2 };
						gsl_vector* w = gsl_vector_alloc(6);
						for (int i = 0; i < 6; i++) {
							gsl_vector_set(w, i, gsl_matrix_get(W, WA_rows[i], WA_cols[i]));
						}
						
						// s=uint32(sum(10.^(5:-1:0).*[A(v1,u) A(v2,u) A(u,v1) A(v2,v1) A(u,v2) A(v1,v2)]));
						gsl_vector* s = gsl_vector_alloc(6);
						for (int i = 0; i < 6; i++) {
							gsl_vector_set(s, i, gsl_matrix_get(A, WA_rows[i], WA_cols[i]));
						}
						
						// ind=(s==M3n);
						int ind = 0;
						for ( ; ind < (int)M3->size1; ind++) {
							gsl_vector_view M3_row_ind = gsl_matrix_row(M3, ind);
							if (compare_vectors(s, &M3_row_ind.vector) == 0) {
								break;
							}
						}
						gsl_vector_free(s);
						if (ind < (int)M3->size1) {
							
							// M=w.*M3(ind,:);
							gsl_vector* M = gsl_vector_alloc(M3->size2);
							gsl_matrix_get_row(M, M3, ind);
							gsl_vector_mul(M, w);
							
							// id=ID3(ind);
							int id = (int)gsl_vector_get(ID3, ind) - 1;
							
							// l=N3(ind);
							int l = (int)gsl_vector_get(N3, ind);
							
							// x=sum(M,2)/l;
							double x = sum(M) / (double)l;
							
							// M(M==0)=1;
							gsl_vector* M_eq_0 = compare_elements(M, fp_equal, 0.0);
							logical_index_assign(M, M_eq_0, 1.0);
							gsl_vector_free(M_eq_0);
							
							// i=prod(M,2)^(1/l);
							double i = std::pow(prod(M), 1.0 / (double)l);
							gsl_vector_free(M);
							
							// q=i/x;
							double q = i / x;
							
							// I(id,[u v1 v2])=I(id,[u v1 v2])+[i i i];
							// Q(id,[u v1 v2])=Q(id,[u v1 v2])+[q q q];
							// F(id,[u v1 v2])=F(id,[u v1 v2])+[1 1 1];
							int IQF_cols[] = { u, v1, v2 };
							for (int j = 0; j < 3; j++) {
								gsl_matrix_set(I, id, IQF_cols[j], gsl_matrix_get(I, id, IQF_cols[j]) + i);
								if (Q != NULL) {
									gsl_matrix_set(*Q, id, IQF_cols[j], gsl_matrix_get(*Q, id, IQF_cols[j]) + q);
								}
								if (F != NULL) {
									gsl_matrix_set(*F, id, IQF_cols[j], gsl_matrix_get(*F, id, IQF_cols[j]) + 1.0);
								}
							}
						}
						
						gsl_vector_free(w);
					}
					
					gsl_vector_free(find_V2);
				}
				
				gsl_vector_free(V2);
			}
			
			gsl_vector_free(find_V1);
		}
		
		gsl_vector_free(V1);
	}
	
	gsl_vector_free(ID3);
	gsl_vector_free(N3);
	gsl_matrix_free(M3);
	gsl_matrix_free(A);
	gsl_matrix_free(As);
	return I;
}

/*
 * Returns per-motif metrics instead of per-motif, per-node metrics.
 */
gsl_vector* bct::motif3struct_wei_v(const gsl_matrix* W, gsl_vector** Q, gsl_vector** F) {
	gsl_matrix* _Q;
	gsl_matrix* _F;
	gsl_matrix* _I = motif3struct_wei(W, &_Q, &_F);
	if (Q != NULL) {
		*Q = sum(_Q, 2);
	}
	gsl_matrix_free(_Q);
	if (F != NULL) {
		*F = sum(_F, 2);
	}
	gsl_matrix_free(_F);
	gsl_vector* I = sum(_I, 2);
	gsl_matrix_free(_I);
	return I;
}
