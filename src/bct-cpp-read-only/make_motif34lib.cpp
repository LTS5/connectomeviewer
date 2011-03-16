#include "bct.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_permute_vector.h>
#include <gsl/gsl_vector.h>
#include <string>

/*
 * Our implementation of the BCT motif library does not include Mn ("M as a
 * single number") because a C++ long is generally not large enough to contain
 * the 12-digit numbers required for the four-node motif library.
 */

bct::motif_mode_enum bct::motif_mode = MILO;

bct::motif_mode_enum bct::get_motif_mode() { return motif_mode; }
void bct::set_motif_mode(motif_mode_enum motif_mode) { bct::motif_mode = motif_mode; }

/*
 * Constructs the three-node motif library.
 */
gsl_matrix* bct::motif3generate(gsl_vector** ID, gsl_vector** N) {
	static gsl_matrix* M = NULL;
	static gsl_vector* _ID = NULL;
	static gsl_vector* _N = NULL;
	if (M == NULL) {
		
		// n=0;
		int n = -1;
		
		// M=false(54,6);
		M = gsl_matrix_calloc(54, 6);
		
		// CL=zeros(54,6,'uint8');
		gsl_matrix* CL = zeros_double(54, 6);
		
		// cl=zeros(1,6,'uint8');
		gsl_vector* cl = zeros_vector_double(6);
		
		double i_nondiag[] = { 1, 2, 3, 5, 6, 7 };
		gsl_vector_view i_nondiag_vv = gsl_vector_view_array(i_nondiag, 6);
		
		// for i=0:2^6-1
		for (int i = 0; i < 64; i++) {
			
			// m=dec2bin(i);
			// m=[num2str(zeros(1,6-length(m)), '%d') m];
			std::string m = dec2bin(i, 6);
			
			// G=str2num ([ ...
			// '0'   ' '  m(3)  ' '  m(5) ;
			// m(1)  ' '  '0'   ' '  m(6) ;
			// m(2)  ' '  m(4)  ' '  '0'   ]);
			gsl_matrix* G = gsl_matrix_calloc(3, 3);
			for (int i = 0; i < 6; i++) {
				int index = (int)i_nondiag[i];
				if (m[i] == '1') {
					ordinal_index_assign(G, index, 1.0);
				}
			}
			
			// Ko=sum(G,2);
			gsl_vector* Ko = sum(G, 2);
			
			// Ki=sum(G,1).';
			gsl_vector* Ki = sum(G, 1);
			
			// if Ko+Ki,
			gsl_vector* Ko_add_Ki = copy(Ko);
			gsl_vector_add(Ko_add_Ki, Ki);
			bool Ko_add_Ki_bool = to_bool(Ko_add_Ki);
			gsl_vector_free(Ko_add_Ki);
			if (Ko_add_Ki_bool) {
				
				// n=n+1;
				n++;
				
				// cl(:)=sortrows([Ko Ki]).';
				gsl_matrix* Ko_Ki = concatenate_rows(Ko, Ki);
				gsl_matrix* Ko_Ki_sorted = sortrows(Ko_Ki);
				gsl_matrix_free(Ko_Ki);
				gsl_matrix* Ko_Ki_transpose = gsl_matrix_alloc(2, G->size1);
				gsl_matrix_transpose_memcpy(Ko_Ki_transpose, Ko_Ki_sorted);
				gsl_matrix_free(Ko_Ki_sorted);
				gsl_vector_free(cl);
				cl = to_vector(Ko_Ki_transpose);
				gsl_matrix_free(Ko_Ki_transpose);
				
				// CL(n,:)=cl;
				gsl_matrix_set_row(CL, n, cl);
				
				// M(n,:)=G([2:4 6:8]);
				gsl_vector* G_nondiag = ordinal_index(G, &i_nondiag_vv.vector);
				gsl_matrix_set_row(M, n, G_nondiag);
				gsl_vector_free(G_nondiag);
			}
			
			gsl_matrix_free(G);
			gsl_vector_free(Ko);
			gsl_vector_free(Ki);
		}
		
		gsl_vector_free(cl);
		
		// [u1 u2 ID]=unique(CL,'rows');
		gsl_matrix* u1 = unique_rows(CL, "last", NULL, &_ID);
		gsl_matrix_free(CL);
		gsl_matrix_free(u1);
		gsl_vector_add_constant(_ID, 1.0);

		// id_mika=  [1  3  4  6  7  8  11];
		int id_mika[] = { 1, 3, 4, 6, 7, 8, 11 };
		
		// id_olaf= -[3  6  1 11  4  7   8];
		int id_olaf[] = { 3, 6, 1, 11, 4, 7, 8};
		
		// %convert IDs into Sporns & Kotter classification
		if (motif_mode == SPORNS) {
			for (int i = 0; i < (int)_ID->size; i++) {
				for (int j = 0; j < 7; j++) {
					if ((int)gsl_vector_get(_ID, i) == id_mika[j]) {
						gsl_vector_set(_ID, i, id_olaf[j]);
						break;
					}
				}
			}
		}
		
		// [X ind]=sortrows(ID);
		gsl_vector* ind_v;
		gsl_vector* X = sortrows(_ID, &ind_v);
		gsl_vector_free(X);
		gsl_permutation* ind = to_permutation(ind_v);
		gsl_vector_free(ind_v);
		
		// ID=ID(ind,:);
		gsl_permute_vector(ind, _ID);
		
		// M=M(ind,:);
		gsl_matrix* M_permuted = permute_rows(ind, M);
		gsl_matrix_free(M);
		M = M_permuted;
		gsl_permutation_free(ind);
		
		// N=sum(M,2);
		_N = sum(M, 2);
	}
	
	if (ID != NULL) {
		*ID = copy(_ID);
	}
	if (N != NULL) {
		*N = copy(_N);
	}
	return copy(M);
}

/*
 * Constructs the four-node motif library.
 */
gsl_matrix* bct::motif4generate(gsl_vector** ID, gsl_vector** N) {
	static gsl_matrix* M = NULL;
	static gsl_vector* _ID = NULL;
	static gsl_vector* _N = NULL;
	if (M == NULL) {
		
		// n=0;
		int n = -1;
		
		// M=false(3834,12);
		M = gsl_matrix_calloc(3834, 12);
		
		// CL=zeros(3834,16,'uint8');
		gsl_matrix* CL = zeros_double(3834, 16);
		
		// cl=zeros(1,16,'uint8');
		gsl_vector* cl = zeros_vector_double(16);
		
		double i_nondiag[] = { 1, 2, 3, 4, 6, 7, 8, 9, 11, 12, 13, 14 };
		gsl_vector_view i_nondiag_vv = gsl_vector_view_array(i_nondiag, 12);
		
		// for i=0:2^12-1
		for (int i = 0; i < 4096; i++) {
			
			// m=dec2bin(i);
			// m=[num2str(zeros(1,12-length(m)), '%d') m];
			std::string m = dec2bin(i, 12);
			
			// G=str2num ([ ...
			// '0'   ' '  m(4)  ' '  m(7)  ' '  m(10) ;
			// m(1)  ' '  '0'   ' '  m(8)  ' '  m(11) ;
			// m(2)  ' '  m(5)  ' '  '0'   ' '  m(12) ;
			// m(3)  ' '  m(6)  ' '  m(9)  ' '  '0'    ]);
			gsl_matrix* G = gsl_matrix_calloc(4, 4);
			for (int i = 0; i < 12; i++) {
				int index = (int)i_nondiag[i];
				if (m[i] == '1') {
					ordinal_index_assign(G, index, 1.0);
				}
			}
			
			// Gs=G+G.';
			gsl_matrix* Gs = gsl_matrix_alloc(4, 4);
			gsl_matrix_transpose_memcpy(Gs, G);
			gsl_matrix_add(Gs, G);
			
			// v=Gs(1,:);
			gsl_vector* v = gsl_vector_alloc(4);
			gsl_matrix_get_row(v, Gs, 0);
			
			// for j=1:2,
			for (int j = 1; j <= 2; j++) {
				
				// v=any(Gs(v~=0,:),1)+v;
				gsl_vector* v_neq_0 = compare_elements(v, fp_not_equal, 0.0);
				gsl_vector* Gs_cols = sequence_double(0, Gs->size2 - 1);
				gsl_matrix* Gs_idx = log_ord_index(Gs, v_neq_0, Gs_cols);
				gsl_vector_free(v_neq_0);
				gsl_vector_free(Gs_cols);
				if (Gs_idx != NULL) {
					gsl_vector* any_Gs_idx = any(Gs_idx, 1);
					gsl_vector_add(v, any_Gs_idx);
					gsl_matrix_free(Gs_idx);
					gsl_vector_free(any_Gs_idx);
				}
			}
			
			gsl_matrix_free(Gs);
			
			// if v
			bool v_bool = to_bool(v);
			gsl_vector_free(v);
			if (v_bool) {
				
				// n=n+1;
				n++;
				
				// G2=(G*G)~=0;
				gsl_matrix* G_mul_G = mul(G, G);
				gsl_matrix* G2 = compare_elements(G_mul_G, fp_not_equal, 0.0);
				gsl_matrix_free(G_mul_G);
				
				// Ko=sum(G,2);
				gsl_vector* Ko = sum(G, 2);
				
				// Ki=sum(G,1).';
				gsl_vector* Ki = sum(G, 1);
				
				// Ko2=sum(G2,2);
				gsl_vector* Ko2 = sum(G2, 2);
				
				// Ki2=sum(G2,1).';
				gsl_vector* Ki2 = sum(G2, 1);
				
				// cl(:)=sortrows([Ki Ko Ki2 Ko2]).';
				gsl_matrix* Ki_Ko = concatenate_rows(Ki, Ko);
				gsl_vector_free(Ki);
				gsl_vector_free(Ko);
				gsl_matrix* Ki_Ko_Ki2 = concatenate_rows(Ki_Ko, Ki2);
				gsl_vector_free(Ki2);
				gsl_matrix_free(Ki_Ko);
				gsl_matrix* Ki_Ko_Ki2_Ko2 = concatenate_rows(Ki_Ko_Ki2, Ko2);
				gsl_vector_free(Ko2);
				gsl_matrix_free(Ki_Ko_Ki2);
				gsl_matrix* Ks_sorted = sortrows(Ki_Ko_Ki2_Ko2);
				gsl_matrix_free(Ki_Ko_Ki2_Ko2);
				gsl_matrix* Ks_transpose = gsl_matrix_alloc(4, G->size1);
				gsl_matrix_transpose_memcpy(Ks_transpose, Ks_sorted);
				gsl_matrix_free(Ks_sorted);
				gsl_vector_free(cl);
				cl = to_vector(Ks_transpose);
				gsl_matrix_free(Ks_transpose);
				
				// CL(n,:)=cl;
				gsl_matrix_set_row(CL, n, cl);
				
				// M(n,:)=G([2:5 7:10 12:15]);
				gsl_vector* G_nondiag = ordinal_index(G, &i_nondiag_vv.vector);
				gsl_matrix_set_row(M, n, G_nondiag);
				gsl_vector_free(G_nondiag);
			}
			
			gsl_matrix_free(G);
		}
		
		gsl_vector_free(cl);
		
		// [u1 u2 ID]=unique(CL,'rows');
		gsl_matrix* u1 = unique_rows(CL, "last", NULL, &_ID);
		gsl_matrix_free(CL);
		gsl_matrix_free(u1);
		gsl_vector_add_constant(_ID, 1.0);
		
		// [X ind]=sortrows(ID);
		gsl_vector* ind_v;
		gsl_vector* X = sortrows(_ID, &ind_v);
		gsl_vector_free(X);
		gsl_permutation* ind = to_permutation(ind_v);
		gsl_vector_free(ind_v);
		
		// ID=ID(ind,:);
		gsl_permute_vector(ind, _ID);
		
		// M=M(ind,:);
		gsl_matrix* M_permuted = permute_rows(ind, M);
		gsl_matrix_free(M);
		M = M_permuted;
		gsl_permutation_free(ind);
		
		// N=sum(M,2);
		_N = sum(M, 2);
	}
	
	if (ID != NULL) {
		*ID = copy(_ID);
	}
	if (N != NULL) {
		*N = copy(_N);
	}
	return copy(M);
}
