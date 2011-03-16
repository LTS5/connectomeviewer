#include "bct.h"
#include <cmath>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

gsl_vector* modularity(const gsl_matrix* B, int N, double m);

/*
 * Detects communities in a directed graph via Newman modularity.  Since GSL
 * solves eigensystems differently from MATLAB, communities may be numbered
 * differently.
 */
double bct::modularity_dir(const gsl_matrix* A, gsl_vector** Ci) {
	if (safe_mode) check_status(A, SQUARE | DIRECTED, "modularity_dir");
	
	// Ki=sum(A,1);
	gsl_vector* Ki = sum(A, 1);
	
	// Ko=sum(A,2);
	gsl_vector* Ko = sum(A, 2);
	
	// N=length(A);
	int N = length(A);
	
	// m=sum(Ki);
	double m = sum(Ki);
	
	// b=A-(Ko*Ki).'/m;
	gsl_matrix* Ko_m = to_column_matrix(Ko);
	gsl_vector_free(Ko);
	gsl_matrix* Ki_m = to_row_matrix(Ki);
	gsl_vector_free(Ki);
	gsl_matrix* Ko_mul_Ki_transpose = mul(Ko_m, Ki_m);
	gsl_matrix_free(Ko_m);
	gsl_matrix_free(Ki_m);
	gsl_matrix_transpose(Ko_mul_Ki_transpose);
	gsl_matrix_scale(Ko_mul_Ki_transpose, 1.0 / m);
	gsl_matrix* b = copy(A);
	gsl_matrix_sub(b, Ko_mul_Ki_transpose);
	gsl_matrix_free(Ko_mul_Ki_transpose);
	
	// B=b+b.';
	gsl_matrix* b_transpose = gsl_matrix_alloc(b->size2, b->size1);
	gsl_matrix_transpose_memcpy(b_transpose, b);
	gsl_matrix* B = b;
	gsl_matrix_add(B, b_transpose);
	gsl_matrix_free(b_transpose);
	
	gsl_vector* _Ci = modularity(B, N, m);
	
	// s=Ci(:,ones(1,N));
	gsl_matrix* s = gsl_matrix_alloc(N, N);
	for (int i = 0; i < N; i++) {
		gsl_matrix_set_col(s, i, _Ci);
	}
	
	// Q=~(s-s.').*B/(2*m);
	gsl_matrix* s_transpose = gsl_matrix_alloc(s->size2, s->size1);
	gsl_matrix_transpose_memcpy(s_transpose, s);
	gsl_matrix_sub(s, s_transpose);
	gsl_matrix_free(s_transpose);
	gsl_matrix* Q_m = logical_not(s);
	gsl_matrix_free(s);
	gsl_matrix_mul_elements(Q_m, B);
	gsl_matrix_free(B);
	gsl_matrix_scale(Q_m, 1.0 / (2.0 * m));
	
	// Q=sum(Q(:));
	gsl_vector* sum_Q_m = sum(Q_m);
	gsl_matrix_free(Q_m);
	double Q = sum(sum_Q_m);
	gsl_vector_free(sum_Q_m);
	
	if (Ci != NULL) *Ci = _Ci; else gsl_vector_free(_Ci);
	return Q;
}

/*
 * Detects communities in an undirected graph via Newman modularity.  Since GSL
 * solves eigensystems differently from MATLAB, communities may be numbered
 * differently.
 */
double bct::modularity_und(const gsl_matrix* A, gsl_vector** Ci) {
	if (safe_mode) check_status(A, SQUARE | UNDIRECTED, "modularity_und");
	
	// K=sum(A);
	gsl_vector* K = sum(A);
	
	// N=length(A);
	int N = length(A);
	
	// m=sum(K);
	double m = sum(K);
	
	// B=A-(K.'*K)/m;
	gsl_matrix* K_m_transpose = to_column_matrix(K);
	gsl_matrix* K_m = to_row_matrix(K);
	gsl_vector_free(K);
	gsl_matrix* K_m_transpose_mul_K_m = mul(K_m_transpose, K_m);
	gsl_matrix_free(K_m_transpose);
	gsl_matrix_free(K_m);
	gsl_matrix_scale(K_m_transpose_mul_K_m, 1.0 / m);
	gsl_matrix* B = copy(A);
	gsl_matrix_sub(B, K_m_transpose_mul_K_m);
	gsl_matrix_free(K_m_transpose_mul_K_m);
	
	gsl_vector* _Ci = modularity(B, N, m);
	
	// s=Ci(:,ones(1,N));
	gsl_matrix* s = gsl_matrix_alloc(N, N);
	for (int i = 0; i < N; i++) {
		gsl_matrix_set_col(s, i, _Ci);
	}
	
	// Q=~(s-s.').*B/m;
	gsl_matrix* s_transpose = gsl_matrix_alloc(s->size2, s->size1);
	gsl_matrix_transpose_memcpy(s_transpose, s);
	gsl_matrix_sub(s, s_transpose);
	gsl_matrix_free(s_transpose);
	gsl_matrix* Q_m = logical_not(s);
	gsl_matrix_free(s);
	gsl_matrix_mul_elements(Q_m, B);
	gsl_matrix_free(B);
	gsl_matrix_scale(Q_m, 1.0 / m);
	
	// Q=sum(Q(:));
	gsl_vector* sum_Q_m = sum(Q_m);
	gsl_matrix_free(Q_m);
	double Q = sum(sum_Q_m);
	gsl_vector_free(sum_Q_m);
	
	if (Ci != NULL) *Ci = _Ci; else gsl_vector_free(_Ci);
	return Q;
}

gsl_vector* modularity(const gsl_matrix* B, int N, double m) {
	using namespace bct;
	
	// Ci=ones(N,1);
	gsl_vector* Ci = ones_vector_double(N);
	
	// cn=1;
	int cn = 1;
	
	// U=[1 0];
	gsl_vector* U = gsl_vector_alloc(2);
	gsl_vector_set(U, 0, 1.0);
	gsl_vector_set(U, 1, 0.0);
	
	// ind=1:N;
	gsl_vector* ind = sequence_double(0, N - 1);
	
	// Bg=B;
	gsl_matrix* Bg = copy(B);
	
	// Ng=N;
	int Ng = N;
	
	gsl_eigen_symmv_workspace* eig = gsl_eigen_symmv_alloc(Bg->size1);
	
	// while U(1)
	while (fp_nonzero(gsl_vector_get(U, 0))) {
		
		// [V D]=eig(Bg);
		// [d1 i1]=max(diag(D));
		// v1=V(:,i1);
		if (eig->size != Bg->size1) {
			gsl_eigen_symmv_free(eig);
			eig = gsl_eigen_symmv_alloc(Bg->size1);
		}
		gsl_vector* eval = gsl_vector_alloc(Bg->size1);
		gsl_matrix* evec = gsl_matrix_alloc(Bg->size1, Bg->size1);
		gsl_matrix* temp = copy(Bg);
		gsl_eigen_symmv(temp, eval, evec, eig);
		gsl_matrix_free(temp);
		gsl_eigen_symmv_sort(eval, evec, GSL_EIGEN_SORT_VAL_DESC);
		gsl_vector_free(eval);
		gsl_vector* v1 = gsl_vector_alloc(evec->size1);
		gsl_matrix_get_col(v1, evec, 0);
		gsl_matrix_free(evec);
		
		// S=ones(Ng,1);
		gsl_matrix* S = ones_double(Ng, 1);
		
		// S(v1<0)=-1;
		gsl_vector* v1_lt_0 = compare_elements(v1, fp_less, 0.0);
		gsl_vector_free(v1);
		logical_index_assign(S, v1_lt_0, -1.0);
		gsl_vector_free(v1_lt_0);
		
 		// q=S.'*Bg*S;
		gsl_matrix* S_transpose = gsl_matrix_alloc(S->size2, S->size1);
		gsl_matrix_transpose_memcpy(S_transpose, S);
		gsl_matrix* S_transpose_mul_Bg = mul(S_transpose, Bg);
		gsl_matrix_free(S_transpose);
		gsl_matrix* q_m = mul(S_transpose_mul_Bg, S);
		gsl_matrix_free(S_transpose_mul_Bg);
		double q = gsl_matrix_get(q_m, 0, 0);
		gsl_matrix_free(q_m);
		
		// if q>1e-10
		if (q > 1e-10) {
			
			// qmax=q;
			double qmax = q;
			
			// Bg(logical(eye(Ng)))=0;
			gsl_matrix* eye_Ng = eye_double(Ng);
			logical_index_assign(Bg, eye_Ng, 0.0);
			gsl_matrix_free(eye_Ng);
			
			// indg=ones(Ng,1);
			gsl_matrix* indg = ones_double(Ng, 1);
			
			// Sit=S;
			gsl_matrix* Sit = copy(S);
			
			// while any(indg);
			gsl_vector* any_indg = any(indg);
			bool any_indg_bool = to_bool(any_indg);
			gsl_vector_free(any_indg);
			while (any_indg_bool) {
				
				// Qit=qmax-4*Sit.*(Bg*Sit);
				gsl_matrix* Qit = mul(Bg, Sit);
				gsl_matrix_mul_elements(Qit, Sit);
				gsl_matrix_scale(Qit, -4.0);
				gsl_matrix_add_constant(Qit, qmax);
				
				// qmax=max(Qit.*indg);
				gsl_matrix* Qit_mul_indg = copy(Qit);
				gsl_matrix_mul_elements(Qit_mul_indg, indg);
				gsl_vector* qmax_v = max(Qit_mul_indg);
				gsl_matrix_free(Qit_mul_indg);
				qmax = gsl_vector_get(qmax_v, 0);
				gsl_vector_free(qmax_v);
				
				// TODO: Fix precision issue (differences of 1e-14 for macaque47/71)
				// imax=(Qit==qmax);
				gsl_matrix* imax = compare_elements(Qit, fp_equal, qmax);
				gsl_matrix_free(Qit);
				
				// Sit(imax)=-Sit(imax);
				gsl_vector* neg_Sit_imax = logical_index(Sit, imax);
				if (neg_Sit_imax != NULL) {
					gsl_vector_scale(neg_Sit_imax, -1.0);
					logical_index_assign(Sit, imax, neg_Sit_imax);
					gsl_vector_free(neg_Sit_imax);
				}
				
				// indg(imax)=nan;
				logical_index_assign(indg, imax, GSL_NAN);
				gsl_matrix_free(imax);
				
				// if qmax>q;
				if (qmax > q) {
					
					// q=qmax;
					q = qmax;
					
					// S=Sit;
					gsl_matrix_free(S);
					S = copy(Sit);
				}
				
				any_indg = any(indg);
				any_indg_bool = to_bool(any_indg);
				gsl_vector_free(any_indg);
			}
			
			// if(abs(sum(S))==Ng
			gsl_vector* sum_S_v = sum(S);
			double sum_S = sum(sum_S_v);
			gsl_vector_free(sum_S_v);
			if (fp_equal(std::abs(sum_S), (double)Ng)) {
				
				// U(1)=[];
				gsl_vector* temp = gsl_vector_alloc(U->size - 1);
				gsl_vector_view U_subv = gsl_vector_subvector(U, 1, U->size - 1);
				gsl_vector_memcpy(temp, &U_subv.vector);
				gsl_vector_free(U);
				U = temp;
			} else {
				
				// cn=cn+1;
				cn++;
				
				if (ind != NULL) {
					
					// Ci(ind(S==1))=U(1);
					gsl_matrix* S_eq_1_m = compare_elements(S, fp_equal, 1.0);
					gsl_vector* S_eq_1 = to_vector(S_eq_1_m);
					gsl_matrix_free(S_eq_1_m);
					gsl_vector* ind_idx = logical_index(ind, S_eq_1);
					gsl_vector_free(S_eq_1);
					if (ind_idx != NULL) {
						ordinal_index_assign(Ci, ind_idx, gsl_vector_get(U, 0));
						gsl_vector_free(ind_idx);
					}
					
					// Ci(ind(S==-1))=cn;
					gsl_matrix* S_eq_neg_1_m = compare_elements(S, fp_equal, -1.0);
					gsl_vector* S_eq_neg_1 = to_vector(S_eq_neg_1_m);
					gsl_matrix_free(S_eq_neg_1_m);
					ind_idx = logical_index(ind, S_eq_neg_1);
					gsl_vector_free(S_eq_neg_1);
					if (ind_idx != NULL) {
						ordinal_index_assign(Ci, ind_idx, (double)cn);
						gsl_vector_free(ind_idx);
					}
				}
				
				// U=[cn U];
				gsl_vector* temp = concatenate((double)cn, U);
				gsl_vector_free(U);
				U = temp;
			}
			
			gsl_matrix_free(indg);
			gsl_matrix_free(Sit);
		} else {
			
			// U(1)=[];
			gsl_vector* temp = gsl_vector_alloc(U->size - 1);
			gsl_vector_view U_subv = gsl_vector_subvector(U, 1, U->size - 1);
			gsl_vector_memcpy(temp, &U_subv.vector);
			gsl_vector_free(U);
			U = temp;
		}
		
		gsl_matrix_free(S);
		
		// ind=find(Ci==U(1));
		gsl_vector* _Ci_eq_U_0 = compare_elements(Ci, fp_equal, gsl_vector_get(U, 0));
		gsl_vector_free(ind);
		ind = find(_Ci_eq_U_0);
		gsl_vector_free(_Ci_eq_U_0);
		
		if (ind != NULL) {
			
			// bg=B(ind,ind);
			gsl_matrix* bg = ordinal_index(B, ind, ind);
			
			// Bg=bg-diag(sum(bg));
			gsl_vector* sum_bg = sum(bg);
			gsl_matrix* diag_sum_bg = diag(sum_bg);
			gsl_vector_free(sum_bg);
			gsl_matrix_free(Bg);
			Bg = bg;
			gsl_matrix_sub(Bg, diag_sum_bg);
			gsl_matrix_free(diag_sum_bg);
			
			// Ng=length(ind);
			Ng = length(ind);
		}
	}
	
	gsl_vector_free(U);
	gsl_vector_free(ind);
	gsl_matrix_free(Bg);
	gsl_eigen_symmv_free(eig);
	
	return Ci;
}
