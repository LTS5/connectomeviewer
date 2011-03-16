#include "bct.h"
#include <ctime>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_vector.h>

/*
 * Returns a randomized graph with equivalent degree sequence to the original
 * weighted undirected graph.  On average, each edge is rewired ITER time.
 * Strength distributions are not preserved for weighted graphs.
 */
gsl_matrix* bct::randmio_und(const gsl_matrix* R, int ITER) {
	if (safe_mode) check_status(R, SQUARE | UNDIRECTED, "randmio_und");
	
	gsl_rng* rng = get_gsl_rng();
	
	// [i j]=find(tril(R));
	gsl_matrix* tril_R = tril(R);
	gsl_matrix* find_tril_R = find_ij(tril_R);
	gsl_matrix_free(tril_R);
	gsl_vector_view i = gsl_matrix_column(find_tril_R, 0);
	gsl_vector_view j = gsl_matrix_column(find_tril_R, 1);
	
	// K=length(i);
	int K = length(&i.vector);
	
	// ITER=K*ITER;
	ITER = K * ITER;
	
	gsl_matrix* _R = copy(R);
	
	// for iter=1:ITER
	for (int iter = 1; iter <= ITER; iter++) {
		
		// while 1
		while (true) {
			
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
			
			// if rand>0.5
			if (gsl_rng_uniform(rng) > 0.5) {
				
				// i(e2)=d; j(e2)=c;
				gsl_vector_set(&i.vector, e2, (double)d);
				gsl_vector_set(&j.vector, e2, (double)c);
				
				// c=i(e2); d=j(e2);
				c = (int)gsl_vector_get(&i.vector, e2);
				d = (int)gsl_vector_get(&j.vector, e2);
			}
			
			// if ~(R(a,d) || R(c,b))
			if (fp_zero(gsl_matrix_get(_R, a, d)) && fp_zero(gsl_matrix_get(_R, c, b))) {
				
				// R(a,d)=R(a,b); R(a,b)=0;
				gsl_matrix_set(_R, a, d, gsl_matrix_get(_R, a, b));
				gsl_matrix_set(_R, a, b, 0.0);
				
				// R(d,a)=R(b,a); R(b,a)=0;
				gsl_matrix_set(_R, d, a, gsl_matrix_get(_R, b, a));
				gsl_matrix_set(_R, b, a, 0.0);
				
				// R(c,b)=R(c,d); R(c,d)=0;
				gsl_matrix_set(_R, c, b, gsl_matrix_get(_R, c, d));
				gsl_matrix_set(_R, c, d, 0.0);
				
				// R(b,c)=R(d,c); R(d,c)=0;
				gsl_matrix_set(_R, b, c, gsl_matrix_get(_R, d, c));
				gsl_matrix_set(_R, d, c, 0.0);
				
				// j(e1) = d;
				gsl_vector_set(&j.vector, e1, (double)d);
				
				// j(e2) = b;
				gsl_vector_set(&j.vector, e2, (double)b);
				
				// break;
				break;
			}
		}
	}
	
	gsl_matrix_free(find_tril_R);
	return _R;
}
