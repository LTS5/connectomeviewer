#include "bct.h"
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Performs a breadth-first search starting at the source node.  Because C++
 * indexing is zero-based, a value of 0 at branch(i) could mean either that node
 * 0 precedes node i or that node i is unreachable.  Check distance(i) for
 * GSL_POSINF to differentiate between these two cases.
 */
gsl_vector* bct::breadth(const gsl_matrix* CIJ, int source, gsl_vector** branch) {
	if (safe_mode) check_status(CIJ, SQUARE, "breadth");
	
	// N = size(CIJ,1);
	int N = CIJ->size1;
	
	// % colors: white, gray, black
	int white = 0;
	int gray = 1;
	int black = 2;
	
	// color = zeros(1,N);
	gsl_vector* color = zeros_vector_double(N);
	
	// distance = inf*ones(1,N);
	gsl_vector* distance = gsl_vector_alloc(N);
	gsl_vector_set_all(distance, GSL_POSINF);
	
	// branch = zeros(1,N);
	if (branch != NULL) {
		*branch = zeros_vector_double(N);
	}
	
	// color(source) = gray;
	gsl_vector_set(color, source, (double)gray);
	
	// distance(source) = 0;
	gsl_vector_set(distance, source, 0.0);
	
	// branch(source) = -1;
	if (branch != NULL) {
		gsl_vector_set(*branch, source, -1.0);
	}
	
	// Q = source;
	gsl_vector* Q = gsl_vector_alloc(1);
	gsl_vector_set(Q, 0, (double)source);
	
	// while ~isempty(Q)
	while (Q != NULL) {
		
		// u = Q(1);
		int u = (int)gsl_vector_get(Q, 0);
		
		// ns = find(CIJ(u,:));
		gsl_vector_const_view CIJ_row_u = gsl_matrix_const_row(CIJ, u);
		gsl_vector* ns = find(&CIJ_row_u.vector);
		
		// for v=ns
		if (ns != NULL) {
			for (int i_ns = 0; i_ns < (int)ns->size; i_ns++) {
				int v = (int)gsl_vector_get(ns, i_ns);
				
				// if (distance(v)==0)
				if ((int)gsl_vector_get(distance, v) == 0) {
					
					// distance(v) = distance(u)+1;
					gsl_vector_set(distance, v, gsl_vector_get(distance, u) + 1.0);
				}
				
				// if (color(v)==white)
				if ((int)gsl_vector_get(color, v) == white) {
					
					// color(v) = gray;
					gsl_vector_set(color, v, (double)gray);
					
					// distance(v) = distance(u)+1;
					gsl_vector_set(distance, v, gsl_vector_get(distance, u) + 1.0);
					
					// branch(v) = u;
					if (branch != NULL) {
						gsl_vector_set(*branch, v, (double)u);
					}
					
					// Q = [Q v];
					gsl_vector* temp = concatenate(Q, (double)v);
					gsl_vector_free(Q);
					Q = temp;
				}
			}
			
			gsl_vector_free(ns);
		}
		
		// Q = Q(2:length(Q));
		gsl_vector* Q_cols = sequence_double(1, length(Q) - 1);
		if (Q_cols != NULL) {
			gsl_vector* temp = ordinal_index(Q, Q_cols);
			gsl_vector_free(Q);
			gsl_vector_free(Q_cols);
			Q = temp;
		} else {
			gsl_vector_free(Q);
			Q = NULL;
		}
		
		// color(u) = black;
		gsl_vector_set(color, u, (double)black);
	}
	
	gsl_vector_free(color);
	return distance;
}
