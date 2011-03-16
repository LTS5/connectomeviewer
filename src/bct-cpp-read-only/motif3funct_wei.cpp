#include "bct.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Counts occurrences of three-node functional motifs in a weighted graph.
 * Returns intensity and (optionally) coherence and motif counts.
 */
gsl_matrix* bct::motif3funct_wei(const gsl_matrix* W, gsl_matrix** Q, gsl_matrix** F) {
	if (safe_mode) check_status(W, SQUARE | WEIGHTED, "motif3funct_wei");
	
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
						
						// a=[A(v1,u);A(v2,u);A(u,v1);A(v2,v1);A(u,v2);A(v1,v2)];
						gsl_matrix* a = gsl_matrix_alloc(6, 1);
						for (int i = 0; i < 6; i++) {
							gsl_matrix_set(a, i, 0, gsl_matrix_get(A, WA_rows[i], WA_cols[i]));
						}
						
						// ind=(M3*a)==N3;
						gsl_matrix* M3_mul_a_m = mul(M3, a);
						gsl_matrix_free(a);
						gsl_vector* M3_mul_a = to_vector(M3_mul_a_m);
						gsl_matrix_free(M3_mul_a_m);
						gsl_vector* ind = compare_elements(M3_mul_a, fp_equal, N3);
						gsl_vector_free(M3_mul_a);
						
						// m=sum(ind);
						int m = (int)sum(ind);
						if (m > 0) {
							
							// M=M3(ind,:).*repmat(w,m,1);
							gsl_vector* M3_cols = sequence_double(0, M3->size2 - 1);
							gsl_matrix* M = log_ord_index(M3, ind, M3_cols);
							gsl_vector_free(M3_cols);
							for (int i = 0; i < (int)M->size1; i++) {
								gsl_vector_view M_row_i = gsl_matrix_row(M, i);
								gsl_vector_mul(&M_row_i.vector, w);
							}
							
							// id=ID3(ind);
							gsl_vector* id = logical_index(ID3, ind);
							gsl_vector_add_constant(id, -1.0);
							
							// l=N3(ind);
							gsl_vector* l = logical_index(N3, ind);
							
							// x=sum(M,2)./l;
							gsl_vector* x = sum(M, 2);
							gsl_vector_div(x, l);
							
							// M(M==0)=1;
							gsl_matrix* M_eq_0 = compare_elements(M, fp_equal, 0.0);
							logical_index_assign(M, M_eq_0, 1.0);
							gsl_matrix_free(M_eq_0);
							
							// i=prod(M,2).^(1./l);
							gsl_vector* prod_M = prod(M, 2);
							gsl_matrix_free(M);
							gsl_vector* l_pow_neg_1 = pow_elements(l, -1.0);
							gsl_vector_free(l);
							gsl_vector* i = pow_elements(prod_M, l_pow_neg_1);
							gsl_vector_free(prod_M);
							gsl_vector_free(l_pow_neg_1);
							
							// q = i./x;
							gsl_vector* q = copy(i);
							gsl_vector_div(q, x);
							gsl_vector_free(x);
							
							// [idu j]=unique(id);
							gsl_vector* j;
							gsl_vector* idu = unique(id, "last", &j);
							gsl_vector_free(id);
							
							// j=[0;j];
							gsl_vector* temp = gsl_vector_alloc(j->size + 1);
							gsl_vector_set(temp, 0, -1.0);
							gsl_vector_view temp_subv = gsl_vector_subvector(temp, 1, j->size);
							gsl_vector_memcpy(&temp_subv.vector, j);
							gsl_vector_free(j);
							j = temp;
							
							// mu=length(idu);
							int mu = length(idu);
							
							// i2=zeros(mu,1);
							gsl_vector* i2 = zeros_vector_double(mu);
							
							// q2=i2; f2=i2;
							gsl_vector* q2 = copy(i2);
							gsl_vector* f2 = copy(i2);
							
							// for h=1:mu
							for (int h = 0; h < mu; h++) {
								
								// i2(h)=sum(i(j(h)+1:j(h+1)));
								int j_h = (int)gsl_vector_get(j, h);
								int j_h_add_1 = (int)gsl_vector_get(j, h + 1);
								gsl_vector* iq_indices = sequence_double(j_h + 1, j_h_add_1);
								gsl_vector* i_idx = ordinal_index(i, iq_indices);
								gsl_vector_set(i2, h, sum(i_idx));
								gsl_vector_free(i_idx);
								
								// q2(h)=sum(q(j(h)+1:j(h+1)));
								gsl_vector* q_idx = ordinal_index(q, iq_indices);
								gsl_vector_free(iq_indices);
								gsl_vector_set(q2, h, sum(q_idx));
								gsl_vector_free(q_idx);
								
								// f2(h)=j(h+1)-j(h);
								gsl_vector_set(f2, h, (double)(j_h_add_1 - j_h));
							}
							gsl_vector_free(i);
							gsl_vector_free(q);
							gsl_vector_free(j);
							
							// I(idu,[u v1 v2])=I(idu,[u v1 v2])+[i2 i2 i2];
							// Q(idu,[u v1 v2])=Q(idu,[u v1 v2])+[q2 q2 q2];
							// F(idu,[u v1 v2])=F(idu,[u v1 v2])+[f2 f2 f2];
							double IQF_cols[] = { (double)u, (double)v1, (double)v2 };
							gsl_vector_view IQF_cols_vv = gsl_vector_view_array(IQF_cols, 3);
							gsl_matrix* I_idx = ordinal_index(I, idu, &IQF_cols_vv.vector);
							gsl_matrix* Q_idx = NULL;
							gsl_matrix* F_idx = NULL;
							if (Q != NULL) {
								Q_idx = ordinal_index(*Q, idu, &IQF_cols_vv.vector);
							}
							if (F != NULL) {
								F_idx = ordinal_index(*F, idu, &IQF_cols_vv.vector);
							}
							for (int j = 0; j < 3; j++) {
								gsl_vector_view I_idx_col_j = gsl_matrix_column(I_idx, j);
								gsl_vector_add(&I_idx_col_j.vector, i2);
								if (Q != NULL) {
									gsl_vector_view Q_idx_col_j = gsl_matrix_column(Q_idx, j);
									gsl_vector_add(&Q_idx_col_j.vector, q2);
								}
								if (F != NULL) {
									gsl_vector_view F_idx_col_j = gsl_matrix_column(F_idx, j);
									gsl_vector_add(&F_idx_col_j.vector, f2);
								}
							}
							gsl_vector_free(i2);
							gsl_vector_free(q2);
							gsl_vector_free(f2);
							ordinal_index_assign(I, idu, &IQF_cols_vv.vector, I_idx);
							gsl_matrix_free(I_idx);
							if (Q != NULL) {
								ordinal_index_assign(*Q, idu, &IQF_cols_vv.vector, Q_idx);
								gsl_matrix_free(Q_idx);
							}
							if (F != NULL) {
								ordinal_index_assign(*F, idu, &IQF_cols_vv.vector, F_idx);
								gsl_matrix_free(F_idx);
							}
							gsl_vector_free(idu);
						}
						
						gsl_vector_free(w);
						gsl_vector_free(ind);
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
gsl_vector* bct::motif3funct_wei_v(const gsl_matrix* W, gsl_vector** Q, gsl_vector** F) {
	gsl_matrix* _Q;
	gsl_matrix* _F;
	gsl_matrix* _I = motif3funct_wei(W, &_Q, &_F);
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
