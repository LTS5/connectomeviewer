#include "bct.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Counts occurrences of four-node structural motifs in a binary graph.
 */
gsl_vector* bct::motif4struct_bin(const gsl_matrix* A, gsl_matrix** F) {
	if (safe_mode) check_status(A, SQUARE | BINARY, "motif4struct_bin");
	
	// load motif34lib M4n ID4
	gsl_vector* ID4;
	gsl_matrix* M4 = motif4generate(&ID4);
	
	// n=length(A);
	int n = length(A);
	
	// F=zeros(199,n);
	if (F != NULL) {
		*F = zeros_double(199, n);
	}
	
	// f=zeros(199,1);
	gsl_vector* f = zeros_vector_double(199);
	
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
								
								// s=uint32(sum(10.^(11:-1:0).*[A(v1,u) A(v2,u) A(v3,u) A(u,v1) A(v2,v1) A(v3,v1) A(u,v2) A(v1,v2) A(v3,v2) A(u,v3) A(v1,v3) A(v2,v3)]));
								int A_rows[] = { v1, v2, v3, u, v2, v3, u, v1, v3, u, v1, v2 };
								int A_cols[] = { u, u, u, v1, v1, v1, v2, v2, v2, v3, v3, v3 };
								gsl_vector* s = gsl_vector_alloc(12);
								for (int i = 0; i < 12; i++) {
									gsl_vector_set(s, i, gsl_matrix_get(A, A_rows[i], A_cols[i]));
								}
								
								// ind=ID4(s==M4n);
								int i_M4 = 0;
								for ( ; i_M4 < (int)M4->size1; i_M4++) {
									gsl_vector_view M4_row_i_M4 = gsl_matrix_row(M4, i_M4);
									if (compare_vectors(s, &M4_row_i_M4.vector) == 0) {
										break;
									}
								}
								gsl_vector_free(s);
								if (i_M4 < (int)M4->size1) {
									int ind = (int)gsl_vector_get(ID4, i_M4) - 1;
									
									// if nargout==2; F(ind,[u v1 v2 v3])=F(ind,[u v1 v2 v3])+1; end
									if (F != NULL) {
										int F_cols[] = { u, v1, v2, v3 };
										for (int i = 0; i < 4; i++) {
											gsl_matrix_set(*F, ind, F_cols[i], gsl_matrix_get(*F, ind, F_cols[i]) + 1.0);
										}
									}
									
									// f(ind)=f(ind)+1;
									gsl_vector_set(f, ind, gsl_vector_get(f, ind) + 1.0);
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
	gsl_matrix_free(M4);
	gsl_matrix_free(As);
	return f;
}
