#include "bct.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <vector>

/*
 * Returns all motif isomorphs for a given motif ID and size.
 */
std::vector<gsl_matrix*> bct::find_motif34(int m, int n) {
	
	// if n==3
	if (n == 3) {
		
		// load motif34lib M3 ID3
		gsl_vector* ID3;
		gsl_matrix* M3 = motif3generate(&ID3);
		
		// ind=find(ID3==m).';
		gsl_vector* ID3_eq_m = compare_elements(ID3, fp_equal, (double)m);
		gsl_vector_free(ID3);
		gsl_vector* ind = find(ID3_eq_m);
		gsl_vector_free(ID3_eq_m);
		
		// M=zeros(3,3,length(ind));
		std::vector<gsl_matrix*> M(length(ind));
		
		int i_nondiag[] = { 1, 2, 3, 5, 6, 7 };
		
		// for i=1:length(ind)
		for (int i = 0; i < length(ind); i++) {
			
			// M(:,:,i)=reshape([0 M3(ind(i),1:3) 0 M3(ind(i),4:6) 0],3,3);
			M[i] = gsl_matrix_calloc(3, 3);
			int ind_i = (int)gsl_vector_get(ind, i);
			for (int j = 0; j < 6; j++) {
				ordinal_index_assign(M[i], i_nondiag[j], gsl_matrix_get(M3, ind_i, j));
			}
		}
		
		gsl_matrix_free(M3);
		gsl_vector_free(ind);
		return M;
		
		// elseif n==4
	} else if (n == 4) {
		
		// load motif34lib M4 ID4;
		gsl_vector* ID4;
		gsl_matrix* M4 = motif4generate(&ID4);
		
		// ind=find(ID4==m).';
		gsl_vector* ID4_eq_m = compare_elements(ID4, fp_equal, (double)m);
		gsl_vector_free(ID4);
		gsl_vector* ind = find(ID4_eq_m);
		gsl_vector_free(ID4_eq_m);
		
		// M=zeros(4,4,length(ind));
		std::vector<gsl_matrix*> M(length(ind));
		
		int i_nondiag[] = { 1, 2, 3, 4, 6, 7, 8, 9, 11, 12, 13, 14 };
		
		// for i=1:length(ind)
		for (int i = 0; i < length(ind); i++) {
			
			// M(:,:,i)=reshape([0 M4(ind(i),1:4) 0 M4(ind(i),5:8) 0 M4(ind(i),9:12) 0],4,4);
			M[i] = gsl_matrix_calloc(4, 4);
			int ind_i = (int)gsl_vector_get(ind, i);
			for (int j = 0; j < 12; j++) {
				ordinal_index_assign(M[i], i_nondiag[j], gsl_matrix_get(M4, ind_i, j));
			}
		}
		
		gsl_matrix_free(M4);
		gsl_vector_free(ind);
		return M;
	} else {
		return std::vector<gsl_matrix*>();
	}
}

/*
 * Returns the motif ID for a given matrix.
 */
int bct::find_motif34(const gsl_matrix* m) {
	
	// n=size(m,1);
	int n = m->size1;
	
	// M=eval(['find(motif' int2str(n) 'struct_bin(m))']);
	gsl_vector* f;
	if (n == 3) {
		f = motif3struct_bin(m);
	} else if (n == 4) {
		f = motif4struct_bin(m);
	} else {
		return 0;
	}
	gsl_vector* M_v = find(f);
	gsl_vector_free(f);
	if (M_v != NULL) {
		int M = (int)gsl_vector_get(M_v, 0);
		gsl_vector_free(M_v);
		return M + 1;
	} else {
		return 0;
	}
}
