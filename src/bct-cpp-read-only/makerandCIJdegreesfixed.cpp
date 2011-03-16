#include "bct.h"
#include <cmath>
#include <ctime>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_permute_vector.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_vector.h>

/*
 * Generates a random directed binary graph with the given in-degree and out-
 * degree sequences.  Returns NULL if the algorithm failed to generate a graph
 * satisfying the given degree sequences.
 */
gsl_matrix* bct::makerandCIJdegreesfixed(const gsl_vector* in, const gsl_vector* out) {
	gsl_rng* rng = get_gsl_rng();
	
	// n = length(in);
	int n = length(in);
	
	// k = sum(in);
	int k = sum(in);
	
	// inInv = zeros(k,1);
	gsl_vector* inInv = zeros_vector_double(k);
	
	// outInv = inInv;
	gsl_vector* outInv = copy(inInv);
	
	// iIn = 1; iOut = 1;
	int iIn = 0;
	int iOut = 0;
	
	// for i = 1:n
	for (int i = 0; i < n; i++) {
		
		// inInv(iIn:iIn+in(i) - 1) = i;
		gsl_vector* inInv_ind = sequence_double(iIn, iIn + (int)gsl_vector_get(in, i) - 1);
		if (inInv_ind != NULL) {
			ordinal_index_assign(inInv, inInv_ind, (double)i);
			gsl_vector_free(inInv_ind);
		}
		
		// outInv(iOut:iOut+out(i) - 1) = i;
		gsl_vector* outInv_ind = sequence_double(iOut, iOut + (int)gsl_vector_get(out, i) - 1);
		if (outInv_ind != NULL) {
			ordinal_index_assign(outInv, outInv_ind, (double)i);
			gsl_vector_free(outInv_ind);
		}
		
		// iIn = iIn+in(i);
		iIn += (int)gsl_vector_get(in, i);
		
		// iOut = iOut+out(i);
		iOut += (int)gsl_vector_get(out, i);
	}
	
	// cij = eye(n);
	gsl_matrix* cij = eye_double(n);
	
	// edges = [outInv(1:k)'; inInv(randperm(k))'];
	gsl_vector* outInv_ind = sequence_double(0, k - 1);
	gsl_vector* edges_row_0 = ordinal_index(outInv, outInv_ind);
	gsl_vector_free(outInv);
	gsl_vector_free(outInv_ind);
	gsl_permutation* inInv_ind = randperm(k);
	gsl_permute_vector(inInv_ind, inInv);
	gsl_permutation_free(inInv_ind);
	gsl_vector* edges_row_1 = inInv;
	gsl_matrix* edges = concatenate_columns(edges_row_0, edges_row_1);
	gsl_vector_free(edges_row_0);
	gsl_vector_free(edges_row_1);
	
	bool flag = true;
	
	// for i = 1:k
	for (int i = 0; i < k && flag; i++) {
		
		// if cij(edges(1,i),edges(2,i)),
		if (fp_nonzero(gsl_matrix_get(cij, (int)gsl_matrix_get(edges, 0, i), (int)gsl_matrix_get(edges, 1, i)))) {
			
			// warningCounter = 1;
			int warningCounter = 1;
			
			// while (1)
			while (true) {
				
				// switchTo = ceil(k*rand);
				int switchTo = (int)std::ceil((k - 1) * gsl_rng_uniform(rng));
				
				// if ~(cij(edges(1,i),edges(2,switchTo)) || cij(edges(1,switchTo),edges(2,i))),
				if (!(fp_nonzero(gsl_matrix_get(cij, (int)gsl_matrix_get(edges, 0, i), (int)gsl_matrix_get(edges, 1, switchTo))) ||
					  fp_nonzero(gsl_matrix_get(cij, (int)gsl_matrix_get(edges, 0, switchTo), (int)gsl_matrix_get(edges, 1, i))))) {
					
					// cij(edges(1,i),edges(2,switchTo)) = 1;
					gsl_matrix_set(cij, (int)gsl_matrix_get(edges, 0, i), (int)gsl_matrix_get(edges, 1, switchTo), 1.0);
					
					// if switchTo < i,
					if (switchTo < i) {
						
						// cij(edges(1,switchTo),edges(2,switchTo)) = 0;
						gsl_matrix_set(cij, (int)gsl_matrix_get(edges, 0, switchTo), (int)gsl_matrix_get(edges, 1, switchTo), 0.0);
						
						// cij(edges(1,switchTo),edges(2,i)) = 1;
						gsl_matrix_set(cij, (int)gsl_matrix_get(edges, 0, switchTo), (int)gsl_matrix_get(edges, 1, i), 1.0);
					}
					
					// temp = edges(2,i);
					double temp = gsl_matrix_get(edges, 1, i);
					
					// edges(2,i) = edges(2,switchTo);
					gsl_matrix_set(edges, 1, i, gsl_matrix_get(edges, 1, switchTo));
					
					// edges(2,switchTo) = temp;
					gsl_matrix_set(edges, 1, switchTo, temp);
					
					// break
					break;
				}
				
				// warningCounter = warningCounter+1;
				warningCounter++;
				
				// if warningCounter == 2*k^2
				if (warningCounter == 2 * k * k) {
					
					// flag = 0;
					flag = false;
					
					// return;
					break;
				}
			}
		} else {
			
			// cij(edges(1,i),edges(2,i)) = 1;
			gsl_matrix_set(cij, (int)gsl_matrix_get(edges, 0, i), (int)gsl_matrix_get(edges, 1, i), 1.0);
		}
	}

	gsl_matrix_free(edges);
	
	// flag = 1;
	if (!flag) {
		gsl_matrix_free(cij);
		return NULL;
	}
	
	// cij = cij - eye(n);
	gsl_matrix* eye_n = eye_double(n);
	gsl_matrix_sub(cij, eye_n);
	gsl_matrix_free(eye_n);
	
	return cij;
}

/*
 * Generates a random directed binary graph with the same in-degree and out-
 * degree sequences of the given graph.  Since the degree sequences are
 * necessarily valid, this function should not return NULL unless the given
 * matrix contains nonzero entries on the main diagonal.
 */
gsl_matrix* bct::makerandCIJdegreesfixed(const gsl_matrix* m) {
	if (safe_mode) check_status(m, SQUARE | NO_LOOPS, "makerandCIJdegreesfixed");
	gsl_matrix* ret;
	do {
		gsl_vector* id;
		gsl_vector* od;
		gsl_vector* deg = degrees_dir(m, &id, &od);
		gsl_vector_free(deg);
		ret = makerandCIJdegreesfixed(id, od);
		gsl_vector_free(id);
		gsl_vector_free(od);
	} while (ret == NULL && has_no_loops(m));
	return ret;
}
