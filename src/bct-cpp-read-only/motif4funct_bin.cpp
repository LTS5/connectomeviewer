#include "bct.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Counts occurrences of four-node functional motifs in a binary graph.
 */
gsl_vector* bct::motif4funct_bin(const gsl_matrix* W, gsl_matrix** F) {
	if (safe_mode) check_status(W, SQUARE | BINARY, "motif4funct_bin");
	
	// load motif34lib M4 ID4 N4
	gsl_vector* ID4;
	gsl_vector* N4;
	gsl_matrix* M4 = motif4generate(&ID4, &N4);
	
	// n=length(W);
	int n = length(W);
	
	// f=zeros(199,1);
	gsl_vector* f = zeros_vector_double(199);
	
	// F=zeros(199,n);
	if (F != NULL) {
		*F = zeros_double(199, n);
	}
	
	// A=1*(W~=0);
	gsl_matrix* A = compare_elements(W, fp_not_equal, 0.0);
	
	// As=A|A.';
	gsl_matrix* A_transpose = gsl_matrix_alloc(A->size2, A->size1);
	gsl_matrix_transpose_memcpy(A_transpose, A);
	gsl_matrix* As = logical_or(A, A_transpose);
	gsl_matrix_free(A_transpose);
	
	// for u=1:n-3
	for (int u = 0; u < n - 3; u++) {
		
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
				
				// V2=V2|([false(1,v1) As(u,v1+1:n)]);
				gsl_vector* V2_1 = V2;
				gsl_vector* V2_2 = gsl_vector_alloc(n);
				gsl_matrix_get_row(V2_2, As, u);
				for (int i = 0; i <= v1; i++) {
					gsl_vector_set(V2_2, i, 0.0);
				}
				V2 = logical_or(V2_1, V2_2);
				gsl_vector_free(V2_1);
				gsl_vector_free(V2_2);
				
				// for v2=find(V2)
				gsl_vector* find_V2 = find(V2);
				if (find_V2 != NULL) {
					for (int i_find_V2 = 0; i_find_V2 < (int)find_V2->size; i_find_V2++) {
						int v2 = (int)gsl_vector_get(find_V2, i_find_V2);
						
						// vz=max(v1,v2);
						int vz = (v1 > v2) ? v1 : v2;
						
						// V3=([false(1,u) As(v2,u+1:n)]);
						gsl_vector* V3 = gsl_vector_alloc(n);
						gsl_matrix_get_row(V3, As, v2);
						for (int i = 0; i <= u; i++) {
							gsl_vector_set(V3, i, 0.0);
						}
						
						// V3(V2)=0;
						logical_index_assign(V3, V2, 0.0);
						
						// V3=V3|([false(1,v2) As(v1,v2+1:n)]);
						gsl_vector* V3_1 = V3;
						gsl_vector* V3_2 = gsl_vector_alloc(n);
						gsl_matrix_get_row(V3_2, As, v1);
						for (int i = 0; i <= v2; i++) {
							gsl_vector_set(V3_2, i, 0.0);
						}
						V3 = logical_or(V3_1, V3_2);
						gsl_vector_free(V3_1);
						gsl_vector_free(V3_2);
						
						// V3(V1)=0;
						logical_index_assign(V3, V1, 0.0);
						
						// V3=V3|([false(1,vz) As(u,vz+1:n)]);
						V3_1 = V3;
						V3_2 = gsl_vector_alloc(n);
						gsl_matrix_get_row(V3_2, As, u);
						for (int i = 0; i <= vz; i++) {
							gsl_vector_set(V3_2, i, 0.0);
						}
						V3 = logical_or(V3_1, V3_2);
						gsl_vector_free(V3_1);
						gsl_vector_free(V3_2);
						
						// for v3=find(V3)
						gsl_vector* find_V3 = find(V3);
						if (find_V3 != NULL ) {
							for (int i_find_V3 = 0; i_find_V3 < (int)find_V3->size; i_find_V3++) {
								int v3 = (int)gsl_vector_get(find_V3, i_find_V3);
								
								// a=[A(v1,u);A(v2,u);A(v3,u);A(u,v1);A(v2,v1);A(v3,v1);A(u,v2);A(v1,v2);A(v3,v2);A(u,v3);A(v1,v3);A(v2,v3)];
								int A_rows[] = { v1, v2, v3, u, v2, v3, u, v1, v3, u, v1, v2 };
								int A_cols[] = { u, u, u, v1, v1, v1, v2, v2, v2, v3, v3, v3 };
								gsl_matrix* a = gsl_matrix_alloc(12, 1);
								for (int i = 0; i < 12; i++) {
									gsl_matrix_set(a, i, 0, gsl_matrix_get(A, A_rows[i], A_cols[i]));
								}
								
								// ind=(M4*a)==N4;
								gsl_matrix* M4_mul_a_m = mul(M4, a);
								gsl_matrix_free(a);
								gsl_vector* M4_mul_a = to_vector(M4_mul_a_m);
								gsl_matrix_free(M4_mul_a_m);
								gsl_vector* ind = compare_elements(M4_mul_a, fp_equal, N4);
								gsl_vector_free(M4_mul_a);
								
								// id=ID4(ind);
								gsl_vector* id = logical_index(ID4, ind);
								gsl_vector_free(ind);
								if (id != NULL) {
									gsl_vector_add_constant(id, -1.0);
								
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
									
									// f2=zeros(mu,1);
									gsl_vector* f2 = zeros_vector_double(mu);
									
									// for h=1:mu
									for (int h = 0; h < mu; h++) {
										
										// f2(h)=j(h+1)-j(h);
										double j_h_add_1 = gsl_vector_get(j, h + 1);
										double j_h = gsl_vector_get(j, h);
										gsl_vector_set(f2, h, j_h_add_1 - j_h);
									}
									gsl_vector_free(j);
									
									// f(idu)=f(idu)+f2;
									gsl_vector* f_idu_add_f2 = ordinal_index(f, idu);
									gsl_vector_add(f_idu_add_f2, f2);
									ordinal_index_assign(f, idu, f_idu_add_f2);
									gsl_vector_free(f_idu_add_f2);
									
									// if nargout==2; F(idu,[u v1 v2 v3])=F(idu,[u v1 v2 v3])+[f2 f2 f2 f2]; end
									if (F != NULL) {
										double F_cols[] = { (double)u, (double)v1, (double)v2, (double)v3 };
										gsl_vector_view F_cols_vv = gsl_vector_view_array(F_cols, 4);
										gsl_matrix* F_idx = ordinal_index(*F, idu, &F_cols_vv.vector);
										for (int i = 0; i < 4; i++) {
											gsl_vector_view F_idx_col_i = gsl_matrix_column(F_idx, i);
											gsl_vector_add(&F_idx_col_i.vector, f2);
										}
										ordinal_index_assign(*F, idu, &F_cols_vv.vector, F_idx);
										gsl_matrix_free(F_idx);
									}
									
									gsl_vector_free(idu);
									gsl_vector_free(f2);
								}
							}
							
							gsl_vector_free(find_V3);
						}
						
						gsl_vector_free(V3);
					}
					
					gsl_vector_free(find_V2);
				}
				
				gsl_vector_free(V2);
			}
			
			gsl_vector_free(find_V1);
		}
		
		gsl_vector_free(V1);
	}
	
	gsl_vector_free(ID4);
	gsl_vector_free(N4);
	gsl_matrix_free(M4);
	gsl_matrix_free(A);
	gsl_matrix_free(As);
	return f;
}
